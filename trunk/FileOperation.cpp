#include "FileOperation.h"

#include "FileOperation/FileOperationDialog.h"
#include "FileOperation/CopyMove.h"

// constructor
cFileOperation::cFileOperation(QMainWindow *qmwParent, QHBoxLayout *qhblOperations)
{
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
} // cFileOperation

// prepare operation
void cFileOperation::Operate(const eOperation eoOperation, cPanel *cpSource, cPanel *cpDestination)
{
	cCopyMove ccmCopyMove(qmwParent, qhblOperations);
	cFileOperationDialog cfodDialog(qmwParent);
	cPanel::sObjects soObjects;
	QFileInfoList qfilSource;
	cFileOperationDialog::eUserAction euaAction;
	QString qsDestination, qsFilter;

	qfilSource = cpSource->GetSelectedItemsList();
	qsDestination = cpDestination->GetPath();

	if (qfilSource.count() == 0) {
		// no items selected
		return;
	} // if

	soObjects = cPanel::GetCount(qfilSource);

	switch (eoOperation) {
		// TODO Process delete operation
		case CopyOperation:	euaAction = cfodDialog.ShowDialog(tr("Copy"),
																				 tr("Co&py %1 files and %2 directories to:").arg(soObjects.Files).arg(soObjects.Directories),
																				 &qsDestination,
																				 &qsFilter);
									break;
		case MoveOperation:	euaAction = cfodDialog.ShowDialog(tr("Move"),
																				 tr("&Move %1 files and %2 directories to:").arg(soObjects.Files).arg(soObjects.Directories),
																				 &qsDestination,
																				 &qsFilter);
									break;
	} // switch

	switch (euaAction) {
		// TODO Operate euaAction
		case cFileOperationDialog::CancelAction:	return;
		case cFileOperationDialog::OkAction:		ccmCopyMove.CopyMove(eoOperation, qfilSource, qsDestination, cCopyMove::ForegroundWindow);
																break;
		case cFileOperationDialog::EnqueueAction:	break;
	} // if
} // Operate