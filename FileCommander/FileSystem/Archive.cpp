#include "FileSystem/Archive.h"

#include <QtCore/QDir>

// destructor
cArchive::~cArchive()
{
	// TODO delete files in qpRoot
} // ~cArchive

// activate current file
const void cArchive::ActivateCurrent(QTreeWidgetItem *qtwiFile)
{
	// TODO ActivateCurrent
} // ActivateCurrent

// add directory into directory table if it's not there already
QHash<QTreeWidgetItem *, tHeaderData> *cArchive::AddDirectory(const tHeaderData &thdHeaderData)
{
	QHash<QTreeWidgetItem *, tHeaderData> *qhDirectory;
	QString qsDirectory;

	if (thdHeaderData.FileAttr & cPackerPlugin::iDIRECTORY) {
		qsDirectory = thdHeaderData.FileName;
	} else {
		qsDirectory = QFileInfo(thdHeaderData.FileName).path();
	} // if else

	if (!qhDirectories.contains(qsDirectory)) {
		// add new directory
		tHeaderData thdDotDot;

		qhDirectory = new QHash<QTreeWidgetItem *, tHeaderData>;
		// add ".." directory
		strcpy(thdDotDot.FileName, "..");
		thdDotDot.FileTime = ToPackerDateTime(QDateTime::currentDateTime());
		thdDotDot.FileAttr = cPackerPlugin::iDIRECTORY;
		qhDirectory->insert(new QTreeWidgetItem(), thdDotDot);

		// add new directory to directory list
		qhDirectories.insert(qsDirectory, qhDirectory);
	} else {
		qhDirectory = qhDirectories.value(qsDirectory);
	} // if else

	return qhDirectory;
} // AddDirectory

// add file to custom file list
QTreeWidgetItem *cArchive::AddToCustomList(QTreeWidgetItem *qtwiFile)
{
	// TODO AddToCustomList
	return &QTreeWidgetItem();
} // AddToCustomList

// begin of searching files
const void cArchive::BeginSearch()
{
	// TODO BeginSearch
} // BeginSearch

// constructor
cArchive::cArchive(const QString &qsDrive, const QString &qsRootPath, const QFileInfo &qfiArchive, const QString &qsPath, QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings, cPackerPlugin *cppPackerPlugin)
{
	this->qfiArchive = qfiArchive;
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
	this->csSettings = csSettings;
	this->cppPackerPlugin = cppPackerPlugin;

	SetPath(qsDrive, qsRootPath, qsPath);
} // cArchive

// create new directory
const void cArchive::CreateDir(const QString &qsName)
{
	// TODO CreateDir
} // CreateDir

// delete files in operation file list
const void cArchive::Delete(const QString &qsFilter, const cFileOperation::eOperationPosition &eopPosition)
{
	// TODO Delete
} // Delete

// check if specified directory exists
const bool cArchive::DirExists(const QString &qsDirectory) const
{
	// TODO DirExists
	return false;
} // DirExists

// searching of files finished
const void cArchive::EndSearch(const bool &bClearCustomOnly /* false */)
{
	// TODO EndSearch
} // EndSearch

// get value from content plugin
const QString cArchive::GetContentPluginValue(const sContentPluginRequest &sContent)
{
	// TODO GetContentPluginValue
	return QString();
} // GetContentPluginValue

// custom file list
const QList<QTreeWidgetItem *> cArchive::GetCustomFileList() const
{
	// TODO GetCustomFileList
	return QList<QTreeWidgetItem *>();
} // GetCustomFileList

// get file name with extension from custom list
const QString cArchive::GetCustomFileNameWithExtension(QTreeWidgetItem *qtwiFile) const
{
	// TODO GetCustomFileNameWithExtension
	return QString();
} // GetCustomFileNameWithExtension

// get file name from custom list with full path
const QString cArchive::GetCustomFilePath(QTreeWidgetItem *qtwiFile)
{
	// TODO GetCustomFilePath
	return QString();
} // GetCustomFilePath

// get tree items for current directory
QList<QTreeWidgetItem *> cArchive::GetDirectoryContent(const bool &bRefresh /* true */)
{
	// TODO GetDirectoryContent
	return QList<QTreeWidgetItem *>();
} // GetDirectoryContent

// get currently selected directory size
const qint64 cArchive::GetDirectorySize() const
{
	// TODO GetDirectorySize
	return qint64();
} // GetDirectorySize

// get current directory name
const QString cArchive::GetDirName() const
{
	// TODO GetDirName
	return QString();
} // GetDirName

// find out disk space information
const cFileSystem::sDiskSpace cArchive::GetDiskSpace() const
{
	// TODO GetDiskSpace
	return cFileSystem::sDiskSpace();
} // GetDiskSpace

// drive represented by file system
const QString &cArchive::GetDrive() const
{
	return qsDrive;
} // GetDrive

#ifdef Q_WS_WIN
// get file attributes
const QString cArchive::GetFileAttr(QTreeWidgetItem *qtwiFile) const
{
	// TODO GetFileAttr
	return QString();
} // GetFileAttr
#endif

// get file extension
const QString cArchive::GetFileExtension(QTreeWidgetItem *qtwiFile) const
{
	// TODO GetFileExtension
	return QString();
} // GetFileExtension

// get icon for specified file
const QIcon cArchive::GetFileIcon(QTreeWidgetItem *qtwiFile) const
{
	// TODO GetFileIcon
	return QIcon();
} // GetFileIcon

// file list of specified file system's type
void *cArchive::GetFileList(const QList<QTreeWidgetItem *> &qlSelected) const
{
	// TODO GetFileList
	return NULL;
} // GetFileList

// get file name without extension
const QString cArchive::GetFileName(QTreeWidgetItem *qtwiFile, const bool &bBracketsAllowed /* true */)
{
	// TODO GetFileName
	return QString();
} // GetFileName

// get file name with extension
const QString cArchive::GetFileNameWithExtension(QTreeWidgetItem *qtwiFile, const bool &bBracketsAllowed /* true */)
{
	// TODO GetFileNameWithExtension
	return QString();
} // GetFileNameWithExtension

// get file name with full path
const QString cArchive::GetFilePath(QTreeWidgetItem *qtwiFile) const
{
	// TODO GetFilePath
	return QString();
} // GetFilePath

// get file size
const qint64 cArchive::GetFileSize(QTreeWidgetItem *qtwiFile) const
{
	// TODO GetFileSize
	return qint64();
} // GetFileSize

// get file's last modified date/time stamp
const QDateTime cArchive::GetLastModified(QTreeWidgetItem *qtwiFile) const
{
	// TODO GetLastModified
	return QDateTime();
} // GetLastModified

// file paths from operation file list
const QStringList cArchive::GetOperationStringList() const
{
	// TODO GetOperationStringList
	return QStringList();
} // GetOperationStringList

// current path on file system
const QString cArchive::GetPath() const
{
	// TODO GetPath
	return QString();
} // GetPath

// retreive path of a file
const QString cArchive::GetPath(QTreeWidgetItem *qtwiFile) const
{
	// TODO GetPath
	return QString();
} // GetPath

// selected directory list for current directory
const QStringList cArchive::GetSelectedDirectoryStringList() const
{
	// TODO GetSelectedDirectoryStringList
	return QStringList();
} // GetSelectedDirectoryStringList

// get text for tab in directory view
const QString cArchive::GetTabText() const
{
	// TODO GetTabText
	return QString();
} // GetTabText

// find out name of the disk
const QString cArchive::GetVolumeName() const
{
	// TODO GetVolumeName
	return QString();
} // GetVolumeName

// set path to root directory
const void cArchive::GoToRootDir()
{
	// TODO GoToRootDir
} // GoToRootDir

// go one directory up if possible
const void cArchive::GoToUpDir()
{
	// TODO GoToUpDir
} // GoToUpDir

// check if file is directory
const bool cArchive::IsDir(QTreeWidgetItem *qtwiFile) const
{
	// TODO IsDir
	return false;
} // IsDir

// check if file is really file
const bool cArchive::IsFile(QTreeWidgetItem *qtwiFile) const
{
	// TODO IsFile
	return false;
} // IsFile

// open archive
const bool cArchive::OpenArchive()
{
	int iI;
	QHash<QString, cPackerPlugin::sPluginInfo> qhPluginsInfo;
	QList<cSettings::sPlugin> qlPackerPlugins;

	qhPluginsInfo = cppPackerPlugin->GetPluginsInfo();
	qlPackerPlugins = csSettings->GetPlugins(cSettings::PackerPlugins);

	for (iI = 0; iI < qlPackerPlugins.count(); iI++) {
		const cSettings::sPlugin *spPlugin;

		spPlugin = &qlPackerPlugins.at(iI);
		if (spPlugin->bEnabled) {
			QStringList qslExtensions;

			qslExtensions = spPlugin->qsExtensions.split(';');
			if (qslExtensions.contains(qfiArchive.suffix(), Qt::CaseInsensitive)) {
				HANDLE hArchive;
				tOpenArchiveData toadArchiveData;

				toadArchiveData.ArcName = new char[qfiArchive.filePath().length() + 1];
				strcpy(toadArchiveData.ArcName, QDir::toNativeSeparators(qfiArchive.filePath()).toLatin1().constData());
				toadArchiveData.OpenMode = PK_OM_LIST;

				hArchive = qhPluginsInfo.value(QFileInfo(spPlugin->qsName).fileName()).toaOpenArchive(&toadArchiveData);

				delete toadArchiveData.ArcName;

				if (hArchive) {
					// archive opened successfully
					spiPluginInfo = &qhPluginsInfo[QFileInfo(spPlugin->qsName).fileName()];
					// read archive files
					ReadArchiveFiles(hArchive);
					// path in archive
					qhPath = qhDirectories.value(".");
					// close archive
					qhPluginsInfo.value(QFileInfo(spPlugin->qsName).fileName()).tcaCloseArchive(hArchive);

					return true;
				} // if
			} // if
		} // if
	} // for

	return false;
} // OpenArchive

// read archive files
const void cArchive::ReadArchiveFiles(const HANDLE &hArchive)
{
	tHeaderData thdHeaderData;

	memset(&thdHeaderData, 0, sizeof(tHeaderData));
	// go through archive files
	while (!spiPluginInfo->trhReadHeader(hArchive, &thdHeaderData)) {
		QHash<QTreeWidgetItem *, tHeaderData> *qhDirectory;

		// add directory
		qhDirectory = AddDirectory(thdHeaderData);

		if (!(thdHeaderData.FileAttr & cPackerPlugin::iDIRECTORY)) {
			// add file
			qhDirectory->insert(new QTreeWidgetItem(), thdHeaderData);
		} // if

		spiPluginInfo->tpfProcessFile(hArchive, PK_SKIP, NULL, NULL);
		memset(&thdHeaderData, 0, sizeof(tHeaderData));
	} // while
} // ReadArchiveFiles

// start retreiving of content delayed values
const void cArchive::RetreiveContentDelayedValues()
{
	// TODO RetreiveContentDelayedValues
} // RetreiveContentDelayedValues

// set file list for file operation
const void cArchive::SetOperationFileList(void *vFileList)
{
	// TODO SetOperationFileList
} // SetOperationFileList

// change path for this file system
const void cArchive::SetPath(const QString &qsDrive, const QString &qsRootPath, const QString &qsPath, const bool &bStartup /* false */)
{
	this->qsDrive = qsDrive;
	this->qsRootPath = qsRootPath;
	
	SetPath(qsPath, bStartup);
} // SetPath

// change path for this file system without drive change
const bool cArchive::SetPath(const QString &qsPath, const bool &bStartup /* false */)
{
	bool bResult;

	bResult = true;
	if (bStartup) {
		// open archive
		bResult = OpenArchive();
	} // if

	// find path in archive
	if (bResult) {
		if (qhDirectories.contains(qsPath)) {
			qhPath = qhDirectories.value(qsPath);
		} else {
			bResult = false;
		} // if else
	} // if

	if (bResult) {
		emit ContentChanged(this);
	} // if else

	// TODO SetPath
	return bResult;
} // SetPath

// custom context menu on right click
const void cArchive::ShowContextMenu(const QPoint &qcPosition
#ifdef Q_WS_WIN
			, const HWND hwParent
#endif
		) const
{
	// TODO ShowContextMenu
} // ShowContextMenu

// converts Qt's date time format to packer's
const int cArchive::ToPackerDateTime(const QDateTime &qdtDateTime) const
{
	int iDateTime;

	iDateTime = (qdtDateTime.date().year() - 1980) << 25;
	iDateTime |= qdtDateTime.date().month() << 21;
	iDateTime |= qdtDateTime.date().day() << 16;
	iDateTime |= qdtDateTime.time().hour() << 11;
	iDateTime |= qdtDateTime.time().minute() << 5;
	iDateTime |= qdtDateTime.time().second() / 2;

	return iDateTime;
} // ToPackerDateTime

// write local files to this file system
const void cArchive::Write(const cFileOperationDialog::eOperation &eoOperation, const QStringList &qslSources, const QString &qsFilter, const QString &qsDestination, const cFileOperation::eOperationPosition &eopPosition)
{
	// TODO Write
} // Write