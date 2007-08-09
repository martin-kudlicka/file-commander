#include "FindFilesDialog/FindFilesThread.h"

#include "Panel.h"

// check search conditions on found file
const bool cFindFilesThread::ConditionSuit(QTreeWidgetItem *qtwiFile) const
{
	bool bOk;

	// search for
	bOk = cfsFileSystem->SuitsFilter(cfsFileSystem->GetFileName(qtwiFile), sfsSearch.qsSearchFor, sfsSearch.bSearchForRegularExpression);
	if (!bOk) {
		return false;
	} // if

	// date/time between
	if (sfsSearch.bDateTimeBetween) {
		if (cfsFileSystem->GetLastModified(qtwiFile) < sfsSearch.qdtFrom || cfsFileSystem->GetLastModified(qtwiFile) > sfsSearch.qdtTo) {
			return false;
		} // if
	} // if

	// date/time not older than
	if (sfsSearch.bNotOlderThan) {
		QDateTime qdtMaxOld;

		if (sfsSearch.qsNotOlderThanType == qsMINUTES) {
			qdtMaxOld = QDateTime::currentDateTime().addSecs(-60 * sfsSearch.iNotOlderThanCount);
		} else {
			if (sfsSearch.qsNotOlderThanType == qsHOURS) {
				qdtMaxOld = QDateTime::currentDateTime().addMSecs(-3600 * sfsSearch.iNotOlderThanCount);
			} else {
				if (sfsSearch.qsNotOlderThanType == qsDAYS) {
					qdtMaxOld = QDateTime::currentDateTime().addDays(-1 * sfsSearch.iNotOlderThanCount);
				} else {
					if (sfsSearch.qsNotOlderThanType == qsWEEKS) {
						qdtMaxOld = QDateTime::currentDateTime().addDays(-7 * sfsSearch.iNotOlderThanCount);
					} else {
						if (sfsSearch.qsNotOlderThanType == qsMONTHS) {
							qdtMaxOld = QDateTime::currentDateTime().addMonths(-1 * sfsSearch.iNotOlderThanCount);
						} else {
							qdtMaxOld = QDateTime::currentDateTime().addMonths(-12 * sfsSearch.iNotOlderThanCount);
						} // if else
					} // if else
				} // if else
			} // if else
		} // if else

		if (cfsFileSystem->GetLastModified(qtwiFile) < qdtMaxOld) {
			return false;
		} // if
	} // if

	// file size
	if (sfsSearch.bFileSize) {
		qint64 qi64CompareSize;

		if (sfsSearch.qsFileSizeType == qsBYTES2) {
			qi64CompareSize = sfsSearch.iFileSizeValue;
		} else {
			if (sfsSearch.qsFileSizeType == qsKILOBYTES2) {
				qi64CompareSize = cPanel::qi64_KILOBYTE * sfsSearch.iFileSizeValue;
			} else {
				if (sfsSearch.qsFileSizeType == qsMEGABYTES2) {
					qi64CompareSize = cPanel::qi64_MEGABYTE * sfsSearch.iFileSizeValue;
				} else {
					qi64CompareSize = cPanel::qi64_GIGABYTE * sfsSearch.iFileSizeValue;
				} // if else
			} // if else
		} // if else

		if (sfsSearch.qsFileSizeComparator == "=") {
			if (cfsFileSystem->GetFileSize(qtwiFile) != qi64CompareSize) {
				return false;
			} // if
		} else {
			if (sfsSearch.qsFileSizeComparator == "<") {
				if (cfsFileSystem->GetFileSize(qtwiFile) >= qi64CompareSize) {
					return false;
				} // if else
			} else {
				if (cfsFileSystem->GetFileSize(qtwiFile) <= qi64CompareSize) {
					return false;
				} // if
			} // if else
		} // if else
	} // if

	// TODO ConditionSuit full text
	/*// full text
	if (sfsCurrentSearch.bSearchForText) {
		if (qfiFile.isFile()) {
			bool bFound;
			QByteArray qbaFile;
			QFile qfFile;
			QString qsPattern;

			if (sfsCurrentSearch.bFullTextHex) {
				// TODO ConditionsSuit Hex
			} else {
				qsPattern = sfsCurrentSearch.qsFullText;
			} // if else

			qfFile.setFileName(qfiFile.filePath());
			if (qfFile.size() < qsPattern.length()) {
				return false;
			} // if
			qfFile.open(QIODevice::ReadOnly);

			bFound = false;
			qbaFile = qfFile.read(qi64SEARCH_BUFFER);
			while (!qbaFile.isEmpty() && !bStop) {
				QTextCursor qtcCursor;
				QTextDocument qtdText;
				QTextDocument::FindFlags ffFlags;

				if (sfsCurrentSearch.bFulTextWholeWords) {
					ffFlags = QTextDocument::FindWholeWords;
				} // if
				if (sfsCurrentSearch.bFullTextCaseSensitive) {
					ffFlags |= QTextDocument::FindCaseSensitively;
				} // if

				qtdText.setPlainText(Qt::codecForHtml(qbaFile)->toUnicode(qbaFile));

				if (sfsCurrentSearch.bFullTextRegularExpression) {
					qtcCursor = qtdText.find(QRegExp(qsPattern), 0, ffFlags);
				} else {
					qtcCursor = qtdText.find(qsPattern, 0, ffFlags);
				} // if else

				if (!qtcCursor.isNull()) {
					bFound = true;
					break;
				} // if

				// next buffer part
				if (qfFile.atEnd()) {
					break;
				} else {
					QApplication::processEvents();
					qfFile.seek(qfFile.pos() - qsPattern.length() + 1);
					qbaFile = qfFile.read(qi64SEARCH_BUFFER);
				} // if else
			} // while
			qfFile.close();

			if (!bFound ^ sfsCurrentSearch.bFullTextNotContainingText) {
				return false;
			} // if
		} else {
			// for directories
			return false;
		} // if else
	} // if*/

	return true;
} // ConditionSuit

// thread's main code
void cFindFilesThread::run()
{
	int iDepth;

	iDepth = 0;
	while (true) {
		int iI;

		QList<QTreeWidgetItem *> qlDirContent;

		for (iI = 0; iI < qlDirContent.count(); iI++) {
			QTreeWidgetItem *qtwiFile;

			qtwiFile = qlDirContent[iI];

			if (ConditionSuit(qtwiFile)) {
				// TODO run ConditionSuit
			} // if

			if (cfsFileSystem->IsDir(qtwiFile)) {
				// TODO run IsDir
			} // if
		} // for

		if (iDepth < sfsSearch.iSubdirectoryDepth) {
			//qfilDirectories += qfilNextDirDepth;
			iDepth++;
		} else {
			break;
		} // if else
	} // while
} // run

// start of searching for files
void cFindFilesThread::Start(const cSettings::sFindSettings &sfsSearch, cFileSystem *cfsFileSystem, const QString qsPath, const bool &bMarking)
{
	this->sfsSearch = sfsSearch;
	this->cfsFileSystem = cfsFileSystem;
	this->bMarking = bMarking;

	cfsFileSystem->SetPath(qsPath);

	run();
} // Start