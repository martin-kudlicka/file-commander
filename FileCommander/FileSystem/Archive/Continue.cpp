#include "FileSystem/Archive/Continue.h"

#include <QtGui/QMessageBox>

// show continue dialog
const void cContinue::Show(const QString &qsError, const QString &qsFilename) const
{
	eChoice ecResponse;

	switch (QMessageBox::warning(NULL, tr("Continue"), tr("Error while extracting file\n%1:\n%2\nContinue?").arg(qsFilename).arg(qsError),
										  QMessageBox::Yes | QMessageBox::No | QMessageBox::YesToAll)) {
		case QMessageBox::Yes:
			ecResponse = Yes;
			break;
		case QMessageBox::No:
			ecResponse = No;
			break;
		case QMessageBox::YesToAll:
			ecResponse = YesToAll;
			break;
		default:
			;
	} // switch

	emit Finished(ecResponse);
} // Show
