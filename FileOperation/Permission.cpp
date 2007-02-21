#include "FileOperation/Permission.h"

// destructor
cPermission::~cPermission()
{
	cpdDialog->deleteLater();
} // ~cPermission

// constructor
cPermission::cPermission(QMainWindow *qmwParent)
{
	cpdDialog = new cPermissionDialog(qmwParent);
} // cPermission

// show conflict dialog
void cPermission::Show(const QString &qsFilename, const QString &qsInformation)
{
	cPermissionDialog::eChoice ecResponse;

	cpdDialog->qlFilename->setText(qsFilename);
	cpdDialog->qlInformation->setText(qsInformation + tr(" Continue?"));

	ecResponse = static_cast<cPermissionDialog::eChoice>(cpdDialog->exec());
	emit Finished(ecResponse);
} // Show