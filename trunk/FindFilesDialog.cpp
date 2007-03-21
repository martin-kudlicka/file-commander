#include "FindFilesDialog.h"

#include "FindFilesDialog/DrivesDialog.h"

// constructor
cFindFilesDialog::cFindFilesDialog(QMainWindow *qmwParent, cPanel *cpPanel)
{
	setParent(qmwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);

	this->cpPanel = cpPanel;

	// fill some options
	qcbSearchIn->insertItem(0, cpPanel->GetPath());
} // cFindFilesDialog

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