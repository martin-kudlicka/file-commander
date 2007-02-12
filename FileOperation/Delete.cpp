#include "FileOperation/Delete.h"

// constructor
cDelete::cDelete(QMainWindow *qmwParent, QHBoxLayout *qhblOperations)
{
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
	bCanceled = false;
	qi64TotalMaximum = 0;
} // cDelete

// create widget for background operation
void cDelete::CreateWidget()
{
	cdwWidget = new cDeleteWidget();
	qhblOperations->insertWidget(cFileRoutine::iQUEUED_OPERATION_POSITION, cdwWidget);
	connect(this, SIGNAL(SetSource(const QString &)), cdwWidget, SLOT(on_cDeleteWidget_SetSource(const QString &)));
	connect(this, SIGNAL(SetTotalMaximum(const qint64 &)), cdwWidget, SLOT(on_cDeleteWidget_SetTotalMaximum(const qint64 &)));
	connect(this, SIGNAL(SetTotalValue(const qint64 &)), cdwWidget, SLOT(on_cDeleteWidget_SetTotalValue(const qint64 &)));
	connect(cdwWidget, SIGNAL(Cancel()), SLOT(on_cd_OperationCanceled()));
} // CreateWidget

// start of delete operation
void cDelete::Delete(const QFileInfoList &qfilSource, const cFileRoutine::eWindow &eStyle)
{
	this->qfilSource = qfilSource;

	if (eStyle == cFileRoutine::ForegroundWindow) {
		cddDialog = new cDeleteDialog(qmwParent);
		cddDialog->setModal(true);
		cddDialog->show();
		connect(this, SIGNAL(SetSource(const QString &)), cddDialog, SLOT(on_cDeleteDialog_SetSource(const QString &)));
		connect(this, SIGNAL(SetTotalMaximum(const qint64 &)), cddDialog, SLOT(on_cDeleteDialog_SetTotalMaximum(const qint64 &)));
		connect(this, SIGNAL(SetTotalValue(const qint64 &)), cddDialog, SLOT(on_cDeleteDialog_SetTotalValue(const qint64 &)));
		connect(cddDialog, SIGNAL(Cancel()), SLOT(on_cd_OperationCanceled()));
		connect(cddDialog, SIGNAL(Background()), SLOT(on_cdDeleteDialog_Background()));
		cdwWidget = NULL;
	} else {
		CreateWidget();
		cddDialog = NULL;
	} // if else

	start();
} // Delete

// delete operation was canceled
void cDelete::on_cd_OperationCanceled()
{
	bCanceled = true;
} // on_cd_OperationCanceled

// delete operation to background
void cDelete::on_cdDeleteDialog_Background()
{
	CreateWidget();
	emit SetTotalMaximum(qi64TotalMaximum);
	cddDialog->deleteLater();
	cddDialog = NULL;
} // on_cdDeleteDialog_Background

// separate thread process
void cDelete::run()
{
	int iI;
	QFileInfoList qfilSources;

	// gather source files
	qfilSources = cFileRoutine::GetSources(qfilSource);
	qi64TotalMaximum = qfilSources.count();

	emit SetTotalMaximum(qi64TotalMaximum);

	// main process
	for (iI = qfilSources.count() - 1; iI >= 0; iI--) {
		QDir qdSource;

		if (cddDialog) {
			// name with path in dialog
			emit SetSource(qfilSources.at(iI).filePath());
		} else {
			// just name in widget
			emit SetSource(qfilSources.at(iI).fileName());
		} // if else

		// TODO run - question for readonly
		if (qfilSources.at(iI).isDir()) {
			qdSource.rmdir(qfilSources.at(iI).filePath());
		} else {
			qdSource.remove(qfilSources.at(iI).filePath());
		} // if else

		emit SetTotalValue(qi64TotalMaximum - iI);
	} // for

	// close dialog or widget
	if (cddDialog) {
		cddDialog->deleteLater();
	} else {
		cdwWidget->deleteLater();
	} // if else
} // run