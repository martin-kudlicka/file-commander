#include "FileSystem/Archive/ArchiveCopy.h"

// constructor
cArchiveCopy::cArchiveCopy(QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings)
{
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
	this->csSettings = csSettings;

	bCanceled = false;
	qi64TotalMaximum = 0;
} // cLocalCopyMove

// start of copy or move operation
const void cArchiveCopy::Copy(const QStringList &qslOperation, const QFileInfo &qfiArchive, const QString &qsDestination, const QString &qsFilter, cPackerPlugin *cppPackerPlugin, cPackerPlugin::sPluginInfo *spiPluginInfo, const cFileOperation::eOperationPosition &eopPosition)
{
	this->qslOperation = qslOperation;
	this->qfiArchive = qfiArchive;
	this->qsDestination = qsDestination;
	this->qsFilter = qsFilter;

	// information windows
	if (eopPosition == cFileOperation::ForegroundOperation) {
		ccmdDialog = new cCopyMoveDialog(qmwParent);
		ccmdDialog->setWindowTitle(tr("Extract"));
		ccmdDialog->setModal(true);
		ccmdDialog->show();
		connect(this, SIGNAL(SetCurrentMaximum(const qint64 &)), ccmdDialog, SLOT(on_cCopyMove_SetCurrentMaximum(const qint64 &)));
		connect(this, SIGNAL(SetCurrentValue(const qint64 &)), ccmdDialog, SLOT(on_cCopyMove_SetCurrentValue(const qint64 &)));
		connect(this, SIGNAL(SetDestination(const QString &)), ccmdDialog, SLOT(on_cCopyMove_SetDestination(const QString &)));
		connect(this, SIGNAL(SetSource(const QString &)), ccmdDialog, SLOT(on_cCopyMove_SetSource(const QString &)));
		connect(this, SIGNAL(SetTotalMaximum(const qint64 &)), ccmdDialog, SLOT(on_cCopyMove_SetTotalMaximum(const qint64 &)));
		connect(this, SIGNAL(SetTotalValue(const qint64 &)), ccmdDialog, SLOT(on_cCopyMove_SetTotalValue(const qint64 &)));
		connect(ccmdDialog, SIGNAL(Cancel()), SLOT(on_cLocalCopyMove_OperationCanceled()));
		connect(ccmdDialog, SIGNAL(Background()), SLOT(on_ccmdCopyMoveDialog_Background()));
		ccmwWidget = NULL;
	} else {
		CreateWidget();
		ccmdDialog = NULL;
	} // if else

	/*// conflict dialog
	connect(this, SIGNAL(ShowConflictDialog(const QString &, const QFileInfo &, const QFileInfo &)), &ccmcConflict, SLOT(Show(const QString &, const QFileInfo &, const QFileInfo &)));
	connect(&ccmcConflict, SIGNAL(Finished(const cCopyMoveConflict::eChoice &)), SLOT(on_ccmcConflict_Finished(const cCopyMoveConflict::eChoice &)));

	// rename dialog
	connect(this, SIGNAL(ShowRenameDialog(const QString &)), &crRename, SLOT(Show(const QString &)));
	connect(&crRename, SIGNAL(Finished(const QString &)), SLOT(on_crRename_Finished(const QString &)));

#ifdef Q_WS_WIN
	// permission dialog
	connect(this, SIGNAL(ShowPermissionDialog(const QString &, const QString &)), &cpPermission, SLOT(Show(const QString &, const QString &)));
	connect(&cpPermission, SIGNAL(Finished(const cPermission::eChoice &)), SLOT(on_cpPermission_Finished(const cPermission::eChoice &)));
#endif

	// retry dialog
	connect(this, SIGNAL(ShowRetryDialog(const QString &, const QString &)), &crRetry, SLOT(Show(const QString &, const QString &)));
	connect(&crRetry, SIGNAL(Finished(const cRetry::eChoice &)), SLOT(on_crRetry_Finished(const cRetry::eChoice &)));

	// disk space dialog
	connect(this, SIGNAL(ShowDiskSpaceDialog(const QString &, const qint64 &, const qint64 &)), &cdsDiskSpace, SLOT(Show(const QString &, const qint64 &, const qint64 &)));
	connect(&cdsDiskSpace, SIGNAL(Finished(const cDiskSpace::eChoice &)), SLOT(on_cdsDiskSpace_Finished(const cDiskSpace::eChoice &)));*/

	this->spiPluginInfo = cppPackerPlugin->LoadPlugin(spiPluginInfo->qlLibrary->fileName());

	start();
} // Copy

// create widget for background operation
const void cArchiveCopy::CreateWidget()
{
	ccmwWidget = new cCopyMoveWidget();
	qhblOperations->insertWidget(cFileOperation::iQUEUED_OPERATION_POSITION, ccmwWidget);
	connect(this, SIGNAL(SetCurrentMaximum(const qint64 &)), ccmwWidget, SLOT(on_cCopyMove_SetCurrentMaximum(const qint64 &)));
	connect(this, SIGNAL(SetCurrentValue(const qint64 &)), ccmwWidget, SLOT(on_cCopyMove_SetCurrentValue(const qint64 &)));
	connect(this, SIGNAL(SetDestination(const QString &)), ccmwWidget, SLOT(on_cCopyMove_SetDestination(const QString &)));
	connect(this, SIGNAL(SetSource(const QString &)), ccmwWidget, SLOT(on_cCopyMove_SetSource(const QString &)));
	connect(this, SIGNAL(SetTotalMaximum(const qint64 &)), ccmwWidget, SLOT(on_cCopyMove_SetTotalMaximum(const qint64 &)));
	connect(this, SIGNAL(SetTotalValue(const qint64 &)), ccmwWidget, SLOT(on_cCopyMove_SetTotalValue(const qint64 &)));
	connect(ccmwWidget, SIGNAL(Cancel()), SLOT(on_cLocalCopyMove_OperationCanceled()));
} // CreateWidget

// move operation to background
const void cArchiveCopy::on_ccmdCopyMoveDialog_Background()
{
	CreateWidget();
	emit SetSource(qsSource);
	emit SetDestination(qsTarget);
	emit SetCurrentMaximum(qi64CurrentMaximum);
	emit SetTotalMaximum(qi64TotalMaximum);
	ccmdDialog->deleteLater();
	ccmdDialog = NULL;
} // on_ccmdCopyMoveDialog_Background

// copy or move operation was canceled
const void cArchiveCopy::on_cLocalCopyMove_OperationCanceled()
{
	bCanceled = true;
} // on_cLocalCopyMove_OperationCanceled

// separate thread process
void cArchiveCopy::run()
{
	// TODO run
	spiPluginInfo.qlLibrary->unload();
	spiPluginInfo.qlLibrary->deleteLater();
} // run