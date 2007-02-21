#include "FileOperation/RenameDialog.h"

// constructor
cRenameDialog::cRenameDialog(QMainWindow *qmwParent)
{
	setParent(qmwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);
} // cRenameDialog