#include "FindFilesDialog.h"

#include "FindFilesDialog/DrivesDialog.h"
#include <QtGui/QFileDialog>
#include <QtGui/QTextDocument>
#include <QtCore/QTextCodec>
#include <QtGui/QTextCursor>
#include "ListerMainWindow.h"
#include <QtGui/QInputDialog>
#include <QtGui/QDirModel>

// constructor
cFindFilesDialog::cFindFilesDialog(QWidget *qwParent, cPanel *cpPanel, cSettings *csSettings, cListerPlugin *clpListerPlugin)
{
	setParent(qwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);
	setWindowModality(Qt::WindowModal);

	this->cpPanel = cpPanel;
	this->csSettings = csSettings;
	this->clpListerPlugin = clpListerPlugin;

	if (cpPanel) {
		// find files called from application's main menu
		qfilSelectedDirectories = cpPanel->GetSelectedItemsFileList(QDir::Dirs);
		qcbSearchIn->insertItem(0, cpPanel->GetPath());
	} else {
		// find files called from file marking
		qcbSearchIn->setEnabled(false);
		qpbBrowse->setEnabled(false);
		qpbDrives->setEnabled(false);
		qsbSubdirectoryDepth->setValue(0);
		qsbSubdirectoryDepth->setEnabled(false);
		qgbFoundFiles->setEnabled(false);
		qpbStart->setEnabled(false);
		qpbCancel->setText(tr("OK"));
	} // if else

	if (qfilSelectedDirectories.count() > 0) {
		qcbSearchInSelectedDirectories->setChecked(true);
	} else {
		qcbSearchInSelectedDirectories->setEnabled(false);
	} // if else
	qdteFrom->setDateTime(QDateTime::currentDateTime());
	qdteTo->setDateTime(QDateTime::currentDateTime());
	qcbOldType->addItems(qslOLDER_THAN);
	qcbFileSizeComparator->addItems(qsFILE_SIZE_COMPARATOR);
	qcbFileSizeType->addItems(qsFILE_SIZE_TYPE);
	qtwSearch->headerItem()->setHidden(true);
	qtwSavedFinds->headerItem()->setHidden(true);

	// refresh list of saved settings
	RefreshSavedSettings();

	// completer
	qcSearchIn.setModel(new QDirModel(&qcSearchIn));
	qcbSearchIn->setCompleter(&qcSearchIn);
} // cFindFilesDialog

// catch dialog close
void cFindFilesDialog::closeEvent(QCloseEvent *event)
{
	if (!bStop) {
		bStop = true;
	} // if
} // closeEvent

// check search conditions on found file
bool cFindFilesDialog::ConditionsSuit(const QFileInfo &qfiFile)
{
	bool bOk;

	// search for
	bOk = cFileRoutine::SuitsFilter(qfiFile.fileName(), sfsCurrentSearch.qsSearchFor, sfsCurrentSearch.bSearchForRegularExpression);
	if (!bOk) {
		return false;
	} // if

	// date/time between
	if (sfsCurrentSearch.bDateTimeBetween) {
		if (qfiFile.lastModified() < sfsCurrentSearch.qdtFrom || qfiFile.lastModified() > sfsCurrentSearch.qdtTo) {
			return false;
		} // if
	} // if

	// date/time not older than
	if (sfsCurrentSearch.bNotOlderThan) {
		QDateTime qdtMaxOld;

		if (sfsCurrentSearch.qsNotOlderThanType == qsMINUTES) {
			qdtMaxOld = QDateTime::currentDateTime().addSecs(-60 * sfsCurrentSearch.iNotOlderThanCount);
		} else {
			if (sfsCurrentSearch.qsNotOlderThanType == qsHOURS) {
				qdtMaxOld = QDateTime::currentDateTime().addMSecs(-3600 * sfsCurrentSearch.iNotOlderThanCount);
			} else {
				if (sfsCurrentSearch.qsNotOlderThanType == qsDAYS) {
					qdtMaxOld = QDateTime::currentDateTime().addDays(-1 * sfsCurrentSearch.iNotOlderThanCount);
				} else {
					if (sfsCurrentSearch.qsNotOlderThanType == qsWEEKS) {
						qdtMaxOld = QDateTime::currentDateTime().addDays(-7 * sfsCurrentSearch.iNotOlderThanCount);
					} else {
						if (sfsCurrentSearch.qsNotOlderThanType == qsMONTHS) {
							qdtMaxOld = QDateTime::currentDateTime().addMonths(-1 * sfsCurrentSearch.iNotOlderThanCount);
						} else {
							qdtMaxOld = QDateTime::currentDateTime().addMonths(-12 * sfsCurrentSearch.iNotOlderThanCount);
						} // if else
					} // if else
				} // if else
			} // if else
		} // if else

		if (qfiFile.lastModified() < qdtMaxOld) {
			return false;
		} // if
	} // if

	// file size
	if (sfsCurrentSearch.bFileSize) {
		qint64 qi64CompareSize;

		if (sfsCurrentSearch.qsFileSizeType == qsBYTES2) {
			qi64CompareSize = sfsCurrentSearch.iFileSizeValue;
		} else {
			if (sfsCurrentSearch.qsFileSizeType == qsKILOBYTES2) {
				qi64CompareSize = cPanel::qi64_KILOBYTE * sfsCurrentSearch.iFileSizeValue;
			} else {
				if (sfsCurrentSearch.qsFileSizeType == qsMEGABYTES2) {
					qi64CompareSize = cPanel::qi64_MEGABYTE * sfsCurrentSearch.iFileSizeValue;
				} else {
					qi64CompareSize = cPanel::qi64_GIGABYTE * sfsCurrentSearch.iFileSizeValue;
				} // if else
			} // if else
		} // if else

		if (sfsCurrentSearch.qsFileSizeComparator == "=") {
			if (qfiFile.size() != qi64CompareSize) {
				return false;
			} // if
		} else {
			if (sfsCurrentSearch.qsFileSizeComparator == "<") {
				if (qfiFile.size() >= qi64CompareSize) {
					return false;
				} // if else
			} else {
				if (qfiFile.size() <= qi64CompareSize) {
					return false;
				} // if
			} // if else
		} // if else
	} // if

	// full text
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
	} // if

	return true;
} // ConditionsSuit

// store settings in sFindSettings structure
cSettings::sFindSettings cFindFilesDialog::GetSettings()
{
	cSettings::sFindSettings sfsFindSettings;

	// general
	sfsFindSettings.qsSearchFor = qcbSearchFor->currentText();
	sfsFindSettings.bSearchForRegularExpression = qcbSearchForRegularExpression->isChecked();
	sfsFindSettings.qsSearchIn = qcbSearchIn->currentText();
	sfsFindSettings.iSubdirectoryDepth = qsbSubdirectoryDepth->value();
	sfsFindSettings.bSearchForText = qgbFullText->isChecked();
	if (qgbFullText->isChecked()) {
		sfsFindSettings.qsFullText = qcbFullText->currentText();
		sfsFindSettings.bFulTextWholeWords = qcbFullTextWholeWords->isChecked();
		sfsFindSettings.bFullTextCaseSensitive = qcbFullTextCaseSensitive->isChecked();
		sfsFindSettings.bFullTextNotContainingText = qcbFullTextNotContainingText->isChecked();
		sfsFindSettings.bFullTextHex = qcbFullTextHex->isChecked();
		sfsFindSettings.bFullTextRegularExpression = qcbFullTextRegularExpression->isChecked();
	} // if
	// advanced
	sfsFindSettings.bDateTimeBetween = qcbDateTimeBetween->isChecked();
	if (qcbDateTimeBetween->isChecked()) {
		sfsFindSettings.qdtFrom = qdteFrom->dateTime();
		sfsFindSettings.qdtTo = qdteTo->dateTime();
	} // if
	sfsFindSettings.bNotOlderThan = qcbDateTimeNotOlderThan->isChecked();
	if (qcbDateTimeNotOlderThan->isChecked()) {
		sfsFindSettings.iNotOlderThanCount = qsbOldCount->value();
		sfsFindSettings.qsNotOlderThanType = qcbOldType->currentText();
	} // if
	sfsFindSettings.bFileSize = qcbFileSize->isChecked();
	if (qcbFileSize->isChecked()) {
		sfsFindSettings.qsFileSizeComparator = qcbFileSizeComparator->currentText();
		sfsFindSettings.iFileSizeValue = qsbFileSize->value();
		sfsFindSettings.qsFileSizeType = qcbFileSizeType->currentText();
	} // if

	return sfsFindSettings;
} // GetSettings

// search files in specified date/time range
void cFindFilesDialog::on_qcbDateTimeBetween_stateChanged(int state)
{
	if (state == Qt::Checked) {
		if (qcbDateTimeNotOlderThan->isChecked()) {
			qcbDateTimeNotOlderThan->setChecked(false);
			qsbOldCount->setEnabled(false);
			qcbOldType->setEnabled(false);
		} // if
		qdteFrom->setEnabled(true);
		qdteTo->setEnabled(true);
	} else {
		qcbDateTimeBetween->setChecked(false);
		qdteFrom->setEnabled(false);
		qdteTo->setEnabled(false);
	} // if else
} // on_qcbDateTimeBetween_stateChanged

// search files not older than specified
void cFindFilesDialog::on_qcbDateTimeNotOlderThan_stateChanged(int state)
{
	if (state == Qt::Checked) {
		if (qcbDateTimeBetween->isChecked()) {
			qcbDateTimeBetween->setChecked(false);
			qdteFrom->setEnabled(false);
			qdteTo->setEnabled(false);
		} // if
		qsbOldCount->setEnabled(true);
		qcbOldType->setEnabled(true);
	} else {
		qcbDateTimeNotOlderThan->setChecked(false);
		qsbOldCount->setEnabled(false);
		qcbOldType->setEnabled(false);
	} // if else
} // on_qcbDateTimeNotOlderThan_stateChanged

// search files by their size
void cFindFilesDialog::on_qcbFileSize_stateChanged(int state)
{
	if (state == Qt::Checked) {
		qcbFileSizeComparator->setEnabled(true);
		qsbFileSize->setEnabled(true);
		qcbFileSizeType->setEnabled(true);
	} else {
		qcbFileSizeComparator->setEnabled(false);
		qsbFileSize->setEnabled(false);
		qcbFileSizeType->setEnabled(false);
	} // if else
} // on_qcbFileSize_stateChanged

// search only in selected directories
void cFindFilesDialog::on_qcbSearchInSelectedDirectories_stateChanged(int state)
{
	if (state == Qt::Checked) {
		qcbSearchIn->setEnabled(false);
		qpbBrowse->setEnabled(false);
		qpbDrives->setEnabled(false);
		qpbSaveFind->setEnabled(false);
	} else {
		qcbSearchIn->setEnabled(true);
		qpbBrowse->setEnabled(true);
		qpbDrives->setEnabled(true);
		qpbSaveFind->setEnabled(true);
	} // if else
} // on_qcbSearchInSelectedDirectories_stateChanged

// search for text in files
void cFindFilesDialog::on_qgbFullText_toggled(bool on)
{
	if (on) {
		qcbFullText->setFocus(Qt::OtherFocusReason);
	} // if
} // on_qgbFullText_toggled

// browse button is clicked on
void cFindFilesDialog::on_qpbBrowse_clicked(bool checked /* false */)
{
	QString qsDirectory;

	qsDirectory = QFileDialog::getExistingDirectory(this, tr("Select directory"), "/");
	if (!qsDirectory.isEmpty()) {
		qcbSearchIn->setEditText(qsDirectory);
	} // if
} // on_qpbBrowse_clicked

// delete find is clicked on
void cFindFilesDialog::on_qpbDeleteFind_clicked(bool checked /* false */)
{
	csSettings->RemoveFindSettings(qtwSavedFinds->selectedItems().at(0)->text(0));
	RefreshSavedSettings();
} // on_qpbDeleteFind_clicked

// drives button is clicked on
void cFindFilesDialog::on_qpbDrives_clicked(bool checked /* false */)
{
	cDrivesDialog *cddDrives;
	QMap<QString, cFileRoutine::sDriveInfo> qmDrives;

	qmDrives = cFileRoutine::GetDrives();
	cddDrives = new cDrivesDialog(this, qmDrives);

	if (cddDrives->exec() == QDialog::Accepted) {
		int iI;
		QList<QTreeWidgetItem *> qlDrives;
		QString qsPath;

		qlDrives = cddDrives->qtwDrives->selectedItems();
		for (iI = 0; iI < qlDrives.count(); iI++) {
			qsPath += qmDrives.value(qlDrives.at(iI)->text(0)).qsPath;
			if (iI < qlDrives.count() - 1) {
				qsPath += ';';
			} // if
		} // for

		qcbSearchIn->setEditText(qsPath);
	} // if

	cddDrives->deleteLater();
} // on_qpbDrives_clicked

// feed to panel button is clicked on
void cFindFilesDialog::on_qpbFeedToPanel_clicked(bool checked /* false */)
{
	int iI;
	QFileInfoList qfilFiles;

	for (iI = 0; iI < qtwSearch->topLevelItemCount(); iI++) {
		qfilFiles.append(qhFiles.value(qtwSearch->topLevelItem(iI)));
	} // for

	cpPanel->FeedToPanel(qfilFiles);
	reject();
} // on_qpbFeedToPanel_clicked

// go to file button is clicked on
void cFindFilesDialog::on_qpbGoToFile_clicked(bool checked /* false */)
{
	cpPanel->GoToFile(qhFiles.value(qtwSearch->selectedItems().at(0)).filePath());
	reject();
} // on_qpbGoToFile_clicked

// load find is clicked on
void cFindFilesDialog::on_qpbLoadFind_clicked(bool checked /* false */)
{
	cSettings::sFindSettings sfsFindSettings;

	sfsFindSettings = csSettings->GetFindSettings(qtwSavedFinds->selectedItems().at(0)->text(0));

	// general
	qcbSearchFor->setEditText(sfsFindSettings.qsSearchFor);
	qcbSearchForRegularExpression->setChecked(sfsFindSettings.bSearchForRegularExpression);
	qcbSearchIn->setEditText(sfsFindSettings.qsSearchIn);
	qsbSubdirectoryDepth->setValue(sfsFindSettings.iSubdirectoryDepth);
	qgbFullText->setChecked(sfsFindSettings.bSearchForText);
	qcbFullText->setEditText(sfsFindSettings.qsFullText);
	qcbFullTextWholeWords->setChecked(sfsFindSettings.bFulTextWholeWords);
	qcbFullTextCaseSensitive->setChecked(sfsFindSettings.bFullTextCaseSensitive);
	qcbFullTextNotContainingText->setChecked(sfsFindSettings.bFullTextNotContainingText);
	qcbFullTextHex->setChecked(sfsFindSettings.bFullTextHex);
	qcbFullTextRegularExpression->setChecked(sfsFindSettings.bFullTextRegularExpression);
	// advanced
	qcbDateTimeBetween->setChecked(sfsFindSettings.bDateTimeBetween);
	qdteFrom->setDateTime(sfsFindSettings.qdtFrom);
	qdteTo->setDateTime(sfsFindSettings.qdtTo);
	qcbDateTimeNotOlderThan->setChecked(sfsFindSettings.bNotOlderThan);
	qsbOldCount->setValue(sfsFindSettings.iNotOlderThanCount);
	qcbOldType->setCurrentIndex(qcbOldType->findText(sfsFindSettings.qsNotOlderThanType));
	qcbFileSize->setChecked(sfsFindSettings.bFileSize);
	qcbFileSizeComparator->setCurrentIndex(qcbFileSizeComparator->findText(sfsFindSettings.qsFileSizeComparator));
	qsbFileSize->setValue(sfsFindSettings.iFileSizeValue);
	qcbFileSizeType->setCurrentIndex(qcbFileSizeType->findText(sfsFindSettings.qsFileSizeType));
} // on_qpbLoadFind_clicked

// save find is clicked on
void cFindFilesDialog::on_qpbSaveFind_clicked(bool checked /* false */)
{
	QString qsName;

	qsName = QInputDialog::getText(this, tr("Save settings"), NULL);

	if (!qsName.isEmpty()) {
		cSettings::sFindSettings sfsFindSettings;

		sfsFindSettings = GetSettings();
		csSettings->SetFindSettings(qsName, sfsFindSettings);
		RefreshSavedSettings();
	} // if
} // on_qpbSaveFind_clicked

// start button is clicked on
void cFindFilesDialog::on_qpbStart_clicked(bool checked /* false */)
{
	qpbStart->setEnabled(false);
	qpbFeedToPanel->setEnabled(false);
	qpbStop->setEnabled(true);

	Start(GetSettings(), false);

	qpbStop->setEnabled(false);
	qpbStart->setEnabled(true);
	if (qtwSearch->topLevelItemCount() > 0) {
		qpbFeedToPanel->setEnabled(true);
	} // if
} // on_qpbStart_clicked

// stop button is clicked on
void cFindFilesDialog::on_qpbStop_clicked(bool checked /* false */)
{
	bStop = true;
} // on_qpbStop_clicked

// view button is clicked on
void cFindFilesDialog::on_qpbView_clicked(bool checked /* false */)
{
	cListerMainWindow *clmwListerWindow;

	clmwListerWindow = new cListerMainWindow(csSettings, clpListerPlugin, qhFiles.value(qtwSearch->selectedItems().at(0)).filePath());
	clmwListerWindow->show();
} // on_qpbView_clicked

// selected item changed in saved finds view
void cFindFilesDialog::on_qtwSavedFinds_itemSelectionChanged()
{
	if (qtwSavedFinds->selectedItems().count() > 0) {
		qpbLoadFind->setEnabled(true);
		qpbDeleteFind->setEnabled(true);
	} else {
		qpbLoadFind->setEnabled(false);
		qpbDeleteFind->setEnabled(false);
	} // if else
} // on_qtwSavedFinds_itemSelectionChanged

// selected item changed in found files list
void cFindFilesDialog::on_qtwSearch_itemSelectionChanged()
{
	if (qtwSearch->selectedItems().count() > 0) {
		if (QFileInfo(qtwSearch->selectedItems().at(0)->text(0)).isFile()) {
			qpbView->setEnabled(true);
		} else {
			qpbView->setEnabled(false);
		} // if else
		qpbGoToFile->setEnabled(true);
	} else {
		qpbView->setEnabled(false);
		qpbGoToFile->setEnabled(false);
	} // if else
} // on_qtwSearch_itemSelectionChanged

// refreshes list of saved settings
void cFindFilesDialog::RefreshSavedSettings()
{
	int iI;
	QStringList qslSavedFinds;

	qtwSavedFinds->clear();

	qslSavedFinds = csSettings->GetSavedFinds();

	for (iI = 0; iI < qslSavedFinds.count(); iI++) {
		QTreeWidgetItem *qtwiFind;

		qtwiFind = new QTreeWidgetItem(qtwSavedFinds);
		qtwiFind->setText(0, qslSavedFinds.at(iI));
	} // for
} // RefreshSavedSettings

// founded files
QFileInfoList cFindFilesDialog::SearchResult()
{
	QFileInfoList qfilFiles;

	QHashIterator<QTreeWidgetItem *, QFileInfo> qhiFile(qhFiles);
	while (qhiFile.hasNext()) {
		qhiFile.next();
		qfilFiles += qhiFile.value();
	} // while

	return qfilFiles;
} // SearchResult

// start of the new search
void cFindFilesDialog::Start(const cSettings::sFindSettings &sfsCurrentSearch, const bool &bMarking)
{
	int iDepth, iI;
	QFileInfoList qfilDirectories;

	bStop = false;
	this->sfsCurrentSearch = sfsCurrentSearch;

	// add source path to directories to search in
	if (qcbSearchInSelectedDirectories->isChecked()) {
		qfilDirectories = qfilSelectedDirectories;
	} else {
		QStringList qslPaths;

		qslPaths = sfsCurrentSearch.qsSearchIn.split(';');
		for (iI = 0; iI < qslPaths.count(); iI++) {
			qfilDirectories.append(qslPaths.at(iI));
		} // for
	} // if else

	// clear previous search
	qtwSearch->clear();
	qhFiles.clear();

	// search through directories
	iDepth = 0;
	while (!qfilDirectories.isEmpty() && !bStop) {
		QFileInfoList qfilNextDirDepth;

		while (!qfilDirectories.isEmpty() && !bStop) {
			QFileInfo qfiDir;
			QFileInfoList qfilDirContent;

			// get content of directory
			qfiDir = qfilDirectories.takeAt(0);
			qfilDirContent = cFileRoutine::GetDirectoryContent(qfiDir.filePath(), QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);

			// check conditions
			for (iI = 0; iI < qfilDirContent.count() && !bStop; iI++) {
				if (ConditionsSuit(qfilDirContent.at(iI))) {
					// file suit conditions
					QTreeWidgetItem *qtwiItem;

					if (!bMarking) {
						QString qsText;

						qtwiItem = new QTreeWidgetItem(qtwSearch);
						qsText = qfilDirContent.at(iI).filePath();
						if (qfilDirContent.at(iI).isDir() && csSettings->GetShowBracketsAroundDirectoryName()) {
							qsText = '[' + qsText + ']';
						} // if
						qtwiItem->setText(0, qsText);
						QApplication::processEvents();
					} // if
					qhFiles.insertMulti(qtwiItem, qfilDirContent.at(iI));
				} // if
			} // for

			// add found directories
			for (iI = 0; iI < qfilDirContent.count(); iI++) {
				if (qfilDirContent.at(iI).isDir()) {
					qfilNextDirDepth.append(qfilDirContent.at(iI));
				} // if
			} // for
		} // while

		if (iDepth < sfsCurrentSearch.iSubdirectoryDepth) {
			qfilDirectories += qfilNextDirDepth;
			iDepth++;
		} else {
			break;
		} // if else
	} // while
} // Start
