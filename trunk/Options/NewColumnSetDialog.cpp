#include "Options/NewColumnSetDialog.h"

// constructor
cNewColumnSetDialog::cNewColumnSetDialog(QDialog *qdParent)
{
	setParent(qdParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);
	qleColumnSet->setFocus();
} // cNewColumnSetDialog