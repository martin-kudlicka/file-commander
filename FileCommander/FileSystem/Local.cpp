#include "FileSystem/Local.h"

#include <QtCore/QDateTime>
#ifdef Q_WS_WIN
#include <windows.h>
#endif

// destructor
cLocal::~cLocal()
{
	ccpdContentPluginDelayed->deleteLater();
} // ~cLocal

// check if current path available
const bool cLocal::CheckPath()
{
	while (!qdDir.exists()) {
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

// get tree items for current directory
const QList<QTreeWidgetItem *> cLocal::GetDirectoryContent()
{
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
} // GetDirectoryContent

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
	QString qsName;
	QFileInfo *qfiFile;

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

// check if file is directory
const bool cLocal::IsDir(QTreeWidgetItem *qtwiFile) const
{
	return qhFiles.value(qtwiFile).isDir();
} // IsDir

// got golumn value from plugin
const void cLocal::on_ccpdContentPluginDelayed_GotColumnValue(const cContentPluginDelayed::sOutput &soOutput) const
{
	emit GotColumnValue(soOutput);
} // on_ccpdContentPluginDelayed_GotColumnValue

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
	qdDir.setPath(qsPath);

	emit ContentChanged(this);
} // SetPath