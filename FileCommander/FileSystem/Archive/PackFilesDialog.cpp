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

	// completer
	qcDestination.setModel(new QDirModel(&qcDestination));
	qcbDestination->setCompleter(&qcDestination);

	// history
	qcbDestination->addItems(csSettings->GetComboBoxHistory(cSettings::PackFilesDestination));

	// setup GUI
	qtwPlugins->header()->hide();
} // cUnpackFilesDialog

// OK button is clicked on
const void cPackFilesDialog::on_qpbOK_clicked(bool checked /* false */)
{
	// save history
	cGUICommon::SetComboBoxHistory(cSettings::PackFilesDestination, qcbDestination, csSettings);

	accept();
} // on_qpbOK_clicked