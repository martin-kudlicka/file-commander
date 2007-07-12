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

	qcbDestination->setEditText(qsDestination);
} // cUnpackFilesDialog

// OK button is clicked on
void cUnpackFilesDialog::on_qpbOK_clicked(bool checked /* false */)
{
	int iIndex;
	QString qsDestination, qsFilter;

	qsDestination = qcbDestination->currentText();
	iIndex = qcbDestination->findText(qsDestination);
	if (iIndex > 0) {
		qcbDestination->removeItem(iIndex);
	} // if
	if (iIndex != 0) {
		qcbDestination->insertItem(0, qsDestination);
		qcbDestination->setEditText(qsDestination);
	} // if
	qsFilter = qcbFilter->currentText();
	iIndex = qcbFilter->findText(qsFilter);
	if (iIndex > 0) {
		qcbFilter->removeItem(iIndex);
	} // if
	if (iIndex != 0) {
		qcbFilter->insertItem(0, qsFilter);
		qcbFilter->setEditText(qsFilter);
	} // if
	csSettings->SetComboBoxHistory(cSettings::UnpackFilesDestination, qcbDestination);
	csSettings->SetComboBoxHistory(cSettings::UnpackFilesFilter, qcbFilter);

	accept();
} // on_qpbOK_clicked