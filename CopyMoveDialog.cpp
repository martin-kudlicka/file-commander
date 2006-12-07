#include "CopyMoveDialog.h"

// constructor
cCopyMoveDialog::cCopyMoveDialog()
{
	setupUi(this);
} // cCopyMoveDialog

// Cancel button clicked on
void cCopyMoveDialog::on_qpbCancel_clicked(bool checked /* false */)
{
	done(CancelAction);
} // on_qpbCancel_clicked

// Enqueue button clicked on
void cCopyMoveDialog::on_qpbEnqueue_clicked(bool checked /* false */)
{
	done(EnqueueAction);
} // on_qpbEnqueue_clicked

// OK button clicked on
void cCopyMoveDialog::on_qpbOK_clicked(bool checked /* false */)
{
	done(OkAction);
} // on_qpbOK_clicked

// shows copy or move dialog
cCopyMoveDialog::eUserAction cCopyMoveDialog::ShowDialog(const QString qsTitle, const QString qsCount, QString *qsDestination, QString *qsFilter)
{
	setWindowTitle(qsTitle);
	qlCount->setText(qsCount);
	qcbDestination->setEditText(*qsDestination);
	qcbFilter->setEditText(*qsFilter);
	return static_cast<eUserAction>(exec());
} // ShowDialog