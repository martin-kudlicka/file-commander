#include "FileSystem/Archive/ArchiveCopy.h"

#include "FileSystem/CopyMoveConflict.h"
#include <QtCore/QDir>
#include "FileSystem/Archive/ArchiveCommon.h"
#include <QtGui/QMessageBox>

QHash<QString, cArchiveCopy *> cArchiveCopy::qhCallback;	///< callback function table for several background operations (static class variable)

// constructor
cArchiveCopy::cArchiveCopy(QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings)
{
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
	this->csSettings = csSettings;

	bCanceled = false;
	qi64TotalMaximum = 0;
} // cArchiveCopy

// continue after unsuccessfull file extraction
const void cArchiveCopy::CheckContinue(const int &iErrorCode, cContinue::eChoice *ecContinue)
{
	if (*ecContinue != cContinue::YesToAll) {
		// show dialog
		emit ShowContinueDialog(GetErrorString(iErrorCode), qsSource);
		// wait for answer
		qsPause.acquire();

		if (ecContinueCurrent == cContinue::YesToAll) {
			// memorize permanent answer
			*ecContinue = cContinue::YesToAll;
		} // if
	} // if
} // CheckContinue

// start of copy or move operation
const void cArchiveCopy::Copy(const QList<tHeaderData> &qlOperation, const QFileInfo &qfiArchive, const QString &qsArchivePath, const QString &qsFilter, const QHash<QString, QHash<QTreeWidgetItem *, tHeaderData> *> &qhDirectories, const QString &qsDestination, cPackerPlugin *cppPackerPlugin, cPackerPlugin::sPluginInfo *spiPluginInfo, const cFileOperation::eOperationPosition &eopPosition, const bool &bFullPath)
{
	this->qlOperation = qlOperation;
	this->qfiArchive = qfiArchive;
	if (bFullPath) {
		if (qsArchivePath != ".") {
			this->qsArchivePath = qsArchivePath;
		} // if
	} else {
		this->qsArchivePath = '.';
	} // if else
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
		connect(this, SIGNAL(SetCurrentValue(const qint64 &)), ccmdDialog, SLOT(on_cCopyMove_SetCurrentValue(const qint64 &)), Qt::BlockingQueuedConnection);
		connect(this, SIGNAL(SetDestination(const QString &)), ccmdDialog, SLOT(on_cCopyMove_SetDestination(const QString &)));
		connect(this, SIGNAL(SetSource(const QString &)), ccmdDialog, SLOT(on_cCopyMove_SetSource(const QString &)));
		connect(this, SIGNAL(SetTotalMaximum(const qint64 &)), ccmdDialog, SLOT(on_cCopyMove_SetTotalMaximum(const qint64 &)));
		connect(this, SIGNAL(SetTotalValue(const qint64 &)), ccmdDialog, SLOT(on_cCopyMove_SetTotalValue(const qint64 &)), Qt::BlockingQueuedConnection);
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
	connect(&crRename, SIGNAL(Finished()), SLOT(on_crRename_Finished()));

#ifdef Q_WS_WIN
	// permission dialog
	connect(&cpPermission, SIGNAL(Finished(const cPermission::eChoice &)), SLOT(on_cpPermission_Finished(const cPermission::eChoice &)));
#endif

	// continue dialog
	connect(this, SIGNAL(ShowContinueDialog(const QString &, const QString &)), &ccContinue, SLOT(Show(const QString &, const QString &)));
	connect(&ccContinue, SIGNAL(Finished(const cContinue::eChoice &)), SLOT(on_ccContinue_Finished(const cContinue::eChoice &)));

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
	connect(this, SIGNAL(SetCurrentValue(const qint64 &)), ccmwWidget, SLOT(on_cCopyMove_SetCurrentValue(const qint64 &)), Qt::BlockingQueuedConnection);
	connect(this, SIGNAL(SetDestination(const QString &)), ccmwWidget, SLOT(on_cCopyMove_SetDestination(const QString &)));
	connect(this, SIGNAL(SetSource(const QString &)), ccmwWidget, SLOT(on_cCopyMove_SetSource(const QString &)));
	connect(this, SIGNAL(SetTotalMaximum(const qint64 &)), ccmwWidget, SLOT(on_cCopyMove_SetTotalMaximum(const qint64 &)));
	connect(this, SIGNAL(SetTotalValue(const qint64 &)), ccmwWidget, SLOT(on_cCopyMove_SetTotalValue(const qint64 &)), Qt::BlockingQueuedConnection);
	connect(ccmwWidget, SIGNAL(Cancel()), SLOT(on_cLocalCopyMove_OperationCanceled()));
} // CreateWidget

// get error string from error code
const QString cArchiveCopy::GetErrorString(const int &iError) const
{
	switch (iError) {
		case E_END_ARCHIVE:
			return tr("no more files in archive");
		case E_NO_MEMORY:
			return tr("not enough memory");
		case E_BAD_DATA:
			return tr("data is bad");
		case E_BAD_ARCHIVE:
			return tr("CRC error in archive data");
		case E_UNKNOWN_FORMAT:
			return tr("archive format unknown");
		case E_EOPEN:
			return tr("Cannot open existing file");
		case E_ECREATE:
			return tr("cannot create file");
		case E_ECLOSE:
			return tr("error closing file");
		case E_EREAD:
			return tr("error reading from file");
		case E_EWRITE:
			return tr("error writing to file");
		case E_SMALL_BUF:
			return tr("buffer too small");
		case E_EABORTED:
			return tr("function aborted by user");
		case E_NO_FILES:
			return tr("no files found");
		case E_TOO_MANY_FILES:
			return tr("too many files to pack");
		case E_NOT_SUPPORTED:
			return tr("function not supported");
	} // switch
	
	return QString();
} // ///< get error string from error code

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

// continue dialog closed with user response
const void cArchiveCopy::on_ccContinue_Finished(const cContinue::eChoice &ecResponse)
{
	ecContinueCurrent = ecResponse;
	qsPause.release();
} // on_ccContinue_Finished

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

#ifdef Q_WS_WIN
// permission dialog closed with user response
const void cArchiveCopy::on_cpPermission_Finished(const cPermission::eChoice &ecResponse)
{
	ecPermissionCurrent = ecResponse;
	qsPause.release();
} // on_cpPermission_Finished
#endif

// rename dialog closed with user's reponse
const void cArchiveCopy::on_crRename_Finished()
{
	qsPause.release();
} // on_crRename_Finished

#ifdef Q_WS_WIN
// callback progress function
int __stdcall cArchiveCopy::ProcessDataProc(char *cFileName, int iSize)
{
	if (cFileName) {
		return qhCallback.value(cFileName)->ProcessDataProc2(cFileName, iSize);
	} else {
		return true;
	} // if else
} // ProcessDataProc

// nonstatic callback progress function
const int cArchiveCopy::ProcessDataProc2(char *cFileName, int iSize)
{
	qint64 qi64CurrentValue, qi64TotalValue;

	this->qi64CurrentValue += iSize;
	this->qi64TotalValue += iSize;

	qi64CurrentValue = this->qi64CurrentValue;
	qi64TotalValue = this->qi64TotalValue;

	emit SetCurrentValue(qi64CurrentValue);
	emit SetTotalValue(qi64TotalValue);

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
			qi64CurrentMaximum = thdHeaderData.UnpSize;
			emit SetCurrentMaximum(qi64CurrentMaximum);

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
				int iErrorCode;
				QDir qdDir;

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
					spiPluginInfo.tpfProcessFile(hArchive, PK_SKIP, NULL, NULL);
					continue;
				} else {
					if (ecrCheck == cFileOperation::Cancel) {
						break;
					} // if
				} // if else

#ifdef Q_WS_WIN
				// check readonly permission
				ecrCheck = cFileOperation::CheckPermission(&cpPermission, qsTarget, &ecPermission, &ecPermissionCurrent, &qsPause);
				if (ecrCheck == cFileOperation::NextFile) {
					qi64TotalValue += thdHeaderData.UnpSize;
					spiPluginInfo.tpfProcessFile(hArchive, PK_SKIP, NULL, NULL);
					continue;
				} else {
					if (ecrCheck == cFileOperation::Cancel) {
						break;
					} // if
				} // if else
#endif

				// make destination path if doesn't exist
				qdDir.mkpath(QFileInfo(qsTarget).path());

				// to call right callback function (several variants for various plugins)
				qhCallback.insert(QFileInfo(qsTarget).fileName(), this);
				qhCallback.insert(qsTarget, this);
				qhCallback.insert(thdHeaderData.FileName, this);
				// extract file
				iErrorCode = spiPluginInfo.tpfProcessFile(hArchive, PK_EXTRACT, NULL, qsTarget.toLatin1().data());
				qhCallback.remove(QFileInfo(qsTarget).fileName());
				qhCallback.remove(qsTarget);
				qhCallback.remove(thdHeaderData.FileName);

				// check for errors
				if (iErrorCode) {
					if (bCanceled) {
						// user abort
						ecContinueCurrent = cContinue::No;
					} else {
						// other fault
						CheckContinue(iErrorCode, &ecContinue);
					} // if else
				} // if
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
