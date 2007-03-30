#include "FindFilesDialog/SaveFindSettingsDialog.h"

// constructor
cSaveFindSettingsDialog::cSaveFindSettingsDialog(QDialog *qdParent)
{
	setParent(qdParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);

	qleName->setFocus(Qt::OtherFocusReason);
} // cSaveFindSettingsDialog