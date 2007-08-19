#include "FileSystem/Local/LocalDelete.h"

#ifdef Q_WS_WIN
#include <Windows.h>
#endif
#include <QtCore/QDir>

// constructor
cLocalDelete::cLocalDelete(cLocal *clFileSystem, QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings)
{
	this->clFileSystem = clFileSystem;
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
	this->csSettings = csSettings;
	bCanceled = false;
	qi64TotalMaximum = 0;
} // cLocalDelete

// delete non empty directory check
cFileOperation::eCheckResult cLocalDelete::CheckDeleteNonEmptyDirectory(const QFileInfoList *qfilSources, cDeleteNonEmptyDirectory::eChoice *ecDeleteNonEmptyDirectory, qint64 *qi64Total)
{
	ecDeleteNonEmptyDirectoryCurrent = cDeleteNonEmptyDirectory::Ask;

	if (*ecDeleteNonEmptyDirectory == cDeleteNonEmptyDirectory::Ask) {
		emit ShowDeleteNonEmptyDirectoryDialog(qfilSources->at(0).filePath());
		// wait for answer
		qsPause.acquire();

		switch (ecDeleteNonEmptyDirectoryCurrent) {
			case cDeleteNonEmptyDirectory::YesToAll:
				*ecDeleteNonEmptyDirectory = cDeleteNonEmptyDirectory::YesToAll;
				break;
			case cDeleteNonEmptyDirectory::NoToAll:
				*ecDeleteNonEmptyDirectory = cDeleteNonEmptyDirectory::NoToAll;
			default:
				;
		} // switch
	} // if

	if (*ecDeleteNonEmptyDirectory == cDeleteNonEmptyDirectory::NoToAll || ecDeleteNonEmptyDirectoryCurrent == cDeleteNonEmptyDirectory::No) {
		// do not delete this list of sources
		*qi64Total += qfilSources->count();
		return cFileOperation::NextFile;
	} // if
	if (ecDeleteNonEmptyDirectoryCurrent == cDeleteNonEmptyDirectory::Cancel) {
		// delete canceled
		return cFileOperation::Cancel;
	} // if
		// else can remove this list of sources

	return cFileOperation::Nothing;
} // CheckDeleteNonEmptyDirectory

#ifdef Q_WS_WIN
// check target file permission
cFileOperation::eCheckResult cLocalDelete::CheckPermission(const QFileInfo *qfiSource, cPermission::eChoice *ecPermission)
{
	if (GetFileAttributes(reinterpret_cast<LPCWSTR>(qfiSource->filePath().unicode())) & FILE_ATTRIBUTE_READONLY) {
		ecPermissionCurrent = cPermission::Ask;

		if (*ecPermission == cPermission::Ask) {
			emit ShowPermissionDialog(qfiSource->fileName(), tr("is readonly."));
			// wait for answer
			qsPause.acquire();

			switch (ecPermissionCurrent) {
				case cPermission::YesToAll:
					*ecPermission = cPermission::YesToAll;
					break;
				case cPermission::NoToAll:
					*ecPermission = cPermission::NoToAll;
				default:
					;
			} // switch

			if (ecPermissionCurrent == cPermission::Cancel) {
				return cFileOperation::Cancel;
			} // if
		} // if
		if (*ecPermission == cPermission::NoToAll || ecPermissionCurrent == cPermission::No) {
			return cFileOperation::NextFile;;
		} else {
			// remove target file readonly permission
			SetFileAttributes(reinterpret_cast<LPCWSTR>(qfiSource->filePath().unicode()), GetFileAttributes(reinterpret_cast<LPCWSTR>(qfiSource->filePath().unicode())) & ~FILE_ATTRIBUTE_READONLY);
		} // if else
	} // if

	return cFileOperation::Nothing;
} // CheckPermission
#endif

// retry if delete unsuccesfull
cFileOperation::eCheckResult cLocalDelete::CheckRetry(const QFileInfo *qfiSource, cRetry::eChoice *ecRetry)
{
	if (*ecRetry != cRetry::SkipAll) {
		QString qsInformation;

		if (qfiSource->isDir()) {
			qsInformation = tr("Can't remove following directory:");
		} else {
			qsInformation = tr("Can't delete following file:");
		} // if else

		emit ShowRetryDialog(qsInformation, qfiSource->filePath());
		// wait for answer
		qsPause.acquire();

		if (ecRetryCurrent == cRetry::SkipAll) {
			// memorize permanent answer
			*ecRetry = cRetry::SkipAll;
		} // if
	} // if
	if (*ecRetry == cRetry::SkipAll || ecRetryCurrent == cRetry::Skip || ecRetryCurrent == cRetry::Abort) {
		// skip this file
		return cFileOperation::NextFile;
	} // if

	return cFileOperation::Nothing;
} // CheckRetry

// create widget for background operation
void cLocalDelete::CreateWidget()
{
	cdwWidget = new cDeleteWidget();
	qhblOperations->insertWidget(cFileOperation::iQUEUED_OPERATION_POSITION, cdwWidget);
	connect(this, SIGNAL(SetSource(const QString &)), cdwWidget, SLOT(on_cDeleteWidget_SetSource(const QString &)));
	connect(this, SIGNAL(SetTotalMaximum(const qint64 &)), cdwWidget, SLOT(on_cDeleteWidget_SetTotalMaximum(const qint64 &)));
	connect(this, SIGNAL(SetTotalValue(const qint64 &)), cdwWidget, SLOT(on_cDeleteWidget_SetTotalValue(const qint64 &)));
	connect(cdwWidget, SIGNAL(Cancel()), SLOT(on_cd_OperationCanceled()));
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
		connect(cddDialog, SIGNAL(Cancel()), SLOT(on_cd_OperationCanceled()));
		connect(cddDialog, SIGNAL(Background()), SLOT(on_cdDeleteDialog_Background()));
		cdwWidget = NULL;
	} else {
		CreateWidget();
		cddDialog = NULL;
	} // if else

#ifdef Q_WS_WIN
	// permission dialog
	connect(this, SIGNAL(ShowPermissionDialog(const QString &, const QString &)), &cpPermission, SLOT(Show(const QString &, const QString &)));
	connect(&cpPermission, SIGNAL(Finished(const cPermission::eChoice &)), SLOT(on_cpPermission_Finished(const cPermission::eChoice &)));
#endif

	// retry dialog
	connect(this, SIGNAL(ShowRetryDialog(const QString &, const QString &)), &crRetry, SLOT(Show(const QString &, const QString &)));
	connect(&crRetry, SIGNAL(Finished(const cRetry::eChoice &)), SLOT(on_crRetry_Finished(const cRetry::eChoice &)));

	// delete non empty directory
	connect(this, SIGNAL(ShowDeleteNonEmptyDirectoryDialog(const QString &)), &cdnedDeleteNonEmptyDir, SLOT(Show(const QString &)));
	connect(&cdnedDeleteNonEmptyDir, SIGNAL(Finished(const cDeleteNonEmptyDirectory::eChoice &)), SLOT(on_cdnedDeleteNonEmptyDirectory_Finished(const cDeleteNonEmptyDirectory::eChoice &)));

	start();
} // Delete

// delete operation was canceled
void cLocalDelete::on_cd_OperationCanceled()
{
	bCanceled = true;
} // on_cd_OperationCanceled

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
		qlSources.append(clFileSystem->GetFiles(qfilSource.at(iI), qsFilter));
		qi64TotalMaximum += qlSources.at(iI).count();
	} // for

	emit SetTotalMaximum(qi64TotalMaximum);

#ifdef Q_WS_WIN
	// get default readonly overwrite permission
	ecPermission = cFileOperation::GetDefaultReadonlyOverwritePermission(csSettings);
#endif
	ecRetry = cRetry::Ask;
	// get default delete non empty directory answer
	if (csSettings->GetAskToDeleteNonEmptyDirectory()) {
		ecDeleteNonEmptyDirectory = cDeleteNonEmptyDirectory::Ask;
	} else {
		ecDeleteNonEmptyDirectory = cDeleteNonEmptyDirectory::YesToAll;
	} // if else

	qi64Total = 0;
	// main process - go through list of sources
	for (iI = 0; iI < qlSources.count(); iI++) {
		cFileOperation::eCheckResult ecrCheck;
		int iJ;
		const QFileInfoList *qfilSources;

		qfilSources = &qlSources.at(iI);

		// check if source contains more items
		if (qfilSources->count() > 1) {
			ecrCheck = CheckDeleteNonEmptyDirectory(qfilSources, &ecDeleteNonEmptyDirectory, &qi64Total);
			if (ecrCheck == cFileOperation::NextFile) {
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
			ecrCheck = CheckPermission(qfiSource, &ecPermission);
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
					ecrCheck = CheckRetry(qfiSource, &ecRetry);
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
} // run
