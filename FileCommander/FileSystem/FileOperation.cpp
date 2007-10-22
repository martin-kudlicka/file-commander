#include "FileSystem/FileOperation.h"

#include <QtCore/QDir>
#include "FileSystem.h"
#include "FileSystem/Local/LocalCommon.h"
#ifdef Q_WS_WIN
#include <windows.h>
#endif

// check existing destination file conflict
const cFileOperation::eCheckResult cFileOperation::CheckConflict(const cFileOperationDialog::eOperation &eoOperation, const cCopyMoveConflict *ccmcConflict, const cRename *crRename, const QString &qsSource, const qint64 &qi64SourceSize, const QDateTime &qdtSourceLastModified, QString &qsTarget, cCopyMoveConflict::eChoice *ecConflict, cCopyMoveConflict::eChoice *ecConflictCurrent, QSemaphore *qsPause)
{
	if (QFile::exists(qsTarget)) {
		*ecConflictCurrent = cCopyMoveConflict::Ask;

		if (*ecConflict == cCopyMoveConflict::Ask) {
			while (true) {
				// no permanent conflict answer yet
				cFileOperation cfoFileOperation;
				QString qsOperation;

				// conflict dialog
				if (eoOperation == cFileOperationDialog::CopyOperation) {
					qsOperation = tr("Copy");
				} else {
					qsOperation = tr("Move");
				} // if else
				connect(&cfoFileOperation, SIGNAL(ShowConflictDialog(const QString &, const QString &, const qint64 &, const QDateTime &, const QFileInfo &)), ccmcConflict, SLOT(Show(const QString &, const QString &, const qint64 &, const QDateTime &, const QFileInfo &)));
				emit cfoFileOperation.ShowConflictDialog(qsOperation, qsSource, qi64SourceSize, qdtSourceLastModified, QFileInfo(qsTarget));
				// wait for answer
				qsPause->acquire();

				// solve conflict
				switch (*ecConflictCurrent) {
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

				if (*ecConflictCurrent == cCopyMoveConflict::Rename) {
					// rename
					QString qsNewFilename;

					// rename dialog
					connect(&cfoFileOperation, SIGNAL(ShowRenameDialog(QString *)), crRename, SLOT(Show(QString *)));
					emit cfoFileOperation.ShowRenameDialog(&qsNewFilename);
					// wait for answer
					qsPause->acquire();

					// solve
					if (!qsNewFilename.isEmpty()) {
						// new file name typed
						qsTarget = QFileInfo(qsTarget).path() + '/' + qsNewFilename;
						if (!QFile::exists(qsTarget)) {
							// rename ok, continue
							break;
						} // if
					} else {
						// cancel
						*ecConflictCurrent = cCopyMoveConflict::Cancel;
						break;
					} // if else
				} else {
					// no rename
					break;
				} // if else
			} // while
			if (*ecConflictCurrent == cCopyMoveConflict::Cancel) {
				// cancel
				return cFileOperation::Cancel;
			} // if
		} // if

		if (*ecConflict == cCopyMoveConflict::SkipAll || *ecConflictCurrent == cCopyMoveConflict::Skip) {
			// skip or skip all -> move onto next file
			return cFileOperation::NextFile;
		} else {
			if (*ecConflict == cCopyMoveConflict::OverwriteAll || *ecConflictCurrent == cCopyMoveConflict::Overwrite) {
				// overwrite, overwrite all -> delete target file
				QFile::remove(qsTarget);
			} else {
				if (*ecConflict == cCopyMoveConflict::OverwriteAllOlder) {
					// overwrite all older
					if (qdtSourceLastModified > QFileInfo(qsTarget).lastModified()) {
						// target file is older -> delete it
						QFile::remove(qsTarget);
					} else {
						// target file is newer -> move onto next file
						return cFileOperation::NextFile;
					} // if else
				} // if
			} // if else
		} // if else
	} // if

	return cFileOperation::Nothing;
} // CheckConflict

// delete non empty directory check
const cFileOperation::eCheckResult cFileOperation::CheckDeleteNonEmptyDirectory(const cDeleteNonEmptyDirectory *cdnedDeleteNonEmptyDir, const QString &qsPath, cDeleteNonEmptyDirectory::eChoice *ecDeleteNonEmptyDirectory, cDeleteNonEmptyDirectory::eChoice *ecDeleteNonEmptyDirectoryCurrent, QSemaphore *qsPause)
{
	*ecDeleteNonEmptyDirectoryCurrent = cDeleteNonEmptyDirectory::Ask;

	if (*ecDeleteNonEmptyDirectory == cDeleteNonEmptyDirectory::Ask) {
		cFileOperation cfoFileOperation;

		// delete non empty directory dialog
		connect(&cfoFileOperation, SIGNAL(ShowDeleteNonEmptyDirectoryDialog(const QString &)), cdnedDeleteNonEmptyDir, SLOT(Show(const QString &)));
		emit cfoFileOperation.ShowDeleteNonEmptyDirectoryDialog(qsPath);
		// wait for answer
		qsPause->acquire();

		switch (*ecDeleteNonEmptyDirectoryCurrent) {
			case cDeleteNonEmptyDirectory::YesToAll:
				*ecDeleteNonEmptyDirectory = cDeleteNonEmptyDirectory::YesToAll;
				break;
			case cDeleteNonEmptyDirectory::NoToAll:
				*ecDeleteNonEmptyDirectory = cDeleteNonEmptyDirectory::NoToAll;
			default:
				;
		} // switch
	} // if

	if (*ecDeleteNonEmptyDirectory == cDeleteNonEmptyDirectory::NoToAll || *ecDeleteNonEmptyDirectoryCurrent == cDeleteNonEmptyDirectory::No) {
		// do not delete this list of sources
		return cFileOperation::NextFile;
	} // if
	if (*ecDeleteNonEmptyDirectoryCurrent == cDeleteNonEmptyDirectory::Cancel) {
		// delete canceled
		return cFileOperation::Cancel;
	} // if
		// else can remove this list of sources

	return cFileOperation::Nothing;
} // CheckDeleteNonEmptyDirectory

// check disk space
const cFileOperation::eCheckResult cFileOperation::CheckDiskSpace(const cDiskSpace *cdsDiskSpace, const QString &qsSource, const QString &qsTarget, const qint64 &qi64SourceSize, cDiskSpace::eChoice *ecDiskSpace, cDiskSpace::eChoice *ecDiskSpaceCurrent, QSemaphore *qsPause)
{
	cFileSystem::sDiskSpace sdsDiskSpace;

	sdsDiskSpace = cLocalCommon::GetDiskSpace(QFileInfo(qsTarget).path());
	if (sdsDiskSpace.qi64Free < qi64SourceSize) {
		*ecDiskSpaceCurrent = cDiskSpace::Ask;

		if (*ecDiskSpace == cDiskSpace::Ask) {
			cFileOperation cfoFileOperation;

			// disk space dialog
			connect(&cfoFileOperation, SIGNAL(ShowDiskSpaceDialog(const QString &, const qint64 &, const qint64 &)), cdsDiskSpace, SLOT(Show(const QString &, const qint64 &, const qint64 &)));
			emit cfoFileOperation.ShowDiskSpaceDialog(qsSource, qi64SourceSize, sdsDiskSpace.qi64Free);
			// wait for answer
			qsPause->acquire();

			// solve
			switch (*ecDiskSpaceCurrent) {
				case cDiskSpace::YesToAll:
					*ecDiskSpace = cDiskSpace::YesToAll;
					break;
				case cDiskSpace::SkipAll:
					*ecDiskSpace = cDiskSpace::SkipAll;
				default:
					;
			} // switch
		} // if

		if (*ecDiskSpace == cDiskSpace::SkipAll || *ecDiskSpaceCurrent == cDiskSpace::Skip) {
			// skip current file
			return cFileOperation::NextFile;
		} else {
			if (*ecDiskSpaceCurrent == cDiskSpace::No) {
				// cancel
				return cFileOperation::Cancel;
			} // if
		} // if else
	} // if

	return cFileOperation::Nothing;
} // CheckDiskSpace

// check target file read only attribute
const cFileOperation::eCheckResult cFileOperation::CheckReadOnlyAttribute(const cPermission *cpPermission, const QString &qsTarget, cPermission::eChoice *ecPermission, cPermission::eChoice *ecPermissionCurrent, QSemaphore *qsPause, const bool &bForce /* false */)
{
	*ecPermissionCurrent = cPermission::Ask;
	if (bForce || (QFile::exists(qsTarget) && GetFileAttributes(reinterpret_cast<LPCWSTR>(qsTarget.unicode())) & FILE_ATTRIBUTE_READONLY)) {
		if (*ecPermission == cPermission::Ask) {
			cFileOperation cfoFileOperation;

			// show permission dialog
			connect(&cfoFileOperation, SIGNAL(ShowPermissionDialog(const QString &, const QString &)), cpPermission, SLOT(Show(const QString &, const QString &)));
			emit cfoFileOperation.ShowPermissionDialog(QFile(qsTarget).fileName(), tr("is readonly."));
			// wait for answer
			qsPause->acquire();

			switch (*ecPermissionCurrent) {
				case cPermission::YesToAll:
					*ecPermission = cPermission::YesToAll;
					break;
				case cPermission::NoToAll:
					*ecPermission = cPermission::NoToAll;
				default:
					;
			} // switch

			if (*ecPermissionCurrent == cPermission::Cancel) {
				return cFileOperation::Cancel;
			} // if
		} // if
		if (*ecPermission == cPermission::NoToAll || *ecPermissionCurrent == cPermission::No) {
			return cFileOperation::NextFile;
		} else {
			if (!bForce) {
				// remove target file readonly permission
				SetFileAttributes(reinterpret_cast<LPCWSTR>(qsTarget.unicode()), GetFileAttributes(reinterpret_cast<LPCWSTR>(qsTarget.unicode())) & ~FILE_ATTRIBUTE_READONLY);
			} // if
		} // if else
	} // if

	return cFileOperation::Nothing;
} // CheckReadOnlyAttribute

// retry if file operation unsuccesfull
const cFileOperation::eCheckResult cFileOperation::CheckRetry(const cRetry *crRetry, const cFileOperationDialog::eOperation &eoOperation, const QFileInfo &qfiSource, cRetry::eChoice *ecRetry, cRetry::eChoice *ecRetryCurrent, QSemaphore *qsPause)
{
	if (*ecRetry != cRetry::SkipAll) {
		cFileOperation cfoFileOperation;
		QString qsInformation;

		switch (eoOperation) {
			case cFileOperationDialog::CopyOperation:
				qsInformation = tr("Can't copy following file:");
				break;
			case cFileOperationDialog::MoveOperation:
				qsInformation = tr("Can't move following file:");
				break;
			case cFileOperationDialog::DeleteOperation:
				if (qfiSource.isDir()) {
					qsInformation = tr("Can't remove following directory:");
				} else {
					qsInformation = tr("Can't delete following file:");
				} // if else
		} // switch

		// show retry dialog
		connect(&cfoFileOperation, SIGNAL(ShowRetryDialog(const QString &, const QString &)), crRetry, SLOT(Show(const QString &, const QString &)));
		emit cfoFileOperation.ShowRetryDialog(qsInformation, qfiSource.filePath());
		// wait for answer
		qsPause->acquire();

		if (*ecRetryCurrent == cRetry::SkipAll) {
			// memorize permanent answer
			*ecRetry = cRetry::SkipAll;
		} // if
	} // if
	if (*ecRetry == cRetry::SkipAll || *ecRetryCurrent == cRetry::Skip || *ecRetryCurrent == cRetry::Abort) {
		// skip this file
		return cFileOperation::NextFile;
	} // if

	return cFileOperation::Nothing;
} // CheckRetry

// default delete non empty directory mode from settings file
const cDeleteNonEmptyDirectory::eChoice cFileOperation::GetDefaultDeleteNonEmptyDirectory(cSettings *csSettings)
{
	if (csSettings->GetAskToDeleteNonEmptyDirectory()) {
		return cDeleteNonEmptyDirectory::Ask;
	} else {
		return cDeleteNonEmptyDirectory::YesToAll;
	} // if else
} // GetDefaultDeleteNonEmptyDirectory

// default overwrite mode from settings file
const cCopyMoveConflict::eChoice cFileOperation::GetDefaultOverwriteMode(cSettings *csSettings)
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

#ifdef Q_WS_WIN
// default readonly overwrite permission
const cPermission::eChoice cFileOperation::GetDefaultReadonlyOverwritePermission(cSettings *csSettings)
{
	cPermission::eChoice ecPermission;
	QString qsOverwrite;

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

	return ecPermission;
} // GetDefaultReadonlyOverwritePermission
#endif

// return file name modified by wildcard
const QString cFileOperation::GetWildcardedName(const QFileInfo &qfiFile, const QString &qsSourcePath, const QString &qsDestination)
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

// check if filename suits filter
const bool cFileOperation::SuitsFilter(const QString &qsName, const QString &qsFilter, const bool &bCaseSensitive /* false */, const bool &bRegularExpression /* false */)
{
	int iI;
	QStringList qslFilter;
	Qt::CaseSensitivity csCaseSensitivity;

	qslFilter = qsFilter.split(';');
	if (qslFilter.count() == 1 && qslFilter.at(0).isEmpty() && !bRegularExpression) {
		qslFilter.append("*.*");
	} // if
#ifdef Q_WS_WIN
	// correct *.* to *
	for (iI = 0; iI < qslFilter.count(); iI++) {
		if (qslFilter.at(iI) == "*.*") {
			qslFilter[iI] = "*";
		} // if
	} // for
#endif

	if (bCaseSensitive) {
		csCaseSensitivity = Qt::CaseSensitive;
	} else {
		csCaseSensitivity = Qt::CaseInsensitive;
	} // if else

	// search for
	for (iI = 0; iI < qslFilter.count(); iI++) {
		QRegExp qreExpression(qslFilter.at(iI), csCaseSensitivity);

		if (bRegularExpression) {
			// regular expression
			if (qreExpression.indexIn(qsName) != -1) {
				return true;
			} // if
		} else {
			// wildcard
			qreExpression.setPatternSyntax(QRegExp::Wildcard);
			if (qreExpression.exactMatch(qsName)) {
				return true;
			} // if
		} // if else
	} // for

	return false;
} // SuitsFilter
