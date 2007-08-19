#include "FileSystem/DeleteDialog.h"

// constructor
cDeleteDialog::cDeleteDialog(QMainWindow *qmwParent)
{
	setParent(qmwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);
} // cDeleteDialog

// set source file
const void cDeleteDialog::on_cDeleteDialog_SetSource(const QString &qsSource) const
{
	qlSource->setText(qsSource);
} // on_cDeleteDialog_SetSource

// set overall maximum
const void cDeleteDialog::on_cDeleteDialog_SetTotalMaximum(const qint64 &qi64Value) const
{
	qpbTotal->setMaximum(qi64Value);
} // on_cDeleteDialog_SetTotalMaximum

// set overall progress
const void cDeleteDialog::on_cDeleteDialog_SetTotalValue(const qint64 &qi64Value) const
{
	qpbTotal->setValue(qi64Value);
} // on_cDeleteDialog_SetTotalValue

// Background button is clicked on
const void cDeleteDialog::on_qpbBackground_clicked(bool checked /* false */) const
{
	emit Background();
} // on_qpbBackground_clicked

// Cancel button is clicked on
const void cDeleteDialog::on_qpbCancel_clicked(bool checked /* false */) const
{
	emit Cancel();
} // on_qpbCancel_clicked
