#include "FileControl/SelectDriveDialog.h"

// constructor
cSelectDriveDialog::cSelectDriveDialog(QMainWindow *qmwParent, const QStringList &qslDrives, const QString &qsDrive)
{
	setParent(qmwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);

	// fill drives
	qcbDrive->addItems(qslDrives);

	// select currently selected drive
	qcbDrive->setCurrentIndex(qcbDrive->findText(qsDrive));
	if (qcbDrive->currentIndex() == -1) {
		qcbDrive->setCurrentIndex(0);
	} // if

	qcbDrive->setFocus(Qt::OtherFocusReason);
} // cSelectDriveDialog
