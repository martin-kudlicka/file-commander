#include "FileOperation/Permission.h"

#include <QtGui/QMessageBox>

// show permission dialog (single thread)
cPermission::eChoice cPermission::Exec(const QString &qsFilename, const QString &qsInformation)
{
	return ShowDialog(qsFilename, qsInformation);
} // Exec

// show conflict dialog (multithread)
void cPermission::Show(const QString &qsFilename, const QString &qsInformation)
{
	eChoice ecResponse;

	ecResponse = ShowDialog(qsFilename, qsInformation);

	emit Finished(ecResponse);
} // Show

// show permission dialog
cPermission::eChoice cPermission::ShowDialog(const QString &qsFilename, const QString &qsInformation)
{
	eChoice ecResponse;

	switch (QMessageBox::warning(NULL, tr("Permission"), tr("File %1 %2 Continue?").arg(qsFilename).arg(qsInformation),
										  QMessageBox::Yes | QMessageBox::No | QMessageBox::YesToAll | QMessageBox::NoToAll | QMessageBox::Cancel)) {
		case QMessageBox::Yes:
			ecResponse = Yes;
			break;
		case QMessageBox::No:
			ecResponse = No;
			break;
		case QMessageBox::YesToAll:
			ecResponse = YesToAll;
			break;
		case QMessageBox::NoToAll:
			ecResponse = NoToAll;
			break;
		case QMessageBox::Cancel:
			ecResponse = Cancel;
		default:
			;
	} // switch

	return ecResponse;
} // ShowDialog