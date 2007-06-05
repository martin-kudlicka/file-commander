#include "FileOperation/Retry.h"

#include <QtGui/QMessageBox>
#include <QtGui/QPushButton>

// show retry dialog (single thread)
cRetry::eChoice cRetry::Exec(const QString &qsInformation, const QString &qsFilename)
{
	return ShowDialog(qsInformation, qsFilename);
} // Exec

// show retry dialog (multithread)
void cRetry::Show(const QString &qsInformation, const QString &qsFilename)
{
	eChoice ecResponse;

	ecResponse = ShowDialog(qsInformation, qsFilename);

	emit Finished(ecResponse);
} // Show

// show retry dialog
cRetry::eChoice cRetry::ShowDialog(const QString &qsInformation, const QString &qsFilename)
{
	eChoice ecResponse;
	QMessageBox qmbDialog;
	QPushButton *qpbAbort, *qpbRetry, *qpbSkip, *qpbSkipAll;

	// prepare dialog
	qmbDialog.setIcon(QMessageBox::Warning);
	qmbDialog.setWindowTitle(tr("Retry"));
	qmbDialog.setText(tr("%1\n%2").arg(qsInformation).arg(qsFilename));
	qpbSkip = qmbDialog.addButton(tr("&Skip"), QMessageBox::NoRole);
	qpbRetry = qmbDialog.addButton(tr("&Retry"), QMessageBox::YesRole);
	qpbSkipAll = qmbDialog.addButton(tr("S&kip all"), QMessageBox::NoRole);
	qpbAbort = qmbDialog.addButton(tr("&Abort"), QMessageBox::RejectRole);

	qmbDialog.exec();

	if (qmbDialog.clickedButton() == qpbSkip) {
		ecResponse = Skip;
	} else {
		if (qmbDialog.clickedButton() == qpbRetry) {
			ecResponse = Retry;
		} else {
			if (qmbDialog.clickedButton() == qpbSkipAll) {
				ecResponse = SkipAll;
			} else {
				ecResponse = Abort;
			} // if else
		} // if else
	} // if else

	return ecResponse;
} // ShowDialog