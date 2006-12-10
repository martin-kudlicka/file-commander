#include "FileOperation/CopyMove.h"

// constructor
cCopyMove::cCopyMove(QMainWindow *qmwParent, QHBoxLayout *qhblOperations)
{
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
	bCanceled = false;
	qi64TotalMaximum = 0;
} // cCopyMove

// copy file
void cCopyMove::Copy(const QString qsSource, const QString qsDestination, qint64 *qi64TotalValue)
{
	QByteArray qbaData;
	QFile qfDestination, qfSource;
	qint64 qi64CurrentValue;

	qfSource.setFileName(qsSource);
	qfDestination.setFileName(qsDestination);
	qfSource.open(QIODevice::ReadOnly);
	qfDestination.open(QIODevice::WriteOnly);

	// set progress bar
	qi64CurrentMaximum = qfSource.size();
	emit SetCurrentMaximum(qi64CurrentMaximum);

	// copy
	qi64CurrentValue = 0;
	while(!qfSource.atEnd() && !bCanceled) {
		qbaData = qfSource.read(qi64BUFFER_SIZE);
		qfDestination.write(qbaData);
		qi64CurrentValue += qbaData.size();
		*qi64TotalValue += qbaData.size();
		emit SetCurrentValue(qi64CurrentValue);
		emit SetTotalValue(*qi64TotalValue);
		QApplication::processEvents();
	} // while
} // Copy

// start of copy or move operation
void cCopyMove::CopyMove(const cFileRoutine::eOperation eoOperation, const QFileInfoList qfilSource, const QString qsDestination, const eWindow eStyle)
{
	this->eoOperation = eoOperation;
	this->qfilSource = qfilSource;
	this->qsDestination = qsDestination;

	if (eStyle == ForegroundWindow) {
		ccmdDialog = new cCopyMoveDialog(qmwParent);
		ccmdDialog->setModal(true);
		ccmdDialog->show();
		connect(this, SIGNAL(SetCurrentMaximum(const qint64)), ccmdDialog, SLOT(on_cCopyMove_SetCurrentMaximum(const qint64)));
		connect(this, SIGNAL(SetCurrentValue(const qint64)), ccmdDialog, SLOT(on_cCopyMove_SetCurrentValue(const qint64)));
		connect(this, SIGNAL(SetDestination(const QString)), ccmdDialog, SLOT(on_cCopyMove_SetDestination(const QString)));
		connect(this, SIGNAL(SetSource(const QString)), ccmdDialog, SLOT(on_cCopyMove_SetSource(const QString)));
		connect(this, SIGNAL(SetTotalMaximum(const qint64)), ccmdDialog, SLOT(on_cCopyMove_SetTotalMaximum(const qint64)));
		connect(this, SIGNAL(SetTotalValue(const qint64)), ccmdDialog, SLOT(on_cCopyMove_SetTotalValue(const qint64)));
		connect(ccmdDialog, SIGNAL(Cancel()), SLOT(on_ccm_OperationCanceled()));
		connect(ccmdDialog, SIGNAL(Background()), SLOT(on_ccmdCopyMoveDialog_Background()));
		ccmwWidget = NULL;
	} else {
		CreateWidget();
		ccmdDialog = NULL;
	} // if else

	start();
} // CopyMove

// create widget for background operation
void cCopyMove::CreateWidget()
{
	ccmwWidget = new cCopyMoveWidget();
	qhblOperations->insertWidget(iQUEUED_OPERATION_POSITION, ccmwWidget);
	connect(this, SIGNAL(SetCurrentMaximum(const qint64)), ccmwWidget, SLOT(on_cCopyMove_SetCurrentMaximum(const qint64)));
	connect(this, SIGNAL(SetCurrentValue(const qint64)), ccmwWidget, SLOT(on_cCopyMove_SetCurrentValue(const qint64)));
	connect(this, SIGNAL(SetDestination(const QString)), ccmwWidget, SLOT(on_cCopyMove_SetDestination(const QString)));
	connect(this, SIGNAL(SetSource(const QString)), ccmwWidget, SLOT(on_cCopyMove_SetSource(const QString)));
	connect(this, SIGNAL(SetTotalMaximum(const qint64)), ccmwWidget, SLOT(on_cCopyMove_SetTotalMaximum(const qint64)));
	connect(this, SIGNAL(SetTotalValue(const qint64)), ccmwWidget, SLOT(on_cCopyMove_SetTotalValue(const qint64)));
	connect(ccmwWidget, SIGNAL(Cancel()), SLOT(on_ccm_OperationCanceled()));
} // CreateWidget

// copy or move operation was canceled
void cCopyMove::on_ccm_OperationCanceled()
{
	bCanceled = true;
} // on_ccm_OperationCanceled

// move operation to background
void cCopyMove::on_ccmdCopyMoveDialog_Background()
{
	CreateWidget();
	emit SetSource(qsSource);
	emit SetDestination(qsTarget);
	emit SetCurrentMaximum(qi64CurrentMaximum);
	emit SetTotalMaximum(qi64TotalMaximum);
	ccmdDialog->deleteLater();
	ccmdDialog = NULL;
} // on_ccmdCopyMoveDialog_Background

// thread code
void cCopyMove::run()
{
	int iI;
	QDir qdDir;
	QFileInfoList qfilSources;
	qint64 qi64TotalValue;
	QString qsSourcePath;

	// gather source files and source path
	qfilSources = cFileRoutine::GetSources(qfilSource);
	qsSourcePath = qfilSource.at(0).path();

	// prepare progress bars
	for (iI = 0; iI < qfilSources.count(); iI++) {
		qi64TotalMaximum += qfilSources.at(iI).size();
	} // for
	emit SetTotalMaximum(qi64TotalMaximum);

	// main process
	qi64TotalValue = 0;
	for (iI = 0; iI < qfilSources.count() && !bCanceled; iI++) {
		// show file names
		qsSource = qfilSources.at(iI).filePath();
		qsTarget = qsDestination + qfilSources.at(iI).filePath().mid(qsSourcePath.length());
		if (ccmdDialog) {
			// name with path in dialog
			emit SetSource(qsSource);
			emit SetDestination(qsTarget);
		} else {
			// just name in widget
			emit SetSource(QFileInfo(qsSource).fileName());
			emit SetDestination(QFileInfo(qsTarget).fileName());
		} // if else
		
		if (qfilSources.at(iI).isDir()) {
			qdDir.mkpath(QFileInfo(qsTarget).filePath());
		} else {
			emit SetCurrentValue(0);

			switch (eoOperation) {
				case cFileRoutine::CopyOperation:	Copy(qfilSources.at(iI).filePath(), qsTarget, &qi64TotalValue);
																if (bCanceled) {
																	// delete unfinished file
																	QFile::remove(qsTarget);
																} // if
																break;
				case cFileRoutine::MoveOperation:	QFile::rename(qfilSources.at(iI).filePath(), qsTarget);
																qdDir.rmdir(qfilSources.at(iI).path());
																qi64TotalValue += qfilSources.at(iI).size();
																emit SetTotalValue(qi64TotalValue);
																break;
			} // switch
		} // if else
	} // for

	// remove source directory when moving files
	if (cFileRoutine::MoveOperation) {
		qdDir.rmdir(qfilSource.at(0).filePath());
	} // if

	// close dialog or widget
	if (ccmdDialog) {
		ccmdDialog->deleteLater();
	} else {
		ccmwWidget->deleteLater();
	} // if else
} // run