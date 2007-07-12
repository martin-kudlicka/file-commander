#include "ArchiveOperation/UnpackFilesDialog.h"

#include <QtGui/QDirModel>

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
	qcbFilter->clearEditText();

	qcbDestination->setEditText(qsDestination);
} // cUnpackFilesDialog

// OK button is clicked on
void cUnpackFilesDialog::on_qpbOK_clicked(bool checked /* false */)
{
	int iIndex;

	iIndex = qcbDestination->findText(qcbDestination->currentText());
	if (iIndex > 0) {
		qcbDestination->removeItem(iIndex);
	} // if
	qcbDestination->insertItem(0, qcbDestination->currentText());
	iIndex = qcbFilter->findText(qcbFilter->currentText());
	if (iIndex > 0) {
		qcbFilter->removeItem(iIndex);
	} // if
	qcbFilter->insertItem(0, qcbFilter->currentText());
	csSettings->SetComboBoxHistory(cSettings::UnpackFilesDestination, qcbDestination);
	csSettings->SetComboBoxHistory(cSettings::UnpackFilesFilter, qcbFilter);

	accept();
} // on_qpbOK_clicked