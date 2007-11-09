#include "FileSystem/Archive/UnpackFilesDialog.h"

#include <QtGui/QDirModel>
#include "GUICommon.h"

// constructor
cUnpackFilesDialog::cUnpackFilesDialog(QMainWindow *qmwParent, const QString &qsDestination, cSettings *csSettings)
{
	setParent(qmwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);

	this->csSettings = csSettings;

	// completer
	qcDestination.setModel(new QDirModel(&qcDestination));
	qcbDestination->setCompleter(&qcDestination);

	// history
	qcbDestination->addItems(csSettings->GetComboBoxHistory(cSettings::UnpackFilesDestination));
	qcbFilter->addItems(csSettings->GetComboBoxHistory(cSettings::UnpackFilesFilter));

	qcbDestination->setEditText(qsDestination);
} // cUnpackFilesDialog

// OK button is clicked on
const void cUnpackFilesDialog::on_qpbOK_clicked(bool checked /* false */)
{
	// save history
	cGUICommon::SetComboBoxHistory(cSettings::UnpackFilesDestination, qcbDestination, csSettings);
	cGUICommon::SetComboBoxHistory(cSettings::UnpackFilesFilter, qcbFilter, csSettings);

	accept();
} // on_qpbOK_clicked
