#include "ListerMainWindow/FindDialog.h"

// constructor
cFindDialog::cFindDialog(QWidget *qwParent, const bool &bPlugin)
{
	setParent(qwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);

	if (bPlugin) {
		qcbRegularExpression->setEnabled(false);
	} // if

	qcbFind->setFocus();
} // cFindDialog