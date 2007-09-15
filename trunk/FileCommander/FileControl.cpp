#include "FileControl.h"

#include <QtCore/QFileInfoList>
#include <QtCore/QDir>
#include "FileSystem/Local.h"
#include "FileControl/Process.h"
#include <QtCore/QDateTime>
#include <QtGui/QInputDialog>
#include "FileControl/SelectDriveDialog.h"

// constructor
cFileControl::cFileControl(QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings, cContentPlugin *ccpContentPlugin, cListerPlugin *clpListerPlugin, cPackerPlugin *cppPackerPlugin)
{
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
	this->csSettings = csSettings;
	this->ccpContentPlugin = ccpContentPlugin;
	this->clpListerPlugin = clpListerPlugin;
	this->cppPackerPlugin = cppPackerPlugin;
	cfsInQueue = NULL;

	// queue widget
	cqwQueue.hide();
	qhblOperations->insertWidget(iQUEUE_WIDGET_POS, &cqwQueue);

	// connections
	connect(this, SIGNAL(AddIntoQueueList(QListWidgetItem *)), &cqwQueue, SLOT(on_cFileOperation_AddIntoQueueList(QListWidgetItem *)));
	connect(&cqwQueue, SIGNAL(RemoveQueuedItems(QList<QListWidgetItem *>)), SLOT(on_cqwQueue_RemoveQueuedItems(QList<QListWidgetItem *>)));
} // cFileControl

// change file system according to new drive with last path there
const bool cFileControl::ChangeFileSystem(cFileSystem *cfsFileSystem, QComboBox *qcbDrive, const QString &qsDrive)
{
	QString qsPath;

	if (qhLastPaths.contains(qsDrive)) {
		qsPath = qhLastPaths.value(qsDrive);
	} else {
		int iI;
		QList<QPair<QString, sDrive> > qlDrives;

		qlDrives = GetDrives();

		for (iI = 0; iI < qlDrives.count(); iI++) {
			const QPair<QString, sDrive> *qpDrive;

			qpDrive = &qlDrives.at(iI);
			if (qpDrive->first == qsDrive) {
				qsPath = qpDrive->second.qsPath;
			} // if
		} // for
	} // if else

	return ChangeFileSystem(cfsFileSystem, qcbDrive, qsDrive, qsPath);
} // ChangeFileSystem

// change file system according to new drive
const bool cFileControl::ChangeFileSystem(cFileSystem *cfsFileSystem, QComboBox *qcbDrive, const QString &qsDrive, const QString &qsPath)
{
	// TODO ChangeFileSystem do this after implementing other than local file system (delete old one, create new one, watch FS list) (copy FTP, at least logon info if same drive as existing FTP)
	sPathInfo spiPathInfo;

	spiPathInfo = GetPathInfo(qsPath);

	if (cfsFileSystem->TryPath(qsPath)) {
		// remember last path
		qhLastPaths.insert(cfsFileSystem->GetDrive(), cfsFileSystem->GetPath());
		// change drive combo box
		qcbDrive->blockSignals(true);
		qcbDrive->setCurrentIndex(qcbDrive->findText(qsDrive));
		qcbDrive->blockSignals(false);
		// set path
		cfsFileSystem->SetPath(qsDrive, spiPathInfo.qsRootPath, qsPath);
	} else {
		QString qsNewDrive;

		qsNewDrive = SelectDrive(qsDrive);
		if (qsNewDrive.isEmpty()) {
			qsNewDrive = qsDrive;
		} // if
		return ChangeFileSystem(cfsFileSystem, qcbDrive, qsNewDrive);
	} // if else

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

	cfsLeft->GetDirectoryContent(&qlLeft, false);
	cfsRight->GetDirectoryContent(&qlRight, false);

	// mark first all files in the right panel, unmark directories
	for (iI = 0; iI < qlRight.count(); iI++) {
		QTreeWidgetItem *qtwiRight;

		qtwiRight = qlRight.at(iI);
		if (cfsRight->IsFile(qtwiRight)) {
			qtwiRight->setSelected(true);
		} else {
			qtwiRight->setSelected(false);
		} // if else
	} // for

	// go through left panel files and compare with right
	for (iI = 0; iI < qlLeft.count(); iI++) {
		QTreeWidgetItem *qtwiLeft;

		qtwiLeft = qlLeft.at(iI);
		if (cfsLeft->IsFile(qtwiLeft)) {
			bool bFound;
			int iJ;

			bFound = false;

			for (iJ = 0; iJ < qlRight.count(); iJ++) {
				QTreeWidgetItem *qtwiRight;

				qtwiRight = qlRight.at(iJ);
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

// copy specified file system
cFileSystem *cFileControl::CopyFileSystem(const cFileSystem *cfsSource, const QString &qsNewPath /* "" */) const
{
	cFileSystem *cfsCopy;
	QString qsDrive, qsPath;

	// copied file system path correction
	if (qsNewPath.length() > 2 && qsNewPath.at(1) == ':') {
		// new path probably on different file system
		sPathInfo spiPathInfo;

		spiPathInfo = GetPathInfo(qsNewPath);
		qsDrive = spiPathInfo.qsDrive;
		qsPath = qsNewPath;
	} else {
		// new path on the same file system (can be relative)
		qsDrive = cfsSource->GetDrive();
		qsPath = cfsSource->GetPath();
	} // if else

	cfsCopy = GetFileSystem(qsDrive, qsPath);

	return cfsCopy;
} // CopyFileSystem

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

// place operation into queue
const void cFileControl::Enqueue(const sOperation &soOperation)
{
	QListWidgetItem *qlwiOperation;
	QString qsItem;

	// queue text
	switch (soOperation.eoType) {
		case cFileOperationDialog::CopyOperation:
			qsItem = tr("copy");
			break;
		case cFileOperationDialog::DeleteOperation:
			qsItem = tr("del");
			break;
		case cFileOperationDialog::MoveOperation:
			qsItem = tr("move");
	} // switch
	qsItem += ": " + soOperation.cfsSource->GetPath();
	if (soOperation.eoType != cFileOperationDialog::DeleteOperation) {
		qsItem += " -> " + soOperation.cfsDestination->GetPath();
	} // if

	// add new item into queue
	qlwiOperation = new QListWidgetItem(qsItem);
	qqOperations.enqueue(QPair<QListWidgetItem *, sOperation>(qlwiOperation, soOperation));
	emit AddIntoQueueList(qlwiOperation);

	ProcessQueue();
} // Enqueue

// get accessible drives
const QList<QPair<QString, cFileControl::sDrive> > cFileControl::GetDrives() const
{
	int iI;
	QFileInfoList qfilDrives;
	QList<QPair<QString, sDrive> > qlDrives;

	// local drives
	qfilDrives = QDir::drives();
	for (iI = 0; iI < qfilDrives.count(); iI++) {
		const QFileInfo *qfiDrive;
		sDrive sdDrive;

		qfiDrive = &qfilDrives.at(iI);
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
		const QPair<QString, sDrive> *qpDrive;

		qpDrive = &qlDrives.at(iI);
		if (qpDrive->first == qsDrive) {
			switch (qpDrive->second.edtType) {
				case Local:	cfsFileSystem = new cLocal(qsDrive, qpDrive->second.qsPath, qsPath, qmwParent, qhblOperations, csSettings, ccpContentPlugin, cppPackerPlugin);
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
		const QPair<QString, sDrive> *qpDrive;

		qpDrive = &qlDrives.at(iI);
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
const void cFileControl::on_cFileSystem_OperationFinished(cFileSystem *cfsFileSystem)
{
	int iI;

	for (iI = 0; iI < qlOperations.count(); iI++) {
		const sOperation *soOperation;

		soOperation = &qlOperations.at(iI);
		if (soOperation->cfsSource == cfsFileSystem || soOperation->cfsDestination == cfsFileSystem) {
			// found finished operation
			if (soOperation->cfsSource == cfsInQueue) {
				// it's queued operation
				cfsInQueue = NULL;
				ProcessQueue();
			} // if
			CloseFileSystem(soOperation->cfsSource);
			CloseFileSystem(soOperation->cfsDestination);
			qlOperations.removeAt(iI);
			break;
		} // if
	} // for
} // on_cFileSystem_OperationFinished

// closing lister window
const void cFileControl::on_cListerMainWindow_Close(cListerMainWindow *clmwLister) const
{
	clmwLister->deleteLater();
} // on_cListerMainWindow_Close

// remove queued items (operations)
const void cFileControl::on_cqwQueue_RemoveQueuedItems(const QList<QListWidgetItem *> &qlItems)
{
	int iI;

	for (iI = 0; iI < qlItems.count(); iI++) {
		int iJ;

		// remove from queued operations
		for (iJ = 0; iJ < qqOperations.count(); iJ++) {
			const QPair<QListWidgetItem *, sOperation> *qpOperation;

			qpOperation = &qqOperations.at(iJ);
			if (qpOperation->first == qlItems.at(iI)) {
				delete qpOperation->first;
				CloseFileSystem(qpOperation->second.cfsSource);
				CloseFileSystem(qpOperation->second.cfsDestination);
				qqOperations.removeAt(iJ);
				break;
			} // if
		} // for
	} // for

	// actualize visibility
	ProcessQueue();
} // on_cqwQueue_RemoveQueuedItems

// file operation selected
const void cFileControl::Operation(const cFileOperationDialog::eOperation &eoOperation, cFileSystem *cfsSource, QList<QTreeWidgetItem *> qlSource, const cFileSystem *cfsDestination, QString qsDestination /* "" */)
{
	cFileOperationDialog cfodDialog(qmwParent, csSettings);
	cFileOperationDialog::eUserAction euaAction;
	int iI;
	QString qsFilter;
	sOperation soOperation;
	sTypeCount stcTypeCount;
	void *vFileList;

	if (eoOperation & cFileOperationDialog::CopyOperation && !cfsSource->CanCopy()
		 || eoOperation & cFileOperationDialog::DeleteOperation && !cfsSource->CanDelete()) {
		 // operation is not allowed
		 return;
	} // if

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
		if (qsDestination.isEmpty()) {
			// called by main button
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
		} else {
			// drag and drop operation
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
	soOperation.cfsSource = CopyFileSystem(cfsSource);
	if (eoOperation != cFileOperationDialog::DeleteOperation) {
		soOperation.cfsDestination = CopyFileSystem(cfsSource, qsDestination);
	} else {
		soOperation.cfsDestination = NULL;
	} // if else
	// operation type

	// copy selected file list to the new source file system
	vFileList = cfsSource->GetFileList(qlSource);
	soOperation.cfsSource->SetOperationFileList(vFileList);
	cfsSource->FreeOperationList(vFileList);

	// correct destination path
	if (eoOperation != cFileOperationDialog::DeleteOperation) {
		if (soOperation.cfsDestination->DirExists(qsDestination)) {
			qsDestination = QDir::cleanPath(qsDestination) += "/*.*";
		} // if
	} // if

	soOperation.eoType = eoOperation;
	soOperation.qsFilter = qsFilter;
	soOperation.qsDestination = qsDestination;

	// process
	switch (euaAction) {
		case cFileOperationDialog::OkAction:
			ProcessOperation(soOperation, cFileOperation::ForegroundOperation);
			break;
		case cFileOperationDialog::EnqueueAction:
			Enqueue(soOperation);
			break;
		default:
			;
	} // if
} // Operation

// process file operation
const void cFileControl::ProcessOperation(const sOperation &soOperation, const cFileOperation::eOperationPosition &eopPosition)
{
	if (soOperation.eoType == cFileOperationDialog::DeleteOperation) {
		// delete
		qlOperations.append(soOperation);
		connect(soOperation.cfsSource, SIGNAL(OperationFinished(cFileSystem *)), SLOT(on_cFileSystem_OperationFinished(cFileSystem *)));
		soOperation.cfsSource->Delete(soOperation.qsFilter, eopPosition);
	} else {
		// copy or move
		qlOperations.append(soOperation);
		if (soOperation.cfsSource->IsLocal()) {
			connect(soOperation.cfsDestination, SIGNAL(OperationFinished(cFileSystem *)), SLOT(on_cFileSystem_OperationFinished(cFileSystem *)));
			soOperation.cfsDestination->Write(soOperation.eoType, soOperation.cfsSource->GetOperationStringList(), soOperation.qsFilter, soOperation.qsDestination, eopPosition);
		} else {
			connect(soOperation.cfsSource, SIGNAL(OperationFinished(cFileSystem *)), SLOT(on_cFileSystem_OperationFinished(cFileSystem *)));
			soOperation.cfsSource->Read(soOperation.eoType, soOperation.qsFilter, soOperation.qsDestination, eopPosition);
		} // if else
	} // if else
} // ProcessOperation

// process first queued operation
const void cFileControl::ProcessQueue()
{
	if (cfsInQueue == NULL && !qqOperations.isEmpty()) {
		QPair<QListWidgetItem *, sOperation> qpOperation;

		qpOperation = qqOperations.dequeue();
		delete qpOperation.first;
		cfsInQueue = qpOperation.second.cfsSource;

		ProcessOperation(qpOperation.second, cFileOperation::BackgroundOperation);
	} // if

	if (!qqOperations.isEmpty()) {
		cqwQueue.show();
	} else {
		cqwQueue.hide();
	} // if else
} // ProcessQueue

// show change drive dialog to set new drive for file system
const QString cFileControl::SelectDrive(const QString &qsDrive) const
{
	cSelectDriveDialog *csddSelectDrive;
	int iI;
	QList<QPair<QString, sDrive> > qlDrives;
	QString qsNewDrive;
	QStringList qslDrives;

	qlDrives = GetDrives();

	for (iI = 0; iI < qlDrives.count(); iI++) {
		qslDrives.append(qlDrives.at(iI).first);
	} // for

	csddSelectDrive = new cSelectDriveDialog(qmwParent, qslDrives, qsDrive);
	if (csddSelectDrive->exec() == QDialog::Accepted) {
		qsNewDrive = csddSelectDrive->qcbDrive->currentText();
	} // if
	csddSelectDrive->deleteLater();

	return qsNewDrive;
} // SelectDrive

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
	// TODO StartTerminal Linux terminal
#endif
} // StartTerminal

// view selected files
const void cFileControl::View(const cFileSystem *cfsFileSystem, const QList<QTreeWidgetItem *> qlSelectedFiles) const
{
	int iI;

	for (iI = 0; iI < qlSelectedFiles.count(); iI++) {
		View(cfsFileSystem, qlSelectedFiles.at(iI));
	} // for
} // View

// view selected file
const void cFileControl::View(const cFileSystem *cfsFileSystem, QTreeWidgetItem *qtwiFile) const
{
	if (cfsFileSystem->IsFile(qtwiFile)) {
		if (!cfsFileSystem->IsLocal()) {
			// TODO View copy file to temp dir
		} // if

		if (csSettings->GetViewerType() == qsINTERNAL) {
			cListerMainWindow *clmwLister;

			clmwLister = new cListerMainWindow(csSettings, clpListerPlugin, cfsFileSystem->GetFilePath(qtwiFile));
			connect(clmwLister, SIGNAL(Close(cListerMainWindow *)), SLOT(on_cListerMainWindow_Close(cListerMainWindow *)));
			clmwLister->show();
		} else {
			cProcess cpProcess;
			QString qsCommand;

			qsCommand = csSettings->GetExternalViewer();
			qsCommand = qsCommand.replace("%1", cfsFileSystem->GetFilePath(qtwiFile));

			cpProcess.StartDetached(qsCommand);
		} // if else
	} // if
} // View
