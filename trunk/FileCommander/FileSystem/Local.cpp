#include "FileSystem/Local.h"

#include <QtCore/QDateTime>
#include "FileSystem/Local/ShellMenu.h"
#include "FileControl/Process.h"
#include "FileSystem/Local/LocalCommon.h"

// destructor
cLocal::~cLocal()
{
	ccpdContentPluginDelayed->deleteLater();

	ClearFileTable(qhFiles);
	qhCustom.clear();

	if (saArchive.caArchive) {
		CloseArchive();
	} // if
} // ~cLocal

// activate current file
const void cLocal::ActivateCurrent(QTreeWidgetItem *qtwiFile)
{
	QFileInfo *qfiFile;

	if (saArchive.caArchive) {
		return saArchive.caArchive->ActivateCurrent(qtwiFile);
	} // if

	qfiFile = &qhFiles[qtwiFile];

	if (qfiFile->isDir()) {
		// directory
#ifdef Q_WS_WIN
		if (PathExists(qfiFile->filePath())) {
#endif
			SetPath(qfiFile->filePath());
#ifdef Q_WS_WIN
		} // if
#endif
	} else {
		// file
		// check if it's supported archive with browsing archive enabled
		if (csSettings->GetTreatArchivesLikeDirectories() && OpenArchive(qhFiles.value(qtwiFile))) {
			// going into archive
			saArchive.caArchive->SetPath("");
		} else {
			// not an (supported) archive or browsing archive disabled
			cProcess cpProcess;

			cpProcess.StartDetached('"' + qfiFile->fileName() + '"', qfiFile->path());
		} // if else
	} // if else
} // ActivateCurrent

// add file to custom file list
QTreeWidgetItem *cLocal::AddToCustomList(QTreeWidgetItem *qtwiFile)
{
	QTreeWidgetItem *qtwiNew;

	if (saArchive.caArchive) {
		return saArchive.caArchive->AddToCustomList(qtwiFile);
	} // if

	qtwiNew = qtwiFile->clone();
	qhCustom.insert(qtwiNew, qhFiles.value(qtwiFile));

	return qtwiNew;
} // AddToCustomList

// begin of searching files
const void cLocal::BeginSearch()
{
	if (saArchive.caArchive) {
		return saArchive.caArchive->BeginSearch();
	} // if

	// clear before next search (especially for branch view)
	qhCustom.clear();
} // BeginSearch

// file system can copy files to local file system
const bool cLocal::CanCopy() const
{
	if (saArchive.caArchive) {
		return saArchive.caArchive->CanCopy();
	} // if

	return true;
} // CanCopy

// file system can create directory in current location
const bool cLocal::CanCreateDir() const
{
	if (saArchive.caArchive) {
		return saArchive.caArchive->CanCreateDir();
	} // if

	return true;
} // CanCreateDir

// file system can delete files
const bool cLocal::CanDelete() const
{
	if (saArchive.caArchive) {
		return saArchive.caArchive->CanDelete();
	} // if

	return true;
} // CanDelete

// check if current path available
const bool cLocal::CheckPath()
{
#ifdef Q_WS_WIN
	while (!PathExists(qdDir.path())) {
#else
	while (!qdDir.exists()) {
#endif
		// invalid path
		if (bUnaccessible || !SetPath("..")) {
			// even root doesn't exists
			if (!bUnaccessible) {
				bUnaccessible = true;
				emit Unaccessible(this);
			} // if
			return false;
		} // if
	} // while

	// path OK
	return true;
} // CheckPath

// clear file table before next fill of it
const void cLocal::ClearFileTable(QHash<QTreeWidgetItem *, QFileInfo> &qhTable) const
{
	QHashIterator<QTreeWidgetItem *, QFileInfo> qhiFile(qhTable);
	while (qhiFile.hasNext()) {
		qhiFile.next();
		delete qhiFile.key();
	} // while
	qhTable.clear();
} // ClearFileTable

// constructor
cLocal::cLocal(const QString &qsDrive, const QString &qsRootPath, const QString &qsPath, QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings, cContentPlugin *ccpContentPlugin, cPackerPlugin *cppPackerPlugin)
{
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
	this->csSettings = csSettings;
	this->ccpContentPlugin = ccpContentPlugin;
	this->cppPackerPlugin = cppPackerPlugin;

	saArchive.caArchive = NULL;
	bUnaccessible = false;

	// signals
	ccpdContentPluginDelayed = new cContentPluginDelayed(ccpContentPlugin);
	connect(ccpdContentPluginDelayed, SIGNAL(GotColumnValue(const cContentPluginDelayed::sOutput &)), SLOT(on_ccpdContentPluginDelayed_GotColumnValue(const cContentPluginDelayed::sOutput &)));
	connect(this, SIGNAL(InterruptContentDelayed()), ccpdContentPluginDelayed, SLOT(on_InterruptContentDelayed()));
	connect(&qfswWatcher, SIGNAL(directoryChanged(const QString &)), SLOT(on_qfswWatcher_directoryChanged(const QString &)));

	// timer
	startTimer(iTIMER_INTERVAL * 1000);

	SetPath(qsDrive, qsRootPath, qsPath, true);
} // cLocal

// close archive
const void cLocal::CloseArchive()
{
	saArchive.caArchive->deleteLater();
	saArchive.caArchive = NULL;
} // CloseArchive

// create new directory
const void cLocal::CreateDir(const QString &qsName)
{
	QString qsNewDirectory;

	qsNewDirectory = qdDir.path() + '/' + qsName;
	qfswWatcher.removePath(qdDir.path());
	qdDir.mkpath(qsNewDirectory);
	qfswWatcher.addPath(qdDir.path());
} // CreateDir

// delete files in operation file list
const void cLocal::Delete(const QString &qsFilter, const cFileOperation::eOperationPosition &eopPosition)
{
	cldDelete = new cLocalDelete(qmwParent, qhblOperations, csSettings);
	connect(cldDelete, SIGNAL(finished()), SLOT(on_cLocalDelete_OperationFinished()));
	cldDelete->Delete(qfilOperation, qsFilter, eopPosition);
} // Delete

// check if specified directory exists
const bool cLocal::DirExists(const QString &qsDirectory) const
{
	QDir qdDir;

	if (saArchive.caArchive) {
		QString qsArchivePath;

		qsArchivePath = QDir::toNativeSeparators(qsDirectory.mid(this->qdDir.path().length() + 1));
		qsArchivePath = qsArchivePath.mid(qsArchivePath.indexOf(QDir::separator()) + 1);
		return saArchive.caArchive->DirExists(qsArchivePath);
	} // if

	qdDir.setPath(qsDirectory);
	return qdDir.exists();
} // DirExists

// searching of files finished
const void cLocal::EndSearch(const bool &bClearCustomOnly /* false */)
{
	
	if (saArchive.caArchive) {
		return saArchive.caArchive->EndSearch(bClearCustomOnly);
	} // if

	if (bClearCustomOnly) {
		ClearFileTable(qhCustom);
	} else {
		// clear file hash table first
		ClearFileTable(qhFiles);

		qhFiles = qhCustom;
	} // if else
} // EndSearch

// free file operation list from memory
const void cLocal::FreeOperationList(void *vFileList) const
{
	if (saArchive.caArchive) {
		return saArchive.caArchive->FreeOperationList(vFileList);
	} // if

	delete static_cast<QFileInfoList *>(vFileList);
} // FreeOperationList

// archive file path
const QFileInfo &cLocal::GetArchiveFilePath() const
{
	return saArchive.caArchive->GetArchiveFilePath();
} // GetArchiveFilePath

// get value from content plugin
const QString cLocal::GetContentPluginValue(const sContentPluginRequest &sContent)
{
	int iFlag;
	QFileInfo *qfiFile;
	QString qsValue;

	qfiFile = &qhFiles[sContent.qtwiFile];

	qsValue = ccpContentPlugin->GetPluginValue(qfiFile->filePath(), sContent.qsPlugin, sContent.qsColumn, sContent.qsUnit, &iFlag);

	if (iFlag == ft_delayed) {
		cContentPluginDelayed::sParameters spParameters;

		// thread input
		spParameters.siInput.qsFilename = qfiFile->filePath();
		spParameters.siInput.qsPlugin = sContent.qsPlugin;
		spParameters.siInput.qsColumn = sContent.qsColumn;
		spParameters.siInput.qsUnit = sContent.qsUnit;

		// thread output
		spParameters.soOutput.qtwiItem = sContent.qtwiFile;
		spParameters.soOutput.iColumn = sContent.iColumn;

		qqContentDelayedParameters.enqueue(spParameters);
	} // if

	return qsValue;
} // GetContentPluginValue

// custom file list
const QList<QTreeWidgetItem *> cLocal::GetCustomFileList() const
{
	if (saArchive.caArchive) {
		return saArchive.caArchive->GetCustomFileList();
	} // if

	return qhCustom.keys();
} // GetCustomFileList

// get file name with extension from custom list
const QString cLocal::GetCustomFileNameWithExtension(QTreeWidgetItem *qtwiFile) const
{
	if (saArchive.caArchive) {
		return saArchive.caArchive->GetCustomFileNameWithExtension(qtwiFile);
	} // if

	return qhCustom.value(qtwiFile).fileName();
} // GetCustomFileName

// get file name from custom list with full path
const QString cLocal::GetCustomFilePath(QTreeWidgetItem *qtwiFile)
{
	QFileInfo *qfiFile;
	QString qsName;

	if (saArchive.caArchive) {
		return saArchive.caArchive->GetCustomFilePath(qtwiFile);
	} // if

	qfiFile = &qhCustom[qtwiFile];

	qsName = qfiFile->filePath();

	if (qfiFile->isDir() && csSettings->GetShowBracketsAroundDirectoryName()) {
		qsName = '[' + qsName + ']';
	} // if

	return qsName;
} // GetCustomFilePath

// get tree items for current directory
const bool cLocal::GetDirectoryContent(QList<QTreeWidgetItem *> *qlFiles, const bool &bRefresh /* true */)
{
	if (saArchive.caArchive) {
		return saArchive.caArchive->GetDirectoryContent(qlFiles, bRefresh);
	} // if

	if (bRefresh) {
		// reload
		bUnaccessible = false;

		if (CheckPath()) {
			// file system accessible
			int iI;
			QDir::Filters fFilters;
			QFileInfoList qfilFiles;

			// interrupt delayed content processing
			emit InterruptContentDelayed();

			// set filter
			fFilters = QDir::Dirs | QDir::Files;
			if (csSettings->GetShowSystemFiles()) {
				fFilters |= QDir::System;
			} // if
			if (csSettings->GetShowHiddenFiles()) {
				fFilters |= QDir::Hidden;
			} // if

			// get files
			qfilFiles = qdDir.entryInfoList(fFilters);

			// clear hash table
			ClearFileTable(qhFiles);

			// add files to hash table
			for (iI = 0; iI < qfilFiles.count(); iI++) {
				const QFileInfo *qfiFile;

				qfiFile = &qfilFiles.at(iI);
				if (qfiFile->fileName() != ".") {
					qhFiles.insert(new QTreeWidgetItem(), *qfiFile);
				} // if
			} // for

			*qlFiles = qhFiles.keys();
			return true;
		} else {
			// file system unaccessible
			return false;
		} // if else
	} else {
		// no reload
		*qlFiles = qhFiles.keys();
		return true;
	} // if else
} // GetDirectoryContent

// get currently selected directory size
const qint64 cLocal::GetDirectorySize() const
{
	int iI;
	QFileInfoList qfilFiles;
	qint64 qi64Size;

	if (saArchive.caArchive) {
		return saArchive.caArchive->GetDirectorySize();
	} // if

	qfilFiles = cLocalCommon::GetFiles(qhFiles.value(qhFiles.constBegin().key()->treeWidget()->currentItem()));

	qi64Size = 0;
	for (iI = 0; iI < qfilFiles.count(); iI++) {
		qi64Size += qfilFiles.at(iI).size();
	} // for

	return qi64Size;
} // GetDirectorySize

// get current directory name
const QString cLocal::GetDirName() const
{
	if (saArchive.caArchive) {
		return saArchive.caArchive->GetDirName();
	} // if

	return qdDir.dirName();
} // GetDirName

// find out disk space information
const cFileSystem::sDiskSpace cLocal::GetDiskSpace() const
{
	return cLocalCommon::GetDiskSpace(qsRootPath);
} // GetDiskSpace

// drive represented by file system
const QString &cLocal::GetDrive() const
{
	return qsDrive;
} // GetDrive

#ifdef Q_WS_WIN
// get file attributes
const QString cLocal::GetFileAttr(QTreeWidgetItem *qtwiFile) const
{
	DWORD dwAttributes;
	QString qsAttributes;

	if (saArchive.caArchive) {
		return saArchive.caArchive->GetFileAttr(qtwiFile);
	} // if

	dwAttributes = GetFileAttributes(reinterpret_cast<LPCWSTR>(qhFiles.value(qtwiFile).filePath().unicode()));

	if (dwAttributes & FILE_ATTRIBUTE_READONLY) {
		qsAttributes = 'r';
	} // if
	if (dwAttributes & FILE_ATTRIBUTE_ARCHIVE) {
		qsAttributes += 'a';
	} // if
	if (dwAttributes & FILE_ATTRIBUTE_HIDDEN) {
		qsAttributes += 'h';
	} // if
	if (dwAttributes & FILE_ATTRIBUTE_SYSTEM) {
		qsAttributes += 's';
	} // if
	if (dwAttributes & FILE_ATTRIBUTE_COMPRESSED) {
		qsAttributes += 'c';
	} // if
	if (dwAttributes & FILE_ATTRIBUTE_ENCRYPTED) {
		qsAttributes += 'e';
	} // if

	return qsAttributes;
} // GetFileAttributes
#endif

// get file extension
const QString cLocal::GetFileExtension(QTreeWidgetItem *qtwiFile) const
{
	if (saArchive.caArchive) {
		return saArchive.caArchive->GetFileExtension(qtwiFile);
	} // if

	return qhFiles.value(qtwiFile).suffix();
} // GetFileExtension

// get icon for specified file
const QIcon cLocal::GetFileIcon(QTreeWidgetItem *qtwiFile) const
{
	if (saArchive.caArchive) {
		return saArchive.caArchive->GetFileIcon(qtwiFile);
	} // if

	return qfipIconProvider.icon(qhFiles.value(qtwiFile));
} // GetFileIcon

// file list of specified file system's type
void *cLocal::GetFileList(const QList<QTreeWidgetItem *> &qlSelected) const
{
	int iI;
	QFileInfoList *qfilFiles;

	if (saArchive.caArchive) {
		return saArchive.caArchive->GetFileList(qlSelected);
	} // if

	qfilFiles = new QFileInfoList();

	for (iI = 0; iI < qlSelected.count(); iI++) {
		qfilFiles->append(qhFiles.value(qlSelected.at(iI)));
	} // for

	return qfilFiles;
} // GetFileList

// get file name without extension
const QString cLocal::GetFileName(QTreeWidgetItem *qtwiFile, const bool &bBracketsAllowed /* true */)
{
	QFileInfo *qfiFile;
	QString qsName;

	if (saArchive.caArchive) {
		return saArchive.caArchive->GetFileName(qtwiFile, bBracketsAllowed);
	} // if

	qfiFile = &qhFiles[qtwiFile];

	if (qfiFile->fileName() == "..") {
		// special handle for ".." directory to show just both points
		qsName = "..";
	} else {
		qsName = qfiFile->completeBaseName();
	} // if else

	if (qfiFile->isDir() && bBracketsAllowed && csSettings->GetShowBracketsAroundDirectoryName()) {
		qsName = '[' + qsName + ']';
	} // if

	return qsName;
} // GetFileName

// get file name with extension
const QString cLocal::GetFileNameWithExtension(QTreeWidgetItem *qtwiFile, const bool &bBracketsAllowed /* true */)
{
	QString qsName;
	QFileInfo *qfiFile;

	if (saArchive.caArchive) {
		return saArchive.caArchive->GetFileNameWithExtension(qtwiFile, bBracketsAllowed);
	} // if

	qfiFile = &qhFiles[qtwiFile];

	qsName = qfiFile->fileName();

	if (qfiFile->isDir() && bBracketsAllowed && csSettings->GetShowBracketsAroundDirectoryName()) {
		qsName = '[' + qsName + ']';
	} // if

	return qsName;
} // GetFileNameWithExtension

// get file name with full path
const QString cLocal::GetFilePath(QTreeWidgetItem *qtwiFile) const
{
	if (saArchive.caArchive) {
		return saArchive.caArchive->GetFilePath(qtwiFile);
	} // if

	return qhFiles.value(qtwiFile).filePath();
} // GetFilePath

// get file size
const qint64 cLocal::GetFileSize(QTreeWidgetItem *qtwiFile) const
{
	if (saArchive.caArchive) {
		return saArchive.caArchive->GetFileSize(qtwiFile);
	} // if

	return qhFiles.value(qtwiFile).size();
} // GetFileSize

// get file's last modified date/time stamp
const QDateTime cLocal::GetLastModified(QTreeWidgetItem *qtwiFile) const
{
	if (saArchive.caArchive) {
		return saArchive.caArchive->GetLastModified(qtwiFile);
	} // if

	return qhFiles.value(qtwiFile).lastModified();
} // GetLastModified

// file paths from operation file list
const QStringList cLocal::GetOperationStringList() const
{
	int iI;
	QStringList qslFiles;

	for (iI = 0; iI < qfilOperation.count(); iI++) {
		qslFiles.append(qfilOperation.at(iI).filePath());
	} // for

	return qslFiles;
} // GetOperationStringList

// current path on file system
const QString cLocal::GetPath() const
{
	QString qsPath;

	if (saArchive.caArchive) {
		qsPath = '/' + saArchive.caArchive->GetPath();
	} // if

	return qdDir.path() + qsPath;
} // GetPath

// retreive path of a file
const QString cLocal::GetPath(QTreeWidgetItem *qtwiFile) const
{
	return qhFiles.value(qtwiFile).path();
} // GetPath

// selected directory list for current directory
const QStringList cLocal::GetSelectedDirectoryStringList() const
{
	QHashIterator<QTreeWidgetItem *, QFileInfo> qhiFile(qhFiles);
	QStringList qslSelectedDirectories;

	while (qhiFile.hasNext()) {
		qhiFile.next();

		if (qhiFile.key()->isSelected() && qhiFile.value().isDir() && qhiFile.value().fileName() != "..") {
			qslSelectedDirectories.append(qhiFile.value().filePath());
		} // if
	} // while

	return qslSelectedDirectories;
} // GetSelectedDirectoryStringList

// selected files in tree view
const QFileInfoList cLocal::GetSelectedFileList() const
{
	QFileInfoList qfilSelected;

	QHashIterator<QTreeWidgetItem *, QFileInfo> qhiFile(qhFiles);
	while (qhiFile.hasNext()) {
		qhiFile.next();

		if (qhiFile.key()->isSelected()) {
			qfilSelected.append(qhiFile.value());
		} // if
	} // while

	return qfilSelected;
} // GetSelectedFileList

// selected items from current directory list
const QList<QTreeWidgetItem *> cLocal::GetSelectedFiles() const
{
	QList<QTreeWidgetItem *> qlSelected;

	if (saArchive.caArchive) {
		return saArchive.caArchive->GetSelectedFiles();
	} // if

	QHashIterator<QTreeWidgetItem *, QFileInfo> qhiFile(qhFiles);
	while (qhiFile.hasNext()) {
		qhiFile.next();

		if (qhiFile.key()->isSelected()) {
			qlSelected.append(qhiFile.key());
		} // if
	} // while

	return qlSelected;
} // GetSelectedFiles

// get text for tab in directory view
const QString cLocal::GetTabText() const
{
	QString qsTabText;

	if (csSettings->GetShowDriveLetter()) {
		qsTabText = qdDir.path().at(0) + QString(": ");
	} // if

	if (qdDir.isRoot()) {
		qsTabText += '/';
	} else {
		qsTabText += qdDir.dirName();
	} // if else

	return qsTabText;
} // GetTabText

// find out name of the disk
const bool cLocal::GetVolumeName(QString *qsName) const
{
#ifdef Q_WS_WIN
	bool bResult;

	qsName->fill(32, uiVOLUME_NAME * sizeof(WCHAR));
	bResult = GetVolumeInformation(reinterpret_cast<LPCWSTR>(qsRootPath.unicode()), reinterpret_cast<LPWSTR>(qsName->data()), qsName->size() / sizeof(WCHAR), NULL, NULL, NULL, NULL, 0);

	if (bResult) {
		*qsName = qsName->left(qsName->trimmed().size() - 1);
		return true;
	} // if

	return bResult;
#else
	*qsName = qsDrive;

	return true;
#endif
} // GetVolumeName

// set path to root directory
const void cLocal::GoToRootDir()
{
	if (saArchive.caArchive) {
		CloseArchive();
	} // if

	SetPath(qsRootPath);
} // GoToRootDir

// go one directory up if possible
const void cLocal::GoToUpDir()
{
	if (saArchive.caArchive) {
		return saArchive.caArchive->GoToUpDir();
	} // if

	SetPath("..");
} // GoToUpDir

// check if file is directory
const bool cLocal::IsDir(QTreeWidgetItem *qtwiFile) const
{
	if (saArchive.caArchive) {
		return saArchive.caArchive->IsDir(qtwiFile);
	} // if

	return qhFiles.value(qtwiFile).isDir();
} // IsDir

// check if file is really file
const bool cLocal::IsFile(QTreeWidgetItem *qtwiFile) const
{
	if (saArchive.caArchive) {
		return saArchive.caArchive->IsFile(qtwiFile);
	} // if

	return qhFiles.value(qtwiFile).isFile();
} // IsFile

// content changed in archive
const void cLocal::on_caArchive_ContentChanged(const cFileSystem *cfsFileSystem) const
{
	emit ContentChanged(this);
} // on_caArchive_ContentChanged

// leave archive file system
const void cLocal::on_caArchive_LeaveFileSystem()
{
	CloseArchive();
	emit ContentChanged(this);
} // on_caArchive_LeaveFileSystem

// operation finished in archive file system
const void cLocal::on_caArchive_OperationFinished(cFileSystem *cfsFileSystem)
{
	CloseArchive();
	emit OperationFinished(this);
} // on_caArchive_OperationFinished

// got golumn value from plugin
const void cLocal::on_ccpdContentPluginDelayed_GotColumnValue(const cContentPluginDelayed::sOutput &soOutput) const
{
	emit GotColumnValue(soOutput);
} // on_ccpdContentPluginDelayed_GotColumnValue

// copy/move operation finished
const void cLocal::on_cLocalCopyMove_OperationFinished()
{
	clcmCopyMove->deleteLater();
	emit OperationFinished(this);
} // on_cLocalCopyMove_OperationFinished

// file operation finished
const void cLocal::on_cLocalDelete_OperationFinished()
{
	cldDelete->deleteLater();
	emit OperationFinished(this);
} // on_cLocalDelete_OperationFinished

// current directory content changed
const void cLocal::on_qfswWatcher_directoryChanged(const QString &path) const
{
	emit ContentChanged(this);
} // on_qfswWatcher_directoryChanged

// try to open archive
const bool cLocal::OpenArchive(const QFileInfo &qfiArchive)
{
	saArchive.caArchive = new cArchive(qsDrive, qsRootPath, qfiArchive, "", qmwParent, qhblOperations, csSettings, cppPackerPlugin);
	connect(saArchive.caArchive, SIGNAL(ContentChanged(const cFileSystem *)), SLOT(on_caArchive_ContentChanged(const cFileSystem *)));
	if (saArchive.caArchive->SetPath("", true)) {
		saArchive.qsFilePath = qfiArchive.filePath();
		connect(saArchive.caArchive, SIGNAL(LeaveFileSystem()), SLOT(on_caArchive_LeaveFileSystem()));

		return true;
	} else {
		CloseArchive();

		return false;
	} // if else
} // OpenArchive

#ifdef Q_WS_WIN
// check if path is valid
const bool cLocal::PathExists(const QString &qsPath) const
{
	bool bResult;
	HANDLE hHandle;
	WIN32_FIND_DATA fdFindData;

	hHandle = FindFirstFile(reinterpret_cast<LPCWSTR>(QString(qsPath + "/*").unicode()), &fdFindData);
	if (hHandle == INVALID_HANDLE_VALUE) {
		bResult = false;
	} else {
		bResult = true;
	} // if else
	FindClose(hHandle);

	return bResult;
} // PathExists
#endif

// write local files to this file system
const void cLocal::Read(const cFileOperationDialog::eOperation &eoOperation, const QString &qsFilter, const QString &qsDestination, const cFileOperation::eOperationPosition &eopPosition, const bool &bFullPath)
{
	if (saArchive.caArchive) {
		connect(saArchive.caArchive, SIGNAL(OperationFinished(cFileSystem *)), SLOT(on_caArchive_OperationFinished(cFileSystem *)));
		saArchive.caArchive->Read(eoOperation, qsFilter, qsDestination, eopPosition, bFullPath);
	} // if
} // Read

// start retreiving of content delayed values
const void cLocal::RetreiveContentDelayedValues()
{
	ccpdContentPluginDelayed->Start(qqContentDelayedParameters);
	qqContentDelayedParameters.clear();
} // RetreiveContentDelayedValues

// set file list for file operation
const void cLocal::SetOperationFileList(void *vFileList)
{
	if (saArchive.caArchive) {
		return saArchive.caArchive->SetOperationFileList(vFileList);
	} // if

	qfilOperation = *static_cast<QFileInfoList *>(vFileList);
} // SetOperationFileList

// change path for this file system
const void cLocal::SetPath(const QString &qsDrive, const QString &qsRootPath, const QString &qsPath, const bool &bStartup /* false */)
{
	if (saArchive.caArchive) {
		if (qsPath.startsWith(saArchive.qsFilePath)) {
			// change still in archive
			return saArchive.caArchive->SetPath(qsDrive, qsRootPath, qsPath.mid(saArchive.qsFilePath.length() + 1), bStartup);
		} else {
			// another path
			CloseArchive();
		} // if else
	} // if

	this->qsDrive = qsDrive;
	this->qsRootPath = qsRootPath;

	if (bUnaccessible) {
		bUnaccessible = false;
	} // if
	
	SetPath(qsPath, bStartup);
} // SetPath

// change path for this file system without drive change
const bool cLocal::SetPath(const QString qsPath, const bool &bStartup /* false */)
{
	bool bResult;
	QString qsNewPath;

	if (saArchive.caArchive) {
		return saArchive.caArchive->SetPath(qsPath, bStartup);
	} // if

	if (!bStartup) {
		qfswWatcher.removePath(qdDir.path());
	} // if

	// find right path
	bResult = true;
	qsNewPath = qsPath;
	while (!qdDir.exists(qsNewPath)) {
		QString qsUpDir;

		qsUpDir = QFileInfo(qsNewPath).path();
		if (qsUpDir == qsNewPath) {
			bResult = false;
			break;
		} else {
			qsNewPath = qsUpDir;
		} // if else
	} // while

	if (bResult) {
		QDir qdNotFile;
		QString qsArchive;

		qdNotFile.setPath(qsNewPath);

		if (!qdNotFile.exists()) {
			// new path is file in fact -> can be archive
			qsArchive = qsNewPath;
			qsNewPath = QFileInfo(qsNewPath).path();
		} // if

		bResult = qdDir.cd(qsNewPath);

		if (!qdNotFile.exists()) {
			// try to open archive
			if (OpenArchive(qsArchive)) {
				saArchive.caArchive->SetPath(qsPath.mid(qsArchive.length() + 1));
			} // if
		} // if

		if (bResult && !saArchive.caArchive) {
			qfswWatcher.addPath(qdDir.path());
			emit ContentChanged(this);
		} // if
	} // if

	return bResult;
} // SetPath

// custom context menu on right click
const cFileSystem::eContextAction cLocal::ShowContextMenu(const QPoint &qcPosition
#ifdef Q_WS_WIN
		, const HWND hwParent
#endif
	)
{
	// shell context menu
	cShellMenu csmMenu(
#ifdef Q_WS_WIN
		hwParent
#endif
	);
	int iI;
	QFileInfoList qfilSelected;
	QStringList qslSelected;

	if (saArchive.caArchive) {
		return saArchive.caArchive->ShowContextMenu(qcPosition
#ifdef Q_WS_WIN
				, hwParent
#endif
			);
	} // if

	qfilSelected = GetSelectedFileList();

#ifdef Q_WS_WIN
	for (iI = 0; iI < qfilSelected.count(); iI++) {
		const QFileInfo *qfiFile;

		qfiFile = &qfilSelected.at(iI);

		if (qfiFile->fileName() == "..") {
			// replace ".." with current path
			qslSelected.append(QDir::toNativeSeparators((qfiFile->path())));
		} else {
			qslSelected.append(QDir::toNativeSeparators((qfiFile->filePath())));
		} // if else
	} // for

	csmMenu.Show(qslSelected, qhFiles.constBegin().key()->treeWidget()->viewport()->mapToGlobal(qcPosition));
#endif

	return ContextNothing;
} // ShowContextMenu

// periodical file system check
void cLocal::timerEvent(QTimerEvent *event)
{
	if (!bUnaccessible) {
		CheckPath();
	} // if
} // timerEvent

// try if path exists on file system
const bool cLocal::TryPath(const QString &qsPath) const
{
	bool bResult;
	QDir qdDir;
	QString qsNewPath;

	bResult = false;
	qsNewPath = qsPath;

	do {
#ifndef Q_WS_WIN
		qdDir.setPath(qsNewPath);
#endif
#ifdef Q_WS_WIN
		if (PathExists(qsNewPath)) {
#else
		if (qdDir.exists()) {
#endif
			bResult = true;
		} else {
			QString qsUpDir;

			qsUpDir = QFileInfo(qsNewPath).path();
			if (qsUpDir == qsNewPath) {
				break;
			} else {
				qsNewPath = qsUpDir;
			} // if else
		} // if else
	} while (!bResult);

	return bResult;
} // TryPath

// file system type
const cFileSystem::eType cLocal::Type() const
{
	if (saArchive.caArchive) {
		return saArchive.caArchive->Type();
	} // if

	return Local;
} // Type

// write local files to this file system
const void cLocal::Write(const cFileOperationDialog::eOperation &eoOperation, const QStringList &qslSources, const QString &qsFilter, const QString &qsDestination, const cFileOperation::eOperationPosition &eopPosition)
{
	clcmCopyMove = new cLocalCopyMove(qmwParent, qhblOperations, csSettings);
	connect(clcmCopyMove, SIGNAL(finished()), SLOT(on_cLocalCopyMove_OperationFinished()));
	clcmCopyMove->CopyMove(eoOperation, qslSources, qsFilter, qsDestination, eopPosition);
} // Write
