#include "FileOperation/Retry.h"

// destructor
cRetry::~cRetry()
{
	crdDialog->deleteLater();
} // ~cRetry

// constructor
cRetry::cRetry(QMainWindow *qmwParent)
{
	crdDialog = new cRetryDialog(qmwParent);
} // cRetry

// show retry dialog
void cRetry::Show(QWidget *qwParent, const QString &qsInformation, const QString &qsFilename)
{
	cRetryDialog::eChoice ecResponse;

	//crdDialog->setParent(qwParent);
	crdDialog->qlInformation->setText(qsInformation);
	crdDialog->qlFilename->setText(qsFilename);

	ecResponse = static_cast<cRetryDialog::eChoice>(crdDialog->exec());

	emit Finished(ecResponse);
} // Show		