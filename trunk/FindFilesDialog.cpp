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
	qdteFrom->setDateTime(QDateTime::currentDateTime());
	qdteTo->setDateTime(QDateTime::currentDateTime());
	qcbOldType->addItems(qslOLDER_THAN);
	qcbFileSizeComparator->addItems(qsFILE_SIZE_COMPARATOR);
	qcbFileSizeType->addItems(qsFILE_SIZE_TYPE);
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

			// Rabin-Karp algorithm
			int iD, iDM, iH1, iH2, iI, iQ;

			iD = 32;
			iQ = 33554393;

			iDM = 1;
			for (iI = 0; iI < qsPattern.length() - 1; iI++) {
				iDM = (iD * iDM) % iQ;
			} // for
			iH1 = 0;
			iH2 = 0;
			qbaFile = qfFile.peek(qsPattern.length());
			for (iI = 0; iI < qsPattern.length(); iI++) {
				iH1 = (iH1 * iD + qsPattern.at(iI).toAscii()) % iQ;
				iH2 = (iH2 * iD + qbaFile.at(iI)) % iQ;
			} // for

			iI = 0;
			while (iH1 != iH2 && iI < qfFile.size() - qsPattern.length()) {
				QByteArray qbaPart;

				qbaPart = qfFile.peek(qsPattern.length() + 1);
				iH2 = (iH2 + iD * iQ - qbaPart.at(0) * iDM) % iQ;
				iH2 = (iH2 * iD + qbaPart.at(qsPattern.length())) % iQ;

				iI++;
				qfFile.seek(iI);
			} // while

			qfFile.close();

			if ((iH1 != iH2) ^ qcbFullTextNotContainingText->isChecked()) {
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
	int iDepth, iI;
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

					qfilSearch += qfilDirContent.at(iI);
					qtwiItem = new QTreeWidgetItem(qtwSearch);
					qtwiItem->setText(0, qfilDirContent.at(iI).filePath());
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
} // on_qpbStart_clicked

// stop button is clicked on
void cFindFilesDialog::on_qpbStop_clicked(bool checked /* false */)
{
	bStop = true;
} // on_qpbStop_clicked