#include "Options/NewFavouriteDirectoryDialog.h"

// constructor
cNewFavouriteDirectoryDialog::cNewFavouriteDirectoryDialog(QDialog *qdParent, const eType &etType, QString *qsDirectory, ePosition *epPosition, bool *bInSubmenu)
{
	setParent(qdParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);

	if (etType == Directory) {
		setWindowTitle(tr("New favourite directory"));
	} else {
		setWindowTitle(tr("New submenu"));
	} // if else
	if (*epPosition == Disabled) {
		qgbPosition->setEnabled(false);
		qcbPutInSubmenu->setEnabled(false);
	} else {
		qrbAfter->setChecked(true);
		if (*bInSubmenu) {
			qcbPutInSubmenu->setChecked(true);
		} else {
			qcbPutInSubmenu->setEnabled(false);
		} // if else
	} // if else

	this->qsDirectory = qsDirectory;
	this->epPosition = epPosition;
	this->bInSubmenu = bInSubmenu;

	qleDirectory->setFocus(Qt::OtherFocusReason);
} // cNewFavouriteDirectoryDialog

// dialog rejected
void cNewFavouriteDirectoryDialog::on_qpbCancel_clicked(bool checked /* false */)
{
	reject();
} // on_qpbCancel_clicked

// dialog accepted
void cNewFavouriteDirectoryDialog::on_qpbOK_clicked(bool checked /* false */)
{
	*qsDirectory = qleDirectory->text();
	if (qgbPosition->isEnabled()) {
		if (qrbBefore->isChecked()) {
			*epPosition = Before;
		} else {
			*epPosition = After;
		} // if else
	} // if
	if (qcbPutInSubmenu->isEnabled()) {
		*bInSubmenu = qcbPutInSubmenu->isChecked();
	} // if
	accept();
} // on_qpbOK_clicked
