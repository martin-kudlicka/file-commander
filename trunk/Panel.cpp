#include "Panel.h"

#include <QDir>
#include <QDateTime>
#include "Panel/Process.h"
#include <QHeaderView>
#include <QProcess>

cSettings::sSort cPanel::ssSort;	///< sort information (static class variable)

// destructor
cPanel::~cPanel()
{
	delete csmMenu;
	ccdContentDelayed->deleteLater();
} // ~cPanel

// actualize volume information - disk name and space
void cPanel::ActualizeVolumeInfo()
{
	cFileRoutine::sDiskSpace sdsInfo;
	QString qsName;

#ifdef Q_WS_WIN
	qsName = cFileRoutine::GetVolumeName(qmDrives->value(qcbDrive->currentText()).qsPath);
#else
	qsName = qcbDrive->currentText();
#endif
	sdsInfo = cFileRoutine::GetDiskSpace(qmDrives->value(qcbDrive->currentText()).qsPath);
	qlDriveInfo->setText(tr("[%1] %2 of %3 free").arg(qsName).arg(GetSizeString(sdsInfo.qi64Free)).arg(GetSizeString(sdsInfo.qi64Total)));
} // ActualizeVolumeInfo

// actualize widgets with info about current directory view
void cPanel::ActualizeWidgets()
{
	QDir qdDir;

	qdDir.setPath(qhTabs.value(qswDir->currentIndex()).swWidgets->qsPath);

	// tab bar
	if (qdDir.dirName() == "") {
		qtbTab->setTabText(qswDir->currentIndex(), qdDir.path().at(0));
	} else {
		qtbTab->setTabText(qswDir->currentIndex(), qdDir.dirName());
	} // if else
	// path

	qcbDrive->blockSignals(true);
	qcbDrive->setCurrentIndex(qcbDrive->findText(qhTabs.value(qswDir->currentIndex()).swWidgets->qsDrive));
	qcbDrive->blockSignals(false);
	ActualizeVolumeInfo();
	qlPath->setText(qhTabs.value(qswDir->currentIndex()).swWidgets->qsPath);
	qlSelected->setText(qhTabs.value(qswDir->currentIndex()).swWidgets->qsSelected);
} // ActualizeWidgets

// add new tab with dir view
void cPanel::AddTab(const cSettings::sTabInfo &stiTabInfo)
{
	cTreeWidget *ctwTree;
	int iIndex;
	sTab stTab;

	// create tab
	qtbTab->addTab("");
	ctwTree = new cTreeWidget();
	ctwTree->setRootIsDecorated(false);
	ctwTree->setContextMenuPolicy(Qt::CustomContextMenu);
	ctwTree->setSelectionMode(QAbstractItemView::ExtendedSelection);
	iIndex = qswDir->addWidget(ctwTree);

	// connect signals to slots
	connect(ctwTree, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(on_ctwTree_customContextMenuRequested(const QPoint &)));
	connect(ctwTree, SIGNAL(itemActivated(QTreeWidgetItem *, int)), SLOT(on_ctwTree_itemActivated(QTreeWidgetItem *, int)));
	connect(ctwTree, SIGNAL(itemSelectionChanged(const cTreeWidget *)), SLOT(on_ctwTree_itemSelectionChanged(const cTreeWidget *)));
	connect(ctwTree, SIGNAL(SpacePressed(QTreeWidgetItem *)), SLOT(on_ctwTree_SpacePressed(QTreeWidgetItem *)));

	// set tab properties
	stTab.qhFiles = new QHash<QTreeWidgetItem *, QFileInfo>;
	stTab.qlColumns = new QList<cSettings::sColumn>;
	stTab.swWidgets = new sWidgets();
	stTab.swWidgets->qsDrive = stiTabInfo.qsDrive;
	stTab.swWidgets->qsPath = stiTabInfo.qsPath;
	stTab.qsColumnSet = stiTabInfo.qsColumnSet;
	qhTabs.insert(iIndex, stTab);

	// add path to watcher
	qfswWatcher.addPath(stiTabInfo.qsPath);

	// set header
	RefreshHeader(iIndex);

	// set sorting
	ctwTree->header()->setSortIndicator(stiTabInfo.ssSort.iSortedColumn, stiTabInfo.ssSort.soSortOrder);

	// connect signals to slots
	connect(ctwTree->header(), SIGNAL(sectionClicked(int)), SLOT(on_qhvTreeHeader_sectionClicked(int)));
} // AddTab

// constructor
cPanel::cPanel(QStackedWidget *qswPanel, QComboBox *qcbDrive, QLabel *qlDriveInfo, QTabBar *qtbTab, QLabel *qlPath, QLabel *qlSelected, cSettings *csSettings, cContent *ccContent, QMap<QString, cFileRoutine::sDriveInfo> *qmDrives)
{
	qswDir = qswPanel;
	this->qcbDrive = qcbDrive;
	this->qlDriveInfo = qlDriveInfo;
	this->qtbTab = qtbTab;
	this->qlPath = qlPath;
	this->qlSelected = qlSelected;
	this->csSettings = csSettings;
	this->ccContent = ccContent;
	this->qmDrives = qmDrives;
	csmMenu = new cShellMenu(
#ifdef Q_WS_WIN
	qswDir->winId()
#endif
	);
	// signals for controls
	connect(qcbDrive, SIGNAL(activated(int)), SLOT(on_qcbDrive_activated(int)));
	connect(qcbDrive, SIGNAL(currentIndexChanged(int)), SLOT(on_qcbDrive_currentIndexChanged(int)));
	connect(&qfswWatcher, SIGNAL(directoryChanged(const QString &)), SLOT(on_qfswWatcher_directoryChanged(const QString &)));

	// timer
	connect(&qtTimer, SIGNAL(timeout()), SLOT(on_qtTimer_timeout()));
	qtTimer.start(iTIMER_INTERVAL);

	// delayed column update
	ccdContentDelayed = new cContentDelayed(ccContent);
	connect(ccdContentDelayed, SIGNAL(GotColumnValue(const cContentDelayed::sOutput &)), SLOT(on_ccdContentDelayed_GotColumnValue(const cContentDelayed::sOutput &)));
	connect(this, SIGNAL(InterruptContentDelayed()), ccdContentDelayed, SLOT(on_InterruptContentDelayed()));
} // cPanel

// edit selected file in editor
void cPanel::EditFile()
{
	QFileInfo qfiFile;

	qfiFile = qhTabs.value(qswDir->currentIndex()).qhFiles->value(static_cast<cTreeWidget *>(qswDir->currentWidget())->currentItem());

	if (qfiFile.isFile()) {
		// TODO EditFile other OS than Windows
#ifdef Q_WS_WIN
		QProcess::startDetached("notepad.exe", QStringList() << qfiFile.filePath());
#endif
	} // if
} // EditFile

// show custom list of files in current dir view
void cPanel::FeedToPanel(QFileInfoList &qfilFiles)
{
	RefreshContent(qswDir->currentIndex(), qfilFiles);
} // FeedToPanel

// count objects
cPanel::sObjects cPanel::GetCount(const QFileInfoList &qfilObjects)
{
	int iI;
	sObjects soCount;

	soCount.Directories = 0;
	soCount.Files = 0;
	for (iI = 0; iI < qfilObjects.count(); iI++) {
		if (qfilObjects.at(iI).isDir()) {
			soCount.Directories++;
		} else {
			soCount.Files++;
		} // if else
	} // for

	return soCount;
} // GetCount

// find index of native column
int cPanel::GetNativeColumnIndex(const QString &qsColumn, const int &iTabIndex)
{
	int iI;

	for (iI = 0; iI < qhTabs.value(iTabIndex).qlColumns->count(); iI++) {
		if (qhTabs.value(iTabIndex).qlColumns->at(iI).qsIdentifier == qsColumn) {
			return iI;
		}
	} // for

	return -1;
} // GetNativeColumnIndex

// get path for current dir
QString cPanel::GetPath()
{
	return qhTabs.value(qswDir->currentIndex()).swWidgets->qsPath;
} // GetPath

// get dir infos of selected items
QFileInfoList cPanel::GetSelectedDirItemsList()
{
	int iI;
	QFileInfoList qfilDirectories;
	QList<QTreeWidgetItem *> qlSelected;

	qlSelected = static_cast<cTreeWidget *>(qswDir->currentWidget())->selectedItems();
	for (iI = 0; iI < qlSelected.count(); iI++) {
		if (qhTabs.value(qswDir->currentIndex()).qhFiles->value(qlSelected.at(iI)).fileName() != ".." && qhTabs.value(qswDir->currentIndex()).qhFiles->value(qlSelected.at(iI)).isDir()) {
			// ignore ".." directories
			qfilDirectories.append(qhTabs.value(qswDir->currentIndex()).qhFiles->value(qlSelected.at(iI)));
		} // if
	} // for

	return qfilDirectories;
} // GetSelectedDirItemsList

// get file infos of selected items
QFileInfoList cPanel::GetSelectedItemsList()
{
	int iI;
	QFileInfoList qfilFiles;
	QList<QTreeWidgetItem *> qlSelected;

	qlSelected = static_cast<cTreeWidget *>(qswDir->currentWidget())->selectedItems();
	for (iI = 0; iI < qlSelected.count(); iI++) {
		if (qhTabs.value(qswDir->currentIndex()).qhFiles->value(qlSelected.at(iI)).fileName() != "..") {
			// ignore ".." directories
			qfilFiles.append(qhTabs.value(qswDir->currentIndex()).qhFiles->value(qlSelected.at(iI)));
		} // if
	} // for

	return qfilFiles;
} // GetSelectedItemsList

// get selected files and directories from current dir view
QStringList cPanel::GetSelectedItemsStringList()
{
	int iI;
	QList<QTreeWidgetItem *> qlSelected;
	QStringList qslSelected;

	qlSelected = static_cast<cTreeWidget *>(qswDir->currentWidget())->selectedItems();
	for (iI = 0; iI < qlSelected.count(); iI++) {
		qslSelected.append(QDir::toNativeSeparators(qhTabs.value(qswDir->currentIndex()).qhFiles->value(qlSelected.at(iI)).filePath()));
	} // for

	return qslSelected;
} // GetSelectedItemsStringList

// "convert" size to string according to setting in options
QString cPanel::GetSizeString(const qint64 &qi64Size)
{
	QString qsFileSizeIn;

	qsFileSizeIn = csSettings->GetFileSizeIn();

	if (qsFileSizeIn == qsDYNAMIC) {
		if (qi64Size < qi64_KILOBYTE) {
			qsFileSizeIn = qsBYTES;
		} else {
			if (qi64Size < qi64_MEGABYTE) {
				qsFileSizeIn = qsKILOBYTES;
			} else {
				if (qi64Size < qi64_GIGABYTE) {
					qsFileSizeIn = qsMEGABYTES;
				} else {
					qsFileSizeIn = qsGIGABYTES;
				} // if else
			} // if else
		} // if else
	} // if

	if (qsFileSizeIn == qsBYTES) {
		return QVariant(qi64Size).toString() + tr(" B");
	} else {
		if (qsFileSizeIn == qsKILOBYTES) {
			return QString("%1 k").arg(static_cast<double>(qi64Size) / qi64_KILOBYTE, 0, 'f', 1);
		} else {
			if (qsFileSizeIn == qsMEGABYTES) {
				return QString("%1 M").arg(static_cast<double>(qi64Size) / qi64_MEGABYTE, 0, 'f', 1);
			} else {
				return QString("%1 G").arg(static_cast<double>(qi64Size) / qi64_GIGABYTE, 0, 'f', 1);
			} // if else
		} // if else
	} // if else
} // GetSizeString

// show dir view with specified file
void cPanel::GoToFile(const QString &qsFile)
{
	// refresh content to new path
	SetPath(QFileInfo(qsFile).path());

	// find the right file
	QHashIterator<QTreeWidgetItem *, QFileInfo> qhiFiles(*qhTabs.value(qswDir->currentIndex()).qhFiles);
	while (qhiFiles.hasNext()) {
		qhiFiles.next();
		if (qhiFiles.value().fileName() == QFileInfo(qsFile).fileName()) {
			static_cast<QTreeWidget *>(qswDir->currentWidget())->clearSelection();
			qhiFiles.key()->setSelected(true);
			break;
		} // if
	} // while
} // GoToFile

// go to root directory
void cPanel::GoToRootDir()
{
	QDir qdDir;

	qdDir.setPath(qhTabs.value(qswDir->currentIndex()).swWidgets->qsPath);
	if(!qdDir.isRoot()) {
		SetPath(qdDir.rootPath());
	} // if
} // GoToRootDir

// go up one level (directory)
void cPanel::GoToUpDir()
{
	QDir qdDir;

	qdDir.setPath(qhTabs.value(qswDir->currentIndex()).swWidgets->qsPath);
	if(!qdDir.isRoot()) {
		SetPath(qhTabs.value(qswDir->currentIndex()).swWidgets->qsPath + "/..");
	} // if
} // GoToUpDir

// got golumn value from plugin
void cPanel::on_ccdContentDelayed_GotColumnValue(const cContentDelayed::sOutput &soOutput)
{
	soOutput.qtwiItem->setText(soOutput.iColumn, soOutput.qsValue);
} // on_ccdContentDelayed_GotColumnValue

// show tree view context menu
void cPanel::on_ctwTree_customContextMenuRequested(const QPoint &pos)
{
	csmMenu->Show(GetSelectedItemsStringList(), static_cast<cTreeWidget *>(qswDir->currentWidget())->viewport()->mapToGlobal(pos));
} // on_ctwTree_customContextMenuRequested

// double click in tree view
void cPanel::on_ctwTree_itemActivated(QTreeWidgetItem *item, int column)
{
	QFileInfo qfiFile;

	qfiFile = qhTabs.value(qswDir->currentIndex()).qhFiles->value(item);
	if (qfiFile.isDir()) {
		// double click on directory -> go into directory
		SetPath(qfiFile.filePath());
	} else {
		// double click on file -> execute it
		cProcess::Execute(qfiFile.filePath());
	} // if else
} // on_ctwTree_itemActivated

// changed selected items in directory view
void cPanel::on_ctwTree_itemSelectionChanged(const cTreeWidget *ctwTree)
{
	int iDirectories, iDirectoriesTotal, iFiles, iFilesTotal, iI;
	qint64 qi64Size, qi64TotalSize;

	// find tab for tree widget
	QHashIterator<uint, sTab> qhiTab(qhTabs);
	while (qhiTab.hasNext()) {
		qhiTab.next();
		if (static_cast<cTreeWidget *>(qswDir->widget(qhiTab.key())) == ctwTree) {
			break;
		} // if
	} // while

	iDirectories = 0;
	iDirectoriesTotal = 0;
	iFiles = 0;
	iFilesTotal = 0;
	qi64Size = 0;
	qi64TotalSize = 0;
	for (iI = 0; iI < ctwTree->topLevelItemCount(); iI++) {
		if (qhiTab.value().qhFiles->value(ctwTree->topLevelItem(iI)).isDir()) {
			iDirectoriesTotal++;
			if (ctwTree->topLevelItem(iI)->isSelected()) {
				iDirectories++;
			} // if
		} else {
			iFilesTotal++;
			qi64TotalSize += qhiTab.value().qhFiles->value(ctwTree->topLevelItem(iI)).size();
			if (ctwTree->topLevelItem(iI)->isSelected()) {
				iFiles++;
				qi64Size += qhiTab.value().qhFiles->value(ctwTree->topLevelItem(iI)).size();
			} // if
		} // if else
	} // for

	qhiTab.value().swWidgets->qsSelected = tr("%1 / %2 in %3 / %4 files and %5 / %6 directories").arg(GetSizeString(qi64Size)).arg(GetSizeString(qi64TotalSize)).arg(iFiles).arg(iFilesTotal).arg(iDirectories).arg(iDirectoriesTotal);
	if (static_cast<cTreeWidget *>(qswDir->currentWidget()) == ctwTree) {
		ActualizeWidgets();
	} // if
} // on_ctwTree_itemSelectionChanged

// space pressed in dir view
void cPanel::on_ctwTree_SpacePressed(QTreeWidgetItem *qtwiItem)
{
	int iColumnExtension, iI;
	QFileInfo qfiFile;
	QFileInfoList qfilFiles;
	qint64 qi64Size;

	qfiFile = qhTabs.value(qswDir->currentIndex()).qhFiles->value(qtwiItem);

	// refresh content plugin values
	for (iI = 0; iI < qhTabs.value(qswDir->currentIndex()).qlColumns->count(); iI++) {
		if (qhTabs.value(qswDir->currentIndex()).qlColumns->at(iI).qsPlugin != qsNO) {
			qtwiItem->setText(iI, ccContent->GetPluginValue(qfiFile.filePath(), qhTabs.value(qswDir->currentIndex()).qlColumns->at(iI).qsPlugin, qhTabs.value(qswDir->currentIndex()).qlColumns->at(iI).qsIdentifier, qhTabs.value(qswDir->currentIndex()).qlColumns->at(iI).qsUnit));
		} // if
	} // for

	if (qfiFile.isFile()) {
		// selected item is file
		return;
	} // if

	iColumnExtension = GetNativeColumnIndex(qsEXTENSION, qswDir->currentIndex());
	if (iColumnExtension == -1) {
		// no place to show occupied space
		return;
	} // if

	qfilFiles = cFileRoutine::GetSources(QFileInfoList() << qfiFile);
	qi64Size = 0;
	for (iI = 0; iI < qfilFiles.count(); iI++) {
		qi64Size += qfilFiles.at(iI).size();
	} // for

	qtwiItem->setText(iColumnExtension, GetSizeString(qi64Size));
} // on_ctwTree_SpacePressed

// drive selected
void cPanel::on_qcbDrive_activated(int index)
{
	static_cast<cTreeWidget *>(qswDir->currentWidget())->setFocus(Qt::OtherFocusReason);
} // on_qcbDrive_activated

// selected drive changes
void cPanel::on_qcbDrive_currentIndexChanged(int index)
{
	if (qswDir->currentIndex() == -1) {
		// tabs are not yet initialized
		return;
	} // if

	qhTabs.value(qswDir->currentIndex()).swWidgets->qsDrive = qcbDrive->currentText();

	QMapIterator<QString, cFileRoutine::sDriveInfo> qmiDrives(*qmDrives);
	while (qmiDrives.hasNext()) {
		qmiDrives.next();
		if (qmiDrives.key() == qcbDrive->currentText()) {
			QDir qdDir;

			if (qdDir.exists(qhLastPaths.value(qcbDrive->currentText()))) {
				SetPath(qhLastPaths.value(qcbDrive->currentText()));
			} else {
				SetPath(qmiDrives.value().qsPath);
			} // if else
			break;
		} // if
	} // while
} // on_qcbDrive_currentIndexChanged

// detect directory modifications
void cPanel::on_qfswWatcher_directoryChanged(const QString &path)
{
	int iI;

	// refresh content in tabs with set directory to path
	for (iI = 0; iI < qhTabs.count(); iI++) {
		if (qhTabs.value(iI).swWidgets->qsPath == path) {
			RefreshContent(iI);
		} // if
	} // for
} // on_qfswWatcher_directoryChanged

// click on header in tree (dir) view
void cPanel::on_qhvTreeHeader_sectionClicked(int logicalIndex)
{
	Sort(qswDir->currentIndex());
} // on_qhvTreeHeader_sectionClicked

// timer's timeout
void cPanel::on_qtTimer_timeout()
{
	ActualizeVolumeInfo();
} // on_qtTimer_timeout

// refresh dir content
void cPanel::RefreshContent(const int &iIndex, QFileInfoList &qfilFiles)
{
	int iI;
	QDir::Filters fFilters;
	QList<cContentDelayed::sParameters> qlParameters;

	// interrupt delayed content processing
	emit InterruptContentDelayed();

	// clear previous file contents
	qhTabs.value(iIndex).qhFiles->clear();

	if (qfilFiles.count() == 0) {
		// get file list
		fFilters = QDir::Dirs | QDir::Files;
		if (csSettings->GetShowSystemFiles()) {
			fFilters |= QDir::System;
		} // if
		if (csSettings->GetShowHiddenFiles()) {
			fFilters |= QDir::Hidden;
		} // if
		qfilFiles = cFileRoutine::GetDirectoryContent(qhTabs.value(iIndex).swWidgets->qsPath, fFilters);
	} // if

	// go through files and add them into file list
	for (iI = 0; iI < qfilFiles.count(); iI++) {
		int iJ;
		QTreeWidgetItem *qtwiFile;

		// omit some entries
		if (qfilFiles.at(iI).fileName() == ".") {
			continue;
		} // if

		// add to internal file list
		qtwiFile = new QTreeWidgetItem();
		qhTabs.value(iIndex).qhFiles->insert(qtwiFile, qfilFiles.at(iI));

		// fill columns
		for (iJ = 0; iJ < qhTabs.value(iIndex).qlColumns->count(); iJ++) {
			if (qhTabs.value(iIndex).qlColumns->at(iJ).qsPlugin == qsNO) {
				// native
				if (qhTabs.value(iIndex).qlColumns->at(iJ).qsIdentifier == qsICON) {
					// icon
					qtwiFile->setIcon(iJ, qfipIcon.icon(qfilFiles.at(iI)));
				} else {
					if (qhTabs.value(iIndex).qlColumns->at(iJ).qsIdentifier == qsNAME) {
						// name
						if (qfilFiles.at(iI).isDir() && qfilFiles.at(iI).fileName() == "..") {
							// special handle for ".." directory to show both points
							qtwiFile->setText(iJ, qfilFiles.at(iI).fileName());
						} else {
							qtwiFile->setText(iJ, qfilFiles.at(iI).completeBaseName());
						} // if
					} else {
						if (qhTabs.value(iIndex).qlColumns->at(iJ).qsIdentifier == qsEXTENSION) {
							// extension
							qtwiFile->setText(iJ, qfilFiles.at(iI).suffix());
						} else {
							if (qhTabs.value(iIndex).qlColumns->at(iJ).qsIdentifier == qsSIZE) {
								// size
								if (qfilFiles.at(iI).isDir()) {
									qtwiFile->setText(iJ, tr("<DIR>"));
								} else {
									qtwiFile->setText(iJ, GetSizeString(qfilFiles.at(iI).size()));
								} // if else
							} else {
								// date
								qtwiFile->setText(iJ, qfilFiles.at(iI).lastModified().toString());
							} // if else
						} // if else
					} // if else
				} // if else
			} else {
				// plugin
				int iFlag;

				qtwiFile->setText(iJ, ccContent->GetPluginValue(qfilFiles.at(iI).filePath(), qhTabs.value(iIndex).qlColumns->at(iJ).qsPlugin, qhTabs.value(iIndex).qlColumns->at(iJ).qsIdentifier, qhTabs.value(iIndex).qlColumns->at(iJ).qsUnit, &iFlag));
				if (iFlag == ft_delayed) {
					cContentDelayed::sParameters spParameters;

					// thread input
					spParameters.siInput.qsFilename = qfilFiles.at(iI).filePath();
					spParameters.siInput.qsPlugin = qhTabs.value(iIndex).qlColumns->at(iJ).qsPlugin;
					spParameters.siInput.qsColumn = qhTabs.value(iIndex).qlColumns->at(iJ).qsIdentifier;
					spParameters.siInput.qsUnit = qhTabs.value(iIndex).qlColumns->at(iJ).qsUnit;

					// thread output
					spParameters.soOutput.qtwiItem = qtwiFile;
					spParameters.soOutput.iColumn = iJ;

					qlParameters.append(spParameters);
				} // if
			} // if else
		} // for
	} // for

	// sort and show files
	Sort(iIndex);

	if (static_cast<cTreeWidget *>(qswDir->widget(iIndex))->topLevelItemCount() > 0) {
		// mark first item
		static_cast<cTreeWidget *>(qswDir->widget(iIndex))->topLevelItem(0)->setSelected(true);
		// focus to the first item
		static_cast<cTreeWidget *>(qswDir->widget(iIndex))->setCurrentItem(static_cast<cTreeWidget *>(qswDir->widget(iIndex))->topLevelItem(0));
	} // if

	if (qlParameters.count() > 0) {
		// start thread to query content plugins values
		ccdContentDelayed->Start(qlParameters);
	} // if
} // RefreshContent

// refresh column's header
void cPanel::RefreshHeader(const int &iIndex)
{
	bool bAutoStretch;
	int iI;
	QStringList qslColumns;
	QTreeWidgetItem *qtwiHeader;

	// clear previous header contents and fill new information
	qhTabs.value(iIndex).qlColumns->clear();
	qslColumns = csSettings->GetColumnsInSet(qhTabs.value(iIndex).qsColumnSet);
	if (qslColumns.count() == 0 && qhTabs.value(iIndex).qsColumnSet == qsFULL) {
		// create default column set (Full) if not created yet
		csSettings->CreateDefaultColumnSet();
		qslColumns = csSettings->GetColumnsInSet(qhTabs.value(iIndex).qsColumnSet);
	} // if
	for (iI = 0; iI < qslColumns.count(); iI++) {
		cSettings::sColumn scColumn;

		scColumn = csSettings->GetColumnInfo(qhTabs.value(iIndex).qsColumnSet, qslColumns.at(iI));
		// test if is native or plugin and loaded
		if (scColumn.qsPlugin == qsNO || ccContent->Loaded(scColumn.qsPlugin)) {
			// ok -> add to column list
			qhTabs.value(iIndex).qlColumns->append(scColumn);
		} // if
	} // for

	// set header according to qlColumns list
	qtwiHeader = new QTreeWidgetItem();
	for (iI = 0; iI < qhTabs.value(iIndex).qlColumns->count(); iI++) {
		qtwiHeader->setText(iI, qhTabs.value(iIndex).qlColumns->at(iI).qsName);
	} // for
	static_cast<cTreeWidget *>(qswDir->widget(iIndex))->setHeaderItem(qtwiHeader);
	bAutoStretch = false;
	// set columns width
	for (iI = 0; iI < qhTabs.value(iIndex).qlColumns->count(); iI++) {
		if (qhTabs.value(iIndex).qlColumns->at(iI).iWidth == 0) {
			static_cast<cTreeWidget *>(qswDir->widget(iIndex))->header()->setResizeMode(iI, QHeaderView::Stretch);
			bAutoStretch = true;
		} else {
			static_cast<cTreeWidget *>(qswDir->widget(iIndex))->header()->resizeSection(iI, qhTabs.value(iIndex).qlColumns->at(iI).iWidth);
		} // if else
	} // for
	if (bAutoStretch) {
		static_cast<cTreeWidget *>(qswDir->widget(iIndex))->header()->setStretchLastSection(false);
	} // if
	// show sort order
	static_cast<cTreeWidget *>(qswDir->widget(iIndex))->header()->setSortIndicatorShown(true);
	// clickable flag
	static_cast<cTreeWidget *>(qswDir->widget(iIndex))->header()->setClickable(true);

	// refresh dir content according to new header
	RefreshContent(iIndex);
} // RefreshHeader

// set new path for current dir view
void cPanel::SetPath(const QString &qsPath)
{
	// remove old path from watcher
	qfswWatcher.removePath(qhTabs.value(qswDir->currentIndex()).swWidgets->qsPath);
	qhTabs.value(qswDir->currentIndex()).swWidgets->qsPath = QDir::cleanPath(qsPath);
	// add new path to watcher
	qfswWatcher.addPath(qhTabs.value(qswDir->currentIndex()).swWidgets->qsPath);
	qhLastPaths.insert(qcbDrive->currentText(), qhTabs.value(qswDir->currentIndex()).swWidgets->qsPath = QDir::cleanPath(qsPath));
	RefreshContent(qswDir->currentIndex());
} // SetPath

// sort dir content and show
void cPanel::Sort(const int &iIndex)
{
	int iColumnName, iI;
	QList<QTreeWidgetItem *> qlDirectories, qlFiles;

	// clear tree (can't use QTreeWidget::clear because it deletes objects too)
	while (static_cast<cTreeWidget *>(qswDir->widget(iIndex))->topLevelItemCount() > 0) {
		static_cast<cTreeWidget *>(qswDir->widget(iIndex))->takeTopLevelItem(0);
	} // while

	// split directories and files
	QHashIterator<QTreeWidgetItem *, QFileInfo> qhiFiles(*qhTabs.value(iIndex).qhFiles);
	while (qhiFiles.hasNext()) {
		qhiFiles.next();
		if (qhiFiles.value().isDir()) {
			qlDirectories.append(qhiFiles.key());
		} else {
			qlFiles.append(qhiFiles.key());
		} // if else
	} // while

	// sort at first by name if possible (to have sorted the rest by filename as second condition)
	iColumnName = GetNativeColumnIndex(qsNAME, iIndex);
	if (iColumnName != -1) {
		ssSort.iSortedColumn = iColumnName;
		ssSort.soSortOrder = Qt::AscendingOrder;
		qStableSort(qlDirectories.begin(), qlDirectories.end(), &cPanel::TreeSortByString);
		for (iI = 0; iI < qlDirectories.count(); iI++) {
			if (qlDirectories.at(iI)->text(ssSort.iSortedColumn) == "..") {
				qlDirectories.move(iI, 0);
				break;
			} // if
		} // for
		qStableSort(qlFiles.begin(), qlFiles.end(), &cPanel::TreeSortByString);
	} // if

	// set sort informations for sorting functions
	ssSort.iSortedColumn = static_cast<cTreeWidget *>(qswDir->widget(iIndex))->sortColumn();
	ssSort.soSortOrder = static_cast<cTreeWidget *>(qswDir->widget(iIndex))->header()->sortIndicatorOrder();
	if ((qhTabs.value(iIndex).qlColumns->at(ssSort.iSortedColumn).qsIdentifier == qsNAME && ssSort.soSortOrder == Qt::DescendingOrder) || qhTabs.value(iIndex).qlColumns->at(ssSort.iSortedColumn).qsIdentifier == qsEXTENSION || qhTabs.value(iIndex).qlColumns->at(ssSort.iSortedColumn).qsPlugin != qsNO) {
		// let ".." be the first if possible
		if (qhTabs.value(iIndex).qhFiles->value(qlDirectories.front()).fileName() == "..") {
			qStableSort(++qlDirectories.begin(), qlDirectories.end(), &cPanel::TreeSortByString);
		} else {
			qStableSort(qlDirectories.begin(), qlDirectories.end(), &cPanel::TreeSortByString);
		} // if else
		qStableSort(qlFiles.begin(), qlFiles.end(), &cPanel::TreeSortByString);
	} // if

	// show the result
	static_cast<cTreeWidget *>(qswDir->widget(iIndex))->addTopLevelItems(qlDirectories);
	static_cast<cTreeWidget *>(qswDir->widget(iIndex))->addTopLevelItems(qlFiles);
} // Sort

// compare items by text
bool cPanel::TreeSortByString(const QTreeWidgetItem *qtwiItem1, const QTreeWidgetItem *qtwiItem2)
{
	if ((qtwiItem1->text(ssSort.iSortedColumn) < qtwiItem2->text(ssSort.iSortedColumn) && ssSort.soSortOrder == Qt::AscendingOrder) || (qtwiItem1->text(ssSort.iSortedColumn) <= qtwiItem2->text(ssSort.iSortedColumn) && ssSort.soSortOrder == Qt::DescendingOrder)) {
		if (ssSort.soSortOrder == Qt::AscendingOrder) {
			return true;
		} else {
			return false;
		} // if else
	} else {
		if (ssSort.soSortOrder == Qt::AscendingOrder) {
			return false;
		} else {
			return true;
		} // if else
	} // if else
} // TreeSortByString