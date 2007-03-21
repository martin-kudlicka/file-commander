#include "FindFilesDialog.h"

#include "FindFilesDialog/DrivesDialog.h"

// constructor
cFindFilesDialog::cFindFilesDialog(QMainWindow *qmwParent, cPanel *cpPanel)
{
	setParent(qmwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);

	this->cpPanel = cpPanel;

	qtwSearch->headerItem()->setHidden(true);

	// fill some options
	qcbSearchIn->insertItem(0, cpPanel->GetPath());
} // cFindFilesDialog

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
		QRegExp qreExpression(qslSearchFor.at(iI), Qt::CaseInsensitive, QRegExp::Wildcard);
		if (qreExpression.exactMatch(qfFile.fileName())) {
			bOk = true;
			break;
		} // if
	} // for
	if (!bOk) {
		return false;
	} // if

	return true;
} // ConditionsSuit

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
	QStringList qslPaths;

	// add source path to directories to search in
	qslPaths = qcbSearchIn->currentText().split(';');
	for (iI = 0; iI < qslPaths.count(); iI++) {
		qfilDirectories.append(qslPaths.at(iI));
	} // for

	// clear previous search
	qfilSearch.clear();
	qtwSearch->clear();

	// search through directories
	while (!qfilDirectories.isEmpty()) {
		QFileInfo qfiDir;
		QFileInfoList qfilDirContent;

		// get content of directory
		qfiDir = qfilDirectories.takeAt(0);
		qfilDirContent = cFileRoutine::GetDirectoryContent(qfiDir.filePath(), QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);

		// check conditions
		for (iI = 0; iI < qfilDirContent.count(); iI++) {
			if (ConditionsSuit(qfilDirContent.at(iI))) {
				// file suit conditions
				QTreeWidgetItem *qtwiItem;

				qfilSearch += qfilDirContent.at(iI);
				qtwiItem = new QTreeWidgetItem(qtwSearch);
				qtwiItem->setText(0, qfilDirContent.at(iI).filePath());
			} // if
		} // for

		// add found directories
		for (iI = 0; iI < qfilDirContent.count(); iI++) {
			if (qfilDirContent.at(iI).isDir()) {
				qfilDirectories.append(qfilDirContent.at(iI));
			} // if
		} // for
	} // while
} // on_qpbStart_clicked