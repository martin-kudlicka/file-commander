#include "FileSystem/Archive/ArchiveCopy.h"

#include "FileSystem/CopyMoveConflict.h"
#include <QtCore/QDir>
#include "FileSystem/Archive/ArchiveCommon.h"

cArchiveCopy *cArchiveCopy::cacCallback;	///< to handle callback in static function (static class variable)

// constructor
cArchiveCopy::cArchiveCopy(QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings)
{
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
	this->csSettings = csSettings;

	bCanceled = false;
	qi64TotalMaximum = 0;

	cacCallback = this;
} // cArchiveCopy

// start of copy or move operation
const void cArchiveCopy::Copy(const QList<tHeaderData> &qlOperation, const QFileInfo &qfiArchive, const QString &qsArchivePath, const QString &qsFilter, const QHash<QString, QHash<QTreeWidgetItem *, tHeaderData> *> &qhDirectories, const QString &qsDestination, cPackerPlugin *cppPackerPlugin, cPackerPlugin::sPluginInfo *spiPluginInfo, const cFileOperation::eOperationPosition &eopPosition, const bool &bFullPath /* true */)
{
	this->qlOperation = qlOperation;
	this->qfiArchive = qfiArchive;
	if (qsArchivePath != ".") {
		this->qsArchivePath = qsArchivePath;
	} // if
	this->qsFilter = qsFilter;
	this->qhDirectories = qhDirectories;
	this->qsDestination = qsDestination;
	this->bFullPath = bFullPath;

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

	// conflict dialog
	connect(&ccmcConflict, SIGNAL(Finished(const cCopyMoveConflict::eChoice &)), SLOT(on_ccmcConflict_Finished(const cCopyMoveConflict::eChoice &)));

	// rename dialog
	connect(&crRename, SIGNAL(Finished(const QString &)), SLOT(on_crRename_Finished(const QString &)));

/*#ifdef Q_WS_WIN
	// permission dialog
	connect(this, SIGNAL(ShowPermissionDialog(const QString &, const QString &)), &cpPermission, SLOT(Show(const QString &, const QString &)));
	connect(&cpPermission, SIGNAL(Finished(const cPermission::eChoice &)), SLOT(on_cpPermission_Finished(const cPermission::eChoice &)));
#endif

	// retry dialog
	connect(this, SIGNAL(ShowRetryDialog(const QString &, const QString &)), &crRetry, SLOT(Show(const QString &, const QString &)));
	connect(&crRetry, SIGNAL(Finished(const cRetry::eChoice &)), SLOT(on_crRetry_Finished(const cRetry::eChoice &)));*/

	// disk space dialog
	connect(&cdsDiskSpace, SIGNAL(Finished(const cDiskSpace::eChoice &)), SLOT(on_cdsDiskSpace_Finished(const cDiskSpace::eChoice &)));

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

// conflict dialog closed with user response
const void cArchiveCopy::on_ccmcConflict_Finished(const cCopyMoveConflict::eChoice &ecResponse)
{
	ecConflictCurrent = ecResponse;
	qsPause.release();
} // on_ccmcConflict_Finished

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

// disk space dialog closed with user response
const void cArchiveCopy::on_cdsDiskSpace_Finished(const cDiskSpace::eChoice &ecResponse)
{
	ecDiskSpaceCurrent = ecResponse;
	qsPause.release();
} // on_cdsDiskSpace_Finished

// copy or move operation was canceled
const void cArchiveCopy::on_cLocalCopyMove_OperationCanceled()
{
	bCanceled = true;
} // on_cLocalCopyMove_OperationCanceled

// rename dialog closed with user's reponse
const void cArchiveCopy::on_crRename_Finished()
{
	qsPause.release();
} // on_crRename_Finished

#ifdef Q_WS_WIN
// callback progress function
int __stdcall cArchiveCopy::ProcessDataProc(char *cFileName, int iSize)
{
	return cacCallback->ProcessDataProc2(cFileName, iSize);
} // ProcessDataProc

// nonstatic callback progress function
const int cArchiveCopy::ProcessDataProc2(char *cFileName, int iSize)
{
	qi64CurrentValue += iSize;
	qi64TotalValue += iSize;
	emit SetCurrentValue(qi64CurrentValue);
	emit SetCurrentMaximum(qi64TotalValue);
	//QApplication::processEvents();

	return !bCanceled;
} // ProcessDataProc2
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
		tHeaderData thdHeaderData;

		memset(&thdHeaderData, 0, sizeof(tHeaderData));
		if (spiPluginInfo.trhReadHeader(hArchive, &thdHeaderData)) {
			// no other file in archive
			break;
		} // if

		if (qslToExtract.contains(thdHeaderData.FileName) && cFileOperation::SuitsFilter(thdHeaderData.FileName, qsFilter)) {
			// extract file
			QString qsSource, qsTarget;

			qsSource = qfiArchive.filePath() + '/' + thdHeaderData.FileName;
			if (bFullPath) {
				qsTarget = thdHeaderData.FileName;
			} else {
				qsTarget = QFileInfo(thdHeaderData.FileName).fileName();
			} // if else
			qsTarget = cFileOperation::GetWildcardedName(QFileInfo(qsTarget), qsArchivePath, qsDestination);

			emit SetSource(qsSource);
			emit SetDestination(qsTarget);
			qi64CurrentValue = 0;
			emit SetCurrentMaximum(thdHeaderData.UnpSize);

			if (thdHeaderData.FileAttr & cPackerPlugin::iDIRECTORY) {
				// extract directory
				if (bFullPath) {
					QDir qdDir;

					qdDir.mkpath(qsTarget);
				} // if
				spiPluginInfo.tpfProcessFile(hArchive, PK_SKIP, NULL, NULL);
			} else {
				// extract file
				cFileOperation::eCheckResult ecrCheck;

				// check disk space on target
				ecrCheck = cFileOperation::CheckDiskSpace(&cdsDiskSpace, qsSource, qsTarget, thdHeaderData.UnpSize, &ecDiskSpace, &ecDiskSpaceCurrent, &qsPause);
				if (ecrCheck == cFileOperation::NextFile) {
					qi64TotalValue += thdHeaderData.UnpSize;
					spiPluginInfo.tpfProcessFile(hArchive, PK_SKIP, NULL, NULL);
					continue;
				} else {
					if (ecrCheck == cFileOperation::Cancel) {
						break;
					} // if
				} // if else

				// conflict solving
				ecrCheck = cFileOperation::CheckConflict(cFileOperationDialog::CopyOperation, &ccmcConflict, &crRename, QFileInfo(qsSource).fileName(), thdHeaderData.UnpSize, cArchiveCommon::ToQDateTime(thdHeaderData.FileTime), qsTarget, &ecConflict, &ecConflictCurrent, &qsPause);
				if (ecrCheck == cFileOperation::NextFile) {
					qi64TotalValue += thdHeaderData.UnpSize;
					continue;
				} else {
					if (ecrCheck == cFileOperation::Cancel) {
						break;
					} // if
				} // if else
			} // if else
		} else {
			// skip file
			spiPluginInfo.tpfProcessFile(hArchive, PK_SKIP, NULL, NULL);
		} // if else

		// stop conditions
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

	// close dialog or widget
	if (ccmdDialog) {
		ccmdDialog->deleteLater();
	} else {
		ccmwWidget->deleteLater();
	} // if else
} // run