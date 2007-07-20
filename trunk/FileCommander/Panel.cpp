#include "Panel.h"

#include "Panel/TreeWidget.h"
#include <QtGui/QHeaderView>
#include <QtCore/QDateTime>
#include <QtGui/QMessageBox>

QStackedWidget *cPanel::qswLastActive;	///< last active panel (static class variable)
cSettings::sSort cPanel::ssSort;			///< sort information (static class variable)

// drive list actualization
const void cPanel::ActualizeDrives() const
{
	QList<QPair<QString, cFileControl::sDrive> > qlDrives;

	qlDrives = cfcFileControl->GetDrives();

	if (qcbDrive->count() != qlDrives.count()) {
		// assume drives have changed
		int iI;
		QString qsDrive;

		qsDrive = qcbDrive->currentText();
		qcbDrive->blockSignals(true);

		for (iI = 0; iI < qlDrives.count(); iI++) {
			qcbDrive->addItem(qlDrives.at(iI).first);
		} // for

		// check for selected drive change, changes are handled in Panel class
		if (qcbDrive->findText(qsDrive) == -1) {
			// selected drive changed
			qcbDrive->blockSignals(false);
			qcbDrive->setCurrentIndex(-1);
		} else {
			// selected drive not changed
			qcbDrive->setCurrentIndex(qcbDrive->findText(qsDrive));
			qcbDrive->blockSignals(false);
		} // if else
	} // if
} // ActualizeDrives

// actualize volume information - disk name and space
const void cPanel::ActualizeVolumeInfo()
{
	if (!qlTabs.isEmpty() && qlTabs.value(qswDirs->currentIndex()).bValid) {
		cFileSystem::sDiskSpace sdsInfo;
		QString qsName;
		sTab *stTab;

		stTab = &qlTabs[qswDirs->currentIndex()];

		qsName = stTab->cfsFileSystem->GetVolumeName();
		sdsInfo = stTab->cfsFileSystem->GetDiskSpace();
		qlDriveInfo->setText(tr("[%1] %2 of %3 free").arg(qsName).arg(GetSizeString(sdsInfo.qi64Free)).arg(GetSizeString(sdsInfo.qi64Total)));
	} // if
} // ActualizeVolumeInfo

// actualize widgets with info about current directory view
const void cPanel::ActualizeWidgets()
{
	sTab *stTab;

	stTab = &qlTabs[qswDirs->currentIndex()];

	SetTabText(qswDirs->currentIndex());

	ActualizeVolumeInfo();
	qlPath->setText(stTab->swWidgets.qsPath);
	qlSelected->setText(stTab->swWidgets.qsSelected);
} // ActualizeWidgets

// add new tab with dir view
const int cPanel::AddTab(const cSettings::sTabInfo &stiTabInfo, const bool &bStartUp /* false */)
{
	// TODO AddTab
	cTreeWidget *ctwTree;
	int /*iI, */iIndex;
	sTab stTab;

	// create tab
	ctwTree = new cTreeWidget();
	ctwTree->setRootIsDecorated(false);
	ctwTree->setContextMenuPolicy(Qt::CustomContextMenu);
	ctwTree->setSelectionMode(QAbstractItemView::ExtendedSelection);
	ctwTree->header()->setSortIndicatorShown(true);
	ctwTree->header()->setClickable(true);
	if (bStartUp || !csSettings->GetOpenNewTabNextToCurrentTab()) {
		iIndex = qswDirs->addWidget(ctwTree);
	} else {
		iIndex = qswDirs->insertWidget(qswDirs->currentIndex() + 1, ctwTree);
	} // if else

	/*// connect signals to slots
	connect(ctwTree, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(on_ctwTree_customContextMenuRequested(const QPoint &)));
	connect(ctwTree, SIGNAL(itemActivated(QTreeWidgetItem *, int)), SLOT(on_ctwTree_itemActivated(QTreeWidgetItem *, int)));
	connect(ctwTree, SIGNAL(itemSelectionChanged(const cTreeWidget *)), SLOT(on_ctwTree_itemSelectionChanged(const cTreeWidget *)));
	connect(ctwTree, SIGNAL(KeyPressed(QKeyEvent *, QTreeWidgetItem *)), SLOT(on_ctwTree_KeyPressed(QKeyEvent *, QTreeWidgetItem *)));*/
	connect(ctwTree, SIGNAL(GotFocus()), SLOT(on_ctwTree_GotFocus()));
	/*connect(ctwTree, SIGNAL(DropEvent(const cTreeWidget::eDropAction &, const QList<QUrl> &, const QString &, const QString &, QTreeWidgetItem *)), SLOT(on_ctwTree_DropEvent(const cTreeWidget::eDropAction &, const QList<QUrl> &, const QString &, const QString &, QTreeWidgetItem *)));
	connect(ctwTree, SIGNAL(DragEvent()), SLOT(on_ctwTree_DragEvent()));
	connect(ctwTree, SIGNAL(MoveEvent(QTreeWidgetItem *)), SLOT(on_ctwTree_MoveEvent(QTreeWidgetItem *)));*/

	// drive letter in combo
	qcbDrive->blockSignals(true);
	qcbDrive->setCurrentIndex(qcbDrive->findText(stiTabInfo.qsDrive));
	qcbDrive->blockSignals(false);

	// set tab properties
	stTab.swWidgets.qsPath = stiTabInfo.qsPath;
	stTab.qsColumnSet = stiTabInfo.qsColumnSet;
	stTab.cfsFileSystem = cfcFileControl->GetFileSystem(stiTabInfo.qsDrive, stiTabInfo.qsPath);

	/*// history
	stTab.shHistory.iPosition = stiTabInfo.shHistory.iPosition;
	for (iI = 0; iI < stiTabInfo.shHistory.qlLastPaths.count(); iI++) {
		sLastPath slpPanel;
		cSettings::sLastPath *slpSetting;

		slpSetting = &stiTabInfo.shHistory.qlLastPaths[iI];

		slpPanel.qsShow = slpSetting->qsShow;
		slpPanel.qsLocalDirectory = slpSetting->qsLocalDirectory;
		slpPanel.qsArchive = slpSetting->qsArchive;
		slpPanel.qsPathInArchive = slpSetting->qsPathInArchive;
		if (slpSetting->qsLocation == qsLOCAL_DIRECTORY) {
			slpPanel.elLocation = LocalDirectory;
		} else {
			slpPanel.elLocation = Archive;
		} // if else

		stTab.shHistory.qlLastPaths.append(slpPanel);
	} // for*/

	qlTabs.insert(iIndex, stTab);

	// add new tab into GUI
	qtbTab->blockSignals(true);
	qtbTab->insertTab(iIndex, QString());
	qtbTab->blockSignals(false);
	SetTabText(iIndex);

	// set sorting
	ctwTree->header()->setSortIndicator(stiTabInfo.ssSort.iSortedColumn, stiTabInfo.ssSort.soSortOrder);

	// set header, refresh content only in first added tab
	RefreshHeader(iIndex);

	// connect other signals to slots
	//connect(ctwTree->header(), SIGNAL(sectionClicked(int)), SLOT(on_qhvTreeHeader_sectionClicked(int)));
	ConnectFileSystem(stTab.cfsFileSystem);

	// connection for file system created later -> need to call for the first time explicitly
	on_cfsFileSystem_ContentChanged(stTab.cfsFileSystem);

	HideOrShowTabBar();

	return iIndex;
} // AddTab

// close all other tabs than selected
const void cPanel::CloseAllOtherTabs(const int &iTabIndex)
{
	if (qlTabs.count() > 1) {
		bool bCloseAllOtherTabs;

		if (csSettings->GetConfirmCloseOfAllTabs()) {
			if (QMessageBox::question(qmwParent, tr("Close all other tabs"), tr("Continue?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
				bCloseAllOtherTabs = true;
			} else {
				bCloseAllOtherTabs = false;
			} // if else
		} else {
			bCloseAllOtherTabs = true;
		} // if else

		if (bCloseAllOtherTabs) {
			int iI;

			for (iI = qlTabs.count() - 1; iI >= 0; iI--) {
				if (iI != iTabIndex) {
					CloseTab(iI);
				} // if
			} // for
		} // if
	} // if
} // CloseAllOtherTabs

// close tab
const void cPanel::CloseTab(const int &iTabIndex)
{
	if (qlTabs.count() > 1) {
		qlTabs.at(iTabIndex).cfsFileSystem->deleteLater();
		qlTabs.removeAt(iTabIndex);
		qswDirs->removeWidget(qswDirs->widget(iTabIndex));
		qtbTab->removeTab(iTabIndex);

		HideOrShowTabBar();
		ActualizeWidgets();

		static_cast<cTreeWidget *>(qswDirs->currentWidget())->setFocus(Qt::OtherFocusReason);
	} // if
} // CloseTab

// connect new file system's signals/slots
const void cPanel::ConnectFileSystem(const cFileSystem *cfsFileSystem) const
{
	connect(cfsFileSystem, SIGNAL(ContentChanged(const cFileSystem *)), SLOT(on_cfsFileSystem_ContentChanged(const cFileSystem *)));
	connect(cfsFileSystem, SIGNAL(GotColumnValue(const cContentPluginDelayed::sOutput &)), SLOT(on_cfsFileSystem_GotColumnValue(const cContentPluginDelayed::sOutput &)));
	connect(cfsFileSystem, SIGNAL(Unaccessible()), SLOT(on_cfsFileSystem_Unaccessible()));
} // ConnectFileSystem

// constructor
cPanel::cPanel(QMainWindow *qmwParent, QStackedWidget *qswDirs, QComboBox *qcbDrive, QLabel *qlDriveInfo, QTabBar *qtbTab, QLabel *qlPath, QLabel *qlSelected, cSettings *csSettings, cContentPlugin *ccpContentPlugin, QLabel *qlGlobalPath, QComboBox *qcbCommand, cFileControl *cfcFileControl, QLineEdit *qleQuickSearch)
{
	this->qmwParent = qmwParent;
	this->qswDirs = qswDirs;
	this->qcbDrive = qcbDrive;
	this->qlDriveInfo = qlDriveInfo;
	this->qtbTab = qtbTab;
	this->qlPath = qlPath;
	this->qlSelected = qlSelected;
	this->csSettings = csSettings;
	this->ccpContentPlugin = ccpContentPlugin;
	this->qlGlobalPath = qlGlobalPath;
	this->qcbCommand = qcbCommand;
	this->cfcFileControl = cfcFileControl;
	this->qleQuickSearch = qleQuickSearch;

	ActualizeDrives();

	// automatic actualizations
	connect(&qtTimer, SIGNAL(timeout()), SLOT(on_qtTimer_timeout()));
	qtTimer.start(iTIMER_INTERVAL);
} // cPanel

// create new tab by duplicate one
const int cPanel::DuplicateTab(const int &iTabIndex)
{
	cTreeWidget *ctwDir;
	sTab *stTab;
	cSettings::sTabInfo stiTabInfo;

	ctwDir = static_cast<cTreeWidget *>(qswDirs->widget(iTabIndex));
	stTab = &qlTabs[iTabIndex];

	stiTabInfo.qsColumnSet = stTab->qsColumnSet;
	stiTabInfo.qsDrive = qcbDrive->currentText();
	stiTabInfo.qsPath = stTab->cfsFileSystem->GetPath();
	stiTabInfo.ssSort.iSortedColumn = ctwDir->sortColumn();
	stiTabInfo.ssSort.soSortOrder = ctwDir->header()->sortIndicatorOrder();

	return AddTab(stiTabInfo);
} // DuplicateTab

// columns for current dir view
const QList<cSettings::sColumn> cPanel::GetColumns() const
{
	return qlTabs.value(qswDirs->currentIndex()).qlColumns;
} // GetColumns

//< column set for current directory view
const QString cPanel::GetColumnSet() const
{
	return qlTabs.value(qswDirs->currentIndex()).qsColumnSet;
} // GetColumnSet

// convert QDateTime to user defined format
const QString cPanel::GetDateTimeString(const QDateTime &qdtDateTime) const
{
	QString qsDateTime;
	
	qsDateTime = csSettings->GetDateTimeDisplay();
	
	qsDateTime.replace("%d", QString("%1").arg(qdtDateTime.date().day(), 2, 10, QChar('0')));
	qsDateTime.replace("%o", QString("%1").arg(qdtDateTime.date().month(), 2, 10, QChar('0')));
	qsDateTime.replace("%y", QString("%1").arg(qdtDateTime.date().year()));
	qsDateTime.replace("%h", QString("%1").arg(qdtDateTime.time().hour(), 2, 10, QChar('0')));
	qsDateTime.replace("%m", QString("%1").arg(qdtDateTime.time().minute(), 2, 10, QChar('0')));
	qsDateTime.replace("%s", QString("%1").arg(qdtDateTime.time().second(), 2, 10, QChar('0')));
	
	return qsDateTime;
} // GetDateTimeString

// find index of native column
const int cPanel::GetNativeColumnIndex(const QString &qsColumn, const int &iTabIndex)
{
	int iI;
	sTab *stTab;

	stTab = &qlTabs[iTabIndex];

	for (iI = 0; iI < stTab->qlColumns.count(); iI++) {
		if (stTab->qlColumns.at(iI).qsIdentifier == qsColumn) {
			return iI;
		}
	} // for

	return -1;
} // GetNativeColumnIndex

// "convert" size to string according to setting in options
const QString cPanel::GetSizeString(const qint64 &qi64Size) const
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

// find out tab index in tab bar
const int cPanel::GetTabIndex(const QPoint &qpPos) const
{
	int iI;

	for (iI = 0; iI < qtbTab->count(); iI++) {
		if (qtbTab->tabRect(iI).contains(qpPos)) {
			return iI;
		} // if
	} // for

	return -1;
} // GetTabIndex

// get available files in tree view
QList<QTreeWidgetItem *> cPanel::GetTreeWidgetItems() const
{
	cTreeWidget *ctwDir;
	int iI;
	QList<QTreeWidgetItem *> qlItems;

	ctwDir = ctwDir = static_cast<cTreeWidget *>(qswDirs->currentWidget());

	for (iI = 0; iI < ctwDir->topLevelItemCount(); iI++) {
		qlItems.append(ctwDir->topLevelItem(iI));
	} // for

	return qlItems;
} // GetTreeWidgetItems

// hide or show tab bar as set in options
const void cPanel::HideOrShowTabBar() const
{
	if (csSettings->GetShowTabBarWithOnlyOneTab()) {
		qtbTab->show();
	} else {
		if (qlTabs.count() == 1) {
			qtbTab->hide();
		} else {
			qtbTab->show();
		} // if else
	} // if else
} // HideOrShowTabBar

// directory content changed for filesystem
const void cPanel::on_cfsFileSystem_ContentChanged(const cFileSystem *cfsFileSystem)
{
	int iI;
	sTab *stTab;

	for (iI = 0; iI < qlTabs.count(); iI++) {
		stTab = &qlTabs[iI];

		if (stTab->cfsFileSystem == cfsFileSystem) {
			break;
		} // if
	} // for

	// refresh content
	if (iI == qswDirs->currentIndex()) {
		RefreshContent(iI);
	} else {
		stTab->bValid = false;
	} // if

	// actualize widgets
	stTab->swWidgets.qsPath = cfsFileSystem->GetPath();
	ActualizeWidgets();
} // on_cfsFileSystem_ContentChanged

// got column value from content plugin
const void cPanel::on_cfsFileSystem_GotColumnValue(const cContentPluginDelayed::sOutput &soOutput) const
{
	soOutput.qtwiItem->setText(soOutput.iColumn, soOutput.qsValue);
} // on_cfsFileSystem_GotColumnValue

// file system unacessible
const void cPanel::on_cfsFileSystem_Unaccessible() const
{
	// TODO on_cfsFileSystem_Unaccessible - change drive dialog
} // on_cfsFileSystem_Unaccessible

// dir view got focus
const void cPanel::on_ctwTree_GotFocus()
{
	if (qswLastActive != qswDirs) {
		qswLastActive = qswDirs;
		qlGlobalPath->setText(qlTabs.at(qswDirs->currentIndex()).swWidgets.qsPath);
		emit GotFocus();
	} // if
} // on_ctwTree_GotFocus

// timer's action
const void cPanel::on_qtTimer_timeout()
{
	ActualizeDrives();
	ActualizeVolumeInfo();
} // on_qtTimer_timeout

// refresh all dir view contents
const void cPanel::RefreshAllContents()
{
	int iI;

	for (iI = 0; iI < qlTabs.count(); iI++) {
		if (qswDirs->currentIndex() == iI) {
			RefreshContent(iI);
		} else {
			qlTabs[iI].bValid = false;
		} // if else
	} // for
} // RefreshAllContents

// refresh all dir view headers
const void cPanel::RefreshAllHeaders()
{
	int iI;

	for (iI = 0; iI < qlTabs.count(); iI++) {
		if (qswDirs->currentIndex() == iI) {
			RefreshHeader(iI, true);
		} else {
			RefreshHeader(iI);
		} // if else
	} // for
} // RefreshAllHeaders

// refresh dir content
const void cPanel::RefreshContent(const int &iIndex)
{
	// TODO RefreshContent
	cTreeWidget *ctwDir;
	int iI;
	QList<QTreeWidgetItem *> qlFiles;
	sTab *stTab;

	ctwDir = static_cast<cTreeWidget *>(qswDirs->widget(iIndex));
	stTab = &qlTabs[iIndex];

	qlFiles = qlTabs.at(iIndex).cfsFileSystem->GetDirectoryContent();

	// fill tree widget items
	for (iI = 0; iI < qlFiles.count(); iI++) {
		int iJ;
		QTreeWidgetItem *qtwiFile;

		qtwiFile = qlFiles.at(iI);
		for (iJ = 0; iJ < stTab->qlColumns.count(); iJ++) {
			cSettings::sColumn *scColumn;

			scColumn = &stTab->qlColumns[iJ];
			if (scColumn->qsPlugin == qsNO) {
				if (scColumn->qsIdentifier == qsICON) {
					qtwiFile->setIcon(iJ, stTab->cfsFileSystem->GetFileIcon(qtwiFile));
				} else {
					if (scColumn->qsIdentifier == qsNAME) {
						qtwiFile->setText(iJ, stTab->cfsFileSystem->GetFileName(qtwiFile));
					} else {
						if (scColumn->qsIdentifier == qsEXTENSION) {
							qtwiFile->setText(iJ, stTab->cfsFileSystem->GetFileExtension(qtwiFile));
						} else {
							if (scColumn->qsIdentifier == qsNAME_WITH_EXTENSION) {
								QString qsNameWithExtension, qsExtension;

								qsNameWithExtension = stTab->cfsFileSystem->GetFileName(qtwiFile);
								qsExtension = stTab->cfsFileSystem->GetFileExtension(qtwiFile);
								if (!qsExtension.isEmpty()) {
									qsNameWithExtension += '.' + qsExtension;
								} // if
								qtwiFile->setText(iJ, qsNameWithExtension);
							} else {
								if (scColumn->qsIdentifier == qsSIZE) {
									if (stTab->cfsFileSystem->IsDir(qtwiFile)) {
										qtwiFile->setText(iJ, tr("<DIR>"));
									} else {
										qint64 qi64Size;

										qi64Size = stTab->cfsFileSystem->GetFileSize(qtwiFile); 
										qtwiFile->setText(iJ, GetSizeString(qi64Size));
										qtwiFile->setData(iJ, Qt::UserRole, qi64Size);
									} // if else
								} else {
									if (scColumn->qsIdentifier == qsDATE_TIME) {
										QDateTime qdtLastModified;

										qdtLastModified = stTab->cfsFileSystem->GetLastModified(qtwiFile);
										qtwiFile->setText(iJ, GetDateTimeString(qdtLastModified));
										qtwiFile->setData(iJ, Qt::UserRole, qdtLastModified);
									}
#ifdef Q_WS_WIN
									else {
										qtwiFile->setText(iJ, stTab->cfsFileSystem->GetFileAttr(qtwiFile));
									} // if else
#endif
								} // if else
							} // if else
						} // if else
					} // if else
				} // if else
			} else {
				// plugin
				cFileSystem::sContentPluginRequest scprContent;

				scprContent.qtwiFile = qtwiFile;
				scprContent.qsPlugin = scColumn->qsPlugin;
				scprContent.qsColumn = scColumn->qsIdentifier;
				scprContent.qsUnit = scColumn->qsUnit;
				scprContent.iColumn = iJ;

				qtwiFile->setText(iJ, stTab->cfsFileSystem->GetContentPluginValue(scprContent));
			} // if else
		} // for
	} // for
	
	// sort and show files
	Sort(iIndex, qlFiles);
	stTab->bValid = true;

	if (ctwDir->topLevelItemCount() > 0) {
		// focus to the first item
		ctwDir->setCurrentItem(ctwDir->topLevelItem(0));
	} // if

	stTab->cfsFileSystem->RetreiveContentDelayedValues();

	//AddHistory(iIndex);
} // RefreshContent

// refresh column's header
const void cPanel::RefreshHeader(const int &iIndex, const bool &bContent /* false */)
{
	bool bAutoStretch;
	cTreeWidget *ctwDir;
	int iI;
	QStringList qslColumns;
	QTreeWidgetItem *qtwiHeader;
	sTab *stTab;

	ctwDir = static_cast<cTreeWidget *>(qswDirs->widget(iIndex));
	stTab = &qlTabs[iIndex];

	// invalidate content of tab
	stTab->bValid = false;

	// show/hide header
	ShowHideHeader(iIndex);

	// clear previous header contents and fill new information
	stTab->qlColumns.clear();
	qslColumns = csSettings->GetColumnsInSet(stTab->qsColumnSet);
	// check for column count in current column set
	if (qslColumns.isEmpty() && stTab->qsColumnSet != qsFULL) {
		// no columns - switch to full column set
		stTab->qsColumnSet = qsFULL;
		qslColumns = csSettings->GetColumnsInSet(stTab->qsColumnSet);
	} // if
	if (qslColumns.isEmpty() && stTab->qsColumnSet == qsFULL) {
		// create default column set (Full) if not created yet or empty
		csSettings->CreateDefaultColumnSet();
		qslColumns = csSettings->GetColumnsInSet(stTab->qsColumnSet);
	} // if
	for (iI = 0; iI < qslColumns.count(); iI++) {
		cSettings::sColumn scColumn;

		scColumn = csSettings->GetColumnInfo(stTab->qsColumnSet, qslColumns.at(iI));
		// test if is native or plugin and loaded
		if (scColumn.qsPlugin == qsNO || ccpContentPlugin->Loaded(scColumn.qsPlugin)) {
			// ok -> add to column list
			stTab->qlColumns.append(scColumn);
		} // if
	} // for

	// set header according to qlColumns list
	qtwiHeader = new QTreeWidgetItem();
	for (iI = 0; iI < stTab->qlColumns.count(); iI++) {
		qtwiHeader->setText(iI, stTab->qlColumns.at(iI).qsName);
	} // for
	ctwDir->setColumnCount(stTab->qlColumns.count());
	ctwDir->setHeaderItem(qtwiHeader);

	// customize header
	// autostretch last section
	bAutoStretch = true;
	for (iI = 0; iI < stTab->qlColumns.count() - 1; iI++) {
		if (stTab->qlColumns.at(iI).iWidth == 0) {
			bAutoStretch = false;
			break;
		} // if
	} // for
	ctwDir->header()->setStretchLastSection(bAutoStretch);
	// set columns width
	for (iI = 0; iI < stTab->qlColumns.count(); iI++) {
		if (stTab->qlColumns.at(iI).iWidth == 0) {
			ctwDir->header()->setResizeMode(iI, QHeaderView::Stretch);
		} else {
			ctwDir->header()->resizeSection(iI, stTab->qlColumns.at(iI).iWidth);
		} // if else
	} // for

	if (bContent) {
		// refresh dir content according to new header
		RefreshContent(iIndex);
	} // if
} // RefreshHeader

// refresh tabs
const void cPanel::RefreshTabs() const
{
	int iI;

	for (iI = 0; iI < qlTabs.count(); iI++) {
		SetTabText(iI);
	} // for

	HideOrShowTabBar();
} // RefreshTabs

// selected another column set for actual directory view
const void cPanel::SetColumnSet(const QString &qsColumnSet)
{
	qlTabs[qswDirs->currentIndex()].qsColumnSet = qsColumnSet;
	// change sort column to prevent sorting by nonexisting column number
	static_cast<cTreeWidget *>(qswDirs->currentWidget())->header()->setSortIndicator(0, Qt::AscendingOrder);
	// refresh header to show changes
	RefreshHeader(qswDirs->currentIndex(), true);
} // SetColumnSet

const void cPanel::SetPath(const QString &qsPath)
{
	cFileControl::sPathInfo spiPathInfo;
	int iIndex;

	spiPathInfo = cfcFileControl->GetPathInfo(qsPath);

	iIndex = qcbDrive->findText(spiPathInfo.qsDrive);
	if (iIndex != -1) {
		// drive exists -> proceed with set path
		sTab *stTab;

		stTab = &qlTabs[qswDirs->currentIndex()];

		// change drive
		qcbDrive->blockSignals(true);
		qcbDrive->setCurrentIndex(iIndex);
		qcbDrive->blockSignals(false);

		// possible change of file system
		if (cfcFileControl->ChangeFileSystem(stTab->cfsFileSystem, spiPathInfo.qsDrive, qsPath)) {
			ConnectFileSystem(stTab->cfsFileSystem);
		} // if

		// set path
		stTab->cfsFileSystem->SetPath(spiPathInfo.qsDrive, spiPathInfo.qsRootPath, qsPath);
	} // if
} // SetPath

// set text in tab bar
const void cPanel::SetTabText(const int &iTabIndex) const
{
	qtbTab->setTabText(iTabIndex, qlTabs.at(iTabIndex).cfsFileSystem->GetTabText());
} // SetTabText

// show or hide header for specified tab
const void cPanel::ShowHideHeader(const int &iTabIndex) const
{
	static_cast<cTreeWidget *>(qswDirs->widget(iTabIndex))->header()->setVisible(csSettings->GetShowDirectoryViewHeader());
} // ShowHideHeader

// show or hide headers in all tabs
const void cPanel::ShowHideHeaders() const
{
	int iI;

	for (iI = 0; iI < qlTabs.count(); iI++) {
		ShowHideHeader(iI);
	} // for
} // ShowHideHeaders

// sort dir content and show
const void cPanel::Sort(const int &iIndex, QList<QTreeWidgetItem *> &qlToSort)
{
	cSettings::sColumn *scSortedColumn;
	cTreeWidget *ctwDir;
	int iColumnName, iI;
	QList<QTreeWidgetItem *> qlDirectories, qlFiles, qlMarked;
	sTab *stTab;

	ctwDir = static_cast<cTreeWidget *>(qswDirs->widget(iIndex));
	stTab = &qlTabs[iIndex];

	// remember marked items and clear tree
	while (ctwDir->topLevelItemCount() > 0) {
		if (ctwDir->topLevelItem(0)->isSelected()) {
			qlMarked.append(ctwDir->takeTopLevelItem(0));
		} else {
			ctwDir->takeTopLevelItem(0);
		} // if else
	} // while

	// split into directories and files
	for (iI = 0; iI < qlToSort.count(); iI++) {
		QTreeWidgetItem *qtwiFile;

		qtwiFile = qlToSort[iI];
		if (stTab->cfsFileSystem->IsDir(qtwiFile)) {
			qlDirectories.append(qtwiFile);
		} else {
			qlFiles.append(qtwiFile);
		} // if else
	} // for

	// sort at first by name if possible (to have sorted the rest by filename as second condition)
	iColumnName = GetNativeColumnIndex(qsNAME, iIndex);
	if (iColumnName == -1) {
		// or by name with extension
		iColumnName = GetNativeColumnIndex(qsNAME_WITH_EXTENSION, iIndex);
	} // if
	if (iColumnName != -1) {
		ssSort.iSortedColumn = iColumnName;
		ssSort.soSortOrder = Qt::AscendingOrder;
		ssSort.bCaseSensitive = csSettings->GetCaseSensitiveSorting();
		qStableSort(qlDirectories.begin(), qlDirectories.end(), &cPanel::TreeSortByString);
		qStableSort(qlFiles.begin(), qlFiles.end(), &cPanel::TreeSortByString);
	} // if

	// set sort informations for sorting functions
	ssSort.iSortedColumn = ctwDir->sortColumn();
	ssSort.soSortOrder = ctwDir->header()->sortIndicatorOrder();
	ssSort.bCaseSensitive = csSettings->GetCaseSensitiveSorting();
	scSortedColumn = &stTab->qlColumns[ssSort.iSortedColumn];
	if ((scSortedColumn->qsIdentifier == qsNAME && ssSort.soSortOrder == Qt::DescendingOrder)
		 || scSortedColumn->qsIdentifier == qsEXTENSION
		 || (scSortedColumn->qsIdentifier == qsNAME_WITH_EXTENSION && ssSort.soSortOrder == Qt::DescendingOrder)
		 || scSortedColumn->qsIdentifier == qsATTRIBUTES
		 || scSortedColumn->qsPlugin != qsNO) {
		// string sort
		qStableSort(qlDirectories.begin(), qlDirectories.end(), &cPanel::TreeSortByString);
		qStableSort(qlFiles.begin(), qlFiles.end(), &cPanel::TreeSortByString);
	} else {
		if (scSortedColumn->qsIdentifier == qsSIZE) {
			// qint64 sort
			qStableSort(qlDirectories.begin(), qlDirectories.end(), &cPanel::TreeSortByQInt64);
			qStableSort(qlFiles.begin(), qlFiles.end(), &cPanel::TreeSortByQInt64);
		} else {
			if (scSortedColumn->qsIdentifier == qsDATE_TIME) {
				// QDateTime sort
				qStableSort(qlDirectories.begin(), qlDirectories.end(), &cPanel::TreeSortByQDateTime);
				qStableSort(qlFiles.begin(), qlFiles.end(), &cPanel::TreeSortByQDateTime);
			} // if else
		} // if else
	} // if else

	// move ".." directory to the first position
	for (iI = 0; iI < qlDirectories.count(); iI++) {
		bool bFound;

		bFound = false;
		if (stTab->cfsFileSystem->GetFileName(qlDirectories.at(iI), false) == "..") {
			bFound = true;
		} // if

		if (bFound) {
			qlDirectories.move(iI, 0);
			break;
		} // if
	} // for

	// show the result
	ctwDir->addTopLevelItems(qlDirectories);
	ctwDir->addTopLevelItems(qlFiles);

	// mark items again
	for (iI = 0; iI < qlMarked.count(); iI++) {
		qlMarked.at(iI)->setSelected(true);
	} // for
} // Sort

// sort by specified column
const void cPanel::SortBy(const int &iColumn)
{
	cTreeWidget *ctwDir;
	Qt::SortOrder soSortOrder;

	ctwDir = static_cast<cTreeWidget *>(qswDirs->currentWidget());

	// change sort order
	if (ctwDir->header()->sortIndicatorSection() == iColumn) {
		soSortOrder = ctwDir->header()->sortIndicatorOrder();
		if (soSortOrder == Qt::AscendingOrder) {
			soSortOrder = Qt::DescendingOrder;
		} else {
			soSortOrder = Qt::AscendingOrder;
		} // if else
	} else {
		soSortOrder = Qt::AscendingOrder;
	} // if else
	ctwDir->header()->setSortIndicator(iColumn, soSortOrder);

	// sort again
	Sort(qswDirs->currentIndex(), GetTreeWidgetItems());
} // SortBy

// compare items by QDateTime
const bool cPanel::TreeSortByQDateTime(const QTreeWidgetItem *qtwiItem1, const QTreeWidgetItem *qtwiItem2)
{
	QDateTime qdtItem1, qdtItem2;

	qdtItem1 = qtwiItem1->data(ssSort.iSortedColumn, Qt::UserRole).toDateTime();
	qdtItem2 = qtwiItem2->data(ssSort.iSortedColumn, Qt::UserRole).toDateTime();

	if ((qdtItem1 < qdtItem2 && ssSort.soSortOrder == Qt::AscendingOrder) || (qdtItem1 <= qdtItem2 && ssSort.soSortOrder == Qt::DescendingOrder)) {
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
} // TreeSortByQDateTime

// compare items by qint64
const bool cPanel::TreeSortByQInt64(const QTreeWidgetItem *qtwiItem1, const QTreeWidgetItem *qtwiItem2)
{
	qint64 qi64Item1, qi64Item2;

	qi64Item1 = qtwiItem1->data(ssSort.iSortedColumn, Qt::UserRole).toLongLong();
	qi64Item2 = qtwiItem2->data(ssSort.iSortedColumn, Qt::UserRole).toLongLong();

	if ((qi64Item1 < qi64Item2 && ssSort.soSortOrder == Qt::AscendingOrder) || (qi64Item1 <= qi64Item2 && ssSort.soSortOrder == Qt::DescendingOrder)) {
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
} // TreeSortByQInt64

// compare items by text
const bool cPanel::TreeSortByString(const QTreeWidgetItem *qtwiItem1, const QTreeWidgetItem *qtwiItem2)
{
	QString qsItem1, qsItem2;

	if (ssSort.bCaseSensitive) {
		qsItem1 = qtwiItem1->text(ssSort.iSortedColumn);
		qsItem2 = qtwiItem2->text(ssSort.iSortedColumn);
	} else {
		qsItem1 = qtwiItem1->text(ssSort.iSortedColumn).toLower();
		qsItem2 = qtwiItem2->text(ssSort.iSortedColumn).toLower();
	} // if else

	if ((qsItem1 < qsItem2 && ssSort.soSortOrder == Qt::AscendingOrder) || (qsItem1 <= qsItem2 && ssSort.soSortOrder == Qt::DescendingOrder)) {
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