#include "Panel.h"

#include "Panel/TreeWidget.h"
#include <QtGui/QHeaderView>

QStackedWidget *cPanel::qswLastActive;	///< last active panel (static class variable)

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

// add new tab with dir view
const int cPanel::AddTab(cSettings::sTabInfo &stiTabInfo, const bool &bStartUp /* false */)
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

	// set tab properties
	stTab.swWidgets.qsDrive = stiTabInfo.qsDrive;
	stTab.swWidgets.qsPath = stiTabInfo.qsPath;
	stTab.qsColumnSet = stiTabInfo.qsColumnSet;

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
	//SetTabText(iIndex);

	// set sorting
	ctwTree->header()->setSortIndicator(stiTabInfo.ssSort.iSortedColumn, stiTabInfo.ssSort.soSortOrder);

	// set header, refresh content only in first added tab
	RefreshHeader(iIndex, qlTabs.count() == 1);

	// connect signals to slots
	//connect(ctwTree->header(), SIGNAL(sectionClicked(int)), SLOT(on_qhvTreeHeader_sectionClicked(int)));

	HideOrShowTabBar();

	return iIndex;
} // AddTab

// constructor
cPanel::cPanel(QStackedWidget *qswDirs, QComboBox *qcbDrive, QLabel *qlDriveInfo, QTabBar *qtbTab, QLabel *qlPath, QLabel *qlSelected, cSettings *csSettings, cContentPlugin *ccpContentPlugin, QLabel *qlGlobalPath, QComboBox *qcbCommand, cFileControl *cfcFileControl, QLineEdit *qleQuickSearch)
{
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

	// automatic actualizations
	connect(&qtTimer, SIGNAL(timeout()), SLOT(on_qtTimer_timeout()));
	qtTimer.start(iTIMER_INTERVAL);
} // cPanel

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
const void cPanel::on_qtTimer_timeout() const
{
	ActualizeDrives();
} // on_qtTimer_timeout

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

// refresh column's header
const void cPanel::RefreshHeader(const int &iIndex, const bool &bContent /* false */)
{
	// TODO RefreshHeader
	bool bAutoStretch;
	int iI;
	QStringList qslColumns;
	QTreeWidgetItem *qtwiHeader;
	sTab *stTab;

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
	static_cast<cTreeWidget *>(qswDirs->widget(iIndex))->setColumnCount(stTab->qlColumns.count());
	static_cast<cTreeWidget *>(qswDirs->widget(iIndex))->setHeaderItem(qtwiHeader);

	// customize header
	// autostretch last section
	bAutoStretch = true;
	for (iI = 0; iI < stTab->qlColumns.count() - 1; iI++) {
		if (stTab->qlColumns.at(iI).iWidth == 0) {
			bAutoStretch = false;
			break;
		} // if
	} // for
	static_cast<cTreeWidget *>(qswDirs->widget(iIndex))->header()->setStretchLastSection(bAutoStretch);
	// set columns width
	for (iI = 0; iI < stTab->qlColumns.count(); iI++) {
		if (stTab->qlColumns.at(iI).iWidth == 0) {
			static_cast<cTreeWidget *>(qswDirs->widget(iIndex))->header()->setResizeMode(iI, QHeaderView::Stretch);
		} else {
			static_cast<cTreeWidget *>(qswDirs->widget(iIndex))->header()->resizeSection(iI, stTab->qlColumns.at(iI).iWidth);
		} // if else
	} // for

	if (bContent) {
		// refresh dir content according to new header
		//RefreshContent(iIndex);
	} // if
} // RefreshHeader

// show or hide header for specified tab
const void cPanel::ShowHideHeader(const int &iTabIndex) const
{
	static_cast<cTreeWidget *>(qswDirs->widget(iTabIndex))->header()->setVisible(csSettings->GetShowDirectoryViewHeader());
} // ShowHideHeader
