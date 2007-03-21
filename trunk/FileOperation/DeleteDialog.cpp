#include "FileOperation/DeleteDialog.h"

// constructor
cDeleteDialog::cDeleteDialog(QMainWindow *qmwParent)
{
	setParent(qmwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);
} // cDeleteDialog

// set source file
void cDeleteDialog::on_cDeleteDialog_SetSource(const QString &qsSource)
{
	qlSource->setText(qsSource);
} // on_cDeleteDialog_SetSource

// set overall maximum
void cDeleteDialog::on_cDeleteDialog_SetTotalMaximum(const qint64 &qi64Value)
{
	qpbTotal->setMaximum(qi64Value);
} // on_cDeleteDialog_SetTotalMaximum

// set overall progress
void cDeleteDialog::on_cDeleteDialog_SetTotalValue(const qint64 &qi64Value)
{
	qpbTotal->setValue(qi64Value);
} // on_cDeleteDialog_SetTotalValue

// Background button is clicked on
void cDeleteDialog::on_qpbBackground_clicked(bool checked /* false */)
{
	emit Background();
} // on_qpbBackground_clicked

// Cancel button is clicked on
void cDeleteDialog::on_qpbCancel_clicked(bool checked /* false */)
{
	emit Cancel();
} // on_qpbCancel_clicked