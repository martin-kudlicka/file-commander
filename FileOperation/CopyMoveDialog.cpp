#include "FileOperation/CopyMoveDialog.h"

// constructor
cCopyMoveDialog::cCopyMoveDialog(QWidget *qwParent)
{
	setParent(qwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);
} // cCopyMoveDialog

void cCopyMoveDialog::on_qpbCancel_clicked(bool checked /* false */)
{
	emit Cancel();
} // on_qpbCancel_clicked