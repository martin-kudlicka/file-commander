#include "FileOperation/Rename.h"

// destructor
cRename::~cRename()
{
	crdDialog->deleteLater();
} // ~cRename

// constructor
cRename::cRename(QMainWindow *qmwParent)
{
	crdDialog = new cRenameDialog(qmwParent);
} // cRename

// show conflict dialog
void cRename::Show(const QString &qsOldFilename)
{
	crdDialog->qleFilename->setText(qsOldFilename);

	if (crdDialog->exec() == QDialog::Accepted) {
		emit Finished(crdDialog->qleFilename->text());
	} else {
		emit Finished(NULL);
	} // if else
} // Show