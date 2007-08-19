#include "FileSystem/Local.h"

#include <QtCore/QDateTime>
#ifdef Q_WS_WIN
#include <windows.h>
#endif
#include "FileSystem/Local/ShellMenu.h"
#include "FileControl/Process.h"
#include "FileSystem/Local/LocalDelete.h"

// destructor
cLocal::~cLocal()
{
	ccpdContentPluginDelayed->deleteLater();

	ClearFileTable(qhFiles);
	qhCustom.clear();
} // ~cLocal

// activate current file
const void cLocal::ActivateCurrent(QTreeWidgetItem *qtwiFile)
{
	QFileInfo *qfiFile;

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
		cProcess cpProcess;

		cpProcess.StartDetached(qfiFile->fileName(), qfiFile->path());
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
cLocal::cLocal(const QString &qsDrive, const QString &qsRootPath, const QString &qsPath, QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings, cContentPlugin *ccpContentPlugin)
{
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
	this->csSettings = csSettings;
	this->ccpContentPlugin = ccpContentPlugin;

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
	cLocalDelete *cldDelete;

	cldDelete = new cLocalDelete(this, qmwParent, qhblOperations, csSettings);
	connect(cldDelete, SIGNAL(finished()), SLOT(on_OperationFinished()));
	// TODO Delete append to queue
	cldDelete->Delete(qfilOperation, qsFilter, eopPosition);
} // Delete

// check if specified directory exists
const bool cLocal::DirExists(const QString &qsDirectory) const
{
	QDir qdDir;

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
const QString cLocal::GetCustomFileNameWithExtension(QTreeWidgetItem *qtwiFile)
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
				QFileInfo *qfiFile;

				qfiFile = &qfilFiles[iI];
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

	qfilFiles = GetFiles(qhFiles.value(qhFiles.constBegin().key()->treeWidget()->currentItem()));

	qi64Size = 0;
	for (iI = 0; iI < qfilFiles.count(); iI++) {
		qi64Size += qfilFiles.at(iI).size();
	} // for

	return qi64Size;
} // GetDirectorySize

// get current directory name
const QString cLocal::GetDirName() const
{
	return qdDir.dirName();
} // GetDirName

// find out disk space information
const cFileSystem::sDiskSpace cLocal::GetDiskSpace() const
{
	sDiskSpace sdsInfo;

#ifdef Q_WS_WIN
	ULARGE_INTEGER uliFree, uliTotal;

	GetDiskFreeSpaceEx(reinterpret_cast<LPCWSTR>(qsRootPath.unicode()), &uliFree, &uliTotal, NULL);
	sdsInfo.qi64Free = uliFree.QuadPart;
	sdsInfo.qi64Total = uliTotal.QuadPart;
#else
	/*struct stat stst;
	struct statfs stfs;

	if ( ::stat(sDirPath.local8Bit(),&stst) == -1 ) return false;
	if ( ::statfs(sDirPath.local8Bit(),&stfs) == -1 ) return false;

	fFree = stfs.f_bavail * ( stst.st_blksize / fKB );
	fTotal = stfs.f_blocks * ( stst.st_blksize / fKB );*/
#endif

	return sdsInfo;
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
	return qhFiles.value(qtwiFile).suffix();
} // GetFileExtension

// get icon for specified file
const QIcon cLocal::GetFileIcon(QTreeWidgetItem *qtwiFile) const
{
	return qfipIconProvider.icon(qhFiles.value(qtwiFile));
} // GetFileIcon

// file list of specified file system's type
void *cLocal::GetFileList(const QList<QTreeWidgetItem *> &qlSelected) const
{
	int iI;
	QFileInfoList *qfilFiles;

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
	return qhFiles.value(qtwiFile).filePath();
} // GetFilePath

// return list of sources (within subdirectories too)
const QFileInfoList cLocal::GetFiles(const QFileInfo &qfiFile, const QString &qsFilter /* "*" */) const
{
	QFileInfoList qfilSources;

	if (qfiFile.isDir()) {
		QFileInfoList qfilDirectories;

		qfilSources.append(qfiFile);
		qfilDirectories.append(qfiFile);

		// process subdirectories
		while (!qfilDirectories.isEmpty()) {
			int iI;
			QDir qdDirContent;
			QFileInfo qfiDir;
			QFileInfoList qfilDirContent;

			// get directory content
			qfiDir = qfilDirectories.takeFirst();
			qdDirContent.setPath(qfiDir.filePath());
			qdDirContent.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
			qfilDirContent = qdDirContent.entryInfoList();

			// filter sources and add next directories
			for (iI = 0; iI < qfilDirContent.count(); iI++) {
				QFileInfo *qfilFile;

				qfilFile = &qfilDirContent[iI];

				if (qfilFile->isDir()) {
					qfilDirectories.append(*qfilFile);
					qfilSources.append(*qfilFile);
				} else {
					if (SuitsFilter(qfilFile->fileName(), qsFilter)) {
						qfilSources.append(*qfilFile);
					} // if
				} // if else
			} // for
		} // while
	} else {
		if (SuitsFilter(qfiFile.fileName(), qsFilter)) {
			qfilSources.append(qfiFile);
		} // if
	} // if else

	return qfilSources;
} // GetFiles

// get file size
const qint64 cLocal::GetFileSize(QTreeWidgetItem *qtwiFile) const
{
	return qhFiles.value(qtwiFile).size();
} // GetFileSize

// get file's last modified date/time stamp
const QDateTime cLocal::GetLastModified(QTreeWidgetItem *qtwiFile) const
{
	return qhFiles.value(qtwiFile).lastModified();
} // GetLastModified

// current path on file system
const QString cLocal::GetPath() const
{
	return qdDir.path();
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
	SetPath(qsRootPath);
} // GoToRootDir

// go one directory up if possible
const void cLocal::GoToUpDir()
{
	SetPath("..");
} // GoToUpDir

// check if file is directory
const bool cLocal::IsDir(QTreeWidgetItem *qtwiFile) const
{
	return qhFiles.value(qtwiFile).isDir();
} // IsDir

// check if file is really file
const bool cLocal::IsFile(QTreeWidgetItem *qtwiFile) const
{
	return qhFiles.value(qtwiFile).isFile();
} // IsFile

// got golumn value from plugin
const void cLocal::on_ccpdContentPluginDelayed_GotColumnValue(const cContentPluginDelayed::sOutput &soOutput) const
{
	emit GotColumnValue(soOutput);
} // on_ccpdContentPluginDelayed_GotColumnValue

// file operation finished
const void cLocal::on_OperationFinished()
{
	// TODO on_OperationFinished
	emit OperationFinished(this);
} // on_OperationFinished

// current directory content changed
const void cLocal::on_qfswWatcher_directoryChanged(const QString &path) const
{
	emit ContentChanged(this);
} // on_qfswWatcher_directoryChanged

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

// start retreiving of content delayed values
const void cLocal::RetreiveContentDelayedValues()
{
	ccpdContentPluginDelayed->Start(qqContentDelayedParameters);
	qqContentDelayedParameters.clear();
} // RetreiveContentDelayedValues

// set file list for file operation
const void cLocal::SetOperationFileList(void *vFileList)
{
	qfilOperation = *static_cast<QFileInfoList *>(vFileList);
} // SetOperationFileList

// change path for this file system
const void cLocal::SetPath(const QString &qsDrive, const QString &qsRootPath, const QString &qsPath, const bool &bStartup /* false */)
{
	this->qsDrive = qsDrive;
	this->qsRootPath = qsRootPath;
	
	SetPath(qsPath, bStartup);
} // SetPath

// change path for this file system without drive change
const bool cLocal::SetPath(const QString &qsPath, const bool &bStartup /* false */)
{
	bool bResult;

	if (!bStartup) {
		qfswWatcher.removePath(qdDir.path());
	} // if
	bResult = qdDir.cd(qsPath);
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
		QFileInfo *qfiFile;

		qfiFile = &qfilSelected[iI];

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