#include "FileControl.h"

#include <QtCore/QFileInfoList>
#include <QtCore/QDir>
#include "FileSystem/Local.h"
#include "FileControl/Process.h"

// constructor
cFileControl::cFileControl(QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings, cContentPlugin *ccpContentPlugin)
{
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
	this->csSettings = csSettings;
	this->ccpContentPlugin = ccpContentPlugin;
} // cFileControl

// change file system according to new drive
const bool cFileControl::ChangeFileSystem(const cFileSystem *cfsFileSystem, const QString &qsDrive, const QString &qsPath) const
{
	// TODO ChangeFileSystem do this after implementing other than local file system (delete old one, create new one)
	return false;
} // ChangeFileSystem

// get accessible drives
const QList<QPair<QString, cFileControl::sDrive> > cFileControl::GetDrives() const
{
	int iI;
	QFileInfoList qfilDrives;
	QList<QPair<QString, sDrive> > qlDrives;

	// local drives
	qfilDrives = QDir::drives();
	for (iI = 0; iI < qfilDrives.count(); iI++) {
		QFileInfo *qfiDrive;
		sDrive sdDrive;

		qfiDrive = &qfilDrives[iI];
		sdDrive.qsPath = qfiDrive->path();
		sdDrive.edtType = Local;
		qlDrives.append(QPair<QString, sDrive>(qfiDrive->path().at(0), sdDrive));
	} // for

	return qlDrives;
} // GetDrives

cFileSystem *cFileControl::GetFileSystem(const QString &qsDrive, const QString &qsPath) const
{
	cFileSystem *cfsFileSystem;
	int iI;
	QList<QPair<QString, sDrive> > qlDrives;

	qlDrives = GetDrives();

	for (iI = 0; iI < qlDrives.count(); iI++) {
		QPair<QString, sDrive> *qpDrive;

		qpDrive = &qlDrives[iI];
		if (qpDrive->first == qsDrive) {
			switch (qpDrive->second.edtType) {
				case Local:	cfsFileSystem = new cLocal(qsDrive, qpDrive->second.qsPath, qsPath, csSettings, ccpContentPlugin);
			} // switch
		} // if
	} // for

	return cfsFileSystem;
} // GetFileSystem

// returns first accessible drive for application
const QPair<QString, cFileControl::sDrive> cFileControl::GetFirstDrive() const
{
	QList<QPair<QString, sDrive> > qlDrives;

	qlDrives = GetDrives();

	return qlDrives.at(0);
} // GetFirstDrive

// information about path
const cFileControl::sPathInfo cFileControl::GetPathInfo(const QString &qsPath) const
{
	int iI;
	QList<QPair<QString, sDrive> > qlDrives;
	sPathInfo spiPathInfo;

	qlDrives = GetDrives();

	for (iI = 0; iI < qlDrives.count(); iI++) {
		QPair<QString, sDrive> *qpDrive;

		qpDrive = &qlDrives[iI];
		if (qsPath.startsWith(qpDrive->second.qsPath)) {
			spiPathInfo.qsDrive = qpDrive->first;
			spiPathInfo.qsRootPath = qpDrive->second.qsPath;
			break;
		} // if
	} // for

	return spiPathInfo;
} // GetPathInfo

// start shell command window
const void cFileControl::StartTerminal(const QString &qsPath) const
{
	#ifdef Q_WS_WIN
	cProcess cpProcess;

	if (QSysInfo::WindowsVersion & QSysInfo::WV_DOS_based) {
		cpProcess.StartDetached("command", qsPath);
	} else {
		cpProcess.StartDetached("cmd", qsPath);
	} // if else
#else
	// TODO Linux terminal
#endif
} // StartTerminal