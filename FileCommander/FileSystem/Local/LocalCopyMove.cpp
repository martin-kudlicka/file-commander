#include "FileSystem/Local/LocalCopyMove.h"

#ifdef Q_WS_WIN
#include <windows.h>
#endif
#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include "FileSystem/Local/LocalCommon.h"

// constructor
cLocalCopyMove::cLocalCopyMove(QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings)
{
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
	this->csSettings = csSettings;

	bCanceled = false;
	qi64TotalMaximum = 0;
	iBufferSize = csSettings->GetCopyMoveBufferSize() * 1024;
	cpPermission = new cPermission(qmwParent);
	crRename = new cRename(qmwParent);
} // cLocalCopyMove

// copy file
const bool cLocalCopyMove::Copy(const QString &qsSource, const QString &qsDestination, qint64 *qi64TotalValue)
{
	QByteArray qbaData;
	QFile qfDestination, qfSource;
	qint64 qi64CurrentValue;

	qfSource.setFileName(qsSource);
	qfDestination.setFileName(qsDestination);
	if (!qfSource.open(QIODevice::ReadOnly)) {
		return false;
	} // if
	if (ecConflictCurrent == cCopyMoveConflict::Append) {
		if (!qfDestination.open(QIODevice::Append)) {
			return false;
		} // if
	} else {
		if (!qfDestination.open(QIODevice::WriteOnly)) {
			return false;
		} // if
	} // if else

	// set progress bar
	qi64CurrentMaximum = qfSource.size();
	emit SetCurrentMaximum(qi64CurrentMaximum);

	// copy
	qi64CurrentValue = 0;
	while(!qfSource.atEnd() && !bCanceled) {
		qbaData = qfSource.read(iBufferSize);
		qfDestination.write(qbaData);
		qi64CurrentValue += qbaData.size();
		*qi64TotalValue += qbaData.size();
		emit SetCurrentValue(qi64CurrentValue);
		emit SetTotalValue(*qi64TotalValue);
		QApplication::processEvents();
	} // while

	return true;
} // Copy

// start of copy or move operation
const void cLocalCopyMove::CopyMove(const cFileOperationDialog::eOperation &eoOperation, const QStringList &qslSources, const QString &qsFilter, const QString &qsDestination, const cFileOperation::eOperationPosition &eopPosition)
{
	this->eoOperation = eoOperation;
	this->qslSources = qslSources;
	this->qsFilter = qsFilter;
	this->qsDestination = qsDestination;

	// information windows
	if (eopPosition == cFileOperation::ForegroundOperation) {
		ccmdDialog = new cCopyMoveDialog(qmwParent);
		switch (eoOperation) {
			case cFileOperationDialog::CopyOperation:
				ccmdDialog->setWindowTitle(tr("Copy"));
				break;
			case cFileOperationDialog::MoveOperation:
				ccmdDialog->setWindowTitle(tr("Move"));
			default:
				;
		} // switch
		ccmdDialog->setModal(true);
		ccmdDialog->show();
		connect(this, SIGNAL(SetCurrentMaximum(const qint64 &)), ccmdDialog, SLOT(on_cCopyMove_SetCurrentMaximum(const qint64 &)));
		connect(this, SIGNAL(SetCurrentValue(const qint64 &)), ccmdDialog, SLOT(on_cCopyMove_SetCurrentValue(const qint64 &)));
		connect(this, SIGNAL(SetDestination(const QString &)), ccmdDialog, SLOT(on_cCopyMove_SetDestination(const QString &)));
		connect(this, SIGNAL(SetSource(const QString &)), ccmdDialog, SLOT(on_cCopyMove_SetSource(const QString &)));
		connect(this, SIGNAL(SetTotalMaximum(const qint64 &)), ccmdDialog, SLOT(on_cCopyMove_SetTotalMaximum(const qint64 &)));
		connect(this, SIGNAL(SetTotalValue(const qint64 &)), ccmdDialog, SLOT(on_cCopyMove_SetTotalValue(const qint64 &)));
		connect(ccmdDialog, SIGNAL(Cancel()), SLOT(on_cLocalCopyMove_OperationCanceled()));
		connect(ccmdDialog, SIGNAL(Background()), SLOT(on_ccmdCopyMoveDialog_Background()));
		ccmwWidget = NULL;
	} else {
		CreateWidget();
		ccmdDialog = NULL;
	} // if else

	// conflict dialog
	connect(&ccmcConflict, SIGNAL(Finished(const cCopyMoveConflict::eChoice &)), SLOT(on_ccmcConflict_Finished(const cCopyMoveConflict::eChoice &)));

	// rename dialog
	connect(crRename, SIGNAL(Finished()), SLOT(on_crRename_Finished()));

#ifdef Q_WS_WIN
	// permission dialog
	connect(cpPermission, SIGNAL(Finished(const cPermission::eChoice &)), SLOT(on_cpPermission_Finished(const cPermission::eChoice &)));
#endif

	// retry dialog
	connect(&crRetry, SIGNAL(Finished(const cRetry::eChoice &)), SLOT(on_crRetry_Finished(const cRetry::eChoice &)));

	// disk space dialog
	connect(&cdsDiskSpace, SIGNAL(Finished(const cDiskSpace::eChoice &)), SLOT(on_cdsDiskSpace_Finished(const cDiskSpace::eChoice &)));

	start();
} // CopyMove

// create widget for background operation
const void cLocalCopyMove::CreateWidget()
{
	ccmwWidget = new cCopyMoveWidget();
	qhblOperations->insertWidget(cFileOperation::iQUEUED_OPERATION_POSITION, ccmwWidget);
	connect(this, SIGNAL(SetCurrentMaximum(const qint64 &)), ccmwWidget, SLOT(on_cCopyMove_SetCurrentMaximum(const qint64 &)));
	connect(this, SIGNAL(SetCurrentValue(const qint64 &)), ccmwWidget, SLOT(on_cCopyMove_SetCurrentValue(const qint64 &)));
	connect(this, SIGNAL(SetDestination(const QString &)), ccmwWidget, SLOT(on_cCopyMove_SetDestination(const QString &)));
	connect(this, SIGNAL(SetSource(const QString &)), ccmwWidget, SLOT(on_cCopyMove_SetSource(const QString &)));
	connect(this, SIGNAL(SetTotalMaximum(const qint64 &)), ccmwWidget, SLOT(on_cCopyMove_SetTotalMaximum(const qint64 &)));
	connect(this, SIGNAL(SetTotalValue(const qint64 &)), ccmwWidget, SLOT(on_cCopyMove_SetTotalValue(const qint64 &)));
	connect(ccmwWidget, SIGNAL(Cancel()), SLOT(on_cLocalCopyMove_OperationCanceled()));
} // CreateWidget

// conflict dialog closed with user response
const void cLocalCopyMove::on_ccmcConflict_Finished(const cCopyMoveConflict::eChoice &ecResponse)
{
	ecConflictCurrent = ecResponse;
	qsPause.release();
} // on_ccmcConflict_Finished

// move operation to background
const void cLocalCopyMove::on_ccmdCopyMoveDialog_Background()
{
	CreateWidget();
	emit SetSource(qsSource);
	emit SetDestination(qsTarget);
	emit SetCurrentMaximum(qi64CurrentMaximum);
	emit SetTotalMaximum(qi64TotalMaximum);
	ccmdDialog->deleteLater();
	ccmdDialog = NULL;
} // on_ccmdCopyMoveDialog_Background

// disk space dialog closed with user response
const void cLocalCopyMove::on_cdsDiskSpace_Finished(const cDiskSpace::eChoice &ecResponse)
{
	ecDiskSpaceCurrent = ecResponse;
	qsPause.release();
} // on_cdsDiskSpace_Finished

// copy or move operation was canceled
const void cLocalCopyMove::on_cLocalCopyMove_OperationCanceled()
{
	bCanceled = true;
} // on_cLocalCopyMove_OperationCanceled

#ifdef Q_WS_WIN
// permission dialog closed with user response
const void cLocalCopyMove::on_cpPermission_Finished(const cPermission::eChoice &ecResponse)
{
	ecPermissionCurrent = ecResponse;
	qsPause.release();
} // on_cpPermission_Finished
#endif

// rename dialog closed with user's reponse
const void cLocalCopyMove::on_crRename_Finished()
{
	qsPause.release();
} // on_crRename_Finished

// retry dialog closed with user response
const void cLocalCopyMove::on_crRetry_Finished(const cRetry::eChoice &ecResponse)
{
	ecRetryCurrent = ecResponse;
	qsPause.release();
} // on_crRetry_Finished

// thread code
void cLocalCopyMove::run()
{
	cCopyMoveConflict::eChoice ecConflict;
	cDiskSpace::eChoice ecDiskSpace;
#ifdef Q_WS_WIN
	cPermission::eChoice ecPermission;
#endif
	cRetry::eChoice ecRetry;
	int iI;
	QDir qdDir;
	QFileInfoList qfilSources;
	qint64 qi64TotalValue;
	QString qsOverwrite;
	QStringList qslSourcePaths;

	// get default source paths
	for (iI = 0; iI < qslSources.count(); iI++) {
		qslSourcePaths.append(QFileInfo(qslSources.at(iI)).path());
	} // for

	// gather source files
	qfilSources = cLocalCommon::GetFiles(qslSources, qsFilter);

	// prepare progress bar
	for (iI = 0; iI < qfilSources.count(); iI++) {
		qi64TotalMaximum += qfilSources.at(iI).size();
	} // for
	emit SetTotalMaximum(qi64TotalMaximum);

	// get default modes
	ecConflict = cFileOperation::GetDefaultOverwriteMode(csSettings);
#ifdef Q_WS_WIN
	ecPermission = cFileOperation::GetDefaultReadonlyOverwritePermission(csSettings);
#endif
	ecRetry = cRetry::Ask;
	ecDiskSpace = cDiskSpace::Ask;

	// main process
	qdDir.setCurrent(qfilSources.at(0).path());
	qi64TotalValue = 0;
	for (iI = 0; iI < qfilSources.count() && !bCanceled; iI++) {
		const QFileInfo *qfiSource;
		int iJ;
		QString qsSourcePath;

		qfiSource = &qfilSources.at(iI);

		// find proper source path
		for (iJ = 0; iJ < qslSourcePaths.count(); iJ++) {
			const QString *qsPath;

			qsPath = &qslSourcePaths.at(iJ);
			if (qfiSource->path().startsWith(*qsPath)) {
				qsSourcePath = *qsPath;
				break;
			} // if
		} // for

		// show file names
		qsSource = qfiSource->filePath();
		qsTarget = cFileOperation::GetWildcardedName(*qfiSource, qsSourcePath, qsDestination);
		if (ccmdDialog) {
			// name with path in dialog
			emit SetSource(qsSource);
			emit SetDestination(qsTarget);
		} else {
			// just name in widget
			emit SetSource(qfiSource->fileName());
			emit SetDestination(QFileInfo(qsTarget).fileName());
		} // if else

		if (qfiSource->isDir()) {
			qdDir.mkpath(qsTarget);
			if (eoOperation == cFileOperationDialog::MoveOperation) {
				qdDir.rmdir(qsSource);
			} // if
		} else {
#ifdef Q_WS_WIN
			DWORD dwAttributes;
#else
			QFile::Permissions pPermissions;
#endif
			cFileOperation::eCheckResult ecrCheck;
			emit SetCurrentValue(0);

			// check disk space on target
			ecrCheck = cFileOperation::CheckDiskSpace(&cdsDiskSpace, qsSource, qsTarget, qfiSource->size(), &ecDiskSpace, &ecDiskSpaceCurrent, &qsPause);
			if (ecrCheck == cFileOperation::NextFile) {
				qi64TotalValue += qfiSource->size();
				continue;
			} else {
				if (ecrCheck == cFileOperation::Cancel) {
					break;
				} // if
			} // if else

			// conflict solving
			ecrCheck = cFileOperation::CheckConflict(eoOperation, &ccmcConflict, crRename, qfiSource->fileName(), qfiSource->size(), qfiSource->lastModified(), qsTarget, &ecConflict, &ecConflictCurrent, &qsPause);
			if (ecrCheck == cFileOperation::NextFile) {
				qi64TotalValue += qfiSource->size();
				continue;
			} else {
				if (ecrCheck == cFileOperation::Cancel) {
					break;
				} // if
			} // if else

			// create destination path
			qdDir.mkpath(QFileInfo(qsTarget).path());

			// memorize source file attributes
#ifdef Q_WS_WIN
			dwAttributes = GetFileAttributes(reinterpret_cast<LPCWSTR>(qfiSource->filePath().unicode()));
#else
			pPermissions = QFile::permissions(qfiSource->filePath());
#endif

#ifdef Q_WS_WIN
			// check readonly permission
			ecrCheck = cFileOperation::CheckReadOnlyAttribute(cpPermission, qsTarget, &ecPermission, &ecPermissionCurrent, &qsPause);
			if (ecrCheck == cFileOperation::NextFile) {
				qi64TotalValue += qfiSource->size();
				continue;
			} else {
				if (ecrCheck == cFileOperation::Cancel) {
					break;
				} // if
			} // if else
#endif

			// copy/move file
			while (true) {
				bool bCopyMoveSuccess;

				switch (eoOperation) {
					case cFileOperationDialog::CopyOperation:
						bCopyMoveSuccess = Copy(qfiSource->filePath(), qsTarget, &qi64TotalValue);
						if (bCanceled) {
							// delete unfinished file
							QFile::remove(qsTarget);
						} else {
#ifdef Q_WS_WIN
							FILETIME ftCreated, ftLastModified;
							HANDLE hDestination;
							SYSTEMTIME stCreated, stLastModified;
#endif
							// set target permissions as source permissions
#ifdef Q_WS_WIN
							SetFileAttributes(reinterpret_cast<LPCWSTR>(qsTarget.unicode()), dwAttributes | FILE_ATTRIBUTE_ARCHIVE);
#else
							QFile::setPermissions(qsTarget, pPermissions);
#endif

							// set last modified date/time as source file
#ifdef Q_WS_WIN
							stCreated.wYear = qfiSource->created().toUTC().date().year();
							stCreated.wMonth = qfiSource->created().toUTC().date().month();
							stCreated.wDayOfWeek = qfiSource->created().toUTC().date().dayOfWeek();
							stCreated.wDay = qfiSource->created().toUTC().date().day();
							stCreated.wHour = qfiSource->created().toUTC().time().hour();
							stCreated.wMinute = qfiSource->created().toUTC().time().minute();
							stCreated.wSecond = qfiSource->created().toUTC().time().second();
							stCreated.wMilliseconds = qfiSource->created().toUTC().time().msec();
							stLastModified.wYear = qfiSource->lastModified().toUTC().date().year();
							stLastModified.wMonth = qfiSource->lastModified().toUTC().date().month();
							stLastModified.wDayOfWeek = qfiSource->lastModified().toUTC().date().dayOfWeek();
							stLastModified.wDay = qfiSource->lastModified().toUTC().date().day();
							stLastModified.wHour = qfiSource->lastModified().toUTC().time().hour();
							stLastModified.wMinute = qfiSource->lastModified().toUTC().time().minute();
							stLastModified.wSecond = qfiSource->lastModified().toUTC().time().second();
							stLastModified.wMilliseconds = qfiSource->lastModified().toUTC().time().msec();

							SystemTimeToFileTime(&stCreated, &ftCreated);
							SystemTimeToFileTime(&stLastModified, &ftLastModified);
							hDestination = CreateFile(reinterpret_cast<LPCWSTR>(qsTarget.unicode()), FILE_WRITE_ATTRIBUTES, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
							SetFileTime(hDestination, &ftCreated, NULL, &ftLastModified);
							CloseHandle(hDestination);
#endif
						} // if else
						break;
					case cFileOperationDialog::MoveOperation:
						if (ecConflictCurrent == cCopyMoveConflict::Append) {
							bCopyMoveSuccess = Copy(qfiSource->filePath(), qsTarget, &qi64TotalValue);
							if (bCopyMoveSuccess) {
								QFile::remove(qfiSource->filePath());
							} // if
						} else {
							bCopyMoveSuccess = QFile::rename(qfiSource->filePath(), qsTarget);
							if (bCopyMoveSuccess) {
								qi64TotalValue += qfiSource->size();
								emit SetTotalValue(qi64TotalValue);
							} // if
						} // if else
						if (qsSourcePath != qfiSource->path()) {
							qdDir.rmdir(qfiSource->path());
						} // if
						break;
					default:
						bCopyMoveSuccess = false;
				} // switch

				if (!bCopyMoveSuccess) {
					ecrCheck = cFileOperation::CheckRetry(&crRetry, eoOperation, *qfiSource, &ecRetry, &ecRetryCurrent, &qsPause);
					if (ecrCheck == cFileOperation::NextFile) {
						qi64TotalValue += qfiSource->size();
						emit SetTotalValue(qi64TotalValue);
						break;
					} // if
					// else try once more
				} else {
					// successfuly copied/moved
					break;
				} // if else
			} // while

			if (ecRetryCurrent == cRetry::Abort) {
				// process aborted
				break;
			} // if
		} // if else
	} // for

	// remove source directory when moving files
	if (eoOperation == cFileOperationDialog::MoveOperation) {
		qdDir.rmdir(qfilSources.at(0).filePath());
	} // if

	// close dialog or widget
	if (ccmdDialog) {
		ccmdDialog->deleteLater();
	} else {
		ccmwWidget->deleteLater();
	} // if else
	cpPermission->deleteLater();
	crRename->deleteLater();
} // run
