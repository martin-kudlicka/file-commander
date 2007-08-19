#include "FileControl.h"

#include <QtCore/QFileInfoList>
#include <QtCore/QDir>
#include "FileSystem/Local.h"
#include "FileControl/Process.h"
#include <QtCore/QDateTime>
#include "ListerMainWindow.h"
#include <QtGui/QInputDialog>

// constructor
cFileControl::cFileControl(QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings, cContentPlugin *ccpContentPlugin, cListerPlugin *clpListerPlugin)
{
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
	this->csSettings = csSettings;
	this->ccpContentPlugin = ccpContentPlugin;
	this->clpListerPlugin = clpListerPlugin;
} // cFileControl

// change file system according to new drive
const bool cFileControl::ChangeFileSystem(const cFileSystem *cfsFileSystem, const QString &qsDrive, const QString &qsPath) const
{
	// TODO ChangeFileSystem do this after implementing other than local file system (delete old one, create new one, wath FS list) (copy FTP, at least logon info if same drive as existing FTP)
	return false;
} // ChangeFileSystem

// close used file system
const void cFileControl::CloseFileSystem(cFileSystem *cfsFileSystem) const
{
	// TODO CloseFileSystem - later - delete local (and from FS list later) (watch for copied FTPs?)
	cfsFileSystem->deleteLater();
} // CloseFileSystem

// compare directories in both panels
const void cFileControl::CompareDirectories(cFileSystem *cfsLeft, cFileSystem *cfsRight) const
{
	int iI;
	QList<QTreeWidgetItem *> qlLeft, qlRight;

	qlLeft = cfsLeft->GetDirectoryContent(false);
	qlRight = cfsRight->GetDirectoryContent(false);

	// mark first all files in the right panel, unmark directories
	for (iI = 0; iI < qlRight.count(); iI++) {
		QTreeWidgetItem *qtwiRight;

		qtwiRight = qlRight[iI];
		if (cfsRight->IsFile(qtwiRight)) {
			qtwiRight->setSelected(true);
		} else {
			qtwiRight->setSelected(false);
		} // if else
	} // for

	// go through left panel files and compare with right
	for (iI = 0; iI < qlLeft.count(); iI++) {
		QTreeWidgetItem *qtwiLeft;

		qtwiLeft = qlLeft[iI];
		if (cfsLeft->IsFile(qtwiLeft)) {
			bool bFound;
			int iJ;

			bFound = false;

			for (iJ = 0; iJ < qlRight.count(); iJ++) {
				QTreeWidgetItem *qtwiRight;

				qtwiRight = qlRight[iJ];
				if (cfsRight->IsFile(qtwiRight)) {
					if (cfsLeft->GetFileName(qtwiLeft) == cfsRight->GetFileName(qtwiRight)) {
						bFound = true;

						if (cfsLeft->GetLastModified(qtwiLeft) == cfsRight->GetLastModified(qtwiRight)) {
							// files are the same - clear mark of both files
							qtwiLeft->setSelected(false);
							qtwiRight->setSelected(false);
						} else {
							if (cfsLeft->GetLastModified(qtwiLeft) > cfsRight->GetLastModified(qtwiRight)) {
								// newer file on the left panel
								qtwiLeft->setSelected(true);
								qtwiRight->setSelected(false);
							} else {
								// newer file on the right panel
								qtwiLeft->setSelected(false);
							} // if else
						} // if else

						break;
					} // if
				} // if
			} // for

			if (!bFound) {
				// file on the left is not on the right panel
				qtwiLeft->setSelected(true);
			} // if
		} else {
			// unmark directories on the left
			qtwiLeft->setSelected(false);
		} // if else
	} // for
} // CompareDirectories

// create new directory
const QString cFileControl::CreateDirectory(cFileSystem *cfsFileSystem) const
{
	// TODO CreateDirectory CanCreateDirectory test
	QString qsName;

	qsName = QInputDialog::getText(qmwParent, tr("New directory"), tr("Enter name of a new directory:"));
	if (!qsName.isEmpty()) {
		QDir qdDir;
		QString qsNewDirectory;

		cfsFileSystem->CreateDir(qsName);
	} // if

	return qsName;
} // CreateDirectory

// edit selected files
const void cFileControl::Edit(const cFileSystem *cfsFileSystem, const QList<QTreeWidgetItem *> qlSelectedFiles) const
{
	int iI;

	for (iI = 0; iI < qlSelectedFiles.count(); iI++) {
		QTreeWidgetItem *qtwiFile;

		qtwiFile = qlSelectedFiles.at(iI);
		if (cfsFileSystem->IsFile(qtwiFile)) {
			// TODO Edit test for local file
			cProcess cpProcess;
			QString qsCommand;

			qsCommand = csSettings->GetExternalEditor();
			qsCommand = qsCommand.replace("%1", cfsFileSystem->GetFilePath(qtwiFile));

			cpProcess.StartDetached(qsCommand);
		} // if
	} // for
} // Edit

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

// count files type
const cFileControl::sTypeCount cFileControl::GetFilesTypeCount(const cFileSystem *cfsFileSystem, const QList<QTreeWidgetItem *> qlFiles) const
{
	int iI;
	sTypeCount stcTypeCount;

	stcTypeCount.DirectoryType = 0;
	stcTypeCount.FileType = 0;

	for (iI = 0; iI < qlFiles.count(); iI++) {
		if (cfsFileSystem->IsDir(qlFiles.at(iI))) {
			stcTypeCount.DirectoryType++;
		} else {
			stcTypeCount.FileType++;
		} // if else
	} // for

	return stcTypeCount;
} // GetFilesTypeCount

cFileSystem *cFileControl::GetFileSystem(const QString &qsDrive, const QString &qsPath) const
{
	// TODO GetFileSystem add FS to some FS list later
	cFileSystem *cfsFileSystem;
	int iI;
	QList<QPair<QString, sDrive> > qlDrives;

	cfsFileSystem = NULL;
	qlDrives = GetDrives();

	for (iI = 0; iI < qlDrives.count(); iI++) {
		QPair<QString, sDrive> *qpDrive;

		qpDrive = &qlDrives[iI];
		if (qpDrive->first == qsDrive) {
			switch (qpDrive->second.edtType) {
				case Local:	cfsFileSystem = new cLocal(qsDrive, qpDrive->second.qsPath, qsPath, qmwParent, qhblOperations, csSettings, ccpContentPlugin);
			} // switch
		} // if

		if (cfsFileSystem) {
			break;
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
	// TODO GetPathInfo with FTP later
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
			spiPathInfo.edtType = qpDrive->second.edtType;
			break;
		} // if
	} // for

	return spiPathInfo;
} // GetPathInfo

// file operation finished
const void cFileControl::on_cFileSystem_OperationFinished(cFileSystem *cfsFileSystem) const
{
	// TODO on_cFileSystem_OperationFinished
	CloseFileSystem(cfsFileSystem);
} // on_cFileSystem_OperationFinished

// file operation selected
const void cFileControl::Operation(const cFileOperationDialog::eOperation &eoOperation, cFileSystem *cfsSource, QList<QTreeWidgetItem *> qlSource, const cFileSystem *cfsDestination) const
{
	cFileOperationDialog cfodDialog(qmwParent, csSettings);
	cFileOperationDialog::eUserAction euaAction;
	int iI;
	QString qsDestination, qsFilter;
	sOperation soOperation;
	sTypeCount stcTypeCount;
	void *vFileList;

	// ignore ".." directory
	for (iI = 0; iI < qlSource.count(); iI++) {
		if (cfsSource->GetFileName(qlSource.at(iI), false) == "..") {
			qlSource.removeAt(iI);
			break;
		} // if
	} // for

	if (qlSource.isEmpty()) {
		return;
	} // if

	// prepare destination path for dialog
	if (eoOperation != cFileOperationDialog::DeleteOperation) {
		qsDestination = cfsDestination->GetPath();

		if (qlSource.count() == 1) {
			// one file selected
			QTreeWidgetItem *qtwiFile;

			qtwiFile = qlSource.at(0);
			if (cfsSource->IsFile(qtwiFile)) {
				qsDestination += '/' + cfsSource->GetFileNameWithExtension(qtwiFile);
			} // if
		} else {
			// many files selected
			qsDestination += "/*.*";
		} // if else
	} // if

	stcTypeCount = GetFilesTypeCount(cfsSource, qlSource);

	euaAction = cFileOperationDialog::CancelAction;
	switch (eoOperation) {
		case cFileOperationDialog::DeleteOperation:
			euaAction = cfodDialog.ShowDialog(eoOperation, tr("&Delete %1 files and %2 directories.").arg(stcTypeCount.FileType).arg(stcTypeCount.DirectoryType), &qsDestination, &qsFilter);
			break;
		case cFileOperationDialog::CopyOperation:
			euaAction = cfodDialog.ShowDialog(eoOperation, tr("Co&py %1 files and %2 directories to:").arg(stcTypeCount.FileType).arg(stcTypeCount.DirectoryType), &qsDestination, &qsFilter);
			break;
		case cFileOperationDialog::MoveOperation:
			euaAction = cfodDialog.ShowDialog(eoOperation, tr("&Move %1 files and %2 directories to:").arg(stcTypeCount.FileType).arg(stcTypeCount.DirectoryType), &qsDestination, &qsFilter);
	} // switch

	if (euaAction == cFileOperationDialog::CancelAction) {
		return;
	} // if

	// create file systems for file operation
	soOperation.cfsSource = GetFileSystem(cfsSource->GetDrive(), cfsSource->GetPath());
	if (eoOperation != cFileOperationDialog::DeleteOperation) {
		if (qsDestination.length() > 2 && qsDestination.at(1) == ':') {
			// absolute path -> can be different file system than source
			sPathInfo spiPathInfo;

			spiPathInfo = GetPathInfo(qsDestination);
			soOperation.cfsDestination = GetFileSystem(spiPathInfo.qsDrive, spiPathInfo.qsRootPath);
		} else {
			// relative path -> same file system as source
			soOperation.cfsDestination = GetFileSystem(cfsSource->GetDrive(), cfsSource->GetPath());
		} // if else
	} else {
		soOperation.cfsDestination = NULL;
	} // if else
	// operation type
	soOperation.eoType = eoOperation;

	// copy selected file list to the new source file system
	vFileList = cfsSource->GetFileList(qlSource);
	soOperation.cfsSource->SetOperationFileList(vFileList);
	delete vFileList;

	// correct destination path
	if (eoOperation != cFileOperationDialog::DeleteOperation) {
		if (cfsSource->DirExists(qsDestination)) {
			QDir::cleanPath(qsDestination) += "/*.*";
		} // if
	} // if

	// process
	switch (euaAction) {
		case cFileOperationDialog::OkAction:
			if (eoOperation == cFileOperationDialog::DeleteOperation) {
				// delete
				connect(soOperation.cfsSource, SIGNAL(OperationFinished(cFileSystem *)), SLOT(on_cFileSystem_OperationFinished(cFileSystem *)));
				soOperation.cfsSource->Delete(qsFilter, cFileOperation::ForegroundOperation);
			} else {
				// copy or move
				// !connect source and destination file systems!
				//ccmCopyMove->CopyMove(eoOperation, qfilSource, qsDestination, qsFilter, cFileRoutine::ForegroundWindow);
			} // if else
			break;
		case cFileOperationDialog::EnqueueAction:
			;//Enque(eoOperation, qfilSource, qsDestination, qsFilter);
	} // if
} // Operation

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

// view selected files
const void cFileControl::View(const cFileSystem *cfsFileSystem, const QList<QTreeWidgetItem *> qlSelectedFiles) const
{
	int iI;

	for (iI = 0; iI < qlSelectedFiles.count(); iI++) {
		QTreeWidgetItem *qtwiFile;

		qtwiFile = qlSelectedFiles.at(iI);
		if (cfsFileSystem->IsFile(qtwiFile)) {
			// TODO View test for local file
			if (csSettings->GetViewerType() == qsINTERNAL) {
				cListerMainWindow *clmwLister;

				clmwLister = new cListerMainWindow(csSettings, clpListerPlugin, cfsFileSystem->GetFilePath(qtwiFile));
				clmwLister->show();
			} else {
				cProcess cpProcess;
				QString qsCommand;

				qsCommand = csSettings->GetExternalViewer();
				qsCommand = qsCommand.replace("%1", cfsFileSystem->GetFilePath(qtwiFile));

				cpProcess.StartDetached(qsCommand);
			} // if else
		} // if
	} // for
} // View

// view selected file
const void cFileControl::View(const cFileSystem *cfsFileSystem, QTreeWidgetItem *qtwiFile) const
{
	QList<QTreeWidgetItem *> qlFile;

	qlFile.append(qtwiFile);
	View(cfsFileSystem, qlFile);
} // View