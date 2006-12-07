#include "FileOperation.h"

#include "FileOperation/FileOperationDialog.h"
#include "FileOperation/CopyMove.h"

// constructor
cFileOperation::cFileOperation(QMainWindow *qmwParent, QHBoxLayout *qhblOperations)
{
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
} // cFileOperation

// lists directory for it's content
QFileInfoList cFileOperation::GetDirectoryContent(const QString qsPath, const QDir::Filters fFilters)
{
	QDir qdDir;

	qdDir.setPath(qsPath);
	qdDir.setFilter(fFilters);
	return qdDir.entryInfoList();
} // GetDirectoryContent

// return list of files only (within subdirectories too)
QFileInfoList cFileOperation::GetFiles(const QFileInfoList qfilFileAndDirList)
{
	int iI;
	QFileInfoList qfilDirectories, qfilFiles;

	for (iI = 0; iI < qfilFileAndDirList.count(); iI++) {
		if (qfilFileAndDirList.at(iI).isDir()) {
			qfilDirectories.append(qfilFileAndDirList.at(iI));
		} else {
			qfilFiles.append(qfilFileAndDirList.at(iI));
		} // if else
	} // for

	while (!qfilDirectories.isEmpty()) {
		QFileInfo qfiDir;
		QFileInfoList qfilDirContent;

		qfiDir = qfilDirectories.takeAt(0);
		qfilDirContent = GetDirectoryContent(qfiDir.filePath(), QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
		for (iI = 0; iI < qfilDirContent.count(); iI++) {
			if (qfilDirContent.at(iI).isDir()) {
				qfilDirectories.append(qfilDirContent.at(iI));
			} else {
				qfilFiles.append(qfilDirContent.at(iI));
			} // if else
		} // for
	} // while

	return qfilFiles;
} // GetFiles

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