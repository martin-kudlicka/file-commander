#include "FileSystem/FileOperation.h"

#include <QtCore/QDir>
#include "FileSystem.h"
#include "FileSystem/Local/LocalCommon.h"

// check disk space
const cFileOperation::eCheckResult cFileOperation::CheckDiskSpace(const cDiskSpace *cdsDiskSpace, const QString &qsSource, const QString &qsTarget, const int &iUnpackedSize, cDiskSpace::eChoice *ecDiskSpace, cDiskSpace::eChoice *ecDiskSpaceCurrent, QSemaphore *qsPause)
{
	cFileSystem::sDiskSpace sdsDiskSpace;

	sdsDiskSpace = cLocalCommon::GetDiskSpace(QFileInfo(qsTarget).path());
	if (sdsDiskSpace.qi64Free > iUnpackedSize) {
		*ecDiskSpaceCurrent = cDiskSpace::Ask;

		if (*ecDiskSpace == cDiskSpace::Ask) {
			cFileOperation cfoFileOperation;

			// disk space dialog
			connect(&cfoFileOperation, SIGNAL(ShowDiskSpaceDialog(const QString &, const qint64 &, const qint64 &)), cdsDiskSpace, SLOT(Show(const QString &, const qint64 &, const qint64 &)));
			emit cfoFileOperation.ShowDiskSpaceDialog(qsSource, iUnpackedSize, sdsDiskSpace.qi64Free);
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
const bool cFileOperation::SuitsFilter(const QString &qsName, const QString &qsFilter, const bool &bRegularExpression /* false */)
{
	int iI;
	QStringList qslFilter;

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

	// search for
	for (iI = 0; iI < qslFilter.count(); iI++) {
		QRegExp qreExpression(qslFilter.at(iI), Qt::CaseInsensitive);

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