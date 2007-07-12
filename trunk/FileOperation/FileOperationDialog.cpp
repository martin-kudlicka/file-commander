#include "FileOperation/FileOperationDialog.h"

#include <QtGui/QFileDialog>
#include <QtGui/QDirModel>

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
cFileOperationDialog::eUserAction cFileOperationDialog::ShowDialog(const cFileRoutine::eOperation &eoOperation, const QString &qsCount, QString *qsDestination, QString *qsFilter, const bool &bArchive /* false */)
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
	if (bArchive) {
		qpbEnqueue->setEnabled(false);
	} // if

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
		int iIndex;
		QString qsDestinationHistory, qsFilterHistory;

		if (eoOperation != cFileRoutine::DeleteOperation) {
			*qsDestination = qcbDestination->currentText();
		} // if
		*qsFilter = qcbFilter->currentText();

		// save history
		if (eoOperation != cFileRoutine::DeleteOperation) {
			qsDestinationHistory = qcbDestination->currentText();
			iIndex = qcbDestination->findText(qsDestinationHistory);
			if (iIndex > 0) {
				qcbDestination->removeItem(iIndex);
			} // if
			if (iIndex != 0) {
				qcbDestination->insertItem(0, qsDestinationHistory);
				qcbDestination->setEditText(qsDestinationHistory);
			} // if
		} // if
		qsFilterHistory = qcbFilter->currentText();
		iIndex = qcbFilter->findText(qsFilterHistory);
		if (iIndex > 0) {
			qcbFilter->removeItem(iIndex);
		} // if
		if (iIndex != 0) {
			qcbFilter->insertItem(0, qsFilterHistory);
			qcbFilter->setEditText(qsFilterHistory);
		} // if
		if (eoOperation != cFileRoutine::DeleteOperation) {
			csSettings->SetComboBoxHistory(cSettings::FileOperationDestination, qcbDestination);
		} // if
		csSettings->SetComboBoxHistory(cSettings::FileOperationFilter, qcbFilter);
	} // if

	return euaAction;
} // ShowDialog
