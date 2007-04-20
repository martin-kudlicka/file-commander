#include "FileOperation/RetryDialog.h"

// constructor
cRetryDialog::cRetryDialog(QMainWindow *qmwParent)
{
	setParent(qmwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);
} // cRetryDialog

// abort button is clicked on
void cRetryDialog::on_qpbAbort_clicked(bool checked /* false */)
{
	done(Abort);
} // on_qpbAbort_clicked

// retry button is clicked on
void cRetryDialog::on_qpRetry_clicked(bool checked /* false */)
{
	done(Retry);
} // on_qpRetry_clicked

// skip button is clicked on
void cRetryDialog::on_qpbSkip_clicked(bool checked /* false */)
{
	done(Skip);
} // on_qpbSkip_clicked

// skip all button is clicked on
void cRetryDialog::on_qpbSkipAll_clicked(bool checked /* false */)
{
	done(SkipAll);
} // on_qpbSkipAll_clicked