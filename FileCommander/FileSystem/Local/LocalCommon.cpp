#include "FileSystem/Local/LocalCommon.h"

#include <QtCore/QDir>
#include "FileSystem/FileOperation.h"
#ifdef Q_WS_WIN
#include <windows.h>
#endif

// return free disk space for specified path
const cFileSystem::sDiskSpace cLocalCommon::GetDiskSpace(const QString &qsPath)
{
	cFileSystem::sDiskSpace sdsInfo;

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
} // qsPath

// return list of sources (within subdirectories too)
const QFileInfoList cLocalCommon::GetFiles(const QFileInfo &qfiFile, const QString &qsFilter /* "*" */)
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
				const QFileInfo *qfilFile;

				qfilFile = &qfilDirContent.at(iI);

				if (qfilFile->isDir()) {
					qfilDirectories.append(*qfilFile);
					qfilSources.append(*qfilFile);
				} else {
					if (cFileOperation::SuitsFilter(qfilFile->fileName(), qsFilter)) {
						qfilSources.append(*qfilFile);
					} // if
				} // if else
			} // for
		} // while
	} else {
		if (cFileOperation::SuitsFilter(qfiFile.fileName(), qsFilter)) {
			qfilSources.append(qfiFile);
		} // if
	} // if else

	return qfilSources;
} // GetFiles

// return list of sources (within subdirectories too)
const QFileInfoList cLocalCommon::GetFiles(const QFileInfoList &qfilFiles, const QString &qsFilter /* "*" */)
{
	int iI;
	QFileInfoList qfilResult;

	for (iI = 0; iI < qfilFiles.count(); iI++) {
		qfilResult += GetFiles(qfilFiles.at(iI), qsFilter);
	} // for

	return qfilResult;
} // GetFiles

// return list of sources (within subdirectories too)
const QFileInfoList cLocalCommon::GetFiles(const QStringList &qslFiles, const QString &qsFilter /* "*" */)
{
	int iI;
	QFileInfoList qfilResult;

	for (iI = 0; iI < qslFiles.count(); iI++) {
		qfilResult += GetFiles(qslFiles.at(iI), qsFilter);
	} // for

	return qfilResult;
} // GetFiles