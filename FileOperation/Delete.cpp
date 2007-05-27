#include "FileOperation/Delete.h"

#ifdef Q_WS_WIN
#include <Windows.h>
#endif

// constructor
cDelete::cDelete(QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings)
{
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
	this->csSettings = csSettings;
	bCanceled = false;
	qi64TotalMaximum = 0;
} // cDelete

// create widget for background operation
void cDelete::CreateWidget()
{
	cdwWidget = new cDeleteWidget();
	qhblOperations->insertWidget(cFileRoutine::iQUEUED_OPERATION_POSITION, cdwWidget);
	connect(this, SIGNAL(SetSource(const QString &)), cdwWidget, SLOT(on_cDeleteWidget_SetSource(const QString &)));
	connect(this, SIGNAL(SetTotalMaximum(const qint64 &)), cdwWidget, SLOT(on_cDeleteWidget_SetTotalMaximum(const qint64 &)));
	connect(this, SIGNAL(SetTotalValue(const qint64 &)), cdwWidget, SLOT(on_cDeleteWidget_SetTotalValue(const qint64 &)));
	connect(cdwWidget, SIGNAL(Cancel()), SLOT(on_cd_OperationCanceled()));
} // CreateWidget

// start of delete operation
void cDelete::Delete(const QFileInfoList &qfilSource, const QString &qsFilter, const cFileRoutine::eWindow &eStyle)
{
	this->qfilSource = qfilSource;
	this->qsFilter = qsFilter;

	if (eStyle == cFileRoutine::ForegroundWindow) {
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
void cDelete::on_cd_OperationCanceled()
{
	bCanceled = true;
} // on_cd_OperationCanceled

// delete operation to background
void cDelete::on_cdDeleteDialog_Background()
{
	CreateWidget();
	emit SetTotalMaximum(qi64TotalMaximum);
	cddDialog->deleteLater();
	cddDialog = NULL;
} // on_cdDeleteDialog_Background

// delete non empty directory dialog closed with user response
void cDelete::on_cdnedDeleteNonEmptyDirectory_Finished(const cDeleteNonEmptyDirectory::eChoice &ecResponse)
{
	ecDeleteNonEmptyDirectoryCurrent = ecResponse;
	qsPause.release();
} // on_cdnedDeleteNonEmptyDirectory_Finished

#ifdef Q_WS_WIN
// permission dialog closed with user response
void cDelete::on_cpPermission_Finished(const cPermission::eChoice &ecResponse)
{
	ecPermissionCurrent = ecResponse;
	qsPause.release();
} // on_cpPermission_Finished
#endif

// retry dialog closed with user response
void cDelete::on_crRetry_Finished(const cRetry::eChoice &ecResponse)
{
	ecRetryCurrent = ecResponse;
	qsPause.release();
} // on_crRetry_Finished

// separate thread process
void cDelete::run()
{
	cDeleteNonEmptyDirectory::eChoice ecDeleteNonEmptyDirectory;
#ifdef Q_WS_WIN
	cPermission::eChoice ecPermission;
#endif
	cRetry::eChoice ecRetry;
	int iI;
	qint64 qi64Total;
	QList<QFileInfoList> qlSources;
#ifdef Q_WS_WIN
	QString qsOverwrite;
#endif
	QString qsSourcePath;

	qsSourcePath = qfilSource.at(0).path();
	qi64TotalMaximum = 0;
	// gather source files
	for (iI = 0; iI < qfilSource.count(); iI++) {
		qlSources.append(cFileRoutine::GetSources(qfilSource.at(iI), qsFilter));
		qi64TotalMaximum += qlSources.at(iI).count();
	} // for

	emit SetTotalMaximum(qi64TotalMaximum);

#ifdef Q_WS_WIN
	// get default readonly overwrite permission
	qsOverwrite = csSettings->GetReadonlyFileOverwrite();
	if (qsOverwrite == qsASK) {
		ecPermission = cPermission::Ask;
	} else {
		if (qsOverwrite == qsYES_TO_ALL) {
			ecPermission = cPermission::YesToAll;
		} else {
			ecPermission = cPermission::NoToAll;
		} // if else
	} // if else
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
		int iJ;
		const QFileInfoList *qfilSources;

		// check if source contains more items
		if (qlSources.at(iI).count() > 1) {
			ecDeleteNonEmptyDirectoryCurrent = cDeleteNonEmptyDirectory::Ask;

			if (ecDeleteNonEmptyDirectory == cDeleteNonEmptyDirectory::Ask) {
				emit ShowDeleteNonEmptyDirectoryDialog(qlSources.at(iI).at(0).filePath());
				// wait for answer
				qsPause.acquire();

				switch (ecDeleteNonEmptyDirectoryCurrent) {
					case cDeleteNonEmptyDirectory::YesToAll:
						ecDeleteNonEmptyDirectory = cDeleteNonEmptyDirectory::YesToAll;
						break;
					case cDeleteNonEmptyDirectory::NoToAll:
						ecDeleteNonEmptyDirectory = cDeleteNonEmptyDirectory::NoToAll;
					default:
						;
				} // switch
			} // if

			if (ecDeleteNonEmptyDirectory == cDeleteNonEmptyDirectory::NoToAll || ecDeleteNonEmptyDirectoryCurrent == cDeleteNonEmptyDirectory::No) {
				// do not delete this list of sources
				qi64Total += qlSources.at(iI).count();
				continue;
			} // if
			if (ecDeleteNonEmptyDirectoryCurrent == cDeleteNonEmptyDirectory::Cancel) {
				// delete canceled
				break;
			} // if
			// else can remove this list of sources
		} // if

		// go through one list of sources (one marked item in directory view)
		qfilSources = &qlSources.at(iI);
		for (iJ = qfilSources->count() - 1; iJ >= 0; iJ--) {
			QDir qdSource;

			if (cddDialog) {
				// name with path in dialog
				emit SetSource(qfilSources->at(iJ).filePath());
			} else {
				// just name in widget
				emit SetSource(qfilSources->at(iJ).fileName());
			} // if else

#ifdef Q_WS_WIN
			// check readonly permission
			if (GetFileAttributes(reinterpret_cast<LPCWSTR>(qfilSources->at(iJ).filePath().unicode())) & FILE_ATTRIBUTE_READONLY) {
				ecPermissionCurrent = cPermission::Ask;

				if (ecPermission == cPermission::Ask) {
					emit ShowPermissionDialog(qfilSources->at(iJ).fileName(), tr("is readonly."));
					// wait for answer
					qsPause.acquire();

					switch (ecPermissionCurrent) {
						case cPermission::YesToAll:
							ecPermission = cPermission::YesToAll;
							break;
						case cPermission::NoToAll:
							ecPermission = cPermission::NoToAll;
						default:
							;
					} // switch

					if (ecPermissionCurrent == cPermission::Cancel) {
						break;
					} // if
				} // if
				if (ecPermission == cPermission::NoToAll || ecPermissionCurrent == cPermission::No) {
					continue;
				} else {
					// remove target file readonly permission
					SetFileAttributes(reinterpret_cast<LPCWSTR>(qfilSources->at(iJ).filePath().unicode()), GetFileAttributes(reinterpret_cast<LPCWSTR>(qfilSources->at(iJ).filePath().unicode())) & ~FILE_ATTRIBUTE_READONLY);
				} // if else
			} // if
#endif

			while (true) {
				bool bSuccess;

				if (qfilSources->at(iJ).isDir()) {
					bSuccess = qdSource.rmdir(qfilSources->at(iJ).filePath());
				} else {
#ifdef Q_WS_WIN
					if (csSettings->GetDeleteToRecycleBin()) {
						SHFILEOPSTRUCT shfsRemove;
						TCHAR tcSource[MAX_PATH + 1];

						memset(tcSource, 0, MAX_PATH + 1);
						memcpy(tcSource, QDir::toNativeSeparators(qfilSources->at(iJ).filePath()).unicode(), qfilSources->at(iJ).filePath().length() * sizeof(WCHAR)); 

						shfsRemove.wFunc = FO_DELETE;
						shfsRemove.pFrom = tcSource;
						shfsRemove.pTo = NULL;
						shfsRemove.fFlags |= FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_SILENT;
						
						bSuccess = !SHFileOperation(&shfsRemove);
					} else {
						bSuccess = qdSource.remove(qfilSources->at(iJ).filePath());
					} // if else
#else
					bSuccess = qdSource.remove(qfilSources->at(iJ).filePath());
#endif
				} // if else

				if (!bSuccess) {
					// not successfuly removed - try to retry
					if (ecRetry != cRetry::SkipAll) {
						QString qsInformation;

						if (qfilSources->at(iJ).isDir()) {
							qsInformation = tr("Can't remove following directory:");
						} else {
							qsInformation = tr("Can't delete following file:");
						} // if else

						emit ShowRetryDialog(qsInformation, qfilSources->at(iJ).filePath());
						// wait for answer
						qsPause.acquire();

						if (ecRetryCurrent == cRetry::SkipAll) {
							// memorize permanent answer
							ecRetry = cRetry::SkipAll;
						} // if
					} // if
					if (ecRetry == cRetry::SkipAll || ecRetryCurrent == cRetry::Skip || ecRetryCurrent == cRetry::Abort) {
						// skip this file
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
