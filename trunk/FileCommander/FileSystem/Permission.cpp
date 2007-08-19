#include "FileSystem/Permission.h"

#include <QtGui/QMessageBox>

// show permission dialog
const void cPermission::Show(const QString &qsFilename, const QString &qsInformation) const
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
			break;
		default:
			ecResponse = Cancel;
	} // switch

	emit Finished(ecResponse);
} // Show
