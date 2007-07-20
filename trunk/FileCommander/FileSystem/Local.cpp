#include "FileSystem/Local.h"

#include <QtCore/QDateTime>
#ifdef Q_WS_WIN
#include <windows.h>
#endif

// constructor
cLocal::cLocal(const QString &qsPath, cSettings *csSettings)
{
	this->csSettings = csSettings;

	qdDir.setPath(qsPath);
} // cLocal

// get tree items for current directory
const QList<QTreeWidgetItem *> cLocal::GetDirectoryContent()
{
	int iI;
	QDir::Filters fFilters;
	QFileInfoList qfilFiles;

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

	// add files to hash table
	qhFiles.clear();
	for (iI = 0; iI < qfilFiles.count(); iI++) {
		qhFiles.insert(new QTreeWidgetItem(), qfilFiles.at(iI));
	} // for

	return qhFiles.keys();
} // GetDirectoryContent

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
const QString cLocal::GetFileName(QTreeWidgetItem *qtwiFile)
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

	if (qfiFile->isDir() && csSettings->GetShowBracketsAroundDirectoryName()) {
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

// check if file is directory
const bool cLocal::IsDir(QTreeWidgetItem *qtwiFile) const
{
	return qhFiles.value(qtwiFile).isDir();
} // IsDir
