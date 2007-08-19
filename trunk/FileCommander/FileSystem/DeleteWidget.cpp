#include "FileSystem/DeleteWidget.h"

// constructor
cDeleteWidget::cDeleteWidget()
{
	setupUi(this);
} // cCopyMoveWidget

// set source file
const void cDeleteWidget::on_cDeleteWidget_SetSource(const QString &qsSource) const
{
	qlSource->setText(qsSource);
} // on_cCopyMove_SetSource

// set overall maximum
const void cDeleteWidget::on_cDeleteWidget_SetTotalMaximum(const qint64 &qi64Value) const
{
	qpbTotal->setMaximum(qi64Value);
} // on_cCopyMove_SetTotalMaximum

// set overall progress
const void cDeleteWidget::on_cDeleteWidget_SetTotalValue(const qint64 &qi64Value) const
{
	qpbTotal->setValue(qi64Value);
} // on_cCopyMove_SetTotalValue

// copy button is clicked on
const void cDeleteWidget::on_qpbCancel_clicked(bool checked /* false */) const
{
	emit Cancel();
} // on_qpbCancel_clicked
