#include "FileOperation/CopyMove.h"

// constructor
cCopyMove::cCopyMove(QMainWindow *qmwParent, QHBoxLayout *qhblOperations)
{
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
	bCanceled = false;
} // cCopyMove

// copy file
void cCopyMove::Copy(const QString qsSource, const QString qsDestination)
{
	QByteArray qbaData;
	QFile qfDestination, qfSource;

	qfSource.setFileName(qsSource);
	qfDestination.setFileName(qsDestination);
	qfSource.open(QIODevice::ReadOnly);
	qfDestination.open(QIODevice::WriteOnly);

	// set progress bar
	if (ccmdDialog) {
		ccmdDialog->qpbCurrent->setMaximum(qfSource.size());
	} else {
		ccmwWidget->qpbCurrent->setMaximum(qfSource.size());
	} // if else

	// copy
	while(!qfSource.atEnd() && !bCanceled) {
		qbaData = qfSource.read(qi64BUFFER_SIZE);
		qfDestination.write(qbaData);
		if (ccmdDialog) {
			ccmdDialog->qpbCurrent->setValue(ccmdDialog->qpbCurrent->value() + qbaData.size());
			ccmdDialog->qpbTotal->setValue(ccmdDialog->qpbTotal->value() + qbaData.size());
		} else {
			ccmwWidget->qpbCurrent->setValue(ccmwWidget->qpbCurrent->value() + qbaData.size());
			ccmwWidget->qpbTotal->setValue(ccmwWidget->qpbTotal->value() + qbaData.size());
		} // if else
		QApplication::processEvents();
	} // while
} // Copy

// start of copy or move operation
void cCopyMove::CopyMove(const cFileOperation::eOperation eoOperation, const QFileInfoList qfilSource, const QString qsDestination, const eWindow eStyle)
{
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
		qhblOperations->addWidget(ccmwWidget);
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
		qsTarget = qsDestination + qfilSources.at(iI).filePath().mid(qsSourcePath.length());
		if (ccmdDialog) {
			ccmdDialog->qlSource->setText(qfilSources.at(iI).filePath());
			ccmdDialog->qlDestination->setText(qsTarget);
		} else {
			ccmwWidget->qlSource->setText(qfilSources.at(iI).fileName());
			ccmwWidget->qlDestination->setText(QFileInfo(qsTarget).fileName());
		} // if else
		
		if (qfilSources.at(iI).isDir()) {
			qdDir.mkpath(QFileInfo(qsTarget).filePath());
		} else {
			if (ccmdDialog) {
					ccmdDialog->qpbCurrent->setValue(0);
				} else {
					ccmwWidget->qpbCurrent->setValue(0);
				} // if else

			switch (eoOperation) {
				case cFileOperation::CopyOperation:	Copy(qfilSources.at(iI).filePath(), qsTarget);
																if (bCanceled) {
																	// delete unfinished file
																	QFile::remove(qsTarget);
																} // if
																break;
				case cFileOperation::MoveOperation:	QFile::rename(qfilSources.at(iI).filePath(), qsTarget);
																qdDir.rmdir(qfilSources.at(iI).path());
																if (ccmdDialog) {
																	ccmdDialog->qpbTotal->setValue(ccmdDialog->qpbTotal->value() + qfilSources.at(iI).size());
																} else {
																	ccmwWidget->qpbTotal->setValue(ccmwWidget->qpbTotal->value() + qfilSources.at(iI).size());
																} // if else
																break;
			} // switch
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
		delete ccmwWidget;
	} // if else
} // CopyMove

// copy or move operation was canceled
void cCopyMove::on_ccm_OperationCanceled()
{
	bCanceled = true;
} // on_ccm_OperationCanceled