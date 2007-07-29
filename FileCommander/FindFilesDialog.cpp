#include "FindFilesDialog.h"

#include <QtGui/QDirModel>
#include <QtGui/QFileDialog>
#include <QtGui/QInputDialog>

const QString qsDAYS = "day(s)";
const QString qsGIGABYTES2 = "gigabyte(s)";
const QString qsHOURS = "hour(s)";
const QString qsKILOBYTES2 = "kilobyte(s)";
const QString qsMEGABYTES2 = "megabyte(s)";
const QString qsMONTHS = "month(s)";
const QString qsWEEKS = "week(s)";
const QString qsYEARS = "year(s)";
const QStringList qslFILE_SIZE_COMPARATOR = (QStringList() << "=" << "<" << ">");
const QStringList qslFILE_SIZE_TYPE = (QStringList() << qsBYTES2 << qsKILOBYTES2 << qsMEGABYTES2 << qsGIGABYTES2);
const QStringList qslOLDER_THAN = (QStringList() << qsMINUTES << qsHOURS << qsDAYS << qsWEEKS << qsMONTHS << qsYEARS);

// constructor
cFindFilesDialog::cFindFilesDialog(QMainWindow *qmwParent, cPanel *cpPanel, cFileControl *cfcFileControl, cSettings *csSettings, cListerPlugin *clpListerPlugin)
{
	int iSelected;

	setParent(qmwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);

	this->cpPanel = cpPanel;
	this->cfcFileControl = cfcFileControl;
	this->csSettings = csSettings;
	this->clpListerPlugin = clpListerPlugin;

	// history
	qcbSearchFor->addItems(csSettings->GetComboBoxHistory(cSettings::FindFilesSearchFor));
	qcbSearchIn->addItems(csSettings->GetComboBoxHistory(cSettings::FindFilesSearchIn));

	if (cpPanel) {
		// called from application's main menu
		iSelected = cpPanel->GetFileSystem()->GetSelectedDirectoryStringList().count();
		qcbSearchIn->setEditText(cpPanel->GetPath());
	} else {
		// called from file marking
		qcbSearchIn->setEnabled(false);
		qpbBrowse->setEnabled(false);
		qpbDrives->setEnabled(false);
		qsbSubdirectoryDepth->setValue(0);
		qsbSubdirectoryDepth->setEnabled(false);
		qgbFoundFiles->setEnabled(false);
		qpbStart->setEnabled(false);
		qpbCancel->setText(tr("OK"));
	} // if else

	if (iSelected > 0) {
		qcbSearchInSelectedDirectories->setChecked(true);
	} else {
		qcbSearchInSelectedDirectories->setEnabled(false);
	} // if else

	qdteFrom->setDateTime(QDateTime::currentDateTime());
	qdteTo->setDateTime(QDateTime::currentDateTime());
	qcbOldType->addItems(qslOLDER_THAN);
	qcbFileSizeComparator->addItems(qslFILE_SIZE_COMPARATOR);
	qcbFileSizeType->addItems(qslFILE_SIZE_TYPE);
	qtwSearch->headerItem()->setHidden(true);
	qtwSavedFinds->headerItem()->setHidden(true);

	// refresh list of saved settings
	RefreshSavedSettings();

	// completer
	qcDirModel.setModel(new QDirModel(&qcDirModel));
	qcbSearchIn->setCompleter(&qcDirModel);
} // cFindFilesDialog

// store settings in sFindSettings structure
const cSettings::sFindSettings cFindFilesDialog::GetSettings() const
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
const void cFindFilesDialog::on_qcbDateTimeBetween_stateChanged(int state) const
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
const void cFindFilesDialog::on_qcbDateTimeNotOlderThan_stateChanged(int state) const
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
const void cFindFilesDialog::on_qcbFileSize_stateChanged(int state) const
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
const void cFindFilesDialog::on_qcbSearchInSelectedDirectories_stateChanged(int state) const
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
const void cFindFilesDialog::on_qgbFullText_toggled(bool on) const
{
	if (on) {
		qcbFullText->setFocus(Qt::OtherFocusReason);
	} // if
} // on_qgbFullText_toggled

// browse button is clicked on
const void cFindFilesDialog::on_qpbBrowse_clicked(bool checked /* false */)
{
	QString qsDirectory;

	qsDirectory = QFileDialog::getExistingDirectory(this, tr("Select directory"), "/");
	if (!qsDirectory.isEmpty()) {
		qcbSearchIn->setEditText(qsDirectory);
	} // if
} // on_qpbBrowse_clicked

// delete find is clicked on
const void cFindFilesDialog::on_qpbDeleteFind_clicked(bool checked /* false */) const
{
	csSettings->RemoveFindSettings(qtwSavedFinds->selectedItems().at(0)->text(0));
	RefreshSavedSettings();
} // on_qpbDeleteFind_clicked

// load find is clicked on
const void cFindFilesDialog::on_qpbLoadFind_clicked(bool checked /* false */) const
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
const void cFindFilesDialog::on_qpbSaveFind_clicked(bool checked /* false */)
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

// selected item changed in saved finds view
const void cFindFilesDialog::on_qtwSavedFinds_itemSelectionChanged() const
{
	if (qtwSavedFinds->selectedItems().count() > 0) {
		qpbLoadFind->setEnabled(true);
		qpbDeleteFind->setEnabled(true);
	} else {
		qpbLoadFind->setEnabled(false);
		qpbDeleteFind->setEnabled(false);
	} // if else
} // on_qtwSavedFinds_itemSelectionChanged

// refreshes list of saved settings
const void cFindFilesDialog::RefreshSavedSettings() const
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