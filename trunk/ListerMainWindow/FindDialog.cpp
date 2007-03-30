#include "ListerMainWindow/FindDialog.h"

// constructor
cFindDialog::cFindDialog(QMainWindow *qmwParent, const bool &bPlugin)
{
	setParent(qmwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);

	if (bPlugin) {
		qcbRegularExpression->setEnabled(false);
	} // if

	qcbFind->setFocus(Qt::OtherFocusReason);
} // cFindDialog