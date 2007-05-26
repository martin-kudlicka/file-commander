#include "FileOperation/CopyMove.h"

#ifdef Q_WS_WIN
#include <windows.h>
#endif
#include <QtCore/QDateTime>

// constructor
cCopyMove::cCopyMove(QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings)
{
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
	this->csSettings = csSettings;

	bCanceled = false;
	qi64TotalMaximum = 0;
	iBufferSize = csSettings->GetCopyMoveBufferSize() * 1024;
} // cCopyMove

// copy file
bool cCopyMove::Copy(const QString &qsSource, const QString &qsDestination, qint64 *qi64TotalValue)
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
void cCopyMove::CopyMove(const cFileRoutine::eOperation &eoOperation, const QFileInfoList &qfilSource, const QString &qsDestination, const QString &qsFilter, const cFileRoutine::eWindow &eStyle)
{
	this->eoOperation = eoOperation;
	this->qfilSource = qfilSource;
	this->qsFilter = qsFilter;
	this->qsDestination = qsDestination;

	// information windows
	if (eStyle == cFileRoutine::ForegroundWindow) {
		ccmdDialog = new cCopyMoveDialog(qmwParent);
		switch (eoOperation) {
			case cFileRoutine::CopyOperation:
				ccmdDialog->setWindowTitle(tr("Copy"));
				break;
			case cFileRoutine::MoveOperation:
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
		connect(ccmdDialog, SIGNAL(Cancel()), SLOT(on_ccm_OperationCanceled()));
		connect(ccmdDialog, SIGNAL(Background()), SLOT(on_ccmdCopyMoveDialog_Background()));
		ccmwWidget = NULL;
	} else {
		CreateWidget();
		ccmdDialog = NULL;
	} // if else

	// conflict dialog
	connect(this, SIGNAL(ShowConflictDialog(const QString &, const QFileInfo &, const QFileInfo &)), &ccmcConflict, SLOT(Show(const QString &, const QFileInfo &, const QFileInfo &)));
	connect(&ccmcConflict, SIGNAL(Finished(const cCopyMoveConflict::eChoice &)), SLOT(on_ccmcConflict_Finished(const cCopyMoveConflict::eChoice &)));

	// rename dialog
	connect(this, SIGNAL(ShowRenameDialog(const QString &)), &crRename, SLOT(Show(const QString &)));
	connect(&crRename, SIGNAL(Finished(const QString &)), SLOT(on_crRename_Finished(const QString &)));

#ifdef Q_WS_WIN
	// permission dialog
	connect(this, SIGNAL(ShowPermissionDialog(const QString &, const QString &)), &cpPermission, SLOT(Show(const QString &, const QString &)));
	connect(&cpPermission, SIGNAL(Finished(const cPermission::eChoice &)), SLOT(on_cpPermission_Finished(const cPermission::eChoice &)));
#endif

	// retry dialog
	connect(this, SIGNAL(ShowRetryDialog(const QString &, const QString &)), &crRetry, SLOT(Show(const QString &, const QString &)));
	connect(&crRetry, SIGNAL(Finished(const cRetry::eChoice &)), SLOT(on_crRetry_Finished(const cRetry::eChoice &)));

	// disk space dialog
	connect(this, SIGNAL(ShowDiskSpaceDialog(const QString &, const qint64 &, const qint64 &)), &cdsDiskSpace, SLOT(Show(const QString &, const qint64 &, const qint64 &)));
	connect(&cdsDiskSpace, SIGNAL(Finished(const cDiskSpace::eChoice &)), SLOT(on_cdsDiskSpace_Finished(const cDiskSpace::eChoice &)));

	start();
} // CopyMove

// create widget for background operation
void cCopyMove::CreateWidget()
{
	ccmwWidget = new cCopyMoveWidget();
	qhblOperations->insertWidget(cFileRoutine::iQUEUED_OPERATION_POSITION, ccmwWidget);
	connect(this, SIGNAL(SetCurrentMaximum(const qint64 &)), ccmwWidget, SLOT(on_cCopyMove_SetCurrentMaximum(const qint64 &)));
	connect(this, SIGNAL(SetCurrentValue(const qint64 &)), ccmwWidget, SLOT(on_cCopyMove_SetCurrentValue(const qint64 &)));
	connect(this, SIGNAL(SetDestination(const QString &)), ccmwWidget, SLOT(on_cCopyMove_SetDestination(const QString &)));
	connect(this, SIGNAL(SetSource(const QString &)), ccmwWidget, SLOT(on_cCopyMove_SetSource(const QString &)));
	connect(this, SIGNAL(SetTotalMaximum(const qint64 &)), ccmwWidget, SLOT(on_cCopyMove_SetTotalMaximum(const qint64 &)));
	connect(this, SIGNAL(SetTotalValue(const qint64 &)), ccmwWidget, SLOT(on_cCopyMove_SetTotalValue(const qint64 &)));
	connect(ccmwWidget, SIGNAL(Cancel()), SLOT(on_ccm_OperationCanceled()));
} // CreateWidget

// return file name modified by wildcard
QString cCopyMove::GetWildcardedName(const QFileInfo &qfiFile, const QString &qsSourcePath, const QString &qsDestination)
{
	int iI;
	QString qsNewFilename;
	QStringList qslOutputs, qslPatterns, qslStrings;

	if (!qsDestination.contains('*') && !qsDestination.contains('?')) {
		// no wildcard in file name
		return qsDestination;
	} // if

	// preparation
	qslPatterns.append(QFileInfo(qsDestination).completeBaseName());
	qslPatterns.append(QFileInfo(qsDestination).suffix());
	qslStrings.append(qfiFile.completeBaseName());
	qslStrings.append(qfiFile.suffix());
	qslOutputs.append("");
	qslOutputs.append("");

	// apply patterns on strings
	for (iI = 0; iI <= 1; iI++) {
		int iPatternPos, iStringPos;

		iStringPos = 0;
		for (iPatternPos = 0; iPatternPos < qslPatterns[iI].length(); iPatternPos++) {
			if (qslPatterns[iI].at(iPatternPos) == '*') {
				// copy rest of the source name
				int iJ;

				for (iJ = iStringPos; iJ < qslStrings[iI].length(); iJ++) {
					qslOutputs[iI] += qslStrings[iI].at(iStringPos);
					iStringPos++;
				} // for
			} else
				if (qslPatterns[iI].at(iPatternPos) == '?') {
					// copy one character of the source name
					if (iStringPos < qslStrings[iI].length()) {
						qslOutputs[iI] += qslStrings[iI].at(iStringPos);
						iStringPos++;
					} // if
				} else {
					// copy character from pattern
					qslOutputs[iI] += qslPatterns[iI].at(iPatternPos);
					iStringPos++;
				} // if else
		} // while
	} // for

	qsNewFilename = qfiFile.path() + '/' + qslOutputs[0] + '.' + qslOutputs[1];
	return QFileInfo(qsDestination).path() + '/' + qsNewFilename.mid(qsSourcePath.length());
} // GetWildcardedName

// copy or move operation was canceled
void cCopyMove::on_ccm_OperationCanceled()
{
	bCanceled = true;
} // on_ccm_OperationCanceled

// conflict dialog closed with user response
void cCopyMove::on_ccmcConflict_Finished(const cCopyMoveConflict::eChoice &ecResponse)
{
	ecConflictCurrent = ecResponse;
	qsPause.release();
} // on_ccmcConflict_Finished

// move operation to background
void cCopyMove::on_ccmdCopyMoveDialog_Background()
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
void cCopyMove::on_cdsDiskSpace_Finished(const cDiskSpace::eChoice &ecResponse)
{
	ecDiskSpaceCurrent = ecResponse;
	qsPause.release();
} // on_cdsDiskSpace_Finished

#ifdef Q_WS_WIN
// permission dialog closed with user response
void cCopyMove::on_cpPermission_Finished(const cPermission::eChoice &ecResponse)
{
	ecPermissionCurrent = ecResponse;
	qsPause.release();
} // on_cpPermission_Finished
#endif

// rename dialog closed with user's reponse
void cCopyMove::on_crRename_Finished(const QString &qsNewFilename)
{
	this->qsNewFilename = qsNewFilename;
	qsPause.release();
} // on_crRename_Finished

// retry dialog closed with user response
void cCopyMove::on_crRetry_Finished(const cRetry::eChoice &ecResponse)
{
	ecRetryCurrent = ecResponse;
	qsPause.release();
} // on_crRetry_Finished

// thread code
void cCopyMove::run()
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
	QString qsOverwrite, qsSourcePath;

	// gather source files and source path
	qfilSources = cFileRoutine::GetSources(qfilSource, true, qsFilter);
	qsSourcePath = qfilSource.at(0).path();

	// prepare progress bars
	for (iI = 0; iI < qfilSources.count(); iI++) {
		qi64TotalMaximum += qfilSources.at(iI).size();
	} // for
	emit SetTotalMaximum(qi64TotalMaximum);

	// get default overwrite mode
	qsOverwrite = csSettings->GetFileOverwrite();
	if (qsOverwrite == qsASK) {
		ecConflict = cCopyMoveConflict::Ask;
	} else {
		if (qsOverwrite == qsOVERWRITE_ALL) {
			ecConflict = cCopyMoveConflict::OverwriteAll;
		} else {
			if (qsOverwrite == qsOVERWRITE_ALL_OLDER) {
				ecConflict = cCopyMoveConflict::OverwriteAllOlder;
			} else {
				ecConflict = cCopyMoveConflict::SkipAll;
			} // if else
		} // if else
	} // if else

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
	ecDiskSpace = cDiskSpace::Ask;

	// main process
	qdDir.setCurrent(qsSourcePath);
	qi64TotalValue = 0;
	for (iI = 0; iI < qfilSources.count() && !bCanceled; iI++) {
		// show file names
		qsSource = qfilSources.at(iI).filePath();
		qsTarget = GetWildcardedName(qfilSources.at(iI).filePath(), qsSourcePath, qsDestination);
		if (ccmdDialog) {
			// name with path in dialog
			emit SetSource(qsSource);
			emit SetDestination(qsTarget);
		} else {
			// just name in widget
			emit SetSource(qfilSources.at(iI).fileName());
			emit SetDestination(QFileInfo(qsTarget).fileName());
		} // if else

		if (qfilSources.at(iI).isDir()) {
			qdDir.mkpath(QFileInfo(qsTarget).filePath());
			if (eoOperation == cFileRoutine::MoveOperation) {
				qdDir.rmdir(qfilSources.at(iI).filePath());
			} // if
		} else {
			cFileRoutine::sDiskSpace sdsDiskSpace;
#ifdef Q_WS_WIN
			DWORD dwAttributes;
#else
			QFile::Permissions pPermissions;
#endif
			emit SetCurrentValue(0);

			// check disk space on target
			sdsDiskSpace = cFileRoutine::GetDiskSpace(QFileInfo(qsTarget).path());
			if (sdsDiskSpace.qi64Free < qfilSources.at(iI).size()) {
				ecDiskSpaceCurrent = cDiskSpace::Ask;

				if (ecDiskSpace == cDiskSpace::Ask) {
					// disk space dialog
					emit ShowDiskSpaceDialog(qsSource, qfilSources.at(iI).size(), sdsDiskSpace.qi64Free);
					// wait for answer
					qsPause.acquire();

					// solve
					switch (ecDiskSpaceCurrent) {
						case cDiskSpace::YesToAll:
							ecDiskSpace = cDiskSpace::YesToAll;
							break;
						case cDiskSpace::SkipAll:
							ecDiskSpace = cDiskSpace::SkipAll;
						default:
							;
					} // switch
				} // if

				if (ecDiskSpace == cDiskSpace::SkipAll || ecDiskSpaceCurrent == cDiskSpace::Skip) {
					// skip current file
					qi64TotalValue += qfilSources.at(iI).size();
					continue;
				} else {
					if (ecDiskSpaceCurrent == cDiskSpace::No) {
						// cancel
						break;
					} // if
				} // if else
			} // if

			// conflict solving
			if (QFile::exists(qsTarget)) {
				ecConflictCurrent = cCopyMoveConflict::Ask;

				if (ecConflict == cCopyMoveConflict::Ask) {
					while (true) {
						// no permanent conflict answer yet
						QString qsOperation;

						// conflict dialog
						if (eoOperation == cFileRoutine::CopyOperation) {
							qsOperation = tr("Copy");
						} else {
							qsOperation = tr("Move");
						} // if else
						emit ShowConflictDialog(qsOperation, QFileInfo(qsSource), QFileInfo(qsTarget));
						// wait for answer
						qsPause.acquire();
						// solve conflict
						switch (ecConflictCurrent) {
							case cCopyMoveConflict::SkipAll:
								ecConflict = cCopyMoveConflict::SkipAll;
								break;
							case cCopyMoveConflict::OverwriteAll:
								ecConflict = cCopyMoveConflict::OverwriteAll;
								break;
							case cCopyMoveConflict::OverwriteAllOlder:
								ecConflict = cCopyMoveConflict::OverwriteAllOlder;
							default:
								;
						} // switch

						// rename dialog
						if (ecConflictCurrent == cCopyMoveConflict::Rename) {
							// rename
							emit ShowRenameDialog(QFileInfo(qsTarget).fileName());
							// wait for answer
							qsPause.acquire();
							if (!qsNewFilename.isEmpty()) {
								// new file name typed
								qsTarget = QFileInfo(qsTarget).path() + '/' + qsNewFilename;
								if (!QFile::exists(qsTarget)) {
									// rename ok, continue
									break;
								} // if
							} else {
								// cancel
								ecConflictCurrent = cCopyMoveConflict::Cancel;
								break;
							} // if else
						} else {
							// no rename
							break;
						} // if else
					} // while
					if (ecConflictCurrent == cCopyMoveConflict::Cancel) {
						// cancel
						break;
					} // if
				} // if

				if (ecConflict == cCopyMoveConflict::SkipAll || ecConflictCurrent == cCopyMoveConflict::Skip) {
					// skip or skip all -> move onto next file
					qi64TotalValue += qfilSources.at(iI).size();
					continue;
				} else {
					if (ecConflict == cCopyMoveConflict::OverwriteAll || ecConflictCurrent == cCopyMoveConflict::Overwrite) {
						// overwrite, overwrite all -> delete target file
						QFile::remove(qsTarget);
					} else {
						if (ecConflict == cCopyMoveConflict::OverwriteAllOlder) {
							// overwrite all older
							if (QFileInfo(qsSource).lastModified() > QFileInfo(qsTarget).lastModified()) {
								// target file is older -> delete it
								QFile::remove(qsTarget);
							} else {
								// target file is newer -> move onto next file
								qi64TotalValue += qfilSources.at(iI).size();
								continue;
							} // if else
						} // if
					} // if else
				} // if else
			} // if

			// create destination path
			qdDir.mkpath(QFileInfo(qsTarget).path());

			// memorize source file attributes
#ifdef Q_WS_WIN
			dwAttributes = GetFileAttributes(reinterpret_cast<LPCWSTR>(qfilSources.at(iI).filePath().unicode()));
#else
			pPermissions = QFile::permissions(qfilSources.at(iI).filePath());
#endif

#ifdef Q_WS_WIN
			// check readonly permission
			ecPermissionCurrent = cPermission::Ask;
			if (QFile::permissions(qsTarget) & QFile::ReadOther) {
				if (ecPermission == cPermission::Ask) {
					emit ShowPermissionDialog(QFile(qsTarget).fileName(), tr("is readonly."));
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
					qi64TotalValue += qfilSources.at(iI).size();
					continue;
				} else {
					// remove target file readonly permission
					SetFileAttributes(reinterpret_cast<LPCWSTR>(qsTarget.unicode()), dwAttributes & ~FILE_ATTRIBUTE_READONLY);
				} // if else
			} // if
#endif

			// copy/move file
			while (true) {
				bool bCopyMoveSuccess;

				switch (eoOperation) {
					case cFileRoutine::CopyOperation:
						bCopyMoveSuccess = Copy(qfilSources.at(iI).filePath(), qsTarget, &qi64TotalValue);
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
							stCreated.wYear = qfilSources.at(iI).created().toUTC().date().year();
							stCreated.wMonth = qfilSources.at(iI).created().toUTC().date().month();
							stCreated.wDayOfWeek = qfilSources.at(iI).created().toUTC().date().dayOfWeek();
							stCreated.wDay = qfilSources.at(iI).created().toUTC().date().day();
							stCreated.wHour = qfilSources.at(iI).created().toUTC().time().hour();
							stCreated.wMinute = qfilSources.at(iI).created().toUTC().time().minute();
							stCreated.wSecond = qfilSources.at(iI).created().toUTC().time().second();
							stCreated.wMilliseconds = qfilSources.at(iI).created().toUTC().time().msec();
							stLastModified.wYear = qfilSources.at(iI).lastModified().toUTC().date().year();
							stLastModified.wMonth = qfilSources.at(iI).lastModified().toUTC().date().month();
							stLastModified.wDayOfWeek = qfilSources.at(iI).lastModified().toUTC().date().dayOfWeek();
							stLastModified.wDay = qfilSources.at(iI).lastModified().toUTC().date().day();
							stLastModified.wHour = qfilSources.at(iI).lastModified().toUTC().time().hour();
							stLastModified.wMinute = qfilSources.at(iI).lastModified().toUTC().time().minute();
							stLastModified.wSecond = qfilSources.at(iI).lastModified().toUTC().time().second();
							stLastModified.wMilliseconds = qfilSources.at(iI).lastModified().toUTC().time().msec();

							SystemTimeToFileTime(&stCreated, &ftCreated);
							SystemTimeToFileTime(&stLastModified, &ftLastModified);
							hDestination = CreateFile(reinterpret_cast<LPCWSTR>(qsTarget.unicode()), FILE_WRITE_ATTRIBUTES, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
							SetFileTime(hDestination, &ftCreated, NULL, &ftLastModified);
							CloseHandle(hDestination);
#endif
						} // if else
						break;
					case cFileRoutine::MoveOperation:
						if (ecConflictCurrent == cCopyMoveConflict::Append) {
							bCopyMoveSuccess = Copy(qfilSources.at(iI).filePath(), qsTarget, &qi64TotalValue);
							if (bCopyMoveSuccess) {
								QFile::remove(qfilSources.at(iI).filePath());
							} // if
						} else {
							bCopyMoveSuccess = QFile::rename(qfilSources.at(iI).filePath(), qsTarget);
							if (bCopyMoveSuccess) {
								qi64TotalValue += qfilSources.at(iI).size();
								emit SetTotalValue(qi64TotalValue);
							} // if
						} // if else
						if (qsSourcePath != qfilSources.at(iI).path()) {
							qdDir.rmdir(qfilSources.at(iI).path());
						} // if
					default:
						;
				} // switch

				if (!bCopyMoveSuccess) {
					if (ecRetry != cRetry::SkipAll) {
						QString qsInformation;

						if (eoOperation == cFileRoutine::CopyOperation) {
							qsInformation = tr("Can't copy following file:");
						} else {
							qsInformation = tr("Can't move following file:");
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
						qi64TotalValue += qfilSources.at(iI).size();
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
	if (eoOperation == cFileRoutine::MoveOperation) {
		qdDir.rmdir(qfilSource.at(0).filePath());
	} // if

	// close dialog or widget
	if (ccmdDialog) {
		ccmdDialog->deleteLater();
	} else {
		ccmwWidget->deleteLater();
	} // if else
} // run
