#include "FileOperation.h"

#include "FileOperation/FileOperationDialog.h"

// constructor
cFileOperation::cFileOperation(QMainWindow *qmwParent, QHBoxLayout *qhblOperations)
{
	// set private variables
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
	ccmInQueue = NULL;

	// queue widget
	cqwQueue.hide();
	qhblOperations->insertWidget(iQUEUE_WIDGET_POS, &cqwQueue);

	// connections
	connect(this, SIGNAL(AddIntoQueueList(QListWidgetItem *)), &cqwQueue, SLOT(on_cFileOperation_AddIntoQueueList(QListWidgetItem *)));
	connect(&cqwQueue, SIGNAL(RemoveQueuedItems(QList<QListWidgetItem *>)), SLOT(on_cqwQueue_RemoveQueuedItems(QList<QListWidgetItem *>)));
} // cFileOperation

// place operation into queue
void cFileOperation::Enque(const cFileRoutine::eOperation eoOperation, const QFileInfoList qfilSource, const QString qsDestination)
{
	QListWidgetItem *qlwiOperation;
	QString qsItem;
	sOperation soOperation;

	switch (eoOperation) {
		case cFileRoutine::CopyOperation:	qsItem = tr("copy");
														break;
		case cFileRoutine::DeleteOperation:	qsItem = tr("del");
														break;
		case cFileRoutine::MoveOperation:	qsItem = tr("move");
														break;
	} // switch
	qsItem += ": " + qfilSource.at(0).path() + " -> " + qsDestination;

	// add new item into queue
	qlwiOperation = new QListWidgetItem(qsItem);
	soOperation.eoOperation = eoOperation;
	soOperation.qfilSource = qfilSource;
	soOperation.qsDestination = qsDestination;
	soOperation.qlwiItem = qlwiOperation;
	qqQperations.enqueue(soOperation);
	AddIntoQueueList(qlwiOperation);

	ProcessQueue();
} // Enque

// copy / move thread finished
void cFileOperation::on_cCopyMove_finished()
{
	int iI;

	for (iI = 0; iI < qlCopyMove.count(); iI++) {
		if (qlCopyMove.at(iI)->isFinished()) {
			if (ccmInQueue == qlCopyMove.at(iI)) {
				// queued operation finished
				ccmInQueue = NULL;
				ProcessQueue();
			} // if
			qlCopyMove.at(iI)->deleteLater();
			qlCopyMove.removeAt(iI);
		} // if
	} // for
} // on_cCopyMove_finished

// remove queued items (operations)
void cFileOperation::on_cqwQueue_RemoveQueuedItems(QList<QListWidgetItem *> qlItems)
{
	int iI;

	for (iI = 0; iI < qlItems.count(); iI++) {
		int iJ;

		for (iJ = 0; iJ < qqQperations.count(); iJ++) {
			if (qqQperations.at(iJ).qlwiItem == qlItems.at(iI)) {
				delete qqQperations.at(iJ).qlwiItem;
				qqQperations.removeAt(iJ);
				break;
			} // if
		} // for
	} // for

	// actualize visibility
	ProcessQueue();
} // on_cqwQueue_RemoveQueuedItems

// prepare operation
void cFileOperation::Operate(const cFileRoutine::eOperation eoOperation, cPanel *cpSource, cPanel *cpDestination /* NULL */)
{
	cCopyMove *ccmCopyMove;
	cFileOperationDialog cfodDialog(qmwParent);
	cPanel::sObjects soObjects;
	QFileInfoList qfilSource;
	cFileOperationDialog::eUserAction euaAction;
	QString qsDestination, qsFilter;

	qfilSource = cpSource->GetSelectedItemsList();
	if (qfilSource.count() == 0) {
		// no items selected
		return;
	} // if

	if (eoOperation != cFileRoutine::DeleteOperation) {
		qsDestination = cpDestination->GetPath();
		if (qfilSource.count() == 1) {
			// one file selected
			qsDestination += '/' + qfilSource.at(0).fileName();
		} else {
			// many files selected
			qsDestination += "/*.*";
		} // if else
	} // if

	soObjects = cPanel::GetCount(qfilSource);

	switch (eoOperation) {
		case cFileRoutine::DeleteOperation:	euaAction = cfodDialog.ShowDialog(eoOperation,
																									 tr("&Delete %1 files and %2 directories.").arg(soObjects.Files).arg(soObjects.Directories),
																									 &qsDestination,
																									 &qsFilter);
														break;
		case cFileRoutine::CopyOperation:	euaAction = cfodDialog.ShowDialog(eoOperation,
																									 tr("Co&py %1 files and %2 directories to:").arg(soObjects.Files).arg(soObjects.Directories),
																									 &qsDestination,
																									 &qsFilter);
														break;
		case cFileRoutine::MoveOperation:	euaAction = cfodDialog.ShowDialog(eoOperation,
																									 tr("&Move %1 files and %2 directories to:").arg(soObjects.Files).arg(soObjects.Directories),
																									 &qsDestination,
																									 &qsFilter);
														break;
	} // switch

	switch (euaAction) {
		case cFileOperationDialog::CancelAction:	return;
		case cFileOperationDialog::OkAction:		if (eoOperation == cFileRoutine::DeleteOperation) {
																	// delete
																	// TODO Operate
																} else {
																	// copy or move
																	ccmCopyMove = new cCopyMove(qmwParent, qhblOperations);
																	connect(ccmCopyMove, SIGNAL(finished()), SLOT(on_cCopyMove_finished()));
																	qlCopyMove.append(ccmCopyMove);
																	ccmCopyMove->CopyMove(eoOperation, qfilSource, qsDestination, cCopyMove::ForegroundWindow);
																} // if else
																break;
		case cFileOperationDialog::EnqueueAction:	Enque(eoOperation, qfilSource, qsDestination);
																break;
	} // if
} // Operate

// process first queued operation
void cFileOperation::ProcessQueue()
{
	if (ccmInQueue == NULL && !qqQperations.isEmpty()) {
		cCopyMove *ccmCopyMove;
		sOperation soOperation;

		soOperation = qqQperations.dequeue();
		delete soOperation.qlwiItem;

		switch (soOperation.eoOperation) {
			case cFileRoutine::CopyOperation:
			case cFileRoutine::MoveOperation:	ccmCopyMove = new cCopyMove(qmwParent, qhblOperations);
															ccmInQueue = ccmCopyMove;
															connect(ccmCopyMove, SIGNAL(finished()), SLOT(on_cCopyMove_finished()));
															qlCopyMove.append(ccmCopyMove);
															ccmCopyMove->CopyMove(soOperation.eoOperation, soOperation.qfilSource, soOperation.qsDestination, cCopyMove::BackgroundWindow);
															break;
			case cFileRoutine::DeleteOperation:	// TODO ProcessQueue
															break;
		} // switch
	} // if

	if (!qqQperations.isEmpty()) {
		cqwQueue.show();
	} else {
		cqwQueue.hide();
	} // if else
} // ProcessQueue