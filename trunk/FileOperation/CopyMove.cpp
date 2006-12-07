#include "FileOperation/CopyMove.h"

#include "FileOperation/CopyMoveDialog.h"
#include "FileOperation/CopyMoveWidget.h"

// constructor
cCopyMove::cCopyMove(QMainWindow *qmwParent, QHBoxLayout *qhblOperations)
{
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
	bCanceled = false;
} // cCopyMove

// start of copy or move operation
void cCopyMove::CopyMove(const cFileOperation::eOperation eoOperation, const QFileInfoList qfilSource, const QString qsDestination, const eWindow eStyle)
{
	cCopyMoveDialog *ccmdDialog;
	cCopyMoveWidget *ccmwWidget;
	int iI;
	QDir qdDir;
	QFileInfoList qfilSources;
	qint64 qi64TotalSize;
	QString qsSourcePath;

	if (eStyle == ForegroundWindow) {
		ccmdDialog = new cCopyMoveDialog(qmwParent);
		ccmdDialog->setModal(true);
		ccmdDialog->show();
		connect(ccmdDialog, SIGNAL(Cancel()), SLOT(on_ccm_OperationCanceled()));
		ccmwWidget = NULL;
	} else {
		ccmwWidget = new cCopyMoveWidget();
		// TODO CopyMove add ccmwWidget to layout
		ccmdDialog = NULL;
	} // if else

	// gather source files and source path
	qfilSources = cFileOperation::GetSources(qfilSource);
	qsSourcePath = qfilSource.at(0).path();

	// prepare progress bars
	qi64TotalSize = 0;
	for (iI = 0; iI < qfilSources.count(); iI++) {
		qi64TotalSize += qfilSources.at(iI).size();
	} // for
	if (ccmdDialog) {
		ccmdDialog->qpbTotal->setMaximum(qi64TotalSize);
	} else {
		ccmwWidget->qpbTotal->setMaximum(qi64TotalSize);
	} // if else

	// main process
	for (iI = 0; iI < qfilSources.count() && !bCanceled; iI++) {
		QString qsTarget;

		// show file names
		if (ccmdDialog) {
			ccmdDialog->qlSource->setText(qfilSources.at(iI).filePath());
		} else {
			ccmwWidget->qlSource->setText(qfilSources.at(iI).filePath());
		} // if else
		QApplication::processEvents();
		
		qsTarget = qsDestination + qfilSources.at(iI).filePath().mid(qsSourcePath.length());
		if (qfilSources.at(iI).isDir()) {
			qdDir.mkpath(QFileInfo(qsTarget).filePath());
		} else {
			switch (eoOperation) {
				case cFileOperation::CopyOperation:	QFile::copy(qfilSources.at(iI).filePath(), qsTarget);
																break;
				case cFileOperation::MoveOperation:	QFile::rename(qfilSources.at(iI).filePath(), qsTarget);
																qdDir.rmdir(qfilSources.at(iI).path());
																break;
			} // switch
		} // if else

		// file completed
		if (ccmdDialog) {
			ccmdDialog->qpbTotal->setValue(ccmdDialog->qpbTotal->value() + qfilSources.at(iI).size());
		} else {
			ccmwWidget->qpbTotal->setValue(ccmwWidget->qpbTotal->value() + qfilSources.at(iI).size());
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

// copy or move operation was canceled
void cCopyMove::on_ccm_OperationCanceled()
{
	bCanceled = true;
} // on_ccm_OperationCanceled