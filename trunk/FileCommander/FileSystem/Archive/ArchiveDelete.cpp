#include "FileSystem/Archive/ArchiveDelete.h"

#include "FileSystem/DeleteNonEmptyDirectory.h"

// add directory content to source list
void cArchiveDelete::AddDirToSourceList(const char cDirectory[260])
{
	QHash<QTreeWidgetItem *, tHeaderData> *qhDirectory;

	qhDirectory = qhDirectories.value(cDirectory);

	QHashIterator<QTreeWidgetItem *, tHeaderData> qhiFile(*qhDirectory);
	while (qhiFile.hasNext()) {
		qhiFile.next();

		if (qhiFile.value().FileAttr & cPackerPlugin::iDIRECTORY) {
			if (!QString(qhiFile.value().FileName).endsWith("..")) {
				AddDirToSourceList(qhiFile.value().FileName);
				qlSource.append(qhiFile.value());
			} // if
		} else {
			qlSource.append(qhiFile.value());
		} // if else
	} // while
} // AddDirToSourceList

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
		int iI;

		// get source file list
		for (iI = qlSource.count() - 1; iI >= 0; iI--) {
			const tHeaderData *thdSource;

			thdSource = &qlSource.at(iI);
			if (thdSource->FileAttr & cPackerPlugin::iDIRECTORY) {
				AddDirToSourceList(thdSource->FileName);
			} // if
		} // for
	} // if

	// close dialog or widget
	if (cddDialog) {
		cddDialog->deleteLater();
	} else {
		cdwWidget->deleteLater();
	} // if else
} // run
