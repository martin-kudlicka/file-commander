#include "FileOperation.h"

#include "FileOperation/FileOperationDialog.h"

// constructor
cFileOperation::cFileOperation(QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings)
{
	// set private variables
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
	this->csSettings = csSettings;
	ccmInQueue = NULL;
	cdInQueue = NULL;

	// queue widget
	cqwQueue.hide();
	qhblOperations->insertWidget(iQUEUE_WIDGET_POS, &cqwQueue);

	// connections
	connect(this, SIGNAL(AddIntoQueueList(QListWidgetItem *)), &cqwQueue, SLOT(on_cFileOperation_AddIntoQueueList(QListWidgetItem *)));
	connect(&cqwQueue, SIGNAL(RemoveQueuedItems(QList<QListWidgetItem *>)), SLOT(on_cqwQueue_RemoveQueuedItems(QList<QListWidgetItem *>)));
} // cFileOperation

// place operation into queue
void cFileOperation::Enque(const cFileRoutine::eOperation &eoOperation, const QFileInfoList &qfilSource, const QString &qsDestination, const QString &qsFilter)
{
	QListWidgetItem *qlwiOperation;
	QString qsItem;
	sOperation soOperation;

	switch (eoOperation) {
		case cFileRoutine::CopyOperation:
			qsItem = tr("copy");
			break;
		case cFileRoutine::DeleteOperation:
			qsItem = tr("del");
			break;
		case cFileRoutine::MoveOperation:
			qsItem = tr("move");
	} // switch
	qsItem += ": " + qfilSource.at(0).path() + " -> " + qsDestination;

	// add new item into queue
	qlwiOperation = new QListWidgetItem(qsItem);
	soOperation.eoOperation = eoOperation;
	soOperation.qfilSource = qfilSource;
	soOperation.qsDestination = qsDestination;
	soOperation.qsFilter = qsFilter;
	soOperation.qlwiItem = qlwiOperation;
	qqQperations.enqueue(soOperation);
	AddIntoQueueList(qlwiOperation);

	ProcessQueue();
} // Enque

// count of objects
cFileOperation::sObjects cFileOperation::GetCount(const QFileInfoList &qfilObjects)
{
	int iI;
	sObjects soCount;

	soCount.Directories = 0;
	soCount.Files = 0;
	for (iI = 0; iI < qfilObjects.count(); iI++) {
		if (qfilObjects.at(iI).isDir()) {
			soCount.Directories++;
		} else {
			soCount.Files++;
		} // if else
	} // for

	return soCount;
} // GetCount

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
			return;
		} // if
	} // for
} // on_cCopyMove_finished

// delete thread finished
void cFileOperation::on_cDelete_finished()
{
	int iI;

	for (iI = 0; iI < qlDelete.count(); iI++) {
		if (qlDelete.at(iI)->isFinished()) {
			if (cdInQueue == qlDelete.at(iI)) {
				// queued operation finished
				cdInQueue = NULL;
				ProcessQueue();
			} // if
			qlDelete.at(iI)->deleteLater();
			qlDelete.removeAt(iI);
			return;
		} // if
	} // for
} // on_cDelete_finished

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
void cFileOperation::Operate(const cFileRoutine::eOperation &eoOperation, const QFileInfoList &qfilSource, QString qsDestination /* QString() */)
{
	cCopyMove *ccmCopyMove;
	cDelete *cdDelete;
	cFileOperationDialog cfodDialog(qmwParent, csSettings);
	sObjects soObjects;
	cFileOperationDialog::eUserAction euaAction;
	QString qsFilter;

	if (qfilSource.count() == 0) {
		// no items selected
		return;
	} // if

	if (eoOperation != cFileRoutine::DeleteOperation) {
		if (qfilSource.count() == 1) {
			// one file selected
			if (qfilSource.at(0).isFile()) {
				qsDestination += '/' + qfilSource.at(0).fileName();
			} // if
		} else {
			// many files selected
			qsDestination += "/*.*";
		} // if else
	} // if

	soObjects = GetCount(qfilSource);

	switch (eoOperation) {
		case cFileRoutine::DeleteOperation:
			euaAction = cfodDialog.ShowDialog(eoOperation,
			tr("&Delete %1 files and %2 directories.").arg(soObjects.Files).arg(soObjects.Directories), &qsDestination, &qsFilter);
			break;
		case cFileRoutine::CopyOperation:
			euaAction = cfodDialog.ShowDialog(eoOperation,
			tr("Co&py %1 files and %2 directories to:").arg(soObjects.Files).arg(soObjects.Directories), &qsDestination, &qsFilter);
			break;
		case cFileRoutine::MoveOperation:
			euaAction = cfodDialog.ShowDialog(eoOperation,
			tr("&Move %1 files and %2 directories to:").arg(soObjects.Files).arg(soObjects.Directories), &qsDestination, &qsFilter);
	} // switch

	switch (euaAction) {
		case cFileOperationDialog::CancelAction:
			return;
		case cFileOperationDialog::OkAction:
			if (eoOperation == cFileRoutine::DeleteOperation) {
				// delete
				cdDelete = new cDelete(qmwParent, qhblOperations
#ifdef Q_WS_WIN
					, csSettings
#endif
					);
				connect(cdDelete, SIGNAL(finished()), SLOT(on_cDelete_finished()));
				qlDelete.append(cdDelete);
				cdDelete->Delete(qfilSource, qsFilter, cFileRoutine::ForegroundWindow);
			} else {
				// copy or move
				QDir qdDir;

				qdDir.setPath(qsDestination);
				if (qdDir.exists()) {
					qsDestination = QDir::cleanPath(qsDestination) + "/*.*";
				} // if
				ccmCopyMove = new cCopyMove(qmwParent, qhblOperations, csSettings);
				connect(ccmCopyMove, SIGNAL(finished()), SLOT(on_cCopyMove_finished()));
				qlCopyMove.append(ccmCopyMove);
				ccmCopyMove->CopyMove(eoOperation, qfilSource, qsDestination, qsFilter, cFileRoutine::ForegroundWindow);
			} // if else
			break;
		case cFileOperationDialog::EnqueueAction:
			Enque(eoOperation, qfilSource, qsDestination, qsFilter);
	} // if
} // Operate

// process first queued operation
void cFileOperation::ProcessQueue()
{
	if (ccmInQueue == NULL && !qqQperations.isEmpty()) {
		cCopyMove *ccmCopyMove;
		cDelete *cdDelete;
		sOperation soOperation;

		soOperation = qqQperations.dequeue();
		delete soOperation.qlwiItem;

		switch (soOperation.eoOperation) {
			case cFileRoutine::CopyOperation:
			case cFileRoutine::MoveOperation:
				ccmCopyMove = new cCopyMove(qmwParent, qhblOperations, csSettings);
				ccmInQueue = ccmCopyMove;
				connect(ccmCopyMove, SIGNAL(finished()), SLOT(on_cCopyMove_finished()));
				qlCopyMove.append(ccmCopyMove);
				ccmCopyMove->CopyMove(soOperation.eoOperation, soOperation.qfilSource, soOperation.qsDestination, soOperation.qsFilter, cFileRoutine::BackgroundWindow);
				break;
			case cFileRoutine::DeleteOperation:
				cdDelete = new cDelete(qmwParent, qhblOperations
#ifdef Q_WS_WIN
					, csSettings
#endif
					);
				cdInQueue = cdDelete;
				connect(cdDelete, SIGNAL(finished()), SLOT(on_cDelete_finished()));
				qlDelete.append(cdDelete);
				cdDelete->Delete(soOperation.qfilSource, soOperation.qsFilter, cFileRoutine::BackgroundWindow);
		} // switch
	} // if

	if (!qqQperations.isEmpty()) {
		cqwQueue.show();
	} else {
		cqwQueue.hide();
	} // if else
} // ProcessQueue
