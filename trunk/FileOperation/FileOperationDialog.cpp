#include "FileOperation/FileOperationDialog.h"

#include <QtGui/QFileDialog>

// constructor
cFileOperationDialog::cFileOperationDialog(QMainWindow *qmwParent, cSettings *csSettings)
{
	setParent(qmwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);

	// assign shortcut
	qpbEnqueue->setText(csSettings->GetShortcut(cSettings::OperationsCategory, qsSHORTCUT__OPERATIONS__DIALOG__ENQUEUE) + ' ' + qpbEnqueue->text());
	qpbEnqueue->setShortcut(QKeySequence(csSettings->GetShortcut(cSettings::OperationsCategory, qsSHORTCUT__OPERATIONS__DIALOG__ENQUEUE)));
} // cCopyMoveDialog

// Browse button clicked on
void cFileOperationDialog::on_qpbBrowse_clicked(bool checked /* false */)
{
	QString qsDirectory;

	qsDirectory = QFileDialog::getExistingDirectory(this, tr("Select directory"), qcbDestination->currentText());
	if (!qsDirectory.isEmpty()) {
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
cFileOperationDialog::eUserAction cFileOperationDialog::ShowDialog(const cFileRoutine::eOperation &eoOperation, const QString &qsCount, QString *qsDestination, QString *qsFilter)
{
	eUserAction euaAction;

	// prepare dialog
	switch (eoOperation) {
		case cFileRoutine::DeleteOperation:
			setWindowTitle(tr("Delete"));
			break;
		case cFileRoutine::CopyOperation:
			setWindowTitle(tr("Copy"));
			break;
		case cFileRoutine::MoveOperation:
			setWindowTitle(tr("Move"));
	} // switch
	qlCount->setText(qsCount);
	qcbDestination->setEditText(*qsDestination);
	qcbFilter->setEditText(*qsFilter);

	if (eoOperation == cFileRoutine::DeleteOperation) {
		qlCount->parentWidget()->setGeometry(qlCount->parentWidget()->geometry().x(),
																						qlCount->parentWidget()->geometry().y(),
																						qlCount->parentWidget()->geometry().width(),
																						qlCount->parentWidget()->geometry().height() - qcbDestination->height());
		setMaximumHeight(geometry().height() - qcbDestination->height());
		qcbFilter->setFocus(Qt::OtherFocusReason);
		delete qcbDestination;
		delete qpbBrowse;
	} else {
		qcbDestination->setFocus(Qt::OtherFocusReason);
	} // if else

	euaAction = static_cast<eUserAction>(exec());

	if (euaAction != CancelAction) {
		if (eoOperation != cFileRoutine::DeleteOperation) {
			*qsDestination = qcbDestination->currentText();
		} // if
		*qsFilter = qcbFilter->currentText();
	} // if

	return euaAction;
} // ShowDialog
