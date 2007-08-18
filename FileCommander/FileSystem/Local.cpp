#include "FileSystem/Local.h"

#include <QtCore/QDateTime>
#ifdef Q_WS_WIN
#include <windows.h>
#endif
#include "FileSystem/Local/ShellMenu.h"
#include "FileControl/Process.h"

// destructor
cLocal::~cLocal()
{
	ccpdContentPluginDelayed->deleteLater();
} // ~cLocal

// activate current file
const void cLocal::ActivateCurrent(QTreeWidgetItem *qtwiFile)
{
	QFileInfo *qfiFile;

	qfiFile = &qhFiles[qtwiFile];

	if (qfiFile->isDir()) {
		// directory
#ifdef Q_WS_WIN
		if (PathExists(qdDir.path() + '/' + qfiFile->fileName())) {
#endif
			qdDir.cd(qfiFile->fileName());
			emit ContentChanged(this);
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

// check if current path available
const bool cLocal::CheckPath()
{
#ifdef Q_WS_WIN
	while (!PathExists(qdDir.path())) {
#else
	while (!qdDir.exists()) {
#endif
		// invalid path
		if (!qdDir.cdUp()) {
			// even root doesn't exists
			emit Unaccessible();
			return false;
		} // if
	} // while

	// path OK
	return true;
} // CheckPath

// constructor
cLocal::cLocal(const QString &qsDrive, const QString &qsRootPath, const QString &qsPath, cSettings *csSettings, cContentPlugin *ccpContentPlugin)
{
	this->csSettings = csSettings;
	this->ccpContentPlugin = ccpContentPlugin;

	ccpdContentPluginDelayed = new cContentPluginDelayed(ccpContentPlugin);
	connect(ccpdContentPluginDelayed, SIGNAL(GotColumnValue(const cContentPluginDelayed::sOutput &)), SLOT(on_ccpdContentPluginDelayed_GotColumnValue(const cContentPluginDelayed::sOutput &)));
	connect(this, SIGNAL(InterruptContentDelayed()), ccpdContentPluginDelayed, SLOT(on_InterruptContentDelayed()));

	SetPath(qsDrive, qsRootPath, qsPath);
} // cLocal

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
			QHashIterator<QTreeWidgetItem *, QFileInfo> qhiFile(qhFiles);
			while (qhiFile.hasNext()) {
				qhiFile.next();
				delete qhiFile.key();
			} // while
			qhFiles.clear();

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
	QString qsExtension, qsName;

	qsName = GetFileName(qtwiFile, false);
	qsExtension = GetFileExtension(qtwiFile);
	if (!qsExtension.isEmpty()) {
		qsName += '.' + qsExtension;
	} // if

	if (qhFiles.value(qtwiFile).isDir() && bBracketsAllowed && csSettings->GetShowBracketsAroundDirectoryName()) {
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

// go one directory up if possible
const void cLocal::GoToUpDir()
{
	if (qdDir.cdUp()) {
		emit ContentChanged(this);
	} // if
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

// change path for this file system
const void cLocal::SetPath(const QString &qsDrive, const QString &qsRootPath, const QString &qsPath)
{
	this->qsDrive = qsDrive;
	this->qsRootPath = qsRootPath;
	
	SetPath(qsPath);
} // SetPath

// change path for this file system without drive change
const void cLocal::SetPath(const QString &qsPath)
{
	qdDir.setPath(qsPath);

	emit ContentChanged(this);
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