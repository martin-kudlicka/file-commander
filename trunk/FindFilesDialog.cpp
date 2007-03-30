#include "FindFilesDialog.h"

#include "FindFilesDialog/DrivesDialog.h"
#include <QFileDialog>

// constructor
cFindFilesDialog::cFindFilesDialog(QMainWindow *qmwParent, cPanel *cpPanel, QFileInfoList &qfilSelectedDirectories)
{
	setParent(qmwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);

	this->cpPanel = cpPanel;
	this->qfilSelectedDirectories = qfilSelectedDirectories;

	if (qfilSelectedDirectories.count() > 0) {
		qcbSearchInSelectedDirectories->setChecked(true);
	} else {
		qcbSearchInSelectedDirectories->setEnabled(false);
	} // if else
	qtwSearch->headerItem()->setHidden(true);

	// fill some options
	qcbSearchIn->insertItem(0, cpPanel->GetPath());
} // cFindFilesDialog

// catch dialog close
void cFindFilesDialog::closeEvent(QCloseEvent *event)
{
	if (!bStop) {
		bStop = true;
	} // if
} // closeEvent

// check search conditions on found file
bool cFindFilesDialog::ConditionsSuit(const QFileInfo &qfFile)
{
	bool bOk;
	int iI;
	QStringList qslSearchFor;

	// search for
	bOk = false;
	qslSearchFor = qcbSearchFor->currentText().split(';');
	for (iI = 0; iI < qslSearchFor.count(); iI++) {
		QRegExp qreExpression(qslSearchFor.at(iI), Qt::CaseInsensitive);

		if (qcbSearchForRegularExpression->isChecked()) {
			// regular expression
			if (qreExpression.indexIn(qfFile.fileName()) != -1) {
				bOk = true;
				break;
			} // if
		} else {
			// wildcard
			qreExpression.setPatternSyntax(QRegExp::Wildcard);
			if (qreExpression.exactMatch(qfFile.fileName())) {
				bOk = true;
				break;
			} // if
		} // if else
	} // for
	if (!bOk) {
		return false;
	} // if

	return true;
} // ConditionsSuit

// search only in selected directories
void cFindFilesDialog::on_qcbSearchInSelectedDirectories_stateChanged(int state)
{
	if (state == Qt::Checked) {
		qcbSearchIn->setEnabled(false);
		qpbBrowse->setEnabled(false);
		qpbDrives->setEnabled(false);
	} else {
		qcbSearchIn->setEnabled(true);
		qpbBrowse->setEnabled(true);
		qpbDrives->setEnabled(true);
	} // if else
} // on_qcbSearchInSelectedDirectories_stateChanged

// browse button is clicked on
void cFindFilesDialog::on_qpbBrowse_clicked(bool checked /* false */)
{
	QString qsDirectory;

	qsDirectory = QFileDialog::getExistingDirectory(this, tr("Select a directory"), "/");
	if (qsDirectory != "") {
		qcbSearchIn->insertItem(0, qsDirectory);
		qcbSearchIn->setCurrentIndex(0);
	} // if
} // on_qpbBrowse_clicked

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

		qcbSearchIn->insertItem(0, qsPath);
		qcbSearchIn->setCurrentIndex(0);
	} // if

	cddDrives->deleteLater();
} // on_qpbDrives_clicked

// start button is clicked on
void cFindFilesDialog::on_qpbStart_clicked(bool checked /* false */)
{
	int iI;
	QFileInfoList qfilDirectories;

	qpbStart->setEnabled(false);
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

	// search through directories
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

				qfilSearch += qfilDirContent.at(iI);
				qtwiItem = new QTreeWidgetItem(qtwSearch);
				qtwiItem->setText(0, qfilDirContent.at(iI).filePath());
				QApplication::processEvents(QEventLoop::AllEvents);
			} // if
		} // for

		// add found directories
		for (iI = 0; iI < qfilDirContent.count(); iI++) {
			if (qfilDirContent.at(iI).isDir()) {
				qfilDirectories.append(qfilDirContent.at(iI));
			} // if
		} // for
	} // while

	qpbStop->setEnabled(false);
	qpbStart->setEnabled(true);
} // on_qpbStart_clicked

// stop button is clicked on
void cFindFilesDialog::on_qpbStop_clicked(bool checked /* false */)
{
	bStop = true;
} // on_qpbStop_clicked