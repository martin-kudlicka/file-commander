#include "FileOperation/CopyMoveDialog.h"

// constructor
cCopyMoveDialog::cCopyMoveDialog(QWidget *qwParent)
{
	setParent(qwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);
} // cCopyMoveDialog