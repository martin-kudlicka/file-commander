#include "FileSystem/Archive/ArchiveCopy.h"

#include "FileSystem/CopyMoveConflict.h"
#include "FileSystem/DiskSpace.h"
#include "FileSystem/Archive/Continue.h"

bool cArchiveCopy::bCanceled;				///< operation in progress is canceled (static class variable)
qint64 cArchiveCopy::qi64CurrentValue;	///< current file progress (static class variable)
qint64 cArchiveCopy::qi64TotalValue;	///< total progress (static class variable)

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
const void cArchiveCopy::Copy(const QList<tHeaderData> &qlOperation, const QFileInfo &qfiArchive, const QString &qsFilter, const QHash<QString, QHash<QTreeWidgetItem *, tHeaderData> *> &qhDirectories, const QString &qsDestination, cPackerPlugin *cppPackerPlugin, cPackerPlugin::sPluginInfo *spiPluginInfo, const cFileOperation::eOperationPosition &eopPosition)
{
	this->qlOperation = qlOperation;
	this->qfiArchive = qfiArchive;
	this->qsFilter = qsFilter;
	this->qhDirectories = qhDirectories;
	this->qsDestination = qsDestination;

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

// get file list to extract and count size of all those files
const QStringList cArchiveCopy::GetFilesToExtractAndCountTotalSize()
{
	int iI;
	QStringList qslFiles;

	for (iI = 0; iI < qlOperation.count(); iI++) {
		const tHeaderData *thdFile;

		thdFile = &qlOperation.at(iI);
		qslFiles.append(thdFile->FileName);
		if (thdFile->FileAttr & cPackerPlugin::iDIRECTORY) {
			qslFiles += GetFilesToExtractAndCountTotalSizeInDirectory(thdFile->FileName);
		} else {
			qi64TotalMaximum += thdFile->UnpSize;
		} // if else
	} // for

	return qslFiles;
} // GetFilesToExtractAndCountTotalSize

// get file list to extract from specified source directory
const QStringList cArchiveCopy::GetFilesToExtractAndCountTotalSizeInDirectory(const char *cDirectory)
{
	QHash<QTreeWidgetItem *, tHeaderData> *qhDirectory;
	QStringList qslFiles;

	qhDirectory = qhDirectories.value(cDirectory);

	QHashIterator<QTreeWidgetItem *, tHeaderData> qhiFile(*qhDirectory);
	while (qhiFile.hasNext()) {
		qhiFile.next();

		if (QString(qhiFile.value().FileName).endsWith("..")) {
			// skip ".." directory
			continue;
		} // if

		qslFiles.append(qhiFile.value().FileName);
		if (qhiFile.value().FileAttr & cPackerPlugin::iDIRECTORY) {
			qslFiles += GetFilesToExtractAndCountTotalSizeInDirectory(qhiFile.value().FileName);
		} else {
			qi64TotalMaximum += qhiFile.value().UnpSize;
		} // if else
	} // while

	return qslFiles;
} // GetFilesToExtractAndCountTotalSizeInDirectory

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

#ifdef Q_WS_WIN
// callback progress function
int __stdcall cArchiveCopy::ProcessDataProc(char *cFileName, int iSize)
{
	qi64CurrentValue += iSize;
	qi64TotalValue += iSize;
	//emit SetCurrentValue(qi64CurrentValue);
	//emit SetCurrentMaximum(qi64TotalValue);
	//QApplication::processEvents();

	return !bCanceled;
} // ProcessDataProc
#endif

// separate thread process
void cArchiveCopy::run()
{
	cContinue::eChoice ecContinue;
	cCopyMoveConflict::eChoice ecConflict;
	cDiskSpace::eChoice ecDiskSpace;
#ifdef Q_WS_WIN
	cPermission::eChoice ecPermission;
#endif
	HANDLE hArchive;
	qint64 qi64TotalValue;
	QStringList qslToExtract;
	tOpenArchiveData toadArchiveData;

	// collect files to extract
	qslToExtract = GetFilesToExtractAndCountTotalSize();

	// prepare progress bar
	emit SetTotalMaximum(qi64TotalMaximum);

	// get default modes
	ecConflict = cFileOperation::GetDefaultOverwriteMode(csSettings);
#ifdef Q_WS_WIN
	ecPermission = cFileOperation::GetDefaultReadonlyOverwritePermission(csSettings);
#endif
	ecContinue = cContinue::Ask;
	ecDiskSpace = cDiskSpace::Ask;

	// open archive
	toadArchiveData.ArcName = new char[qfiArchive.filePath().length() + 1];
	strcpy(toadArchiveData.ArcName, qfiArchive.filePath().toLatin1().constData());
	toadArchiveData.OpenMode = PK_OM_EXTRACT;
	hArchive = spiPluginInfo.toaOpenArchive(&toadArchiveData);

#ifdef Q_WS_WIN
	// set callback function
	spiPluginInfo.tspdpSetProcessDataProc(hArchive, &cArchiveCopy::ProcessDataProc);
#endif

	// extract files
	qi64TotalValue = 0;
	while (true) {
		cContinue::eChoice ecContinueCurrent;
		cCopyMoveConflict::eChoice ecConflictCurrent;
		cDiskSpace::eChoice ecDiskSpaceCurrent;
#ifdef Q_WS_WIN
		cPermission::eChoice ecPermissionCurrent;
#endif
		tHeaderData thdHeaderData;

		memset(&thdHeaderData, 0, sizeof(tHeaderData));
		if (spiPluginInfo.trhReadHeader(hArchive, &thdHeaderData)) {
			// no other file in archive
			break;
		} // if

		if (qslToExtract.contains(thdHeaderData.FileName) && cFileOperation::SuitsFilter(thdHeaderData.FileName, qsFilter)) {
			// extract file
		} // if

		// cancel conditions
		if (ecDiskSpaceCurrent == cDiskSpace::No || ecConflictCurrent == cCopyMoveConflict::Cancel || ecContinueCurrent == cContinue::No
#ifdef Q_WS_WIN
			 || ecPermissionCurrent == cPermission::Cancel
#endif
			) {
			// process aborted
			break;
		} // if
	} // while

	// close archive
	spiPluginInfo.tcaCloseArchive(hArchive);
	delete toadArchiveData.ArcName;

	spiPluginInfo.qlLibrary->unload();
	spiPluginInfo.qlLibrary->deleteLater();
} // run