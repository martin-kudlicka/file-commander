#include "FileOperation.h"

#include "FileOperation/FileOperationDialog.h"

// constructor
cFileOperation::cFileOperation(QMainWindow *qmwParent, QHBoxLayout *qhblOperations)
{
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
} // cFileOperation

// place operation into queue
void cFileOperation::Enque(const cFileRoutine::eOperation eoOperation, const QFileInfoList qfilSource, const QString qsDestination)
{
	sOperation soOperation;

	soOperation.eoOperation = eoOperation;
	soOperation.qfilSource = qfilSource;
	soOperation.qsDestination = qsDestination;

	qqQperations.enqueue(soOperation);

	if (qqQperations.count() == 1) {
		// first operation added
		ProcessQueue();
	} // if
} // Enque

// copy / move thread finished
void cFileOperation::on_cCopyMove_finished()
{
	int iI;

	for (iI = 0; iI < qlCopyMove.count(); iI++) {
		if (qlCopyMove.at(iI)->isFinished()) {
			qlCopyMove.at(iI)->deleteLater();
			qlCopyMove.removeAt(iI);
		} // if
	} // for
} // on_cCopyMove_finished

// prepare operation
void cFileOperation::Operate(const cFileRoutine::eOperation eoOperation, cPanel *cpSource, cPanel *cpDestination)
{
	cCopyMove *ccmCopyMove;
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
		case cFileRoutine::CopyOperation:	euaAction = cfodDialog.ShowDialog(tr("Copy"),
																									 tr("Co&py %1 files and %2 directories to:").arg(soObjects.Files).arg(soObjects.Directories),
																									 &qsDestination,
																									 &qsFilter);
														break;
		case cFileRoutine::MoveOperation:	euaAction = cfodDialog.ShowDialog(tr("Move"),
																									 tr("&Move %1 files and %2 directories to:").arg(soObjects.Files).arg(soObjects.Directories),
																									 &qsDestination,
																									 &qsFilter);
														break;
	} // switch

	switch (euaAction) {
		// TODO Operate delete
		case cFileOperationDialog::CancelAction:	return;
		case cFileOperationDialog::OkAction:		ccmCopyMove = new cCopyMove(qmwParent, qhblOperations);
																connect(ccmCopyMove, SIGNAL(finished()), SLOT(on_cCopyMove_finished()));
																qlCopyMove.append(ccmCopyMove);
																ccmCopyMove->CopyMove(eoOperation, qfilSource, qsDestination, cCopyMove::ForegroundWindow);
																break;
		case cFileOperationDialog::EnqueueAction:	Enque(eoOperation, qfilSource, qsDestination);
																break;
	} // if
} // Operate

// process first queued operation
void cFileOperation::ProcessQueue()
{
	while (!qqQperations.empty()) {
		cCopyMove ccmCopyMove(qmwParent, qhblOperations);
		sOperation soOperation;

		soOperation = qqQperations.first();
		switch (soOperation.eoOperation) {
			// TODO ProcessQueue delete
			case cFileRoutine::CopyOperation:
			case cFileRoutine::MoveOperation:	ccmCopyMove.CopyMove(soOperation.eoOperation, soOperation.qfilSource, soOperation.qsDestination, cCopyMove::BackgroundWindow);
															break;
			case cFileRoutine::DeleteOperation:	break;
		} // switch
		qqQperations.dequeue();
	} // while
} // ProcessQueue