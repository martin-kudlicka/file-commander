#include "FileControl/FileOperationDialog.h"

#include <QtGui/QFileDialog>
#include <QtGui/QDirModel>
#include <QtGui/QLineEdit>
#include "GUICommon.h"

// constructor
cFileOperationDialog::cFileOperationDialog(QMainWindow *qmwParent, cSettings *csSettings)
{
	setParent(qmwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);

	this->csSettings = csSettings;

	// assign shortcut
	qpbEnqueue->setText(csSettings->GetShortcut(cSettings::OperationsCategory, qsSHORTCUT__OPERATIONS__DIALOG__ENQUEUE) + ' ' + qpbEnqueue->text());
	qpbEnqueue->setShortcut(QKeySequence(csSettings->GetShortcut(cSettings::OperationsCategory, qsSHORTCUT__OPERATIONS__DIALOG__ENQUEUE)));

	// completer
	qcDestination.setModel(new QDirModel(&qcDestination));
	qcbDestination->setCompleter(&qcDestination);

	// history
	qcbDestination->addItems(csSettings->GetComboBoxHistory(cSettings::FileOperationDestination));
	qcbFilter->addItems(csSettings->GetComboBoxHistory(cSettings::FileOperationFilter));
} // cCopyMoveDialog

// Browse button clicked on
const void cFileOperationDialog::on_qpbBrowse_clicked(bool checked /* false */)
{
	QString qsDirectory;

	qsDirectory = QFileDialog::getExistingDirectory(this, tr("Select directory"), qcbDestination->currentText());
	if (!qsDirectory.isEmpty()) {
		qcbDestination->setEditText(qsDirectory);
	} // if
} // on_qpbBrowse_clicked

// Cancel button clicked on
const void cFileOperationDialog::on_qpbCancel_clicked(bool checked /* false */)
{
	done(CancelAction);
} // on_qpbCancel_clicked

// Enqueue button clicked on
const void cFileOperationDialog::on_qpbEnqueue_clicked(bool checked /* false */)
{
	done(EnqueueAction);
} // on_qpbEnqueue_clicked

// OK button clicked on
const void cFileOperationDialog::on_qpbOK_clicked(bool checked /* false */)
{
	done(OkAction);
} // on_qpbOK_clicked

// shows copy or move dialog
const cFileOperationDialog::eUserAction cFileOperationDialog::ShowDialog(const eOperation &eoOperation, const QString &qsCount, QString *qsDestination, QString *qsFilter)
{
	eUserAction euaAction;

	// prepare dialog
	switch (eoOperation) {
		case DeleteOperation:
			setWindowTitle(tr("Delete"));
			break;
		case CopyOperation:
			setWindowTitle(tr("Copy"));
			break;
		case MoveOperation:
			setWindowTitle(tr("Move"));
	} // switch
	qlCount->setText(qsCount);
	qcbDestination->setEditText(*qsDestination);

	if (eoOperation == DeleteOperation) {
		setMaximumHeight(geometry().height() - qcbDestination->height());
		qcbFilter->setFocus(Qt::OtherFocusReason);
		delete qcbDestination;
		delete qpbBrowse;
	} else {
		qcbDestination->lineEdit()->setSelection(0, qcbDestination->currentText().length());
		qcbDestination->setFocus(Qt::OtherFocusReason);
	} // if else

	euaAction = static_cast<eUserAction>(exec());

	if (euaAction != CancelAction) {
		if (eoOperation != DeleteOperation) {
			*qsDestination = qcbDestination->currentText();
		} // if
		*qsFilter = qcbFilter->currentText();

		// save history
		if (eoOperation != DeleteOperation) {
			cGUICommon::SetComboBoxHistory(cSettings::FileOperationDestination, qcbDestination, csSettings);
		} // if
		cGUICommon::SetComboBoxHistory(cSettings::FileOperationFilter, qcbFilter, csSettings);
	} // if

	return euaAction;
} // ShowDialog
