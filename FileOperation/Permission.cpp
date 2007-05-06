#include "FileOperation/Permission.h"

#include <QMessageBox>

// show conflict dialog
void cPermission::Show(const QString &qsFilename, const QString &qsInformation)
{
	eChoice ecResponse;

	switch (QMessageBox::warning(NULL, tr("Permission"), tr("File %1 %2 Continue?").arg(qsFilename).arg(qsInformation),
										  QMessageBox::Yes | QMessageBox::No | QMessageBox::YesToAll | QMessageBox::NoToAll | QMessageBox::Cancel)) {
		case QMessageBox::Yes:			ecResponse = Yes;
												break;
		case QMessageBox::No:			ecResponse = No;
												break;
		case QMessageBox::YesToAll:	ecResponse = YesToAll;
												break;
		case QMessageBox::NoToAll:		ecResponse = NoToAll;
												break;
		case QMessageBox::Cancel:		ecResponse = Cancel;
												break;
	} // switch

	emit Finished(ecResponse);
} // Show