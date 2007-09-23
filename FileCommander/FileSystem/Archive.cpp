#include "FileSystem/Archive.h"

#include <QtCore/QDir>
#include "FileSystem/Archive/ArchiveCommon.h"
#include "FileSystem/Archive/ArchiveFilePropertiesDialog.h"
#include <QtGui/QMenu>

// destructor
cArchive::~cArchive()
{
	QHashIterator<QString, QHash<QTreeWidgetItem *, tHeaderData> *> qhDirectory(qhDirectories);
	while (qhDirectory.hasNext()) {
		qhDirectory.next();
		ClearFileTable(*qhDirectory.value());
		delete qhDirectory.value();
	} // while

	if (bCustom) {
		ClearFileTable(qhFiles);
	} // if
	qhCustom.clear();
} // ~cArchive

// activate current file
const void cArchive::ActivateCurrent(QTreeWidgetItem *qtwiFile)
{
	tHeaderData *thdFile;

	thdFile = &qhPath->operator [](qtwiFile);

	if (thdFile->FileAttr & cPackerPlugin::iDIRECTORY) {
		// directory activated
		SetPath(qhPath->value(qtwiFile).FileName);
	} else {
		// file activated
		cArchiveFilePropertiesDialog cafpdFileProperties(qmwParent, *thdFile);

		cafpdFileProperties.exec();
	} // if else
} // ActivateCurrent

// add directory into directory table if it's not there already
QHash<QTreeWidgetItem *, tHeaderData> *cArchive::AddDirectory(const tHeaderData &thdHeaderData)
{
	QHash<QTreeWidgetItem *, tHeaderData> *qhDirectory;
	QString qsDirectory;

	if (thdHeaderData.FileAttr & cPackerPlugin::iDIRECTORY) {
		qsDirectory = thdHeaderData.FileName;
	} else {
		qsDirectory = QDir::toNativeSeparators(QFileInfo(thdHeaderData.FileName).path());
	} // if else

	if (!qhDirectories.contains(qsDirectory)) {
		// add new directory
		QString qsDotDot;
		tHeaderData thdDotDot;

		// add ".." directory
		qsDotDot = qsDirectory + "/..";
		strcpy(thdDotDot.FileName, qsDotDot.toLatin1().constData());
		thdDotDot.FileTime = ToPackerDateTime(QDateTime::currentDateTime());
		thdDotDot.FileAttr = cPackerPlugin::iDIRECTORY;
		qhDirectory = new QHash<QTreeWidgetItem *, tHeaderData>;
		qhDirectory->insert(new QTreeWidgetItem(), thdDotDot);

		// add new directory to directory list
		qhDirectories.insert(qsDirectory, qhDirectory);

		if (qsDirectory != ".") {
			QHash<QTreeWidgetItem *, tHeaderData> *qhUpper;
			tHeaderData thdDirectory, thdUpper;

			// set upper directory
			strcpy(thdUpper.FileName, QFileInfo(qsDirectory).path().toLatin1().constData());
			thdUpper.FileTime = ToPackerDateTime(QDateTime::currentDateTime());
			thdUpper.FileAttr = cPackerPlugin::iDIRECTORY;
			qhUpper = AddDirectory(thdUpper);

			// add new directory
			strcpy(thdDirectory.FileName, qsDirectory.toLatin1().constData());
			thdDirectory.FileTime = ToPackerDateTime(QDateTime::currentDateTime());
			thdDirectory.FileAttr = cPackerPlugin::iDIRECTORY;
			qhUpper->insert(new QTreeWidgetItem, thdDirectory);
		} // if
	} else {
		// directory already exists
		qhDirectory = qhDirectories.value(qsDirectory);
	} // if else

	return qhDirectory;
} // AddDirectory

// add file to custom file list
QTreeWidgetItem *cArchive::AddToCustomList(QTreeWidgetItem *qtwiFile)
{
	QTreeWidgetItem *qtwiNew;

	qtwiNew = qtwiFile->clone();
	qhCustom.insert(qtwiNew, qhFiles.value(qtwiFile));

	return qtwiNew;
} // AddToCustomList

// begin of searching files
const void cArchive::BeginSearch()
{
	// clear before next search (especially for branch view)
	qhCustom.clear();
} // BeginSearch

// file system can copy files to local file system
const bool cArchive::CanCopy() const
{
	return true;
} // CanCopy

// file system can create directory in current location
const bool cArchive::CanCreateDir() const
{
	return false;
} // CanCreateDir

// file system can delete files
const bool cArchive::CanDelete() const
{
	// TODO CanDelete
	return false;
} // CanDelete

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

// clear file table before next fill of it
const void cArchive::ClearFileTable(QHash<QTreeWidgetItem *, tHeaderData> &qhTable) const
{
	QHashIterator<QTreeWidgetItem *, tHeaderData> qhiFile(qhTable);
	while (qhiFile.hasNext()) {
		qhiFile.next();
		delete qhiFile.key();
	} // while
	qhTable.clear();
} // ClearFileTable

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
	QString qsPath;

	if (qsDirectory.isEmpty()) {
		qsPath = '.';
	} else {
		qsPath = QDir::cleanPath(qsDirectory);
	} // if else

	return qhDirectories.contains(qsPath);
} // DirExists

// searching of files finished
const void cArchive::EndSearch(const bool &bClearCustomOnly /* false */)
{
	if (bClearCustomOnly) {
		ClearFileTable(qhCustom);
	} else {
		// clear file hash table first
		if (bCustom) {
			ClearFileTable(qhFiles);
		} else {
			qhFiles.clear();
			bCustom = true;
		} // if else

		qhFiles = qhCustom;
	} // if else
} // EndSearch

// free file operation list from memory
const void cArchive::FreeOperationList(void *vFileList) const
{
	delete static_cast<QList<tHeaderData> *>(vFileList);
} // FreeOperationList

// archive file path
const QFileInfo &cArchive::GetArchiveFilePath() const
{
	return qfiArchive;
} // GetArchiveFilePath

// get value from content plugin
const QString cArchive::GetContentPluginValue(const sContentPluginRequest &sContent)
{
	// TODO GetContentPluginValue
	return QString();
} // GetContentPluginValue

// custom file list
const QList<QTreeWidgetItem *> cArchive::GetCustomFileList() const
{
	return qhCustom.keys();
} // GetCustomFileList

// get file name with extension from custom list
const QString cArchive::GetCustomFileNameWithExtension(QTreeWidgetItem *qtwiFile) const
{
	return QFileInfo(qhCustom.value(qtwiFile).FileName).fileName();
} // GetCustomFileNameWithExtension

// get file name from custom list with full path
const QString cArchive::GetCustomFilePath(QTreeWidgetItem *qtwiFile)
{
	QString qsName;
	tHeaderData *thdFile;

	thdFile = &qhCustom[qtwiFile];

	qsName = thdFile->FileName;

	if (thdFile->FileAttr & cPackerPlugin::iDIRECTORY && csSettings->GetShowBracketsAroundDirectoryName()) {
		qsName = '[' + qsName + ']';
	} // if

	return qsName;
} // GetCustomFilePath

// get tree items for current directory
const bool cArchive::GetDirectoryContent(QList<QTreeWidgetItem *> *qlFiles, const bool &bRefresh /* true */)
{
	if (bRefresh) {
		// reload
		int iDontShow;
		QHashIterator<QTreeWidgetItem *, tHeaderData> qhiFile(*qhPath);

		if (bCustom) {
			ClearFileTable(qhFiles);
			bCustom = false;
		} else {
			qhFiles.clear();
		} // if else

		if (!csSettings->GetShowSystemFiles()) {
			iDontShow = cPackerPlugin::iSYSTEM;
		} else {
			iDontShow = 0;
		} // if else
		if (!csSettings->GetShowHiddenFiles()) {
			iDontShow |= cPackerPlugin::iHIDDEN;
		} // if

		while (qhiFile.hasNext()) {
			qhiFile.next();

			if (!(qhiFile.value().FileAttr & iDontShow)) {
				qhFiles.insert(qhiFile.key(), qhiFile.value());
			} // if
		} // while
	} // if

	*qlFiles = qhFiles.keys();
	return true;
} // GetDirectoryContent

// get currently selected directory size
const qint64 cArchive::GetDirectorySize() const
{
	int iI;
	qint64 qi64Size;
	QList<tHeaderData> qlFiles;
	QString qsDirectory;

	qsDirectory = qhFiles.value(qhFiles.constBegin().key()->treeWidget()->currentItem()).FileName;
	qlFiles = GetFiles(qhDirectories.value(qsDirectory));

	qi64Size = 0;
	for (iI = 0; iI < qlFiles.count(); iI++) {
		qi64Size += qlFiles.at(iI).UnpSize;
	} // for

	return qi64Size;
} // GetDirectorySize

// get current directory name
const QString cArchive::GetDirName() const
{
	QString qsPath;

	qsPath = qhDirectories.key(qhPath);
	if (qsPath == ".") {
		return qfiArchive.fileName();
	} else {
		return QFileInfo(qsPath).fileName();
	} // if else
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
	int iAttributes;
	QString qsAttributes;
	tHeaderData *thdFile;

	thdFile = &qhPath->operator [](qtwiFile);

	if (thdFile->FileAttr & cPackerPlugin::iREAD_ONLY) {
		iAttributes = FILE_ATTRIBUTE_READONLY;
	} else {
		iAttributes = 0;
	} // if else
	if (thdFile->FileAttr & cPackerPlugin::iHIDDEN) {
		iAttributes |= FILE_ATTRIBUTE_HIDDEN;
	} // if
	if (thdFile->FileAttr & cPackerPlugin::iSYSTEM) {
		iAttributes |= FILE_ATTRIBUTE_SYSTEM;
	} // if
	if (thdFile->FileAttr & cPackerPlugin::iARCHIVE) {
		iAttributes |= FILE_ATTRIBUTE_ARCHIVE;
	} // if

	if (iAttributes & FILE_ATTRIBUTE_READONLY) {
		qsAttributes = 'r';
	} // if
	if (iAttributes & FILE_ATTRIBUTE_ARCHIVE) {
		qsAttributes += 'a';
	} // if
	if (iAttributes & FILE_ATTRIBUTE_HIDDEN) {
		qsAttributes += 'h';
	} // if
	if (iAttributes & FILE_ATTRIBUTE_SYSTEM) {
		qsAttributes += 's';
	} // if

	return qsAttributes;
} // GetFileAttr
#endif

// get file extension
const QString cArchive::GetFileExtension(QTreeWidgetItem *qtwiFile) const
{
	return QFileInfo(qhPath->value(qtwiFile).FileName).suffix();
} // GetFileExtension

// get icon for specified file
const QIcon cArchive::GetFileIcon(QTreeWidgetItem *qtwiFile) const
{
	if (qhPath->value(qtwiFile).FileAttr & cPackerPlugin::iDIRECTORY) {
		return qfipIconProvider.icon(QFileIconProvider::Folder);
	} else {
		return qfipIconProvider.icon(QFileIconProvider::File);
	} // if else
} // GetFileIcon

// file list of specified file system's type
void *cArchive::GetFileList(const QList<QTreeWidgetItem *> &qlSelected) const
{
	int iI;
	QList<tHeaderData> *qlFiles;

	qlFiles = new QList<tHeaderData>();

	for (iI = 0; iI < qlSelected.count(); iI++) {
		qlFiles->append(qhPath->value(qlSelected.at(iI)));
	} // for

	return qlFiles;
} // GetFileList

// get file name without extension
const QString cArchive::GetFileName(QTreeWidgetItem *qtwiFile, const bool &bBracketsAllowed /* true */)
{
	QString qsName;
	tHeaderData *thdFile;

	thdFile = &qhPath->operator [](qtwiFile);

	if (QFileInfo(thdFile->FileName).fileName() == "..") {
		// special handle for ".." directory to show just both points
		qsName = "..";
	} else {
		qsName = QFileInfo(thdFile->FileName).completeBaseName();
	} // if else

	if (thdFile->FileAttr & cPackerPlugin::iDIRECTORY && bBracketsAllowed && csSettings->GetShowBracketsAroundDirectoryName()) {
		qsName = '[' + qsName + ']';
	} // if

	return qsName;
} // GetFileName

// get file name with extension
const QString cArchive::GetFileNameWithExtension(QTreeWidgetItem *qtwiFile, const bool &bBracketsAllowed /* true */)
{
	QString qsName;
	tHeaderData *thdFile;

	thdFile = &qhPath->operator [](qtwiFile);

	qsName = QFileInfo(thdFile->FileName).fileName();

	if (thdFile->FileAttr & cPackerPlugin::iDIRECTORY && bBracketsAllowed && csSettings->GetShowBracketsAroundDirectoryName()) {
		qsName = '[' + qsName + ']';
	} // if

	return qsName;
} // GetFileNameWithExtension

// get file name with full path
const QString cArchive::GetFilePath(QTreeWidgetItem *qtwiFile) const
{
	return qhPath->value(qtwiFile).FileName;
} // GetFilePath

// get archive files for specified path within subdirectories
const QList<tHeaderData> cArchive::GetFiles(const QHash<QTreeWidgetItem *, tHeaderData> *qhPath) const
{
	QList<tHeaderData> qlFiles;

	QHashIterator<QTreeWidgetItem *, tHeaderData> qhiFile(*qhPath);
	while (qhiFile.hasNext()) {
		qhiFile.next();
		if (qhiFile.value().FileAttr & cPackerPlugin::iDIRECTORY) {
			if (!QString(qhiFile.value().FileName).endsWith("..")) {
				qlFiles += GetFiles(qhDirectories.value(qhiFile.value().FileName));
			} // if
		} else {
			qlFiles.append(qhiFile.value());
		} // if else
	} // while

	return qlFiles;
} // GetFiles

// get file size
const qint64 cArchive::GetFileSize(QTreeWidgetItem *qtwiFile) const
{
	return qhPath->value(qtwiFile).UnpSize;
} // GetFileSize

// get file's last modified date/time stamp
const QDateTime cArchive::GetLastModified(QTreeWidgetItem *qtwiFile) const
{
	return cArchiveCommon::ToQDateTime(qhPath->value(qtwiFile).FileTime);
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
	QString qsPath;

	qsPath = qhDirectories.key(qhPath);
	if (qsPath == ".") {
		qsPath = qfiArchive.fileName();
	} else {
		qsPath = qfiArchive.fileName() + '/' + qsPath;
	} // if else

	return qsPath;
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

// selected items from current directory list
const QList<QTreeWidgetItem *> cArchive::GetSelectedFiles() const
{
	QList<QTreeWidgetItem *> qlSelected;

	QHashIterator<QTreeWidgetItem *, tHeaderData> qhiFile(qhFiles);
	while (qhiFile.hasNext()) {
		qhiFile.next();

		if (qhiFile.key()->isSelected()) {
			qlSelected.append(qhiFile.key());
		} // if
	} // while

	return qlSelected;
} // GetSelectedFiles

// get text for tab in directory view
const QString cArchive::GetTabText() const
{
	// TODO GetTabText
	return QString();
} // GetTabText

// find out name of the disk
const bool cArchive::GetVolumeName(QString *qsName) const
{
	// TODO GetVolumeName
	return false;
} // GetVolumeName

// set path to root directory
const void cArchive::GoToRootDir()
{
	// TODO GoToRootDir
} // GoToRootDir

// go one directory up if possible
const void cArchive::GoToUpDir()
{
	QString qsPath;

	qsPath = qhDirectories.key(qhPath);
	SetPath(qsPath + "/..");
} // GoToUpDir

// check if file is directory
const bool cArchive::IsDir(QTreeWidgetItem *qtwiFile) const
{
	return qhPath->value(qtwiFile).FileAttr & cPackerPlugin::iDIRECTORY;
} // IsDir

// check if file is really file
const bool cArchive::IsFile(QTreeWidgetItem *qtwiFile) const
{
	return !(qhPath->value(qtwiFile).FileAttr & cPackerPlugin::iDIRECTORY);
} // IsFile

// archive file copy operation finished
const void cArchive::on_cArchiveCopy_OperationFinished()
{
	cacCopy->deleteLater();
	emit OperationFinished(this);
} // on_cArchiveCopy_OperationFinished

// open archive
const bool cArchive::OpenArchive()
{
	int iI;
	QHash<QString, cPackerPlugin::sPluginInfo> *qhPluginsInfo;
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

				hArchive = qhPluginsInfo->value(QFileInfo(spPlugin->qsName).fileName()).toaOpenArchive(&toadArchiveData);

				delete toadArchiveData.ArcName;

				if (hArchive) {
					// archive opened successfully
					spiPluginInfo = &qhPluginsInfo->operator [](QFileInfo(spPlugin->qsName).fileName());
					// read archive files
					ReadArchiveFiles(hArchive);
					// path in archive
					qhPath = qhDirectories.value(".");
					// close archive
					qhPluginsInfo->value(QFileInfo(spPlugin->qsName).fileName()).tcaCloseArchive(hArchive);

					return true;
				} // if
			} // if
		} // if
	} // for

	return false;
} // OpenArchive

// write local files to this file system
const void cArchive::Read(const cFileOperationDialog::eOperation &eoOperation, const QString &qsFilter, const QString &qsDestination, const cFileOperation::eOperationPosition &eopPosition, const bool &bFullPath)
{
	cacCopy = new cArchiveCopy(qmwParent, qhblOperations, csSettings);
	connect(cacCopy, SIGNAL(finished()), SLOT(on_cArchiveCopy_OperationFinished()));
	cacCopy->Copy(qlOperation, qfiArchive, qhDirectories.key(qhPath), qsFilter, qhDirectories, qsDestination, cppPackerPlugin, spiPluginInfo, eopPosition, bFullPath);
} // Read

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
	qlOperation = *static_cast<QList<tHeaderData> *>(vFileList);
} // SetOperationFileList

// change path for this file system
const void cArchive::SetPath(const QString &qsDrive, const QString &qsRootPath, const QString &qsPath, const bool &bStartup /* false */)
{
	this->qsDrive = qsDrive;
	this->qsRootPath = qsRootPath;
	
	SetPath(qsPath, bStartup);
} // SetPath

// change path for this file system without drive change
const bool cArchive::SetPath(const QString qsPath, const bool &bStartup /* false */)
{
	bool bResult;
	QString qsNewPath;

	if (bStartup) {
		// just try to open archive
		return OpenArchive();
	} // if

	bResult = true;
	// find path in archive
	if (QFileInfo(qsPath).fileName() == "..") {
		if (qsPath == "./..") {
			// going from archive
			bResult = false;
			emit LeaveFileSystem();
		} else {
			// correct new path
			qsNewPath = QFileInfo(QFileInfo(qsPath).path()).path();
		} // if else
	} else {
		if (qsPath.isEmpty()) {
			qsNewPath = '.';
		} else {
			qsNewPath = qsPath;
		} // if else
	} // if else

	if (bResult) {
		if (qhDirectories.contains(qsNewPath)) {
			qhPath = qhDirectories.value(qsNewPath);
		} else {
			bResult = false;
		} // if else

		if (bResult) {
			emit ContentChanged(this);
		} // if
	} // if

	return bResult;
} // SetPath

// custom context menu on right click
const cFileSystem::eContextAction cArchive::ShowContextMenu(const QPoint &qcPosition
#ifdef Q_WS_WIN
			, const HWND hwParent
#endif
		)
{
	QAction *qaCopy, *qaProperties, *qaSelected;
	QMenu qmArchive;

	// archive context menu items
	qaCopy = qmArchive.addAction(tr("&Copy"));
	qmArchive.addSeparator();
	qaProperties = qmArchive.addAction(tr("&Properties"));

	qaSelected = qmArchive.exec((qhFiles.constBegin().key()->treeWidget())->viewport()->mapToGlobal(qcPosition));

	if (qaSelected == qaCopy) {
		return ContextCopyAction;
	} else {
		if (qaSelected == qaProperties) {
			ActivateCurrent(qhFiles.constBegin().key()->treeWidget()->currentItem());
		} // if

		return ContextNothing;
	} // if else
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

// try if path exists on file system
const bool cArchive::TryPath(const QString &qsPath) const
{
	// TODO TryPath
	return false;
} // TryPath

// file system type
const cFileSystem::eType cArchive::Type() const
{
	return Archive;
} // Type

// write local files to this file system
const void cArchive::Write(const cFileOperationDialog::eOperation &eoOperation, const QStringList &qslSources, const QString &qsFilter, const QString &qsDestination, const cFileOperation::eOperationPosition &eopPosition)
{
	// TODO Write
} // Write
