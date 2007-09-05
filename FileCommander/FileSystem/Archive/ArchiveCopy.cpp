#include "FileSystem/Archive/ArchiveCopy.h"

// constructor
cArchiveCopy::cArchiveCopy(QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings)
{
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
	this->csSettings = csSettings;

	//bCanceled = false;
	//qi64TotalMaximum = 0;
} // cLocalCopyMove

// start of copy or move operation
const void cArchiveCopy::Copy(const QStringList &qslOperation, const QFileInfo &qfiArchive, const QString &qsDestination, const QString &qsFilter, cPackerPlugin *cppPackerPlugin, cPackerPlugin::sPluginInfo *spiPluginInfo, const cFileOperation::eOperationPosition &eopPosition)
{
	this->qslOperation = qslOperation;
	this->qfiArchive = qfiArchive;
	this->qsDestination = qsDestination;
	this->qsFilter = qsFilter;

	this->spiPluginInfo = cppPackerPlugin->LoadPlugin(spiPluginInfo->qlLibrary->fileName());

	start();
} // Copy

// separate thread process
void cArchiveCopy::run()
{
	// TODO run
	spiPluginInfo.qlLibrary->unload();
	spiPluginInfo.qlLibrary->deleteLater();
} // run