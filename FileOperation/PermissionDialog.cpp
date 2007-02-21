#include "FileOperation/PermissionDialog.h"

// constructor
cPermissionDialog::cPermissionDialog(QMainWindow *qmwParent)
{
	setParent(qmwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);
} // cPermissionDialog

// cancel button is clicked on
void cPermissionDialog::on_qpbCancel_clicked(bool checked /* false */)
{
	done(Cancel);
} // on_qpbCancel_clicked

// no button is clicked on
void cPermissionDialog::on_qpbNo_clicked(bool checked /* false */)
{
	done(No);
} // on_qpbNo_clicked

// no to all button is clicked on
void cPermissionDialog::on_qpbNoToAll_clicked(bool checked /* false */)
{
	done(NoToAll);
} // on_qpbNoToAll_clicked

// yes button is clicked on
void cPermissionDialog::on_qpbYes_clicked(bool checked /* false */)
{
	done(Yes);
} // on_qpbYes_clicked

// yes to all button is clicked on
void cPermissionDialog::on_qpbYesToAll_clicked(bool checked /* false */)
{
	done(YesToAll);
} // on_qpbYesToAll_clicked