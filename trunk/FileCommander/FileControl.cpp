#include "FileControl.h"

#include <QtCore/QFileInfoList>
#include <QtCore/QDir>
#include "FileSystem/Local.h"
#include "FileControl/Process.h"
#include <QtCore/QDateTime>
#include <QtGui/QInputDialog>
#include "FileControl/SelectDriveDialog.h"
#include "FileSystem/Archive/UnpackFilesDialog.h"
#include <QtGui/QMessageBox>
#include "FileSystem/Archive/ArchiveCommon.h"

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
const bool cFileControl::ChangeFileSystem(cFileSystem **cfsFileSystem, QComboBox *qcbDrive, const QString &qsDrive)
{
	QString qsPath;

	if (qhLastPaths.contains(qsDrive)) {
		qsPath = qhLastPaths.value(qsDrive);
	} else {
		qsPath = GetDrivePath(qsDrive);
	} // if else

	return ChangeFileSystem(cfsFileSystem, qcbDrive, qsDrive, qsPath);
} // ChangeFileSystem

// change file system according to new drive
const bool cFileControl::ChangeFileSystem(cFileSystem **cfsFileSystem, QComboBox *qcbDrive, const QString &qsDrive, const QString &qsPath)
{
	// TODO ChangeFileSystem do this after implementing other than local file system (delete old one, create new one, watch FS list) (copy FTP, at least logon info if same drive as existing FTP)
	sPathInfo spiPathInfo;

	spiPathInfo = GetPathInfo(qsPath);

	if (*cfsFileSystem && (*cfsFileSystem)->TryPath(qsPath)) {
		// remember last path
		qhLastPaths.insert((*cfsFileSystem)->GetDrive(), (*cfsFileSystem)->GetPath());
		// change drive combo box
		qcbDrive->blockSignals(true);
		qcbDrive->setCurrentIndex(qcbDrive->findText(qsDrive));
		qcbDrive->blockSignals(false);
		// set path
		(*cfsFileSystem)->SetPath(qsDrive, spiPathInfo.qsRootPath, qsPath);
	} else {
		QString qsNewDrive;

		qsNewDrive = SelectDrive(qsDrive);
		if (qsNewDrive.isEmpty()) {
			qsNewDrive = qsDrive;
		} // if

		if (!*cfsFileSystem) {
			*cfsFileSystem = GetFileSystem(qsNewDrive, GetDrivePath(qsNewDrive));
		} // if
		return ChangeFileSystem(cfsFileSystem, qcbDrive, qsNewDrive);
	} // if else

	return false;
} // ChangeFileSystem

// close used file system
const void cFileControl::CloseFileSystem(cFileSystem *cfsFileSystem)
{
	qlFileSystems.removeAll(cfsFileSystem);
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
cFileSystem *cFileControl::CopyFileSystem(const cFileSystem *cfsSource, const QString &qsNewPath /* "" */)
{
	cFileSystem *cfsCopy;
	QString qsDrive, qsPath;

	if (cfsSource->Type() == cFileSystem::Local) {
		// local file system
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
	} else {
		// archive file system
		cfsCopy = GetFileSystem(cFileSystem::Archive, cfsSource->GetArchiveFilePath());
		if (cfsCopy) {
			// set archive path
			QString qsPath;

			qsPath = cfsSource->GetPath();
			qsPath = qsPath.mid(cfsSource->GetArchiveFilePath().filePath().length() + 1);
			cfsCopy->SetPath(qsPath);
		} // if
	} // if else

	return cfsCopy;
} // CopyFileSystem

// correct destination path by adding "/*.*" if possible
const QString cFileControl::CorrectDestinationPath(const cFileSystem *cfsFileSystem, const QString &qsPath) const
{
	if (cfsFileSystem->DirExists(qsPath)) {
		return QDir::cleanPath(qsPath) += "/*.*";
	} else {
		return qsPath;
	} // if else
} // CorrectDestinationPath

// create new directory
const QString cFileControl::CreateDirectory(cFileSystem *cfsFileSystem) const
{
	if (cfsFileSystem->CanCreateDir()) {
		QString qsName;

		qsName = QInputDialog::getText(qmwParent, tr("New directory"), tr("Enter name of a new directory:"));
		if (!qsName.isEmpty()) {
			QDir qdDir;
			QString qsNewDirectory;

			cfsFileSystem->CreateDir(qsName);
		} // if

		return qsName;
	} else {
		return QString();
	} // if else
} // CreateDirectory

// edit selected files
const void cFileControl::Edit(const cFileSystem *cfsFileSystem, const QList<QTreeWidgetItem *> &qlSelectedFiles) const
{
	int iI;

	for (iI = 0; iI < qlSelectedFiles.count(); iI++) {
		QTreeWidgetItem *qtwiFile;

		qtwiFile = qlSelectedFiles.at(iI);
		if (cfsFileSystem->IsFile(qtwiFile) && cfsFileSystem->Type() == cFileSystem::Local) {
			cProcess cpProcess;
			QString qsCommand;

			qsCommand = csSettings->GetExternalEditor();
			qsCommand = qsCommand.replace("%1", '"' + cfsFileSystem->GetFilePath(qtwiFile) + '"');

			cpProcess.StartDetached(qsCommand);
		} // if
	} // for
} // Edit

// place operation into queue
const void cFileControl::Enqueue(const cFileSystem::sOperation &soOperation)
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
	qsItem += ": " + soOperation.sspSource.cfsSource->GetPath();
	if (soOperation.eoType != cFileOperationDialog::DeleteOperation) {
		qsItem += " -> " + soOperation.sdpDestination.cfsDestination->GetPath();
	} // if

	// add new item into queue
	qlwiOperation = new QListWidgetItem(qsItem);
	qqOperations.enqueue(QPair<QListWidgetItem *, cFileSystem::sOperation>(qlwiOperation, soOperation));
	emit AddIntoQueueList(qlwiOperation);

	ProcessQueue();
} // Enqueue

// prepare destination path for dialog
const QString cFileControl::GetDialogDestinationPath(cFileSystem *cfsSource, const QList<QTreeWidgetItem *> qlSource, const QString &qsDestinationPath, const QString &qsDestinationDragAndDrop /* "" */) const
{
	QString qsNewDestination;

	if (qsDestinationDragAndDrop.isEmpty()) {
		// called by main button
		qsNewDestination = qsDestinationPath;

		if (qlSource.count() == 1) {
			// one file selected
			QTreeWidgetItem *qtwiFile;

			qtwiFile = qlSource.at(0);
			if (cfsSource->IsFile(qtwiFile)) {
				qsNewDestination += '/' + cfsSource->GetFileNameWithExtension(qtwiFile);
			} else {
				qsNewDestination += "/*.*";
			} // if else
		} else {
			// many files selected
			qsNewDestination += "/*.*";
		} // if else
	} else {
		// drag and drop operation
		qsNewDestination = qsDestinationDragAndDrop + "/*.*";
	} // if else

	return qsNewDestination.replace("//", "/");
} // GetDialogDestinationPath

// find root path for the drive
const QString cFileControl::GetDrivePath(const QString &qsDrive) const
{
	int iI;
	QList<QPair<QString, sDrive> > qlDrives;

	qlDrives = GetDrives();

	for (iI = 0; iI < qlDrives.count(); iI++) {
		const QPair<QString, sDrive> *qpDrive;

		qpDrive = &qlDrives.at(iI);
		if (qpDrive->first == qsDrive) {
			return qpDrive->second.qsPath;
		} // if
	} // for

	return QString();
} // GetDrivePath

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
		sdDrive.etType = cFileSystem::Local;
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

// get specified file system type
cFileSystem *cFileControl::GetFileSystem(const cFileSystem::eType &etType, const QFileInfo &qfiArchivePath /* QFileInfo() */)
{
	cFileSystem *cfsFileSystem;

	cfsFileSystem = NULL;

	if (etType == cFileSystem::Local) {
		// local file system
		int iI;
		QList<QPair<QString, sDrive> > qlDrives;

		qlDrives = GetDrives();

		for (iI = 0; iI < qlDrives.count(); iI++) {
			const QPair<QString, sDrive> *qpDrive;

			qpDrive = &qlDrives.at(iI);
			if (qpDrive->second.etType == etType) {
				cfsFileSystem = new cLocal(qpDrive->first, qpDrive->second.qsPath, qpDrive->second.qsPath, qmwParent, qhblOperations, csSettings, ccpContentPlugin, cppPackerPlugin);
				break;
			} // if
		} // for
	} else {
		// archive file system
		cfsFileSystem = new cArchive("", "", qfiArchivePath, "", qmwParent, qhblOperations, csSettings, cppPackerPlugin);
		if (!cfsFileSystem->SetPath("", true)) {
			CloseFileSystem(cfsFileSystem);
			cfsFileSystem = NULL;
		} // if
	} // if else

	if (cfsFileSystem) {
		// add new file system to file system list
		qlFileSystems.append(cfsFileSystem);
	} // if

	return cfsFileSystem;
} // GetFileSystem

cFileSystem *cFileControl::GetFileSystem(const QString &qsDrive, const QString &qsPath)
{
	cFileSystem *cfsFileSystem;
	int iI;
	QList<QPair<QString, sDrive> > qlDrives;

	cfsFileSystem = NULL;
	qlDrives = GetDrives();

	for (iI = 0; iI < qlDrives.count(); iI++) {
		const QPair<QString, sDrive> *qpDrive;

		qpDrive = &qlDrives.at(iI);
		if (qpDrive->first == qsDrive) {
			if (qpDrive->second.etType == cFileSystem::Local) {
				cfsFileSystem = new cLocal(qsDrive, qpDrive->second.qsPath, qsPath, qmwParent, qhblOperations, csSettings, ccpContentPlugin, cppPackerPlugin);
				break;
			} // if
		} // if
	} // for

	if (cfsFileSystem) {
		// add new file system to file system list
		qlFileSystems.append(cfsFileSystem);
	} // if

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
			spiPathInfo.etType = qpDrive->second.etType;
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
		const cFileSystem::sOperation *soOperation;

		soOperation = &qlOperations.at(iI);
		if (soOperation->sspSource.cfsSource == cfsFileSystem || soOperation->sdpDestination.cfsDestination == cfsFileSystem) {
			// found finished operation
			int iJ;

			if (soOperation->sspSource.cfsSource == cfsInQueue) {
				// it's queued operation
				cfsInQueue = NULL;
				ProcessQueue();
			} // if

			// check file systems in panel to refresh
			for (iJ = 0; iJ < qlFileSystems.count(); iJ++) {
				cFileSystem *cfsInPanel;

				cfsInPanel = qlFileSystems[iJ];
				if (cfsInPanel->IsInPanel()) {
					// force resfresh of file system content in some occasions
					if (cfsInPanel->Type() == cFileSystem::Archive && soOperation->sspSource.cfsSource->Type() == cFileSystem::Archive && cfsInPanel->GetArchiveFilePath() == soOperation->sspSource.cfsSource->GetArchiveFilePath() && soOperation->eoType == cFileOperationDialog::DeleteOperation) {
						// force refresh when deleting files from archive
						cfsInPanel->ForceRefresh();
					} // if
				} // if
			} // for

			CloseFileSystem(soOperation->sspSource.cfsSource);
			CloseFileSystem(soOperation->sdpDestination.cfsDestination);
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

// configure packer plugin button is clicked on
const void cFileControl::on_cpfdDialog_qpbConfigurePackerPlugin_clicked(bool checked /* false */)
{
#ifdef Q_WS_WIN
	cPackerPlugin::sPluginInfo spiPluginInfo;

	spiPluginInfo = cArchiveCommon::GetPackerPluginInfo(cpfdDialog->qtwPlugins->currentItem()->parent()->text(0), cppPackerPlugin->GetPluginsInfo());
	spiPluginInfo.tcpConfigurePacker(cpfdDialog->winId(), spiPluginInfo.hmLibrary);
#endif
	// TODO on_qpbConfigurePackerPlugin_clicked other than Windows OS
} // on_cpfdDialog_qpbConfigurePackerPlugin_clicked

// another packer plugin is selected
const void cFileControl::on_cpfdDialog_qtwPlugins_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
	// Configure button enable/disable
	if (current->flags() & Qt::ItemIsSelectable) {
		cPackerPlugin::sPluginInfo spiPluginInfo;

		spiPluginInfo = cArchiveCommon::GetPackerPluginInfo(current->parent()->text(0), cppPackerPlugin->GetPluginsInfo());
		if (spiPluginInfo.iCapabilities & PK_CAPS_OPTIONS) {
			cpfdDialog->qpbConfigurePackerPlugin->setEnabled(true);
		} else {
			cpfdDialog->qpbConfigurePackerPlugin->setEnabled(false);
		} // if else

		// change extension of archive
		cpfdDialog->qcbDestination->setEditText(cpfdDialog->qcbDestination->currentText().left(cpfdDialog->qcbDestination->currentText().length() - QFileInfo(cpfdDialog->qcbDestination->currentText()).suffix().length()) + current->text(0));
	} else {
		// select extension instead of plugin
		cpfdDialog->qtwPlugins->setCurrentItem(current->child(0));
	} // if else
} // on_cpfdDialog_qtwPlugins_currentItemChanged

// remove queued items (operations)
const void cFileControl::on_cqwQueue_RemoveQueuedItems(const QList<QListWidgetItem *> &qlItems)
{
	int iI;

	for (iI = 0; iI < qlItems.count(); iI++) {
		int iJ;

		// remove from queued operations
		for (iJ = 0; iJ < qqOperations.count(); iJ++) {
			const QPair<QListWidgetItem *, cFileSystem::sOperation> *qpOperation;

			qpOperation = &qqOperations.at(iJ);
			if (qpOperation->first == qlItems.at(iI)) {
				delete qpOperation->first;
				CloseFileSystem(qpOperation->second.sspSource.cfsSource);
				CloseFileSystem(qpOperation->second.sdpDestination.cfsDestination);
				qqOperations.removeAt(iJ);
				break;
			} // if
		} // for
	} // for

	// actualize visibility
	ProcessQueue();
} // on_cqwQueue_RemoveQueuedItems

// file operation selected
const void cFileControl::Operation(const cFileOperationDialog::eOperation &eoOperation, cFileSystem *cfsSource, QList<QTreeWidgetItem *> qlSource, const cFileSystem *cfsDestination /* NULL */, const bool bPack /* false */, const QString &qsDestinationDragAndDrop /* "" */, QFileInfoList qfilLocalSource /* QFileInfoList() */)
{
	sTypeCount stcTypeCount;
	QString qsSource;

	if (cfsSource) {
		// operation called by main button
		int iI;

		if (/*eoOperation & cFileOperationDialog::CopyOperation && !cfsSource->CanCopy()
			 || */eoOperation & cFileOperationDialog::DeleteOperation && !cfsSource->CanDelete()) {
			 // operation is not allowed
			 QMessageBox::warning(qmwParent, tr("Delete files"), tr("Delete operation is not supported by plugin."));
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

		stcTypeCount = GetFilesTypeCount(cfsSource, qlSource);
	} else {
		// drag and drop operation
		int iI;

		stcTypeCount.DirectoryType = 0;
		stcTypeCount.FileType = 0;

		for (iI = 0; iI < qfilLocalSource.count(); iI++) {
			if (qfilLocalSource.at(iI).isDir()) {
				stcTypeCount.DirectoryType++;
			} else {
				stcTypeCount.FileType++;
			} // if else
		} // for
	} // if else

	// source label
	switch (eoOperation) {
		case cFileOperationDialog::DeleteOperation:
			qsSource = tr("&Delete %1 files and %2 directories.").arg(stcTypeCount.FileType).arg(stcTypeCount.DirectoryType);
			break;
		case cFileOperationDialog::CopyOperation:
			qsSource = tr("Co&py %1 files and %2 directories to:").arg(stcTypeCount.FileType).arg(stcTypeCount.DirectoryType);
			break;
		case cFileOperationDialog::MoveOperation:
			qsSource = tr("&Move %1 files and %2 directories to:").arg(stcTypeCount.FileType).arg(stcTypeCount.DirectoryType);
	} // switch

	if (!bPack && (eoOperation & cFileOperationDialog::DeleteOperation || cfsDestination->Type() == cFileSystem::Local)) {
		// delete operation or local destination
		cFileOperationDialog cfodDialog(qmwParent, csSettings);
		cFileOperationDialog::eUserAction euaAction;
		QString qsDestination, qsFilter;

		// prepare destination path for dialog
		if (eoOperation != cFileOperationDialog::DeleteOperation) {
			qsDestination = GetDialogDestinationPath(cfsSource, qlSource, cfsDestination->GetPath(), qsDestinationDragAndDrop);
		} // if

		euaAction = cFileOperationDialog::CancelAction;
		switch (eoOperation) {
			case cFileOperationDialog::DeleteOperation:
				euaAction = cfodDialog.ShowDialog(eoOperation, qsSource, &qsDestination, &qsFilter);
				break;
			case cFileOperationDialog::CopyOperation:
				euaAction = cfodDialog.ShowDialog(eoOperation, qsSource, &qsDestination, &qsFilter);
				break;
			case cFileOperationDialog::MoveOperation:
				euaAction = cfodDialog.ShowDialog(eoOperation, qsSource, &qsDestination, &qsFilter);
		} // switch

		if (euaAction != cFileOperationDialog::CancelAction) {
			cFileSystem::sSourceParameters sspSource;
			cFileSystem::sDestinationParameters sdpDestination;

			sspSource.cfsSource = cfsSource;
			sspSource.qsFilter = qsFilter;
			sdpDestination.bFullPath = true;
			sdpDestination.qsDestination = qsDestination;
			PreProcessOperation(eoOperation, euaAction, sspSource, qlSource, sdpDestination, qfilLocalSource);
		} // if
	} else {
		// archive destination
		cFileOperationDialog::eUserAction euaAction;
		QString qsDestination;

		cpfdDialog = new cPackFilesDialog(qmwParent, csSettings);
		connect(cpfdDialog->qtwPlugins, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), SLOT(on_cpfdDialog_qtwPlugins_currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)));
		connect(cpfdDialog->qpbConfigurePackerPlugin, SIGNAL(clicked(bool)), SLOT(on_cpfdDialog_qpbConfigurePackerPlugin_clicked(bool)));

		// build plugins tree
		cArchiveCommon::CreatePluginsTree(cpfdDialog->qtwPlugins, csSettings);
		cArchiveCommon::SelectDefaultPlugin(cpfdDialog->qtwPlugins, csSettings);

		// label
		cpfdDialog->qlPack->setText(qsSource);

		// prepare destination file name
		if (cfsDestination->Type() == cFileSystem::Archive) {
			// destination is already archive -> update archive
			qsDestination = cfsDestination->GetArchiveFilePath().filePath();
		} else {
			// destination is local -> new archive
			if (cfsSource) {
				qsDestination = cfsDestination->GetPath();
				if (qlSource.count() == 1) {
					qsDestination += '/' + QFileInfo(cfsSource->GetFileName(qlSource.at(0), false)).completeBaseName();
				} else {
					QString qsArchiveName;

					qsArchiveName = cfsSource->GetDirName();

					if (qsArchiveName.isEmpty()) {
						QString qsVolumeName;

						cfsSource->GetVolumeName(&qsVolumeName);
						qsDestination += '/' + qsVolumeName;
					} // if
				} // if else
			} // if
			// add extension
			qsDestination += '.' + cpfdDialog->qtwPlugins->currentItem()->text(0);
		} // if else
		cpfdDialog->qcbDestination->setEditText(QDir::cleanPath(qsDestination));

		// default dialog settings
		cpfdDialog->qcbPackPathNames->setChecked(csSettings->GetPackerPackPathNames());
		cpfdDialog->qcbIncludingSubdirectories->setChecked(csSettings->GetPackerIncludingSubdirectories());
		if (bPack) {
			cpfdDialog->qcbMoveToArchive->setChecked(csSettings->GetPackerMoveToArchive());
		} else {
			if (eoOperation == cFileOperationDialog::MoveOperation) {
				cpfdDialog->qcbMoveToArchive->setChecked(true);
			} // if
		} // if else
		cpfdDialog->qcbOneArchivePerFileOrDirectory->setChecked(csSettings->GetPackerOneArchivePerFileOrDirectory());

		euaAction = static_cast<cFileOperationDialog::eUserAction>(cpfdDialog->exec());
		if (euaAction != cFileOperationDialog::CancelAction) {
			cFileSystem::sDestinationParameters sdpDestination;
			cFileSystem::sSourceParameters sspSource;

			sspSource.cfsSource = cfsSource;
			sspSource.qsFilter = cpfdDialog->qcbFilter->currentText();
			sspSource.bSubdirectories = cpfdDialog->qcbIncludingSubdirectories->isChecked();
			sdpDestination.qsDestination = cpfdDialog->qcbDestination->currentText();
			sdpDestination.bFullPath = cpfdDialog->qcbPackPathNames->isChecked();
			sdpDestination.bMoveTo = cpfdDialog->qcbMoveToArchive->isChecked();
			sdpDestination.bOneArchivePerSource = cpfdDialog->qcbOneArchivePerFileOrDirectory->isChecked();
			PreProcessOperation(cFileOperationDialog::PackOperation, euaAction, sspSource, qlSource, sdpDestination, qfilLocalSource);
		} // if

		cpfdDialog->deleteLater();
	} // if else
} // Operation

// preprocess file operation
const void cFileControl::PreProcessOperation(const cFileOperationDialog::eOperation &eoOperation, const cFileOperationDialog::eUserAction &euaAction, const cFileSystem::sSourceParameters &sspSource, const QList<QTreeWidgetItem *> &qlSource, cFileSystem::sDestinationParameters &sdpDestination, QFileInfoList qfilLocalSource /* QFileInfoList() */)
{
	cFileSystem::sOperation soOperation;

	// create file systems for file operation
	if (sspSource.cfsSource) {
		// operation called by main button
		soOperation.sspSource.cfsSource = CopyFileSystem(sspSource.cfsSource);
	} else {
		// drag and drop operation
		soOperation.sspSource.cfsSource = GetFileSystem(cFileSystem::Local);
	} // if else
	if (eoOperation != cFileOperationDialog::DeleteOperation) {
		soOperation.sdpDestination.cfsDestination = CopyFileSystem(soOperation.sspSource.cfsSource, sdpDestination.qsDestination);
	} else {
		soOperation.sdpDestination.cfsDestination = NULL;
	} // if else
	// operation type

	// copy selected file list to the new source file system
	if (sspSource.cfsSource) {
		// operation called by main button
		void *vFileList;

		vFileList = sspSource.cfsSource->GetFileList(qlSource);
		soOperation.sspSource.cfsSource->SetOperationFileList(vFileList);
		sspSource.cfsSource->FreeOperationList(vFileList);
	} else {
		// drag and drop operation
		soOperation.sspSource.cfsSource->SetOperationFileList(&qfilLocalSource);
	} // if else

	// correct destination path
	if (eoOperation != cFileOperationDialog::DeleteOperation) {
		sdpDestination.qsDestination = CorrectDestinationPath(soOperation.sdpDestination.cfsDestination, sdpDestination.qsDestination);
	} // if

	soOperation.eoType = eoOperation;

	// process
	switch (euaAction) {
		case cFileOperationDialog::OkAction:
			soOperation.eopPosition = cFileOperation::ForegroundOperation;
			ProcessOperation(soOperation);
			break;
		case cFileOperationDialog::EnqueueAction:
			soOperation.eopPosition = cFileOperation::BackgroundOperation;
			Enqueue(soOperation);
			break;
		default:
			;
	} // if
} // PreProcessOperation

// process file operation
const void cFileControl::ProcessOperation(const cFileSystem::sOperation &soOperation)
{
	if (soOperation.eoType == cFileOperationDialog::DeleteOperation) {
		// delete
		qlOperations.append(soOperation);
		connect(soOperation.sspSource.cfsSource, SIGNAL(OperationFinished(cFileSystem *)), SLOT(on_cFileSystem_OperationFinished(cFileSystem *)));
		soOperation.sspSource.cfsSource->Delete(soOperation);
	} else {
		// copy or move
		qlOperations.append(soOperation);
		if (soOperation.sspSource.cfsSource->Type() == cFileSystem::Local) {
			// local file system
			connect(soOperation.sdpDestination.cfsDestination, SIGNAL(OperationFinished(cFileSystem *)), SLOT(on_cFileSystem_OperationFinished(cFileSystem *)));
			soOperation.sdpDestination.cfsDestination->Write(soOperation);
		} else {
			// archive file system
			if (soOperation.eoType == cFileOperationDialog::PackOperation) {
				connect(soOperation.sspSource.cfsSource, SIGNAL(OperationFinished(cFileSystem *)), SLOT(on_cFileSystem_OperationFinished(cFileSystem *)));
				//soOperation.sspSource.cfsSource->Write(soOperation);
			} else {
				connect(soOperation.sspSource.cfsSource, SIGNAL(OperationFinished(cFileSystem *)), SLOT(on_cFileSystem_OperationFinished(cFileSystem *)));
				soOperation.sspSource.cfsSource->Read(soOperation);
			} // if else
		} // if else
	} // if else
} // ProcessOperation

// process first queued operation
const void cFileControl::ProcessQueue()
{
	if (cfsInQueue == NULL && !qqOperations.isEmpty()) {
		QPair<QListWidgetItem *, cFileSystem::sOperation> qpOperation;

		qpOperation = qqOperations.dequeue();
		delete qpOperation.first;
		cfsInQueue = qpOperation.second.sspSource.cfsSource;

		ProcessOperation(qpOperation.second);
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

// unpack selected files
const void cFileControl::UnpackSelectedFiles(cFileSystem *cfsFileSystem, const QList<QTreeWidgetItem *> &qlSelectedFiles, const QString &qsDestination)
{
	cUnpackFilesDialog *cufdDialog;
	int iI;
	QList<QTreeWidgetItem *> qlSources;
	QString qsPreparedDestination;

	// files only
	for (iI = 0; iI < qlSelectedFiles.count(); iI++) {
		QTreeWidgetItem *qlSelectedFile;

		qlSelectedFile = qlSelectedFiles.at(iI);

		if (cfsFileSystem->Type() == cFileSystem::Local) {
			// local file system -> ignore directories
			if (cfsFileSystem->IsFile(qlSelectedFile)) {
				qlSources.append(qlSelectedFile);
			} // if
		} else {
			// (probably) archive file system -> ignore just ".." directory
			if (cfsFileSystem->GetFileName(qlSelectedFile, false) != "..") {
				qlSources.append(qlSelectedFile);
			} // if
		} // if else
	} // for

	if (qlSources.isEmpty()) {
		// nothing to unpack
		return;
	} // if

	qsPreparedDestination = GetDialogDestinationPath(cfsFileSystem, qlSelectedFiles, "", qsDestination);
	cufdDialog = new cUnpackFilesDialog(qmwParent, qsPreparedDestination, csSettings);

	if (cufdDialog->exec() == QDialog::Accepted) {
		qsPreparedDestination = CorrectDestinationPath(cfsFileSystem, cufdDialog->qcbDestination->currentText());

		if (cfsFileSystem->Type() == cFileSystem::Local) {
			// local file system -> sources can be archive files
			for (iI = 0; iI < qlSources.count(); iI++) {
				cFileSystem *cfsArchive;
				QTreeWidgetItem *qlSource;

				qlSource = qlSources.at(iI);
				cfsArchive = GetFileSystem(cFileSystem::Archive, cfsFileSystem->GetFilePath(qlSource));

				if (cfsArchive) {
					QList<QTreeWidgetItem *> qlFiles;
					QString qsExtractPath;
					cFileSystem::sDestinationParameters sdpDestination;
					cFileSystem::sSourceParameters sspSource;

					if (cufdDialog->qcbSeparatedSubdirectory->isChecked()) {
						// modify destination path
						QFileInfo qfiPath;

						qfiPath.setFile(qsPreparedDestination);
						if (qfiPath.fileName() == "*.*") {
							qsExtractPath = qfiPath.path();
						} else {
							qsExtractPath = qsPreparedDestination;
						} // if else
						qsExtractPath = QDir::cleanPath(qsExtractPath) + '/' + cfsFileSystem->GetFileName(qlSource) + "/*.*";
					} else {
						qsExtractPath = qsPreparedDestination;
					} // if else

					cfsArchive->GetDirectoryContent(&qlFiles);

					sspSource.cfsSource = cfsArchive;
					sspSource.qsFilter = cufdDialog->qcbFilter->currentText();
					sdpDestination.bFullPath = cufdDialog->qcbUnpackWithFullPath->isChecked();
					sdpDestination.qsDestination = qsExtractPath;
					PreProcessOperation(cFileOperationDialog::CopyOperation, cFileOperationDialog::EnqueueAction, sspSource, qlFiles, sdpDestination);
					CloseFileSystem(cfsArchive);
				} else {
					// unsupported archive file or not archive file
					QMessageBox::warning(qmwParent, tr("Unpack archive"), tr("Archive %1 not supported.").arg(cfsFileSystem->GetFileNameWithExtension(qlSource)));
				} // if else
			} // for
		} else {
			// nonlocal -> (probably) already in archive
			cFileSystem::sDestinationParameters sdpDestination;
			cFileSystem::sSourceParameters sspSource;

			sspSource.cfsSource = cfsFileSystem;
			sspSource.qsFilter = cufdDialog->qcbFilter->currentText();
			sdpDestination.bFullPath = cufdDialog->qcbUnpackWithFullPath->isChecked();
			sdpDestination.qsDestination = qsPreparedDestination;
			PreProcessOperation(cFileOperationDialog::CopyOperation, cFileOperationDialog::OkAction, sspSource, qlSources, sdpDestination);
		} // if else
	} // if

	cufdDialog->deleteLater();
} // UnpackSelectedFiles

// view selected files
const void cFileControl::View(const cFileSystem *cfsFileSystem, const QList<QTreeWidgetItem *> &qlSelectedFiles) const
{
	int iI;

	for (iI = 0; iI < qlSelectedFiles.count(); iI++) {
		View(cfsFileSystem, qlSelectedFiles.at(iI));
	} // for
} // View

// view selected file
const void cFileControl::View(const cFileSystem *cfsFileSystem, QTreeWidgetItem *qtwiFile) const
{
	if (cfsFileSystem->IsFile(qtwiFile) && cfsFileSystem->Type() == cFileSystem::Local) {
		if (csSettings->GetViewerType() == qsINTERNAL) {
			cListerMainWindow *clmwLister;

			clmwLister = new cListerMainWindow(csSettings, clpListerPlugin, cfsFileSystem->GetFilePath(qtwiFile));
			connect(clmwLister, SIGNAL(Close(cListerMainWindow *)), SLOT(on_cListerMainWindow_Close(cListerMainWindow *)));
			clmwLister->show();
		} else {
			cProcess cpProcess;
			QString qsCommand;

			qsCommand = csSettings->GetExternalViewer();
			qsCommand = qsCommand.replace("%1", '"' + cfsFileSystem->GetFilePath(qtwiFile) + '"');

			cpProcess.StartDetached(qsCommand);
		} // if else
	} // if
} // View
