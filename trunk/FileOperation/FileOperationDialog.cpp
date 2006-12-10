#include "FileOperation/FileOperationDialog.h"

#include <QFileDialog>

// constructor
cFileOperationDialog::cFileOperationDialog(QWidget *qwParent)
{
	setParent(qwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);
} // cCopyMoveDialog

// Browse button clicked on
void cFileOperationDialog::on_qpbBrowse_clicked(bool checked /* false */)
{
	QString qsDirectory;

	qsDirectory = QFileDialog::getExistingDirectory(this, tr("Select directory"), qcbDestination->currentText());
	if (qsDirectory != "") {
		qcbDestination->setEditText(qsDirectory);
	} // if
} // on_qpbBrowse_clicked

// Cancel button clicked on
void cFileOperationDialog::on_qpbCancel_clicked(bool checked /* false */)
{
	done(CancelAction);
} // on_qpbCancel_clicked

// Enqueue button clicked on
void cFileOperationDialog::on_qpbEnqueue_clicked(bool checked /* false */)
{
	done(EnqueueAction);
} // on_qpbEnqueue_clicked

// OK button clicked on
void cFileOperationDialog::on_qpbOK_clicked(bool checked /* false */)
{
	done(OkAction);
} // on_qpbOK_clicked

// shows copy or move dialog
cFileOperationDialog::eUserAction cFileOperationDialog::ShowDialog(const QString qsTitle, const QString qsCount, QString *qsDestination, QString *qsFilter)
{
	eUserAction euaAction;

	setWindowTitle(qsTitle);
	qlCount->setText(qsCount);
	qcbDestination->setEditText(*qsDestination);
	qcbDestination->setFocus(Qt::ActiveWindowFocusReason);
	qcbFilter->setEditText(*qsFilter);

	euaAction = static_cast<eUserAction>(exec());

	if (euaAction != CancelAction) {
		*qsDestination = qcbDestination->currentText();
		*qsFilter = qcbFilter->currentText();
	} // if

	return euaAction;
} // ShowDialog