#include "FileSystem/DeleteNonEmptyDirectory.h"

#include <QtGui/QMessageBox>

// show conflict dialog
const void cDeleteNonEmptyDirectory::Show(const QString &qsDirectory) const
{
	eChoice ecResponse;

	switch (QMessageBox::warning(NULL, tr("Delete non empty directory"), tr("Delete %1 directory?").arg(qsDirectory),
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

	emit Finished(ecResponse);
} // Show
