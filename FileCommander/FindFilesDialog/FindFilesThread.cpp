#include "FindFilesDialog/FindFilesThread.h"

#include "Panel.h"
#include <QtCore/QFile>
#include <QtGui/QTextCursor>
#include <QtGui/QTextDocument>
#include <QtCore/QTextCodec>

// check search conditions on found file
const bool cFindFilesThread::ConditionSuit(QTreeWidgetItem *qtwiFile) const
{
	bool bOk;

	// search for
	bOk = cFileOperation::SuitsFilter(cfsFileSystem->GetFileNameWithExtension(qtwiFile, false), sfsSearch.qsSearchFor, sfsSearch.bSearchForRegularExpression);
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

	// full text
	if (sfsSearch.bSearchForText) {
		// TODO ConditionsSuit non local files
		if (cfsFileSystem->IsFile(qtwiFile)) {
			bool bFound;
			QByteArray qbaFile;
			QFile qfFile;
			QString qsPattern;

			if (sfsSearch.bFullTextHex) {
				// TODO ConditionsSuit Hex
			} else {
				qsPattern = sfsSearch.qsFullText;
			} // if else

			qfFile.setFileName(cfsFileSystem->GetFilePath(qtwiFile));
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

				if (sfsSearch.bFulTextWholeWords) {
					ffFlags = QTextDocument::FindWholeWords;
				} // if
				if (sfsSearch.bFullTextCaseSensitive) {
					ffFlags |= QTextDocument::FindCaseSensitively;
				} // if

				qtdText.setPlainText(Qt::codecForHtml(qbaFile)->toUnicode(qbaFile));

				if (sfsSearch.bFullTextRegularExpression) {
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
					qfFile.seek(qfFile.pos() - qsPattern.length() + 1);
					qbaFile = qfFile.read(qi64SEARCH_BUFFER);
				} // if else
			} // while
			qfFile.close();

			if (!bFound ^ sfsSearch.bFullTextNotContainingText) {
				return false;
			} // if
		} else {
			// for directories
			return false;
		} // if else
	} // if

	return true;
} // ConditionSuit

// interrupt thread searching
const void cFindFilesThread::on_StopSearching()
{
	bStop = true;
} // on_StopSearching

// thread's main code
void cFindFilesThread::run()
{
	bool bFirstStep;
	int iDepth;

	bFirstStep = true;
	iDepth = 0;

	while (!qqDirectories.isEmpty() && !bStop) {
		int iI;
		QList<QTreeWidgetItem *> qlDirContent;

		// get content of directory
		cfsFileSystem->SetPath(qqDirectories.dequeue());
		if (ecfCalledFrom == SelectFiles) {
			cfsFileSystem->GetDirectoryContent(&qlDirContent, false);
		} else {
			cfsFileSystem->GetDirectoryContent(&qlDirContent);
		} // if else

		// check conditions
		for (iI = 0; iI < qlDirContent.count() && !bStop; iI++) {
			bool bAllowDotDot;
			QTreeWidgetItem *qtwiFile;

			qtwiFile = qlDirContent.at(iI);

			if (cfsFileSystem->GetFileName(qtwiFile, false) == "." || cfsFileSystem->GetFileName(qtwiFile, false) == "..") {
				if (ecfCalledFrom == BranchView && bFirstStep && cfsFileSystem->GetFileName(qtwiFile, false) == "..") {
					// allow ".." directory in branch view in starting directory
					bAllowDotDot = true;
				} else {
					// skip "." and ".."  directories
					continue;
				} // if
			} else {
				bAllowDotDot = false;
			} // if else

			if (bAllowDotDot || ConditionSuit(qtwiFile)) {
				QTreeWidgetItem *qtwiFound;

				// add file to custom list
				qtwiFound = cfsFileSystem->AddToCustomList(qtwiFile);

				// add to list of found files in dialog
				emit Found(qtwiFound, cfsFileSystem);
			} // if

			// add found directories (excluding first "..")
			if (!bAllowDotDot && cfsFileSystem->IsDir(qtwiFile) && iDepth < sfsSearch.iSubdirectoryDepth) {
				qqDirectories.enqueue(cfsFileSystem->GetFilePath(qtwiFile));
			} // if
		} // for

		if (iDepth < sfsSearch.iSubdirectoryDepth) {
			iDepth++;
		} else {
			break;
		} // if else

		if (bFirstStep) {
			bFirstStep = false;
		} // if
	} // while

	cfsFileSystem->SetPath(qsPath);
} // run

// start of searching for files
void cFindFilesThread::Start(const cSettings::sFindSettings &sfsSearch, cFileSystem *cfsFileSystem, const QString qsPath, const eCalledFrom &ecfCalledFrom /* FindFilesDialog */)
{
	this->sfsSearch = sfsSearch;
	this->cfsFileSystem = cfsFileSystem;
	this->qsPath = cfsFileSystem->GetPath();
	this->ecfCalledFrom = ecfCalledFrom;
	bStop = false;

	qqDirectories.clear();
	qqDirectories.enqueue(qsPath);

	if (ecfCalledFrom == FindFilesDialog) {
		start();
	} else {
		run();
	} // if else
} // Start
