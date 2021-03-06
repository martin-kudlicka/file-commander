#include "FileSystem/Local/LocalDelete.h"

#ifdef Q_WS_WIN
#include <Windows.h>
#endif
#include <QtCore/QDir>
#include "FileSystem/Local/LocalCommon.h"

// constructor
cLocalDelete::cLocalDelete(QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings)
{
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
	this->csSettings = csSettings;

	bCanceled = false;
	qi64TotalMaximum = 0;
	cdnedDeleteNonEmptyDir = new cDeleteNonEmptyDirectory(qmwParent);
	cpPermission = new cPermission(qmwParent);
} // cLocalDelete

// create widget for background operation
void cLocalDelete::CreateWidget()
{
	cdwWidget = new cDeleteWidget();
	qhblOperations->insertWidget(cFileOperation::iQUEUED_OPERATION_POSITION, cdwWidget);
	connect(this, SIGNAL(SetSource(const QString &)), cdwWidget, SLOT(on_cDeleteWidget_SetSource(const QString &)));
	connect(this, SIGNAL(SetTotalMaximum(const qint64 &)), cdwWidget, SLOT(on_cDeleteWidget_SetTotalMaximum(const qint64 &)));
	connect(this, SIGNAL(SetTotalValue(const qint64 &)), cdwWidget, SLOT(on_cDeleteWidget_SetTotalValue(const qint64 &)));
	connect(cdwWidget, SIGNAL(Cancel()), SLOT(on_cLocalDelete_OperationCanceled()));
} // CreateWidget

// start of delete operation
void cLocalDelete::Delete(const QFileInfoList &qfilSource, const QString &qsFilter, const cFileOperation::eOperationPosition &eopPosition)
{
	this->qfilSource = qfilSource;
	this->qsFilter = qsFilter;

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

#ifdef Q_WS_WIN
	// permission dialog
	connect(cpPermission, SIGNAL(Finished(const cPermission::eChoice &)), SLOT(on_cpPermission_Finished(const cPermission::eChoice &)));
#endif

	// retry dialog
	connect(&crRetry, SIGNAL(Finished(const cRetry::eChoice &)), SLOT(on_crRetry_Finished(const cRetry::eChoice &)));

	// delete non empty directory
	connect(cdnedDeleteNonEmptyDir, SIGNAL(Finished(const cDeleteNonEmptyDirectory::eChoice &)), SLOT(on_cdnedDeleteNonEmptyDirectory_Finished(const cDeleteNonEmptyDirectory::eChoice &)));

	start();
} // Delete

// delete operation to background
void cLocalDelete::on_cdDeleteDialog_Background()
{
	CreateWidget();
	emit SetTotalMaximum(qi64TotalMaximum);
	cddDialog->deleteLater();
	cddDialog = NULL;
} // on_cdDeleteDialog_Background

// delete non empty directory dialog closed with user response
void cLocalDelete::on_cdnedDeleteNonEmptyDirectory_Finished(const cDeleteNonEmptyDirectory::eChoice &ecResponse)
{
	ecDeleteNonEmptyDirectoryCurrent = ecResponse;
	qsPause.release();
} // on_cdnedDeleteNonEmptyDirectory_Finished

// delete operation was canceled
void cLocalDelete::on_cLocalDelete_OperationCanceled()
{
	bCanceled = true;
} // on_cLocalDelete_OperationCanceled

#ifdef Q_WS_WIN
// permission dialog closed with user response
void cLocalDelete::on_cpPermission_Finished(const cPermission::eChoice &ecResponse)
{
	ecPermissionCurrent = ecResponse;
	qsPause.release();
} // on_cpPermission_Finished
#endif

// retry dialog closed with user response
void cLocalDelete::on_crRetry_Finished(const cRetry::eChoice &ecResponse)
{
	ecRetryCurrent = ecResponse;
	qsPause.release();
} // on_crRetry_Finished

// separate thread process
void cLocalDelete::run()
{
	cDeleteNonEmptyDirectory::eChoice ecDeleteNonEmptyDirectory;
#ifdef Q_WS_WIN
	cPermission::eChoice ecPermission;
#endif
	cRetry::eChoice ecRetry;
	int iI;
	qint64 qi64Total;
	QList<QFileInfoList> qlSources;
	QString qsSourcePath;

	qsSourcePath = qfilSource.at(0).path();
	qi64TotalMaximum = 0;
	// gather source files
	for (iI = 0; iI < qfilSource.count(); iI++) {
		qlSources.append(cLocalCommon::GetFiles(qfilSource.at(iI), qsFilter));
		qi64TotalMaximum += qlSources.at(iI).count();
	} // for

	emit SetTotalMaximum(qi64TotalMaximum);

#ifdef Q_WS_WIN
	// get default readonly overwrite permission
	ecPermission = cFileOperation::GetDefaultReadonlyOverwritePermission(csSettings);
#endif
	ecRetry = cRetry::Ask;
	ecDeleteNonEmptyDirectory = cFileOperation::GetDefaultDeleteNonEmptyDirectory(csSettings);

	qi64Total = 0;
	// main process - go through list of sources
	for (iI = 0; iI < qlSources.count(); iI++) {
		cFileOperation::eCheckResult ecrCheck;
		int iJ;
		const QFileInfoList *qfilSources;

		qfilSources = &qlSources.at(iI);

		// check if source contains more items
		if (qfilSources->count() > 1) {
			ecrCheck = cFileOperation::CheckDeleteNonEmptyDirectory(cdnedDeleteNonEmptyDir, qfilSources->at(0).filePath(), &ecDeleteNonEmptyDirectory, &ecDeleteNonEmptyDirectoryCurrent, &qsPause);
			if (ecrCheck == cFileOperation::NextFile) {
				qi64Total += qfilSources->count();
				continue;
			} else {
				if (ecrCheck == cFileOperation::Cancel) {
					break;
				} // if
			} // if else
		} // if

		// go through one list of sources (one marked item in directory view)
		for (iJ = qfilSources->count() - 1; iJ >= 0 && !bCanceled; iJ--) {
			QDir qdSource;
			const QFileInfo *qfiSource;

			qfiSource = &qfilSources->at(iJ);

			if (cddDialog) {
				// name with path in dialog
				emit SetSource(qfiSource->filePath());
			} else {
				// just name in widget
				emit SetSource(qfiSource->fileName());
			} // if else

#ifdef Q_WS_WIN
			// check readonly permission
			ecrCheck = cFileOperation::CheckReadOnlyAttribute(cpPermission, qfiSource->filePath(), &ecPermission, &ecPermissionCurrent, &qsPause);
			if (ecrCheck == cFileOperation::NextFile) {
				continue;
			} else {
				if (ecrCheck == cFileOperation::Cancel) {
					break;
				} // if
			} // if else
#endif

			while (true) {
				bool bSuccess;

				if (qfiSource->isDir()) {
					bSuccess = qdSource.rmdir(qfiSource->filePath());
				} else {
#ifdef Q_WS_WIN
					if (csSettings->GetDeleteToRecycleBin()) {
						// delete file to recycle bin
						SHFILEOPSTRUCT shfsRemove;
						TCHAR tcSource[MAX_PATH + 1];

						memset(tcSource, 0, MAX_PATH + 1);
						memcpy(tcSource, QDir::toNativeSeparators(qfiSource->filePath()).unicode(), qfiSource->filePath().length() * sizeof(WCHAR)); 

						shfsRemove.wFunc = FO_DELETE;
						shfsRemove.pFrom = tcSource;
						shfsRemove.pTo = NULL;
						shfsRemove.fFlags |= FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_SILENT;
						
						bSuccess = !SHFileOperation(&shfsRemove);
					} else {
						// delete file without recycle bin
#endif
						bSuccess = qdSource.remove(qfiSource->filePath());
#ifdef Q_WS_WIN
					} // if else
#endif
				} // if else

				if (!bSuccess) {
					// not successfuly removed - try to retry
					ecrCheck = cFileOperation::CheckRetry(&crRetry, cFileOperationDialog::DeleteOperation, *qfiSource, &ecRetry, &ecRetryCurrent, &qsPause);
					if (ecrCheck == cFileOperation::NextFile) {
						break;
					} // if
					// else try once more
				} else {
					// successfuly removed/deleted
					break;
				} // if else
			} // while

			if (ecRetryCurrent == cRetry::Abort) {
				// process aborted
				break;
			} // if

			emit SetTotalValue(qi64Total);
			qi64Total++;
		} // for
	} // for

	// close dialog or widget
	if (cddDialog) {
		cddDialog->deleteLater();
	} else {
		cdwWidget->deleteLater();
	} // if else
	cdnedDeleteNonEmptyDir->deleteLater();
	cpPermission->deleteLater();
} // run
