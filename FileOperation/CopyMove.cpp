#include "FileOperation/CopyMove.h"

#include "FileOperation/CopyMoveDialog.h"
#include "FileOperation/CopyMoveWidget.h"

// constructor
cCopyMove::cCopyMove(QMainWindow *qmwParent, QHBoxLayout *qhblOperations)
{
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
} // cCopyMove

// start of copy or move operation
void cCopyMove::CopyMove(const cFileOperation::eOperation eoOperation, const QFileInfoList qfilSource, const QString qsDestination, const eWindow eStyle)
{
	cCopyMoveDialog *ccmdDialog;
	cCopyMoveWidget *ccmwWidget;

	if (eStyle == ForegroundWindow) {
		ccmdDialog = new cCopyMoveDialog(qmwParent);
		ccmwWidget = NULL;
	} else {
		ccmwWidget = new cCopyMoveWidget();
		ccmdDialog = NULL;
	} // if else
} // CopyMove