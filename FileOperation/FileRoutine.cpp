#include "FileOperation/FileRoutine.h"

// lists directory for it's content
QFileInfoList cFileRoutine::GetDirectoryContent(const QString qsPath, const QDir::Filters fFilters)
{
	QDir qdDir;

	qdDir.setPath(qsPath);
	qdDir.setFilter(fFilters);
	return qdDir.entryInfoList();
} // GetDirectoryContent

// return list of sources (within subdirectories too)
QFileInfoList cFileRoutine::GetSources(const QFileInfoList qfilFileAndDirList)
{
	int iI;
	QFileInfoList qfilDirectories, qfilSources;

	qfilSources = qfilFileAndDirList;

	for (iI = 0; iI < qfilFileAndDirList.count(); iI++) {
		if (qfilFileAndDirList.at(iI).isDir()) {
			qfilDirectories.append(qfilFileAndDirList.at(iI));
		} // if
	} // for

	while (!qfilDirectories.isEmpty()) {
		QFileInfo qfiDir;
		QFileInfoList qfilDirContent;

		qfiDir = qfilDirectories.takeAt(0);
		qfilDirContent = GetDirectoryContent(qfiDir.filePath(), QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
		qfilSources += qfilDirContent;
		for (iI = 0; iI < qfilDirContent.count(); iI++) {
			if (qfilDirContent.at(iI).isDir()) {
				qfilDirectories.append(qfilDirContent.at(iI));
			} // if
		} // for
	} // while

	return qfilSources;
} // GetSources