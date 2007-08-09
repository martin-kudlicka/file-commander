#include "FileSystem.h"

// destructor
cFileSystem::~cFileSystem()
{
} // ~cFileSystem

// check if filename suits filter
const bool cFileSystem::SuitsFilter(const QString &qsName, const QString &qsFilter, const bool &bRegularExpression /* false */) const
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