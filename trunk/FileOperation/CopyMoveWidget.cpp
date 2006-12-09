#include "FileOperation/CopyMoveWidget.h"

// constructor
cCopyMoveWidget::cCopyMoveWidget()
{
	setupUi(this);
} // cCopyMoveWidget

// set maximum for current file
void cCopyMoveWidget::on_cCopyMove_SetCurrentMaximum(const qint64 qi64Value)
{
	qpbCurrent->setMaximum(qi64Value);
} // on_cCopyMove_SetCurrentMaximum

// set progress for current file
void cCopyMoveWidget::on_cCopyMove_SetCurrentValue(const qint64 qi64Value)
{
	qpbCurrent->setValue(qi64Value);
} // on_cCopyMove_SetCurrentValue

// set destination file
void cCopyMoveWidget::on_cCopyMove_SetDestination(const QString qsDestination)
{
	qlDestination->setText(qsDestination);
} // on_cCopyMove_SetDestination

// set source file
void cCopyMoveWidget::on_cCopyMove_SetSource(const QString qsSource)
{
	qlSource->setText(qsSource);
} // on_cCopyMove_SetSource

// set overall maximum
void cCopyMoveWidget::on_cCopyMove_SetTotalMaximum(const qint64 qi64Value)
{
	qpbTotal->setMaximum(qi64Value);
} // on_cCopyMove_SetTotalMaximum

// set overall progress
void cCopyMoveWidget::on_cCopyMove_SetTotalValue(const qint64 qi64Value)
{
	qpbTotal->setValue(qi64Value);
} // on_cCopyMove_SetTotalValue

// copy button is clicked on
void cCopyMoveWidget::on_qpbCancel_clicked(bool checked /* false */)
{
	emit Cancel();
} // on_qpbCancel_clicked