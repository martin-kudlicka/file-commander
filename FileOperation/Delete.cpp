#include "FileOperation/Delete.h"

#ifdef Q_WS_WIN
#include <Windows.h>
#endif

// constructor
cDelete::cDelete(QMainWindow *qmwParent, QHBoxLayout *qhblOperations
#ifdef Q_WS_WIN
					  , cSettings *csSettings)
#else
					  )
#endif
{
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
#ifdef Q_WS_WIN
	this->csSettings = csSettings;
#endif
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
#ifdef Q_WS_WIN
	cPermission::eChoice ecPermission;
#endif
	cRetry::eChoice ecRetry;
	int iI;
	QFileInfoList qfilSources;
#ifdef Q_WS_WIN
	QString qsOverwrite;
#endif

	// gather source files
	qfilSources = cFileRoutine::GetSources(qfilSource, true, qsFilter);
	qi64TotalMaximum = qfilSources.count();

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

	// main process
	for (iI = qfilSources.count() - 1; iI >= 0; iI--) {
		QDir qdSource;

		if (cddDialog) {
			// name with path in dialog
			emit SetSource(qfilSources.at(iI).filePath());
		} else {
			// just name in widget
			emit SetSource(qfilSources.at(iI).fileName());
		} // if else

#ifdef Q_WS_WIN
		// check readonly permission
		ecPermissionCurrent = cPermission::Ask;
		if (GetFileAttributes(reinterpret_cast<LPCWSTR>(qfilSources.at(iI).filePath().unicode())) & FILE_ATTRIBUTE_READONLY) {
			if (ecPermission == cPermission::Ask) {
				emit ShowPermissionDialog(qfilSources.at(iI).fileName(), tr("is readonly."));
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
				SetFileAttributes(reinterpret_cast<LPCWSTR>(qfilSources.at(iI).filePath().unicode()), GetFileAttributes(reinterpret_cast<LPCWSTR>(qfilSources.at(iI).filePath().unicode())) & ~FILE_ATTRIBUTE_READONLY);
			} // if else
		} // if
#endif

		while (true) {
			bool bSuccess;

			if (qfilSources.at(iI).isDir()) {
				bSuccess = qdSource.rmdir(qfilSources.at(iI).filePath());
			} else {
#ifdef Q_WS_WIN
				if (csSettings->GetDeleteToRecycleBin()) {
					SHFILEOPSTRUCT shfsRemove;
					TCHAR tcSource[MAX_PATH + 1];

					memset(tcSource, 0, MAX_PATH + 1);
					memcpy(tcSource, QDir::toNativeSeparators(qfilSources.at(iI).filePath()).unicode(), qfilSources.at(iI).filePath().length() * sizeof(WCHAR)); 

					shfsRemove.wFunc = FO_DELETE;
					shfsRemove.pFrom = tcSource;
					shfsRemove.pTo = NULL;
					shfsRemove.fFlags |= FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_SILENT;
					
					bSuccess = !SHFileOperation(&shfsRemove);
				} else {
					bSuccess = qdSource.remove(qfilSources.at(iI).filePath());
				} // if else
#else
				bSuccess = qdSource.remove(qfilSources.at(iI).filePath());
#endif
			} // if else

			if (!bSuccess) {
				if (ecRetry != cRetry::SkipAll) {
					QString qsInformation;

					if (qfilSources.at(iI).isDir()) {
						qsInformation = tr("Can't remove following directory:");
					} else {
						qsInformation = tr("Can't delete following file:");
					} // if else

					emit ShowRetryDialog(qsInformation, qfilSources.at(iI).filePath());
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

		emit SetTotalValue(qi64TotalMaximum - iI);
	} // for

	// close dialog or widget
	if (cddDialog) {
		cddDialog->deleteLater();
	} else {
		cdwWidget->deleteLater();
	} // if else
} // run
