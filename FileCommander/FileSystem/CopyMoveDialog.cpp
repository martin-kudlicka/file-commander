#include "FileSystem/CopyMoveDialog.h"

// constructor
cCopyMoveDialog::cCopyMoveDialog(QMainWindow *qmwParent)
{
	setParent(qmwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);
} // cCopyMoveDialog

// set maximum for current file
const void cCopyMoveDialog::on_cCopyMove_SetCurrentMaximum(const qint64 &qi64Value) const
{
	qpbCurrent->setMaximum(qi64Value);
} // on_cCopyMove_SetCurrentMaximum

// set progress for current file
const void cCopyMoveDialog::on_cCopyMove_SetCurrentValue(const qint64 &qi64Value) const
{
	qpbCurrent->setValue(qi64Value);
} // on_cCopyMove_SetCurrentValue

// set destination file
const void cCopyMoveDialog::on_cCopyMove_SetDestination(const QString &qsDestination) const
{
	qlDestination->setText(qsDestination);
} // on_cCopyMove_SetDestination

// set source file
const void cCopyMoveDialog::on_cCopyMove_SetSource(const QString &qsSource) const
{
	qlSource->setText(qsSource);
} // on_cCopyMove_SetSource

// set overall maximum
const void cCopyMoveDialog::on_cCopyMove_SetTotalMaximum(const qint64 &qi64Value) const
{
	qpbTotal->setMaximum(qi64Value);
} // on_cCopyMove_SetTotalMaximum

// set overall progress
const void cCopyMoveDialog::on_cCopyMove_SetTotalValue(const qint64 &qi64Value) const
{
	qpbTotal->setValue(qi64Value);
} // on_cCopyMove_SetTotalValue

// Background button is clicked on
const void cCopyMoveDialog::on_qpbBackground_clicked(bool checked /* false */) const
{
	emit Background();
} // on_qpbBackground_clicked

// Cancel button is clicked on
const void cCopyMoveDialog::on_qpbCancel_clicked(bool checked /* false */) const
{
	emit Cancel();
} // on_qpbCancel_clicked
