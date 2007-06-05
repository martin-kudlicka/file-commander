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
QFileInfoList cFileRoutine::GetSources(const QFileInfo &qfiFile, const QString &qsFilter /* "*" */)
{
	QFileInfoList qfilSources;

	if (qfiFile.isDir()) {
		QFileInfoList qfilDirectories;

		qfilSources.append(qfiFile);
		qfilDirectories.append(qfiFile);

		// process subdirectories
		while (!qfilDirectories.isEmpty()) {
			int iI;
			QFileInfo qfiDir;
			QFileInfoList qfilDirContent;

			qfiDir = qfilDirectories.takeAt(0);
			qfilDirContent = GetDirectoryContent(qfiDir.filePath(), QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);

			// filter sources and add next directories
			for (iI = 0; iI < qfilDirContent.count(); iI++) {
				if (qfilDirContent.at(iI).isDir()) {
					qfilDirectories.append(qfilDirContent.at(iI));
					qfilSources.append(qfilDirContent.at(iI));
				} else {
					if (SuitsFilter(qfilDirContent.at(iI).fileName(), qsFilter)) {
						qfilSources.append(qfilDirContent.at(iI));
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
} // GetSources

// return list of sources (within subdirectories too)
QFileInfoList cFileRoutine::GetSources(const QFileInfoList &qfilFileAndDirList, const QString &qsFilter /* "*" */)
{
	int iI;
	QFileInfoList qfilSources;

	for (iI = 0; iI < qfilFileAndDirList.count(); iI++) {
		qfilSources += GetSources(qfilFileAndDirList.at(iI), qsFilter);
	} // for

	return qfilSources;
} // GetSources

#ifdef Q_WS_WIN
// find out name of the disk
QString cFileRoutine::GetVolumeName(const QString &qsRootPath)
{
	QString qsName;

	qsName.fill(32, uiVOLUME_NAME * sizeof(WCHAR));
	GetVolumeInformation(reinterpret_cast<LPCWSTR>(qsRootPath.unicode()), reinterpret_cast<LPWSTR>(qsName.data()), qsName.size() / sizeof(WCHAR), NULL, NULL, NULL, NULL, 0);
	return qsName.left(qsName.trimmed().size() - 1);
} // GetVolumeName
#endif

// return file name modified by wildcard
QString cFileRoutine::GetWildcardedName(const QFileInfo &qfiFile, const QString &qsSourcePath, const QString &qsDestination)
{
	int iI;
	QString qsNewFilename;
	QStringList qslOutputs, qslPatterns, qslStrings;

	if (!qsDestination.contains('*') && !qsDestination.contains('?')) {
		// no wildcard in file name
		return qsDestination;
	} // if

	// preparation
	qslPatterns.append(QFileInfo(qsDestination).completeBaseName());
	qslPatterns.append(QFileInfo(qsDestination).suffix());
	qslStrings.append(qfiFile.completeBaseName());
	qslStrings.append(qfiFile.suffix());
	qslOutputs.append("");
	qslOutputs.append("");

	// apply patterns on strings
	for (iI = 0; iI <= 1; iI++) {
		int iPatternPos, iStringPos;

		iStringPos = 0;
		for (iPatternPos = 0; iPatternPos < qslPatterns[iI].length(); iPatternPos++) {
			if (qslPatterns[iI].at(iPatternPos) == '*') {
				// copy rest of the source name
				int iJ;

				for (iJ = iStringPos; iJ < qslStrings[iI].length(); iJ++) {
					qslOutputs[iI] += qslStrings[iI].at(iStringPos);
					iStringPos++;
				} // for
			} else
				if (qslPatterns[iI].at(iPatternPos) == '?') {
					// copy one character of the source name
					if (iStringPos < qslStrings[iI].length()) {
						qslOutputs[iI] += qslStrings[iI].at(iStringPos);
						iStringPos++;
					} // if
				} else {
					// copy character from pattern
					qslOutputs[iI] += qslPatterns[iI].at(iPatternPos);
					iStringPos++;
				} // if else
		} // while
	} // for

	qsNewFilename = qfiFile.path() + '/' + qslOutputs[0] + '.' + qslOutputs[1];
	return QFileInfo(qsDestination).path() + '/' + qsNewFilename.mid(qsSourcePath.length());
} // GetWildcardedName

// check if filename suits filter
bool cFileRoutine::SuitsFilter(const QString &qsName, const QString &qsFilter, const bool &bRegularExpression /* false */)
{
	int iI;
	QStringList qslFilter;

	qslFilter = qsFilter.split(';');
	if (qslFilter.count() == 1 && qslFilter.at(0).isEmpty() && !bRegularExpression) {
		qslFilter.append("*.*");
	} // if
#ifdef Q_WS_WIN
	// correct *.* to *
	for (iI = 0; iI < qslFilter.count(); iI++) {
		if (qslFilter.at(iI) == "*.*") {
			qslFilter[iI] = "*";
		} // if
	} // for
#endif

	// search for
	for (iI = 0; iI < qslFilter.count(); iI++) {
		QRegExp qreExpression(qslFilter.at(iI), Qt::CaseInsensitive);

		if (bRegularExpression) {
			// regular expression
			if (qreExpression.indexIn(qsName) != -1) {
				return true;
			} // if
		} else {
			// wildcard
			qreExpression.setPatternSyntax(QRegExp::Wildcard);
			if (qreExpression.exactMatch(qsName)) {
				return true;
			} // if
		} // if else
	} // for

	return false;
} // SuitsFilter
