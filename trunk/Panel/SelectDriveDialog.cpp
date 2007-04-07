#include "Panel/SelectDriveDialog.h"

// constructor
cSelectDriveDialog::cSelectDriveDialog(QMainWindow *qmwParent, const QMap<QString, cFileRoutine::sDriveInfo> &qmDrives, const QString &qsDrive)
{
	setParent(qmwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);

	// fill drives
	QMapIterator<QString, cFileRoutine::sDriveInfo> qmiDrive(qmDrives);
	while (qmiDrive.hasNext()) {
		qmiDrive.next();
		qcbDrive->addItem(qmiDrive.key());
	} // while

	// select currently selected drive
	qcbDrive->setCurrentIndex(qcbDrive->findText(qsDrive));
	if (qcbDrive->currentIndex() == -1) {
		qcbDrive->setCurrentIndex(0);
	} // if

	qcbDrive->setFocus(Qt::OtherFocusReason);
} // cSelectDriveDialog