#include "FileSystem/Local/LocalCopyMove.h"

#ifdef Q_WS_WIN
#include <windows.h>
#endif
#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include "FileSystem/Local/LocalCommon.h"
#include "FileSystem.h"

// constructor
cLocalCopyMove::cLocalCopyMove(QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings)
{
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
	this->csSettings = csSettings;

	bCanceled = false;
	qi64TotalMaximum = 0;
	iBufferSize = csSettings->GetCopyMoveBufferSize() * 1024;
} // cLocalCopyMove

// check existing destination file conflict
const cFileOperation::eCheckResult cLocalCopyMove::CheckConflict(const QFileInfo &qfiSource, cCopyMoveConflict::eChoice *ecConflict, qint64 *qi64TotalValue)
{
	if (QFile::exists(qsTarget)) {
		ecConflictCurrent = cCopyMoveConflict::Ask;

		if (*ecConflict == cCopyMoveConflict::Ask) {
			while (true) {
				// no permanent conflict answer yet
				QString qsOperation;

				// conflict dialog
				if (eoOperation == cFileOperationDialog::CopyOperation) {
					qsOperation = tr("Copy");
				} else {
					qsOperation = tr("Move");
				} // if else
				emit ShowConflictDialog(qsOperation, qfiSource, QFileInfo(qsTarget));
				// wait for answer
				qsPause.acquire();
				// solve conflict
				switch (ecConflictCurrent) {
					case cCopyMoveConflict::SkipAll:
						*ecConflict = cCopyMoveConflict::SkipAll;
						break;
					case cCopyMoveConflict::OverwriteAll:
						*ecConflict = cCopyMoveConflict::OverwriteAll;
						break;
					case cCopyMoveConflict::OverwriteAllOlder:
						*ecConflict = cCopyMoveConflict::OverwriteAllOlder;
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
				return cFileOperation::Cancel;
			} // if
		} // if

		if (*ecConflict == cCopyMoveConflict::SkipAll || ecConflictCurrent == cCopyMoveConflict::Skip) {
			// skip or skip all -> move onto next file
			*qi64TotalValue += qfiSource.size();
			return cFileOperation::NextFile;
		} else {
			if (*ecConflict == cCopyMoveConflict::OverwriteAll || ecConflictCurrent == cCopyMoveConflict::Overwrite) {
				// overwrite, overwrite all -> delete target file
				QFile::remove(qsTarget);
			} else {
				if (*ecConflict == cCopyMoveConflict::OverwriteAllOlder) {
					// overwrite all older
					if (QFileInfo(qsSource).lastModified() > QFileInfo(qsTarget).lastModified()) {
						// target file is older -> delete it
						QFile::remove(qsTarget);
					} else {
						// target file is newer -> move onto next file
						*qi64TotalValue += qfiSource.size();
						return cFileOperation::NextFile;
					} // if else
				} // if
			} // if else
		} // if else
	} // if

	return cFileOperation::Nothing;
} // CheckConflict

// check disk space
const cFileOperation::eCheckResult cLocalCopyMove::CheckDiskSpace(const qint64 &qi64SourceSize, cDiskSpace::eChoice *ecDiskSpace, qint64 *qi64TotalValue)
{
	cFileSystem::sDiskSpace sdsDiskSpace;

	sdsDiskSpace = cLocalCommon::GetDiskSpace(QFileInfo(qsTarget).path());
	if (sdsDiskSpace.qi64Free < qi64SourceSize) {
		ecDiskSpaceCurrent = cDiskSpace::Ask;

		if (*ecDiskSpace == cDiskSpace::Ask) {
			// disk space dialog
			emit ShowDiskSpaceDialog(qsSource, qi64SourceSize, sdsDiskSpace.qi64Free);
			// wait for answer
			qsPause.acquire();

			// solve
			switch (ecDiskSpaceCurrent) {
				case cDiskSpace::YesToAll:
					*ecDiskSpace = cDiskSpace::YesToAll;
					break;
				case cDiskSpace::SkipAll:
					*ecDiskSpace = cDiskSpace::SkipAll;
				default:
					;
			} // switch
		} // if

		if (*ecDiskSpace == cDiskSpace::SkipAll || ecDiskSpaceCurrent == cDiskSpace::Skip) {
			// skip current file
			*qi64TotalValue += qi64SourceSize;
			return cFileOperation::NextFile;
		} else {
			if (ecDiskSpaceCurrent == cDiskSpace::No) {
				// cancel
				return cFileOperation::Cancel;
			} // if
		} // if else
	} // if

	return cFileOperation::Nothing;
} // CheckDiskSpace

#ifdef Q_WS_WIN
// check target file permission
const cFileOperation::eCheckResult cLocalCopyMove::CheckPermission(const qint64 &qi64SourceSize, cPermission::eChoice *ecPermission, qint64 *qi64TotalValue)
{
	ecPermissionCurrent = cPermission::Ask;
	if (QFile::permissions(qsTarget) & QFile::ReadOther) {
		if (*ecPermission == cPermission::Ask) {
			// show permission dialog
			emit ShowPermissionDialog(QFile(qsTarget).fileName(), tr("is readonly."));

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
			*qi64TotalValue += qi64SourceSize;
			return cFileOperation::NextFile;
		} else {
			// remove target file readonly permission
			SetFileAttributes(reinterpret_cast<LPCWSTR>(qsTarget.unicode()), GetFileAttributes(reinterpret_cast<LPCWSTR>(qsTarget.unicode())) & ~FILE_ATTRIBUTE_READONLY);
		} // if else
	} // if

	return cFileOperation::Nothing;
} // CheckPermission
#endif

// retry if copy/move unsuccesfull
const cFileOperation::eCheckResult cLocalCopyMove::CheckRetry(const QFileInfo &qfiSource, cRetry::eChoice *ecRetry, qint64 *qi64TotalValue)
{
	if (*ecRetry != cRetry::SkipAll) {
		QString qsInformation;

		if (eoOperation == cFileOperationDialog::CopyOperation) {
			qsInformation = tr("Can't copy following file:");
		} else {
			qsInformation = tr("Can't move following file:");
		} // if else

		emit ShowRetryDialog(qsInformation, qfiSource.filePath());
		// wait for answer
		qsPause.acquire();

		if (ecRetryCurrent == cRetry::SkipAll) {
			// memorize permanent answer
			*ecRetry = cRetry::SkipAll;
		} // if
	} // if
	if (*ecRetry == cRetry::SkipAll || ecRetryCurrent == cRetry::Skip || ecRetryCurrent == cRetry::Abort) {
		// skip this file
		*qi64TotalValue += qfiSource.size();
		emit SetTotalValue(*qi64TotalValue);
		return cFileOperation::NextFile;
	} // if

	return cFileOperation::Nothing;
} // CheckRetry

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
const void cLocalCopyMove::CopyMove(const cFileOperationDialog::eOperation &eoOperation, const QStringList &qslSources, const QString &qsDestination, const QString &qsFilter, const cFileOperation::eOperationPosition &eopPosition)
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

// default overwrite mode from settings file
const cCopyMoveConflict::eChoice cLocalCopyMove::GetDefaultOverwriteMode() const
{
	cCopyMoveConflict::eChoice ecConflict;
	QString qsOverwrite;

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

	return ecConflict;
} // GetDefaultOverwriteMode

// return file name modified by wildcard
const QString cLocalCopyMove::GetWildcardedName(const QFileInfo &qfiFile, const QString &qsSourcePath, const QString &qsDestination) const
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
		const QString *qsPattern;

		iStringPos = 0;
		qsPattern = &qslPatterns.at(iI);
		for (iPatternPos = 0; iPatternPos < qsPattern->length(); iPatternPos++) {
			const QString *qsString;

			qsString = &qslStrings.at(iI);
			if (qsPattern->at(iPatternPos) == '*') {
				// copy rest of the source name
				int iJ;

				for (iJ = iStringPos; iJ < qsString->length(); iJ++) {
					qslOutputs[iI] += qsString->at(iStringPos);
					iStringPos++;
				} // for
			} else
				if (qsPattern->at(iPatternPos) == '?') {
					// copy one character of the source name
					if (iStringPos < qsString->length()) {
						qslOutputs[iI] += qsString->at(iStringPos);
						iStringPos++;
					} // if
				} else {
					// copy character from pattern
					qslOutputs[iI] += qsPattern->at(iPatternPos);
					iStringPos++;
				} // if else
		} // while
	} // for

	qsNewFilename = qfiFile.path() + '/' + qslOutputs.at(0) + '.' + qslOutputs.at(1);
	return QDir::cleanPath(QFileInfo(qsDestination).path() + '/' + qsNewFilename.mid(qsSourcePath.length()));
} // GetWildcardedName

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
const void cLocalCopyMove::on_crRename_Finished(const QString &qsNewFilename)
{
	this->qsNewFilename = qsNewFilename;
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
	QString qsOverwrite, qsSourcePath;

	// gather source files and source path
	qfilSources = cLocalCommon::GetFiles(qslSources, qsFilter);
	qsSourcePath = qfilSources.at(0).path();

	// prepare progress bars
	for (iI = 0; iI < qfilSources.count(); iI++) {
		qi64TotalMaximum += qfilSources.at(iI).size();
	} // for
	emit SetTotalMaximum(qi64TotalMaximum);

	// get default modes
	ecConflict = GetDefaultOverwriteMode();
#ifdef Q_WS_WIN
	ecPermission = cFileOperation::GetDefaultReadonlyOverwritePermission(csSettings);
#endif
	ecRetry = cRetry::Ask;
	ecDiskSpace = cDiskSpace::Ask;

	// main process
	qdDir.setCurrent(qsSourcePath);
	qi64TotalValue = 0;
	for (iI = 0; iI < qfilSources.count() && !bCanceled; iI++) {
		const QFileInfo *qfiSource;

		qfiSource = &qfilSources.at(iI);
		// show file names
		qsSource = qfiSource->filePath();
		qsTarget = GetWildcardedName(*qfiSource, qsSourcePath, qsDestination);
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
			ecrCheck = CheckDiskSpace(qfiSource->size(), &ecDiskSpace, &qi64TotalValue);
			if (ecrCheck == cFileOperation::NextFile) {
				continue;
			} else {
				if (ecrCheck == cFileOperation::Cancel) {
					break;
				} // if
			} // if else

			// conflict solving
			ecrCheck = CheckConflict(*qfiSource, &ecConflict, &qi64TotalValue);
			if (ecrCheck == cFileOperation::NextFile) {
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
			ecrCheck = CheckPermission(qfiSource->size(), &ecPermission, &qi64TotalValue);
			if (ecrCheck == cFileOperation::NextFile) {
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
					ecrCheck = CheckRetry(*qfiSource, &ecRetry, &qi64TotalValue);
					if (ecrCheck == cFileOperation::NextFile) {
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
} // run
