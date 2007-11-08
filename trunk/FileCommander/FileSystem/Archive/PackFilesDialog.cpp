#include "FileSystem/Archive/PackFilesDialog.h"

#include <QtGui/QDirModel>
#include <QtGui/QHeaderView>

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
	int iIndex;
	QString qsDestination;

	qsDestination = qcbDestination->currentText();
	iIndex = qcbDestination->findText(qsDestination);
	if (iIndex > 0) {
		qcbDestination->removeItem(iIndex);
	} // if
	if (iIndex != 0) {
		qcbDestination->insertItem(0, qsDestination);
		qcbDestination->setEditText(qsDestination);
	} // if
	csSettings->SetComboBoxHistory(cSettings::UnpackFilesDestination, qcbDestination);

	accept();
} // on_qpbOK_clicked