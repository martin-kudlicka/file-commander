#include "FileControl.h"

#include <QtCore/QFileInfoList>
#include <QtCore/QDir>
#include "FileSystem/Local.h"

// constructor
cFileControl::cFileControl(QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings, cContentPlugin *ccpContentPlugin)
{
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
	this->csSettings = csSettings;
	this->ccpContentPlugin = ccpContentPlugin;
} // cFileControl

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

cFileSystem *cFileControl::GetFileSystem(const QString &qsDrive) const
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
				case Local:	cfsFileSystem = new cLocal(qpDrive->second.qsPath, csSettings, ccpContentPlugin);
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
