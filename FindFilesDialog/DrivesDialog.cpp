#include "FindFilesDialog/DrivesDialog.h"

cDrivesDialog::cDrivesDialog(QDialog *qdParent, const QMap<QString, cFileRoutine::sDriveInfo> &qmDrives)
{
	setParent(qdParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);

	// remove top header from tree widget
	qtwDrives->headerItem()->setHidden(true);

	// fill drives info
	QMapIterator<QString, cFileRoutine::sDriveInfo> qmiDrives(qmDrives);
	while (qmiDrives.hasNext()) {
		QTreeWidgetItem *qtwiDrive;

		qmiDrives.next();
		qtwiDrive = new QTreeWidgetItem(qtwDrives);
		qtwiDrive->setText(0, qmiDrives.key());
	} // while
} // cDrivesDialog

// selected drives changed
void cDrivesDialog::on_qtwDrives_itemSelectionChanged()
{
	if (qtwDrives->selectedItems().count() > 0) {
		qpbOK->setEnabled(true);
	} else {
		qpbOK->setEnabled(false);
	} // if else
} // on_qtwDrives_itemSelectionChanged