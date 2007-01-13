#include "FileOperation/FileRoutine.h"

// lists directory for it's content
QFileInfoList cFileRoutine::GetDirectoryContent(const QString &qsPath, const QDir::Filters &fFilters)
{
	QDir qdDir;

	qdDir.setPath(qsPath);
	qdDir.setFilter(fFilters);
	return qdDir.entryInfoList();
} // GetDirectoryContent

// detect drives in system
QMap<QString, cFileRoutine::sDriveInfo> cFileRoutine::GetDrives()
{
	int iI;
	QFileInfoList qfilDrives;
	QMap<QString, cFileRoutine::sDriveInfo> qmDrives;

	qfilDrives = QDir::drives();

	for (iI = 0; iI < qfilDrives.count(); iI++) {
		sDriveInfo sdiDrive;

		sdiDrive.qsPath = qfilDrives.at(iI).path();
		qmDrives.insert(qfilDrives.at(iI).path().at(0), sdiDrive);
		// TODO GetDrives - get drive type
	} // for

	return qmDrives;
} // GetDrives

// return list of sources (within subdirectories too)
QFileInfoList cFileRoutine::GetSources(const QFileInfoList &qfilFileAndDirList)
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
		qfilDirContent = GetDirectoryContent(qfiDir.filePath(), QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden);
		qfilSources += qfilDirContent;
		for (iI = 0; iI < qfilDirContent.count(); iI++) {
			if (qfilDirContent.at(iI).isDir()) {
				qfilDirectories.append(qfilDirContent.at(iI));
			} // if
		} // for
	} // while

	return qfilSources;
} // GetSources