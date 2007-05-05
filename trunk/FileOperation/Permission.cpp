#include "FileOperation/Permission.h"

#include <QMessageBox>

// show conflict dialog
void cPermission::Show(const QString &qsFilename, const QString &qsInformation)
{
	eChoice ecResponse;
	QMessageBox qmbDialog;

	// prepare dialog
	qmbDialog.setIcon(QMessageBox::Warning);
	qmbDialog.setWindowTitle(tr("Permission"));
	qmbDialog.setText(tr("File %1\n%2 Continue?").arg(qsFilename).arg(qsInformation));
	qmbDialog.addButton(QMessageBox::Yes);
	qmbDialog.addButton(QMessageBox::No);
	qmbDialog.addButton(QMessageBox::YesToAll);
	qmbDialog.addButton(QMessageBox::NoToAll);
	qmbDialog.addButton(QMessageBox::Cancel);

	switch (qmbDialog.exec()) {
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