#include "FileSystem/Archive/PackFilesDialog.h"

#include <QtGui/QDirModel>
#include <QtGui/QHeaderView>
#include "GUICommon.h"

// constructor
cPackFilesDialog::cPackFilesDialog(QMainWindow *qmwParent, cSettings *csSettings)
{
	setParent(qmwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);

	this->csSettings = csSettings;

	// shortcut
	qpbEnqueue->setText(csSettings->GetShortcut(cSettings::OperationsCategory, qsSHORTCUT__OPERATIONS__DIALOG__ENQUEUE) + ' ' + qpbEnqueue->text());
	qpbEnqueue->setShortcut(QKeySequence(csSettings->GetShortcut(cSettings::OperationsCategory, qsSHORTCUT__OPERATIONS__DIALOG__ENQUEUE)));

	// completer
	qcDestination.setModel(new QDirModel(&qcDestination));
	qcbDestination->setCompleter(&qcDestination);

	// history
	qcbDestination->addItems(csSettings->GetComboBoxHistory(cSettings::PackFilesDestination));
	qcbFilter->addItems(csSettings->GetComboBoxHistory(cSettings::PackFilesFilter));

	// setup GUI
	qtwPlugins->header()->hide();
} // cUnpackFilesDialog

// Cancel button clicked on
const void cPackFilesDialog::on_qpbCancel_clicked(bool checked /* false */)
{
	done(CancelAction);
} // on_qpbCancel_clicked

// Enqueue button clicked on
const void cPackFilesDialog::on_qpbEnqueue_clicked(bool checked /* false */)
{
	done(EnqueueAction);
} // on_qpbEnqueue_clicked

// OK button is clicked on
const void cPackFilesDialog::on_qpbOK_clicked(bool checked /* false */)
{
	// save history
	cGUICommon::SetComboBoxHistory(cSettings::PackFilesDestination, qcbDestination, csSettings);
	cGUICommon::SetComboBoxHistory(cSettings::PackFilesFilter, qcbFilter, csSettings);

	done(OkAction);
} // on_qpbOK_clicked
