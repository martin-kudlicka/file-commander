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
	int iI;
	QDir qdDir;
	QFileInfoList qfilSourceFiles;
	qint64 qi64TotalSize;
	QString qsSourcePath;

	if (eStyle == ForegroundWindow) {
		ccmdDialog = new cCopyMoveDialog(qmwParent);
		ccmdDialog->setModal(true);
		ccmdDialog->show();
		ccmwWidget = NULL;
	} else {
		ccmwWidget = new cCopyMoveWidget();
		// TODO CopyMove add ccmwWidget to layout
		ccmdDialog = NULL;
	} // if else

	// gather source files and source path
	qfilSourceFiles = cFileOperation::GetFiles(qfilSource);
	qsSourcePath = qfilSource.at(0).path();

	// prepare progress bars
	qi64TotalSize = 0;
	for (iI = 0; iI < qfilSourceFiles.count(); iI++) {
		qi64TotalSize += qfilSourceFiles.at(iI).size();
	} // for
	if (ccmdDialog) {
		ccmdDialog->qpbTotal->setMaximum(qi64TotalSize);
	} else {
		ccmwWidget->qpbTotal->setMaximum(qi64TotalSize);
	} // if else

	// main process
	for (iI = 0; iI < qfilSourceFiles.count(); iI++) {
		QString qsTarget;

		// show file names
		if (ccmdDialog) {
			ccmdDialog->qlSource->setText(qfilSourceFiles.at(iI).filePath());
		} else {
			ccmwWidget->qlSource->setText(qfilSourceFiles.at(iI).filePath());
		} // if else
		QApplication::processEvents();
		
		qsTarget = qsDestination + qfilSourceFiles.at(iI).filePath().mid(qsSourcePath.length());
		qdDir.mkpath(QFileInfo(qsTarget).path());
		switch (eoOperation) {
			case cFileOperation::CopyOperation:	QFile::copy(qfilSourceFiles.at(iI).filePath(), qsTarget);
															break;
			case cFileOperation::MoveOperation:	QFile::rename(qfilSourceFiles.at(iI).filePath(), qsTarget);
															qdDir.rmdir(qfilSourceFiles.at(iI).path());
															break;
		} // switch

		// file completed
		if (ccmdDialog) {
			ccmdDialog->qpbTotal->setValue(ccmdDialog->qpbTotal->value() + qfilSourceFiles.at(iI).size());
		} else {
			ccmwWidget->qpbTotal->setValue(ccmwWidget->qpbTotal->value() + qfilSourceFiles.at(iI).size());
		} // if else
	} // for

	// remove source directory when moving files
	if (cFileOperation::MoveOperation) {
		qdDir.rmdir(qfilSource.at(0).filePath());
	} // if

	// close dialog or widget
	if (ccmdDialog) {
		ccmdDialog->deleteLater();
	} else {
		ccmwWidget->deleteLater();
	} // if else
} // CopyMove