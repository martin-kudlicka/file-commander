#include "FileOperation.h"

#include "FileOperation/FileOperationDialog.h"
#include "FileOperation/CopyMove.h"

// constructor
cFileOperation::cFileOperation(QMainWindow *qmwParent, QHBoxLayout *qhblOperations)
{
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
} // cFileOperation

// place operation into queue
void cFileOperation::Enque(const eOperation eoOperation, const QFileInfoList qfilSource, const QString qsDestination)
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

// lists directory for it's content
QFileInfoList cFileOperation::GetDirectoryContent(const QString qsPath, const QDir::Filters fFilters)
{
	QDir qdDir;

	qdDir.setPath(qsPath);
	qdDir.setFilter(fFilters);
	return qdDir.entryInfoList();
} // GetDirectoryContent

// return list of sources (within subdirectories too)
QFileInfoList cFileOperation::GetSources(const QFileInfoList qfilFileAndDirList)
{
	int iI;
	QFileInfoList qfilDirectories, qfilSources;

	qfilSources = qfilFileAndDirList;

	for (iI = 0; iI < qfilFileAndDirList.count(); iI++) {
		if (qfilFileAndDirList.at(iI).isDir()) {
			qfilDirectories.append(qfilFileAndDirList.at(iI));
		} // if
	} // for

	while (!qfilDirectories.isEmpty()) {
		QFileInfo qfiDir;
		QFileInfoList qfilDirContent;

		qfiDir = qfilDirectories.takeAt(0);
		qfilDirContent = GetDirectoryContent(qfiDir.filePath(), QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
		qfilSources += qfilDirContent;
		for (iI = 0; iI < qfilDirContent.count(); iI++) {
			if (qfilDirContent.at(iI).isDir()) {
				qfilDirectories.append(qfilDirContent.at(iI));
			} // if
		} // for
	} // while

	return qfilSources;
} // GetSources

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
		// TODO Operate delete
		case cFileOperationDialog::CancelAction:	return;
		case cFileOperationDialog::OkAction:		ccmCopyMove.CopyMove(eoOperation, qfilSource, qsDestination, cCopyMove::ForegroundWindow);
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
			case CopyOperation:
			case MoveOperation:		ccmCopyMove.CopyMove(soOperation.eoOperation, soOperation.qfilSource, soOperation.qsDestination, cCopyMove::BackgroundWindow);
											break;
			case DeleteOperation:	break;
		} // switch
		qqQperations.dequeue();
	} // while
} // ProcessQueue