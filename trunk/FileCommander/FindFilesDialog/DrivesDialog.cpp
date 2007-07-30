#include "FindFilesDialog/DrivesDialog.h"

cDrivesDialog::cDrivesDialog(QDialog *qdParent, const QStringList &qslDrives)
{
	int iI;

	setParent(qdParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);

	// remove top header from tree widget
	qtwDrives->headerItem()->setHidden(true);

	// fill drives info
	for (iI = 0; iI < qslDrives.count(); iI++) {
		QTreeWidgetItem *qtwiDrive;

		qtwiDrive = new QTreeWidgetItem(qtwDrives);
		qtwiDrive->setText(0, qslDrives.at(iI));
	} // for
} // cDrivesDialog

// selected drives changed
const void cDrivesDialog::on_qtwDrives_itemSelectionChanged() const
{
	if (qtwDrives->selectedItems().count() > 0) {
		qpbOK->setEnabled(true);
	} else {
		qpbOK->setEnabled(false);
	} // if else
} // on_qtwDrives_itemSelectionChanged
