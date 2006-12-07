#include "FileOperation.h"

#include "FileOperationDialog.h"

// constructor, set parent window for dialogs
cFileOperation::cFileOperation(QMainWindow *qmwParent)
{
	this->qmwParent = qmwParent;
} // cFileOperation

// prepare operation
void cFileOperation::Operate(const eOperation eoOperation, cPanel *cpSource, cPanel *cpDestination)
{
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

	if (euaAction == cFileOperationDialog::CancelAction) {
		return;
	} // if

	// TODO Operate - some process
} // Operate