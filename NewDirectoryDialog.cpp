#include "NewDirectoryDialog.h"

// constructor
cNewDirectoryDialog::cNewDirectoryDialog(QMainWindow *qmwParent)
{
	setParent(qmwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);

	qleDirectory->setFocus(Qt::OtherFocusReason);
} // cNewDirectoryDialog