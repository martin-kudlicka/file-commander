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

	if (caArchive) {
		caArchive->deleteLater();
	} // if
} // ~cLocal

// activate current file
const void cLocal::ActivateCurrent(QTreeWidgetItem *qtwiFile)
{
	QFileInfo *qfiFile;

	if (caArchive) {
		return caArchive->ActivateCurrent(qtwiFile);
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
		if (!(csSettings->GetTreatArchivesLikeDirectories() && OpenArchive(qhFiles.value(qtwiFile)))) {
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

	qtwiNew = qtwiFile->clone();
	qhCustom.insert(qtwiNew, qhFiles.value(qtwiFile));

	return qtwiNew;
} // AddToCustomList

// begin of searching files
const void cLocal::BeginSearch()
{
	// clear before next search (especially for branch view)
	qhCustom.clear();
} // BeginSearch

// file system can copy files to local file system
const bool cLocal::CanCopy() const
{
	if (caArchive) {
		return caArchive->CanCopy();
	} // if

	return true;
} // CanCopy

// file system can delete files
const bool cLocal::CanDelete() const
{
	if (caArchive) {
		return caArchive->CanDelete();
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
		if (!SetPath("..")) {
			// even root doesn't exists
			emit Unaccessible();
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

	caArchive = NULL;

	ccpdContentPluginDelayed = new cContentPluginDelayed(ccpContentPlugin);
	connect(ccpdContentPluginDelayed, SIGNAL(GotColumnValue(const cContentPluginDelayed::sOutput &)), SLOT(on_ccpdContentPluginDelayed_GotColumnValue(const cContentPluginDelayed::sOutput &)));
	connect(this, SIGNAL(InterruptContentDelayed()), ccpdContentPluginDelayed, SLOT(on_InterruptContentDelayed()));

	connect(&qfswWatcher, SIGNAL(directoryChanged(const QString &)), SLOT(on_qfswWatcher_directoryChanged(const QString &)));

	SetPath(qsDrive, qsRootPath, qsPath, true);
} // cLocal

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

	if (caArchive) {
		QString qsArchivePath;

		qsArchivePath = QDir::toNativeSeparators(qsDirectory.mid(this->qdDir.path().length() + 1));
		qsArchivePath = qsArchivePath.mid(qsArchivePath.indexOf(QDir::separator()) + 1);
		return caArchive->DirExists(qsArchivePath);
	} // if

	qdDir.setPath(qsDirectory);
	return qdDir.exists();
} // DirExists

// searching of files finished
const void cLocal::EndSearch(const bool &bClearCustomOnly /* false */)
{
	if (bClearCustomOnly) {
		ClearFileTable(qhCustom);
	} else {
		// clear file hash table first
		ClearFileTable(qhFiles);

		qhFiles = qhCustom;
	} // if else
} // EndSearch

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
	return qhCustom.keys();
} // GetCustomFileList

// get file name with extension from custom list
const QString cLocal::GetCustomFileNameWithExtension(QTreeWidgetItem *qtwiFile) const
{
	return qhCustom.value(qtwiFile).fileName();
} // GetCustomFileName

// get file name from custom list with full path
const QString cLocal::GetCustomFilePath(QTreeWidgetItem *qtwiFile)
{
	QFileInfo *qfiFile;
	QString qsName;

	qfiFile = &qhCustom[qtwiFile];

	qsName = qfiFile->filePath();

	if (qfiFile->isDir() && csSettings->GetShowBracketsAroundDirectoryName()) {
		qsName = '[' + qsName + ']';
	} // if

	return qsName;
} // GetCustomFilePath

// get tree items for current directory
QList<QTreeWidgetItem *> cLocal::GetDirectoryContent(const bool &bRefresh /* true */)
{
	if (caArchive) {
		return caArchive->GetDirectoryContent(bRefresh);
	} // if

	if (bRefresh) {
		// reload
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

			return qhFiles.keys();
		} else {
			// file system unacessible
			return QList<QTreeWidgetItem *>();
		} // if else
	} else {
		// no reload
		return qhFiles.keys();
	} // if else
} // GetDirectoryContent

// get currently selected directory size
const qint64 cLocal::GetDirectorySize() const
{
	int iI;
	QFileInfoList qfilFiles;
	qint64 qi64Size;

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
	if (caArchive) {
		return caArchive->GetDirName();
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

	if (caArchive) {
		return caArchive->GetFileAttr(qtwiFile);
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
	if (caArchive) {
		return caArchive->GetFileExtension(qtwiFile);
	} // if

	return qhFiles.value(qtwiFile).suffix();
} // GetFileExtension

// get icon for specified file
const QIcon cLocal::GetFileIcon(QTreeWidgetItem *qtwiFile) const
{
	if (caArchive) {
		return caArchive->GetFileIcon(qtwiFile);
	} // if

	return qfipIconProvider.icon(qhFiles.value(qtwiFile));
} // GetFileIcon

// file list of specified file system's type
void *cLocal::GetFileList(const QList<QTreeWidgetItem *> &qlSelected) const
{
	int iI;
	QFileInfoList *qfilFiles;

	if (caArchive) {
		return caArchive->GetFileList(qlSelected);
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

	if (caArchive) {
		return caArchive->GetFileName(qtwiFile, bBracketsAllowed);
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

	if (caArchive) {
		return caArchive->GetFileNameWithExtension(qtwiFile, bBracketsAllowed);
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
	if (caArchive) {
		return caArchive->GetFilePath(qtwiFile);
	} // if

	return qhFiles.value(qtwiFile).filePath();
} // GetFilePath

// get file size
const qint64 cLocal::GetFileSize(QTreeWidgetItem *qtwiFile) const
{
	if (caArchive) {
		return caArchive->GetFileSize(qtwiFile);
	} // if

	return qhFiles.value(qtwiFile).size();
} // GetFileSize

// get file's last modified date/time stamp
const QDateTime cLocal::GetLastModified(QTreeWidgetItem *qtwiFile) const
{
	if (caArchive) {
		return caArchive->GetLastModified(qtwiFile);
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

	if (caArchive) {
		qsPath = '/' + caArchive->GetPath();
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

		if (qhiFile.key()->isSelected() && qhiFile.value().isDir()) {
			qslSelectedDirectories.append(qhiFile.value().filePath());
		} // if
	} // while

	return qslSelectedDirectories;
} // GetSelectedDirectoryStringList

// selected files in tree view
const QFileInfoList cLocal::GetSelectedFiles() const
{
	QHashIterator<QTreeWidgetItem *, QFileInfo> qhiFile(qhFiles);
	QFileInfoList qfilSelected;

	while (qhiFile.hasNext()) {
		qhiFile.next();

		if (qhiFile.key()->isSelected()) {
			qfilSelected.append(qhiFile.value());
		} // if
	} // while

	return qfilSelected;
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
const QString cLocal::GetVolumeName() const
{
	QString qsName;

#ifdef Q_WS_WIN
	qsName.fill(32, uiVOLUME_NAME * sizeof(WCHAR));
	GetVolumeInformation(reinterpret_cast<LPCWSTR>(qsRootPath.unicode()), reinterpret_cast<LPWSTR>(qsName.data()), qsName.size() / sizeof(WCHAR), NULL, NULL, NULL, NULL, 0);
	qsName = qsName.left(qsName.trimmed().size() - 1);
#else
	qsName = qsDrive;
#endif

	return qsName;
} // GetVolumeName

// set path to root directory
const void cLocal::GoToRootDir()
{
	if (caArchive) {
		caArchive->deleteLater();
		caArchive = NULL;
	} // if

	SetPath(qsRootPath);
} // GoToRootDir

// go one directory up if possible
const void cLocal::GoToUpDir()
{
	if (caArchive) {
		return caArchive->GoToUpDir();
	} // if

	SetPath("..");
} // GoToUpDir

// check if file is directory
const bool cLocal::IsDir(QTreeWidgetItem *qtwiFile) const
{
	if (caArchive) {
		return caArchive->IsDir(qtwiFile);
	} // if

	return qhFiles.value(qtwiFile).isDir();
} // IsDir

// check if file is really file
const bool cLocal::IsFile(QTreeWidgetItem *qtwiFile) const
{
	if (caArchive) {
		return caArchive->IsFile(qtwiFile);
	} // if

	return qhFiles.value(qtwiFile).isFile();
} // IsFile

// local file system test
const bool cLocal::IsLocal() const
{
	if (caArchive) {
		return caArchive->IsLocal();
	} // if

	return true;
} // IsLocal

// content changed in archive
const void cLocal::on_caArchive_ContentChanged(const cFileSystem *cfsFileSystem) const
{
	emit ContentChanged(this);
} // on_caArchive_ContentChanged

// leave archive file system
const void cLocal::on_caArchive_LeaveFileSystem()
{
	caArchive->deleteLater();
	caArchive = NULL;
	emit ContentChanged(this);
} // on_caArchive_LeaveFileSystem

// operation finished in archive file system
const void cLocal::on_caArchive_OperationFinished(cFileSystem *cfsFileSystem)
{
	caArchive->deleteLater();
	caArchive = NULL;
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
	caArchive = new cArchive(qsDrive, qsRootPath, qfiArchive, "", qmwParent, qhblOperations, csSettings, cppPackerPlugin);
	connect(caArchive, SIGNAL(ContentChanged(const cFileSystem *)), SLOT(on_caArchive_ContentChanged(const cFileSystem *)));
	if (caArchive->SetPath(".", true)) {
		connect(caArchive, SIGNAL(LeaveFileSystem()), SLOT(on_caArchive_LeaveFileSystem()));

		return true;
	} else {
		caArchive->deleteLater();
		caArchive = NULL;

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
const void cLocal::Read(const cFileOperationDialog::eOperation &eoOperation, const QString &qsFilter, const QString &qsDestination, const cFileOperation::eOperationPosition &eopPosition)
{
	if (caArchive) {
		connect(caArchive, SIGNAL(OperationFinished(cFileSystem *)), SLOT(on_caArchive_OperationFinished(cFileSystem *)));
		caArchive->Read(eoOperation, qsFilter, qsDestination, eopPosition);
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
	if (caArchive) {
		return caArchive->SetOperationFileList(vFileList);
	} // if

	qfilOperation = *static_cast<QFileInfoList *>(vFileList);
} // SetOperationFileList

// change path for this file system
const void cLocal::SetPath(const QString &qsDrive, const QString &qsRootPath, const QString &qsPath, const bool &bStartup /* false */)
{
	if (caArchive) {
		return caArchive->SetPath(qsDrive, qsRootPath, qsPath, bStartup);
	} // if

	this->qsDrive = qsDrive;
	this->qsRootPath = qsRootPath;
	
	SetPath(qsPath, bStartup);
} // SetPath

// change path for this file system without drive change
const bool cLocal::SetPath(const QString &qsPath, const bool &bStartup /* false */)
{
	bool bResult;
	QDir qdNotFile;
	QString qsNewPath;

	if (caArchive) {
		return caArchive->SetPath(qsPath, bStartup);
	} // if

	if (!bStartup) {
		qfswWatcher.removePath(qdDir.path());
	} // if

	// find right path
	qsNewPath = qsPath;
	while (!qdDir.exists(qsNewPath)) {
		qsNewPath = QFileInfo(qsNewPath).path();
	} // while

	qdNotFile.setPath(qsNewPath);
	if (!qdNotFile.exists()) {
		// can be path to archive
		if (OpenArchive(qsNewPath)) {
			caArchive->SetPath(qsPath.mid(qsNewPath.length() + 1));
		} // if

		qsNewPath = QFileInfo(qsNewPath).path();
	} // if

	bResult = qdDir.cd(qsNewPath);
	qfswWatcher.addPath(qdDir.path());

	if (bResult) {
		emit ContentChanged(this);
	} // if

	return bResult;
} // SetPath

// custom context menu on right click
const void cLocal::ShowContextMenu(const QPoint &qcPosition
#ifdef Q_WS_WIN
		, const HWND hwParent
#endif
	) const
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

	qfilSelected = GetSelectedFiles();

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
} // ShowContextMenu

// write local files to this file system
const void cLocal::Write(const cFileOperationDialog::eOperation &eoOperation, const QStringList &qslSources, const QString &qsFilter, const QString &qsDestination, const cFileOperation::eOperationPosition &eopPosition)
{
	clcmCopyMove = new cLocalCopyMove(qmwParent, qhblOperations, csSettings);
	connect(clcmCopyMove, SIGNAL(finished()), SLOT(on_cLocalCopyMove_OperationFinished()));
	clcmCopyMove->CopyMove(eoOperation, qslSources, qsFilter, qsDestination, eopPosition);
} // Write