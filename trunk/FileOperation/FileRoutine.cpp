#include "FileOperation/FileRoutine.h"

#ifdef Q_WS_WIN
#include <Windows.h>
#endif

// lists directory for it's content
QFileInfoList cFileRoutine::GetDirectoryContent(const QString &qsPath, const QDir::Filters &fFilters)
{
	QDir qdDir;

	qdDir.setPath(qsPath);
	qdDir.setFilter(fFilters);
	return qdDir.entryInfoList();
} // GetDirectoryContent

// find out disk space information
cFileRoutine::sDiskSpace cFileRoutine::GetDiskSpace(const QString &qsPath)
{
	sDiskSpace sdsInfo;

#ifdef Q_WS_WIN
	ULARGE_INTEGER uliFree, uliTotal;

	GetDiskFreeSpaceEx(reinterpret_cast<LPCWSTR>(qsPath.unicode()), &uliFree, &uliTotal, NULL);
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

#ifdef Q_WS_WIN
// find out name of the disk
QString cFileRoutine::GetVolumeName(const QString &qsRootPath)
{
	QString qsName;

	qsName.fill(32, uiVOLUME_NAME * 2);
	GetVolumeInformation(reinterpret_cast<LPCWSTR>(qsRootPath.unicode()), reinterpret_cast<LPWSTR>(qsName.data()), qsName.size() / 2, NULL, NULL, NULL, NULL, NULL);
	return qsName.left(qsName.trimmed().size() - 1);
} // GetVolumeName
#endif