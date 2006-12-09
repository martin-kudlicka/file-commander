#include "FileOperation/CopyMove.h"

// constructor
cCopyMove::cCopyMove(QMainWindow *qmwParent, QHBoxLayout *qhblOperations)
{
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
	bCanceled = false;
} // cCopyMove

// copy file
void cCopyMove::Copy(const QString qsSource, const QString qsDestination, qint64 *qi64Total)
{
	QByteArray qbaData;
	QFile qfDestination, qfSource;
	qint64 qi64Current;

	qfSource.setFileName(qsSource);
	qfDestination.setFileName(qsDestination);
	qfSource.open(QIODevice::ReadOnly);
	qfDestination.open(QIODevice::WriteOnly);

	// set progress bar
	emit SetCurrentMaximum(qfSource.size());

	// copy
	qi64Current = 0;
	while(!qfSource.atEnd() && !bCanceled) {
		qbaData = qfSource.read(qi64BUFFER_SIZE);
		qfDestination.write(qbaData);
		qi64Current += qbaData.size();
		*qi64Total += qbaData.size();
		emit SetCurrentValue(qi64Current);
		emit SetTotalValue(*qi64Total);
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
		ccmwWidget = NULL;
	} else {
		ccmwWidget = new cCopyMoveWidget();
		qhblOperations->addWidget(ccmwWidget);
		connect(this, SIGNAL(SetCurrentMaximum(const qint64)), ccmwWidget, SLOT(on_cCopyMove_SetCurrentMaximum(const qint64)));
		connect(this, SIGNAL(SetCurrentValue(const qint64)), ccmwWidget, SLOT(on_cCopyMove_SetCurrentValue(const qint64)));
		connect(this, SIGNAL(SetDestination(const QString)), ccmwWidget, SLOT(on_cCopyMove_SetDestination(const QString)));
		connect(this, SIGNAL(SetSource(const QString)), ccmwWidget, SLOT(on_cCopyMove_SetSource(const QString)));
		connect(this, SIGNAL(SetTotalMaximum(const qint64)), ccmwWidget, SLOT(on_cCopyMove_SetTotalMaximum(const qint64)));
		connect(this, SIGNAL(SetTotalValue(const qint64)), ccmwWidget, SLOT(on_cCopyMove_SetTotalValue(const qint64)));
		ccmdDialog = NULL;
	} // if else

	start();
} // CopyMove

// copy or move operation was canceled
void cCopyMove::on_ccm_OperationCanceled()
{
	bCanceled = true;
} // on_ccm_OperationCanceled

// thread code
void cCopyMove::run()
{
	int iI;
	QDir qdDir;
	QFileInfoList qfilSources;
	qint64 qi64Total, qi64TotalSize;
	QString qsSourcePath;

	// gather source files and source path
	qfilSources = cFileRoutine::GetSources(qfilSource);
	qsSourcePath = qfilSource.at(0).path();

	// prepare progress bars
	qi64TotalSize = 0;
	for (iI = 0; iI < qfilSources.count(); iI++) {
		qi64TotalSize += qfilSources.at(iI).size();
	} // for
	emit SetTotalMaximum(qi64TotalSize);

	// main process
	qi64Total = 0;
	for (iI = 0; iI < qfilSources.count() && !bCanceled; iI++) {
		QString qsTarget;

		// show file names
		qsTarget = qsDestination + qfilSources.at(iI).filePath().mid(qsSourcePath.length());
		emit SetSource(qfilSources.at(iI).filePath());
		emit SetDestination(qsTarget);
		
		if (qfilSources.at(iI).isDir()) {
			qdDir.mkpath(QFileInfo(qsTarget).filePath());
		} else {
			emit SetCurrentValue(0);

			switch (eoOperation) {
				case cFileRoutine::CopyOperation:	Copy(qfilSources.at(iI).filePath(), qsTarget, &qi64Total);
																if (bCanceled) {
																	// delete unfinished file
																	QFile::remove(qsTarget);
																} // if
																break;
				case cFileRoutine::MoveOperation:	QFile::rename(qfilSources.at(iI).filePath(), qsTarget);
																qdDir.rmdir(qfilSources.at(iI).path());
																qi64Total += qfilSources.at(iI).size();
																emit SetTotalValue(qi64Total);
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