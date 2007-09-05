#include "FileSystem/FileOperation.h"

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