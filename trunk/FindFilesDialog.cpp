#include "FindFilesDialog.h"

#include "FindFilesDialog/DrivesDialog.h"
#include <QFileDialog>
#include <QTextDocument>
#include <QTextCodec>
#include <QTextCursor>
#include "FindFilesDialog/SaveFindSettingsDialog.h"
#include "ListerMainWindow.h"

// constructor
cFindFilesDialog::cFindFilesDialog(QWidget *qwParent, cPanel *cpPanel, cSettings *csSettings, cLister *clLister)
{
	setParent(qwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);
	setWindowModality(Qt::WindowModal);

	this->cpPanel = cpPanel;
	this->csSettings = csSettings;
	this->clLister = clLister;

	if (cpPanel) {
		// find files called from application's main menu
		qfilSelectedDirectories = cpPanel->GetSelectedDirItemsList();
		qcbSearchIn->insertItem(0, cpPanel->GetPath());
	} else {
		// find files called from file marking
		qcbSearchIn->setEnabled(false);
		qpbBrowse->setEnabled(false);
		qpbDrives->setEnabled(false);
		qsbSubdirectoryDepth->setValue(0);
		qsbSubdirectoryDepth->setEnabled(false);
		qgbFoundFiles->hide();
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
	int iI;
	QStringList qslSearchFor;

	bOk = false;
	qslSearchFor = qcbSearchFor->currentText().split(';');
	if (qslSearchFor.count() == 1 && qslSearchFor.at(0) == "" && !qcbSearchForRegularExpression->isChecked()) {
		qslSearchFor.append("*.*");
	} // if
#ifdef Q_WS_WIN
	// correct *.* to *
	for (iI = 0; iI < qslSearchFor.count(); iI++) {
		if (qslSearchFor.at(iI) == "*.*") {
			qslSearchFor[iI] = "*";
		} // if
	} // for
#endif

	// search for
	for (iI = 0; iI < qslSearchFor.count(); iI++) {
		QRegExp qreExpression(qslSearchFor.at(iI), Qt::CaseInsensitive);

		if (qcbSearchForRegularExpression->isChecked()) {
			// regular expression
			if (qreExpression.indexIn(qfiFile.fileName()) != -1) {
				bOk = true;
				break;
			} // if
		} else {
			// wildcard
			qreExpression.setPatternSyntax(QRegExp::Wildcard);
			if (qreExpression.exactMatch(qfiFile.fileName())) {
				bOk = true;
				break;
			} // if
		} // if else
	} // for
	if (!bOk) {
		return false;
	} // if

	// date/time between
	if (qcbDateTimeBetween->isChecked()) {
		if (qfiFile.lastModified() < qdteFrom->dateTime() || qfiFile.lastModified() > qdteTo->dateTime()) {
			return false;
		} // if
	} // if

	// date/time not older than
	if (qcbDateTimeNotOlderThan->isChecked()) {
		QDateTime qdtMaxOld;

		if (qcbOldType->currentText() == qsMINUTES) {
			qdtMaxOld = QDateTime::currentDateTime().addSecs(-60 * qsbOldCount->value());
		} else {
			if (qcbOldType->currentText() == qsHOURS) {
				qdtMaxOld = QDateTime::currentDateTime().addMSecs(-3600 * qsbOldCount->value());
			} else {
				if (qcbOldType->currentText() == qsDAYS) {
					qdtMaxOld = QDateTime::currentDateTime().addDays(-1 * qsbOldCount->value());
				} else {
					if (qcbOldType->currentText() == qsWEEKS) {
						qdtMaxOld = QDateTime::currentDateTime().addDays(-7 * qsbOldCount->value());
					} else {
						if (qcbOldType->currentText() == qsMONTHS) {
							qdtMaxOld = QDateTime::currentDateTime().addMonths(-1 * qsbOldCount->value());
						} else {
							qdtMaxOld = QDateTime::currentDateTime().addMonths(-12 * qsbOldCount->value());
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
	if (qcbFileSize->isChecked()) {
		qint64 qi64CompareSize;

		if (qcbFileSizeType->currentText() == qsBYTES2) {
			qi64CompareSize = qsbFileSize->value();
		} else {
			if (qcbFileSizeType->currentText() == qsKILOBYTES2) {
				qi64CompareSize = cPanel::qi64_KILOBYTE * qsbFileSize->value();
			} else {
				if (qcbFileSizeType->currentText() == qsMEGABYTES2) {
					qi64CompareSize = cPanel::qi64_MEGABYTE * qsbFileSize->value();
				} else {
					qi64CompareSize = cPanel::qi64_GIGABYTE * qsbFileSize->value();
				} // if else
			} // if else
		} // if else

		if (qcbFileSizeComparator->currentText() == "=") {
			if (qfiFile.size() != qi64CompareSize) {
				return false;
			} // if
		} else {
			if (qcbFileSizeComparator->currentText() == "<") {
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
	if (qcbSearchForText->isChecked()) {
		if (qfiFile.isFile()) {
			bool bFound;
			QByteArray qbaFile;
			QFile qfFile;
			QString qsPattern;

			if (qcbFullTextHex->isChecked()) {
				// TODO ConditionsSuit Hex
			} else {
				qsPattern = qcbFullText->currentText();
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

				if (qcbFullTextWholeWords->isChecked()) {
					ffFlags = QTextDocument::FindWholeWords;
				} // if
				if (qcbFullTextCaseSensitive->isChecked()) {
					ffFlags |= QTextDocument::FindCaseSensitively;
				} // if

				qtdText.setPlainText(Qt::codecForHtml(qbaFile)->toUnicode(qbaFile));

				if (qcbFullTextRegularExpression->isChecked()) {
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

			if (!bFound ^ qcbFullTextNotContainingText->isChecked()) {
				return false;
			} // if
		} else {
			// for directories
			return false;
		} // if else
	} // if

	return true;
} // ConditionsSuit

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

// search for text in files
void cFindFilesDialog::on_qcbSearchForText_stateChanged(int state)
{
	if (state == Qt::Checked) {
		qcbFullText->setEnabled(true);
		qcbFullTextWholeWords->setEnabled(true);
		qcbFullTextCaseSensitive->setEnabled(true);
		qcbFullTextNotContainingText->setEnabled(true);
		qcbFullTextHex->setEnabled(true);
		qcbFullTextRegularExpression->setEnabled(true);
		qcbFullText->setFocus(Qt::OtherFocusReason);
	} else {
		qcbFullText->setEnabled(false);
		qcbFullTextWholeWords->setEnabled(false);
		qcbFullTextCaseSensitive->setEnabled(false);
		qcbFullTextNotContainingText->setEnabled(false);
		qcbFullTextHex->setEnabled(false);
		qcbFullTextRegularExpression->setEnabled(false);
	} // if else
} // on_qcbSearchForText_stateChanged

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

// browse button is clicked on
void cFindFilesDialog::on_qpbBrowse_clicked(bool checked /* false */)
{
	QString qsDirectory;

	qsDirectory = QFileDialog::getExistingDirectory(this, tr("Select a directory"), "/");
	if (qsDirectory != "") {
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
	qcbSearchForText->setChecked(sfsFindSettings.bSearchForText);
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
	cSaveFindSettingsDialog csfsdDialog(this);

	if (csfsdDialog.exec() == QDialog::Accepted) {
		cSettings::sFindSettings sfsFindSettings;

		// general
		sfsFindSettings.qsSearchFor = qcbSearchFor->currentText();
		sfsFindSettings.bSearchForRegularExpression = qcbSearchForRegularExpression->isChecked();
		sfsFindSettings.qsSearchIn = qcbSearchIn->currentText();
		sfsFindSettings.iSubdirectoryDepth = qsbSubdirectoryDepth->value();
		sfsFindSettings.bSearchForText = qcbSearchForText->isChecked();
		if (qcbSearchForText->isChecked()) {
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

		csSettings->SetFindSettings(csfsdDialog.qleName->text(), sfsFindSettings);
		RefreshSavedSettings();
	} // if
} // on_qpbSaveFind_clicked

// start button is clicked on
void cFindFilesDialog::on_qpbStart_clicked(bool checked /* false */)
{
	int iDepth, iI;
	QFileInfoList qfilDirectories;

	qpbStart->setEnabled(false);
	qpbFeedToPanel->setEnabled(false);
	bStop = false;
	qpbStop->setEnabled(true);

	// add source path to directories to search in
	if (qcbSearchInSelectedDirectories->isChecked()) {
		qfilDirectories = qfilSelectedDirectories;
	} else {
		QStringList qslPaths;

		qslPaths = qcbSearchIn->currentText().split(';');
		for (iI = 0; iI < qslPaths.count(); iI++) {
			qfilDirectories.append(qslPaths.at(iI));
		} // for
	} // if else

	// clear previous search
	qfilSearch.clear();
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
					QString qsText;
					QTreeWidgetItem *qtwiItem;

					qfilSearch += qfilDirContent.at(iI);
					qtwiItem = new QTreeWidgetItem(qtwSearch);
					qsText = qfilDirContent.at(iI).filePath();
					if (qfilDirContent.at(iI).isDir() && csSettings->GetShowBracketsAroundDirectoryName()) {
						qsText = '[' + qsText + ']';
					} // if
					qtwiItem->setText(0, qsText);
					qhFiles.insert(qtwiItem, qfilDirContent.at(iI));
					QApplication::processEvents();
				} // if
			} // for

			// add found directories
			for (iI = 0; iI < qfilDirContent.count(); iI++) {
				if (qfilDirContent.at(iI).isDir()) {
					qfilNextDirDepth.append(qfilDirContent.at(iI));
				} // if
			} // for
		} // while

		if (iDepth < qsbSubdirectoryDepth->value()) {
			qfilDirectories += qfilNextDirDepth;
			iDepth++;
		} else {
			break;
		} // if else
	} // while

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

	clmwListerWindow = new cListerMainWindow(csSettings, clLister, qhFiles.value(qtwSearch->selectedItems().at(0)).filePath());
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