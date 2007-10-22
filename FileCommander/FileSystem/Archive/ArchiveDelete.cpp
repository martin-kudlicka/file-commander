#include "FileSystem/Archive/ArchiveDelete.h"

#include "FileSystem/DeleteNonEmptyDirectory.h"
#include <QtCore/QDir>

// add directory content to source list
void cArchiveDelete::AddDirToSourceList(const char cDirectory[260], QStringList *qslDirectories, QStringList *qslFiles, QStringList *qslIgnore, cPermission::eChoice *ecPermission)
{
	QHash<QTreeWidgetItem *, tHeaderData> *qhDirectory;

	qhDirectory = qhDirectories.value(cDirectory);

	QHashIterator<QTreeWidgetItem *, tHeaderData> qhiFile(*qhDirectory);
	while (qhiFile.hasNext()) {
		cFileOperation::eCheckResult ecrCheck;

		qhiFile.next();

		// check readonly permission
		ecrCheck = CheckReadOnlyAttribute(&qhiFile.value(), ecPermission);
		if (ecrCheck == cFileOperation::NextFile) {
			if (!(qhiFile.value().FileAttr & cPackerPlugin::iDIRECTORY)) {
				qslIgnore->append(QDir::toNativeSeparators(QFileInfo(qhiFile.value().FileName).path()));
			} // if
			continue;
		} else {
			if (ecrCheck == cFileOperation::Cancel) {
				break;
			} // if
		} // if else

		if (qhiFile.value().FileAttr & cPackerPlugin::iDIRECTORY) {
			if (!QString(qhiFile.value().FileName).endsWith("..")) {
				qslDirectories->prepend(qhiFile.value().FileName);
				AddDirToSourceList(qhiFile.value().FileName, qslDirectories, qslFiles, qslIgnore, ecPermission);
			} // if
		} else {
			qslFiles->append(qhiFile.value().FileName);
		} // if else
	} // while
} // AddDirToSourceList

// check target file read only attribute
const cFileOperation::eCheckResult cArchiveDelete::CheckReadOnlyAttribute(const tHeaderData *thdFile, cPermission::eChoice *ecPermission)
{
	if (thdFile->FileAttr & cPackerPlugin::iREAD_ONLY) {
		return cFileOperation::CheckReadOnlyAttribute(&cpPermission, thdFile->FileName, ecPermission, &ecPermissionCurrent, &qsPause, true);
	} else {
		return cFileOperation::Nothing;
	} // if else
} // CheckReadOnlyAttribute

// constructor
cArchiveDelete::cArchiveDelete(QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings)
{
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
	this->csSettings = csSettings;
	bCanceled = false;
} // cLocalDelete

// create widget for background operation
void cArchiveDelete::CreateWidget()
{
	cdwWidget = new cDeleteWidget();
	qhblOperations->insertWidget(cFileOperation::iQUEUED_OPERATION_POSITION, cdwWidget);
	connect(this, SIGNAL(SetSource(const QString &)), cdwWidget, SLOT(on_cDeleteWidget_SetSource(const QString &)));
	connect(this, SIGNAL(SetTotalMaximum(const qint64 &)), cdwWidget, SLOT(on_cDeleteWidget_SetTotalMaximum(const qint64 &)));
	connect(this, SIGNAL(SetTotalValue(const qint64 &)), cdwWidget, SLOT(on_cDeleteWidget_SetTotalValue(const qint64 &)));
	connect(cdwWidget, SIGNAL(Cancel()), SLOT(on_cLocalDelete_OperationCanceled()));
} // CreateWidget

// start of delete operation
void cArchiveDelete::Delete(const QString &qsArchiveFilePath, const QList<tHeaderData> &qlSource, const QHash<QString, QHash<QTreeWidgetItem *, tHeaderData> *> &qhDirectories, const QString &qsFilter, cPackerPlugin::sPluginInfo *spiPluginInfo, const cFileOperation::eOperationPosition &eopPosition)
{
	this->qsArchiveFilePath = qsArchiveFilePath;
	this->qlSource = qlSource;
	this->qhDirectories = qhDirectories;
	this->qsFilter = qsFilter;
	this->spiPluginInfo = spiPluginInfo;

	if (eopPosition == cFileOperation::ForegroundOperation) {
		cddDialog = new cDeleteDialog(qmwParent);
		cddDialog->setModal(true);
		cddDialog->show();
		connect(this, SIGNAL(SetSource(const QString &)), cddDialog, SLOT(on_cDeleteDialog_SetSource(const QString &)));
		connect(this, SIGNAL(SetTotalMaximum(const qint64 &)), cddDialog, SLOT(on_cDeleteDialog_SetTotalMaximum(const qint64 &)));
		connect(this, SIGNAL(SetTotalValue(const qint64 &)), cddDialog, SLOT(on_cDeleteDialog_SetTotalValue(const qint64 &)));
		connect(cddDialog, SIGNAL(Cancel()), SLOT(on_cLocalDelete_OperationCanceled()));
		connect(cddDialog, SIGNAL(Background()), SLOT(on_cdDeleteDialog_Background()));
		cdwWidget = NULL;
	} else {
		CreateWidget();
		cddDialog = NULL;
	} // if else

	// delete non empty directory
	connect(&cdnedDeleteNonEmptyDir, SIGNAL(Finished(const cDeleteNonEmptyDirectory::eChoice &)), SLOT(on_cdnedDeleteNonEmptyDirectory_Finished(const cDeleteNonEmptyDirectory::eChoice &)));

	// permission dialog
	connect(&cpPermission, SIGNAL(Finished(const cPermission::eChoice &)), SLOT(on_cpPermission_Finished(const cPermission::eChoice &)));

	start();
} // Delete

// delete operation to background
void cArchiveDelete::on_cdDeleteDialog_Background()
{
	CreateWidget();
	emit SetTotalMaximum(qi64TotalMaximum);
	cddDialog->deleteLater();
	cddDialog = NULL;
} // on_cdDeleteDialog_Background

// delete non empty directory dialog closed with user response
void cArchiveDelete::on_cdnedDeleteNonEmptyDirectory_Finished(const cDeleteNonEmptyDirectory::eChoice &ecResponse)
{
	ecDeleteNonEmptyDirectoryCurrent = ecResponse;
	qsPause.release();
} // on_cdnedDeleteNonEmptyDirectory_Finished

// delete operation was canceled
void cArchiveDelete::on_cLocalDelete_OperationCanceled()
{
	bCanceled = true;
} // on_cLocalDelete_OperationCanceled

// permission dialog closed with user response
void cArchiveDelete::on_cpPermission_Finished(const cPermission::eChoice &ecResponse)
{
	ecPermissionCurrent = ecResponse;
	qsPause.release();
} // on_cpPermission_Finished

// separate thread process
void cArchiveDelete::run()
{
	cDeleteNonEmptyDirectory::eChoice ecDeleteNonEmptyDirectory;

	ecDeleteNonEmptyDirectory = cFileOperation::GetDefaultDeleteNonEmptyDirectory(csSettings);

	// check if selected directories are empty
	if (ecDeleteNonEmptyDirectory == cDeleteNonEmptyDirectory::Ask) {
		int iI;

		for (iI = qlSource.count() - 1; iI >= 0; iI--) {
			const tHeaderData *thdSource;

			thdSource = &qlSource.at(iI);
			if (thdSource->FileAttr & cPackerPlugin::iDIRECTORY) {
				if (qhDirectories.value(thdSource->FileName)->count() > 1) {
					// directory has more files than ".." only
					cFileOperation::eCheckResult ecrCheck;

					ecrCheck = cFileOperation::CheckDeleteNonEmptyDirectory(&cdnedDeleteNonEmptyDir, thdSource->FileName, &ecDeleteNonEmptyDirectory, &ecDeleteNonEmptyDirectoryCurrent, &qsPause);
					if (ecrCheck == cFileOperation::NextFile) {
						// skip this directory
						qlSource.removeAt(iI);
					} else {
						if (ecrCheck == cFileOperation::Cancel) {
							// cancel whole operation
							break;
						} // if
					} // if
				} // if
			} // if
		} // for
	} // if

	if (ecDeleteNonEmptyDirectoryCurrent != cDeleteNonEmptyDirectory::Cancel) {
		cPermission::eChoice ecPermission;
		int iI;
		QStringList qslDirectories, qslFiles, qslIgnore;

		// get default readonly overwrite permission
		ecPermission = cFileOperation::GetDefaultReadonlyOverwritePermission(csSettings);

		// get source file list
		for (iI = qlSource.count() - 1; iI >= 0 && ecPermissionCurrent != cPermission::Cancel; iI--) {
			const tHeaderData *thdSource;
			cFileOperation::eCheckResult ecrCheck;

			thdSource = &qlSource.at(iI);

			// check readonly permission
			ecrCheck = CheckReadOnlyAttribute(thdSource, &ecPermission);
			if (ecrCheck == cFileOperation::NextFile) {
				continue;
			} else {
				if (ecrCheck == cFileOperation::Cancel) {
					break;
				} // if
			} // if else

			if (thdSource->FileAttr & cPackerPlugin::iDIRECTORY) {
				qslDirectories.append(thdSource->FileName);
				AddDirToSourceList(thdSource->FileName, &qslDirectories, &qslFiles, &qslIgnore, &ecPermission);
			} else {
				qslFiles.append(thdSource->FileName);
			} // if else
		} // for

		// check if can continue
		if (ecPermissionCurrent != cPermission::Cancel) {
			char *cFileList;
			uint uiFileListPos, uiTotalLength;

			// check ignore list and extend it
			for (iI = qslIgnore.count() - 1; iI >= 0; iI--) {
				QString qsPath;

				qsPath = qslIgnore.at(iI);
				while (qsPath != ".") {
					qslDirectories.removeAll(qsPath);
					qsPath = QFileInfo(qsPath).path();
				} // while
			} // for

			// get total length of file names to delete
			uiTotalLength = 0;
			for (iI = 0; iI < qslDirectories.count(); iI++) {
				uiTotalLength += qslDirectories.at(iI).length();
			} // for
			for (iI = 0; iI < qslFiles.count(); iI++) {
				uiTotalLength += qslFiles.at(iI).length();
			} // for

			// create file list to delete
			cFileList = new char[uiTotalLength + qslDirectories.count() + qslFiles.count() + 1];
			memset(cFileList, 0, uiTotalLength + qslDirectories.count() + qslFiles.count() + 1);
			uiFileListPos = 0;
			for (iI = 0; iI < qslFiles.count(); iI++) {
				strcpy(cFileList + uiFileListPos, qslFiles.at(iI).toLocal8Bit().constData());
				uiFileListPos += qslFiles.at(iI).length() + 1;
			} // for
			for (iI = 0; iI < qslDirectories.count(); iI++) {
				strcpy(cFileList + uiFileListPos, qslDirectories.at(iI).toLocal8Bit().constData());
				uiFileListPos += qslDirectories.at(iI).length() + 1;
			} // for

			// delete files
			spiPluginInfo->tdfDeleteFiles(qsArchiveFilePath.toLocal8Bit().data(), cFileList);

			delete cFileList;
		} // if
	} // if

	// close dialog or widget
	if (cddDialog) {
		cddDialog->deleteLater();
	} else {
		cdwWidget->deleteLater();
	} // if else
} // run
