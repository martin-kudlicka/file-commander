#include "FileOperation/DeleteWidget.h"

// constructor
cDeleteWidget::cDeleteWidget()
{
	setupUi(this);
} // cCopyMoveWidget

// set source file
void cDeleteWidget::on_cDeleteWidget_SetSource(const QString &qsSource)
{
	qlSource->setText(qsSource);
} // on_cCopyMove_SetSource

// set overall maximum
void cDeleteWidget::on_cDeleteWidget_SetTotalMaximum(const qint64 &qi64Value)
{
	qpbTotal->setMaximum(qi64Value);
} // on_cCopyMove_SetTotalMaximum

// set overall progress
void cDeleteWidget::on_cDeleteWidget_SetTotalValue(const qint64 &qi64Value)
{
	qpbTotal->setValue(qi64Value);
} // on_cCopyMove_SetTotalValue

// copy button is clicked on
void cDeleteWidget::on_qpbCancel_clicked(bool checked /* false */)
{
	emit Cancel();
} // on_qpbCancel_clicked