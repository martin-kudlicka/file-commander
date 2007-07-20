#include "MainWindow.h"

#include <QtGui/QHeaderView>
#include <QtGui/QDirModel>
#include "OptionsDialog.h"
#include "Common/About.h"
#include "Panel/TreeWidget.h"

const QString qsFULL_SCREEN = "FullScreen";
const QString qsMAXIMIZED = "Maximized";

// destructor
cMainWindow::~cMainWindow()
{
	if (csSettings.GetSaveSettingsOnQuit()) {
		SaveSettings();
	} // if

	cpRight->deleteLater();
	cpLeft->deleteLater();
	cfcFileControl->deleteLater();
	delete cpPlugins;
} // ~cMainWindow

// actualize column sets submenu
const void cMainWindow::ActualizeColumnSets()
{
	int iI;
	QString qsActiveColumnSet;
	QStringList qslColumnSets;

	qsActiveColumnSet = cpSource->GetColumnSet();
	qagColumnSets->actions().clear();
	qmColumnSets.clear();
	qslColumnSets = csSettings.GetColumnSets();

	for (iI = 0; iI < qslColumnSets.count(); iI++) {
		QAction *qaColumnSet;
		QString *qsColumnSet;

		qsColumnSet = &qslColumnSets[iI];

		qaColumnSet = qmColumnSets.addAction(*qsColumnSet);
		// put action to action group to prevent checking more than one action in a time
		qagColumnSets->addAction(qaColumnSet);
		qaColumnSet->setCheckable(true);
		if (qsActiveColumnSet == *qsColumnSet) {
			qaColumnSet->setChecked(true);
		} // if
	} // for
} // ActualizeColumnSets

// assign shortcuts
const void cMainWindow::AssignShortcuts()
{
	// tabs
	qaTabBarDuplicateTab->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::PanelsCategory, qsSHORTCUT__PANELS__TABS__DUPLICATE_TAB)));
	qaTabBarCloseTab->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::PanelsCategory, qsSHORTCUT__PANELS__TABS__CLOSE_TAB)));
	qaTabBarCloseAllOtherTabs->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::PanelsCategory, qsSHORTCUT__PANELS__TABS__CLOSE_ALL_OTHER_TABS)));
	// directory view
	qsLeftDrive = new QShortcut(QKeySequence(csSettings.GetShortcut(cSettings::PanelsCategory, qsSHORTCUT__PANELS__DIRECTORY_VIEW__DRIVE_LEFT)), this);
	qsRightDrive = new QShortcut(QKeySequence(csSettings.GetShortcut(cSettings::PanelsCategory, qsSHORTCUT__PANELS__DIRECTORY_VIEW__DRIVE_RIGHT)), this);
	// main buttons
	qpbTerminal->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::PanelsCategory, qsSHORTCUT__PANELS__MAIN_BUTTON__TERMINAL)));
	qpbTerminal->setText(csSettings.GetShortcut(cSettings::PanelsCategory, qsSHORTCUT__PANELS__MAIN_BUTTON__TERMINAL) + ' ' + qpbTerminal->text());
	qpbView->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::PanelsCategory, qsSHORTCUT__PANELS__MAIN_BUTTON__VIEW)));
	qpbView->setText(csSettings.GetShortcut(cSettings::PanelsCategory, qsSHORTCUT__PANELS__MAIN_BUTTON__VIEW) + ' ' + qpbView->text());
	qpbEdit->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::PanelsCategory, qsSHORTCUT__PANELS__MAIN_BUTTON__EDIT)));
	qpbEdit->setText(csSettings.GetShortcut(cSettings::PanelsCategory, qsSHORTCUT__PANELS__MAIN_BUTTON__EDIT) + ' ' + qpbEdit->text());
	qpbCopy->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::PanelsCategory, qsSHORTCUT__PANELS__MAIN_BUTTON__COPY)));
	qpbCopy->setText(csSettings.GetShortcut(cSettings::PanelsCategory, qsSHORTCUT__PANELS__MAIN_BUTTON__COPY) + ' ' + qpbCopy->text());
	qpbMove->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::PanelsCategory, qsSHORTCUT__PANELS__MAIN_BUTTON__MOVE)));
	qpbMove->setText(csSettings.GetShortcut(cSettings::PanelsCategory, qsSHORTCUT__PANELS__MAIN_BUTTON__MOVE) + ' ' + qpbMove->text());
	qpbNewDirectory->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::PanelsCategory, qsSHORTCUT__PANELS__MAIN_BUTTON__NEW_DIRECTORY)));
	qpbNewDirectory->setText(csSettings.GetShortcut(cSettings::PanelsCategory, qsSHORTCUT__PANELS__MAIN_BUTTON__NEW_DIRECTORY) + ' ' + qpbNewDirectory->text());
	qpbDelete->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::PanelsCategory, qsSHORTCUT__PANELS__MAIN_BUTTON__DELETE)));
	qpbDelete->setText(csSettings.GetShortcut(cSettings::PanelsCategory, qsSHORTCUT__PANELS__MAIN_BUTTON__DELETE) + ' ' + qpbDelete->text());
	// main menu
	qaUnpackSelectedFiles->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::MainMenuCategory, qsSHORTCUT__MAIN_MENU__FILE__UNPACK_SELECTED_FILES)));
	qaQuit->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::MainMenuCategory, qsSHORTCUT__MAIN_MENU__FILE__QUIT)));
	qaSelectGroup->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::MainMenuCategory, qsSHORTCUT__MAIN_MENU__MARK__SELECT_GROUP)));
	qaUnselectGroup->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::MainMenuCategory, qsSHORTCUT__MAIN_MENU__MARK__UNSELECT_GROUP)));
	qaSelectAll->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::MainMenuCategory, qsSHORTCUT__MAIN_MENU__MARK__SELECT_ALL)));
	qaUnselectAll->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::MainMenuCategory, qsSHORTCUT__MAIN_MENU__MARK__UNSELECT_ALL)));
	qaInvertSelection->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::MainMenuCategory, qsSHORTCUT__MAIN_MENU__MARK__INVERT_SELECTION)));
	qaCompareDirectories->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::MainMenuCategory, qsSHORTCUT__MAIN_MENU__MARK__COMPARE_DIRECTORIES)));
	qaSearch->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::MainMenuCategory, qsSHORTCUT__MAIN_MENU__COMMANDS__SEARCH)));
	qaFavouriteDirectories->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::MainMenuCategory, qsSHORTCUT__MAIN_MENU__COMMANDS__FAVOURITE_DIRECTORIES)));
	qaBranchView->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::MainMenuCategory, qsSHORTCUT__MAIN_MENU__COMMANDS__BRANCH_VIEW)));
	qaFullScreen->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::MainMenuCategory, qsSHORTCUT__MAIN_MENU__COMMANDS__FULLSCREEN)));
	qaReverseOrder->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::MainMenuCategory, qsSHORTCUT__MAIN_MENU__PANEL__REVERSE_ORDER)));
	qaRefresh->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::MainMenuCategory, qsSHORTCUT__MAIN_MENU__PANEL__REFRESH)));
	qaOptions->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::MainMenuCategory, qsSHORTCUT__MAIN_MENU__CONFIGURATION__OPTIONS)));
	qaSavePosition->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::MainMenuCategory, qsSHORTCUT__MAIN_MENU__CONFIGURATION__SAVE_POSITION)));
	qaSaveSettings->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::MainMenuCategory, qsSHORTCUT__MAIN_MENU__CONFIGURATION__SAVE_SETTINGS)));
	// sort by
	if (qagSortBy->actions().count() > 0) {
		SetSortByActions();
	} // if
} // AssignShortcuts

// creates main window
cMainWindow::cMainWindow()
{
	// TODO cMainWindow
	cSettings::sMainWindowState smwsState;
	QVBoxLayout *qvblTabBar;

	// load plugins
	cpPlugins = new cPlugins(&csSettings);
	cpPlugins->Load();

	// setup GUI
	setupUi(this);

	// main window parameters
	smwsState = csSettings.GetWindowState();
	if (smwsState.iHeight != 0) {
		setGeometry(geometry().x(), geometry().y(), smwsState.iWidth, smwsState.iHeight);
	} // if
	if (smwsState.qsWindowState == qsMAXIMIZED) {
		setWindowState(Qt::WindowMaximized);
	} else {
		if (smwsState.qsWindowState == qsFULL_SCREEN) {
			setWindowState(Qt::WindowFullScreen);
		} // if
	} // if else

	// add left tab
	qtbLeft.setContextMenuPolicy(Qt::CustomContextMenu);
	connect(&qtbLeft, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(on_qtbLeft_customContextMenuRequested(const QPoint &)));
	qvblTabBar = static_cast<QVBoxLayout *>(qswLeft->parentWidget()->layout());
	qvblTabBar->insertWidget(iTAB_POS, &qtbLeft);
	qtbLeft.setFocusPolicy(Qt::NoFocus);
	// add right tab
	qtbRight.setContextMenuPolicy(Qt::CustomContextMenu);
	connect(&qtbRight, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(on_qtbRight_customContextMenuRequested(const QPoint &)));
	qvblTabBar = static_cast<QVBoxLayout *>(qswRight->parentWidget()->layout());
	qvblTabBar->insertWidget(iTAB_POS, &qtbRight);
	qtbRight.setFocusPolicy(Qt::NoFocus);

	// remove default widgets
	qswLeft->removeWidget(qswLeft->widget(0));
	qswRight->removeWidget(qswRight->widget(0));

	// background operations layout
	qhblBackgroundOperations = new QHBoxLayout(qpbCopy->parentWidget()->layout()->parentWidget());
	static_cast<QVBoxLayout *>(qpbCopy->parentWidget()->layout()->parentWidget()->layout())->insertLayout(0, qhblBackgroundOperations);
	qhblBackgroundOperations->addStretch();

	// drive combo boxes preparation
	qtwLeftDrives = new QTreeView(qcbLeftDrive);
	qtwLeftDrives->setRootIsDecorated(false);
	qtwLeftDrives->header()->hide();
	qtwLeftDrives->setModel(qcbLeftDrive->model());
	qcbLeftDrive->setView(qtwLeftDrives);
	qtwRightDrives = new QTreeView(qcbRightDrive);
	qtwRightDrives->setRootIsDecorated(false);
	qtwRightDrives->header()->hide();
	qtwRightDrives->setModel(qcbRightDrive->model());
	qcbRightDrive->setView(qtwRightDrives);

	// file control class initizalization
	cfcFileControl = new cFileControl(this, qhblBackgroundOperations, &csSettings, cpPlugins->ccpContentPlugin);

	// create panels
	cpLeft = new cPanel(qswLeft, qcbLeftDrive, qlLeftDriveInfo, &qtbLeft, qlLeftPath, qlLeftSelected, &csSettings, cpPlugins->ccpContentPlugin, qlGlobalPath, qcbCommand, cfcFileControl, qleLeftQuickSearch);
	cpRight = new cPanel(qswRight, qcbRightDrive, qlRightDriveInfo, &qtbRight, qlRightPath, qlRightSelected, &csSettings, cpPlugins->ccpContentPlugin, qlGlobalPath, qcbCommand, cfcFileControl, qleRightQuickSearch);

	// quick searches
	qleLeftQuickSearch->hide();
	qleRightQuickSearch->hide();

	// create tab context menu
	qaTabBarDuplicateTab = qmTabBar.addAction(tr("&Duplicate tab"));
	addAction(qaTabBarDuplicateTab);
	qmTabBar.addSeparator();
	qaTabBarCloseTab = qmTabBar.addAction(tr("&Close tab"));
	addAction(qaTabBarCloseTab);
	qaTabBarCloseAllOtherTabs = qmTabBar.addAction(tr("Close &all other tabs"));
	addAction(qaTabBarCloseAllOtherTabs);

	// column sets submenu
	qaColumnSet->setMenu(&qmColumnSets);

	// variables initialization
	qagSortBy = new QActionGroup(this);
	qagColumnSets = new QActionGroup(this);
	qcDirModel.setModel(new QDirModel(&qcDirModel));
	qcbCommand->setCompleter(&qcDirModel);

	// shortcuts
	AssignShortcuts();

	// connections
	connect(cpLeft, SIGNAL(GotFocus()), SLOT(on_cpLeft_GotFocus()));
	/*connect(cpLeft, SIGNAL(Delete()), SLOT(on_cpPanel_Delete()));
	connect(cpLeft, SIGNAL(CopyArchiveFiles()), SLOT(on_cpPanel_CopyArchiveFiles()));*/
	connect(cpRight, SIGNAL(GotFocus()), SLOT(on_cpRight_GotFocus()));
	/*connect(cpRight, SIGNAL(Delete()), SLOT(on_cpPanel_Delete()));
	connect(cpRight, SIGNAL(CopyArchiveFiles()), SLOT(on_cpPanel_CopyArchiveFiles()));
	connect(qsLeftDrive, SIGNAL(activated()), SLOT(on_qsLeftDrive_activated()));
	connect(qsRightDrive, SIGNAL(activated()), SLOT(on_qsRightDrive_activated()));
	connect(qsHistoryBack, SIGNAL(activated()), SLOT(on_qsHistoryBack_activated()));
	connect(qsHistoryFront, SIGNAL(activated()), SLOT(on_qsHistoryFront_activated()));
	connect(qaTabBarDuplicateTab, SIGNAL(triggered(bool)), SLOT(on_qaTabBarDuplicateTab_triggered(bool)));
	connect(qaTabBarCloseTab, SIGNAL(triggered(bool)), SLOT(on_qaTabBarCloseTab_triggered(bool)));
	connect(qaTabBarCloseAllOtherTabs, SIGNAL(triggered(bool)), SLOT(on_qaTabBarCloseAllOtherTabs_triggered(bool)));*/
	connect(qagSortBy, SIGNAL(triggered(QAction *)), SLOT(on_qagSortBy_triggered(QAction *)));
	/*connect(&qmFavouriteDirectories, SIGNAL(triggered(QAction *)), SLOT(on_qmFavouriteDirectories_triggered(QAction *)));
	connect(&qmLeftHistoryDirectoryList, SIGNAL(aboutToShow()), SLOT(on_qmLeftHistoryDirectoryList_aboutToShow()));
	connect(&qmRightHistoryDirectoryList, SIGNAL(aboutToShow()), SLOT(on_qmRightHistoryDirectoryList_aboutToShow()));
	connect(&qmLeftHistoryDirectoryList, SIGNAL(triggered(QAction *)), SLOT(on_qmLeftHistoryDirectoryList_triggered(QAction *)));
	connect(&qmRightHistoryDirectoryList, SIGNAL(triggered(QAction *)), SLOT(on_qmRightHistoryDirectoryList_triggered(QAction *)));*/
	connect(&qmColumnSets, SIGNAL(triggered(QAction *)), SLOT(on_qmColumnSets_triggered(QAction *)));

	// show before change drive dialog can appear
	show();

	// load tabs
	LoadTabs(cSettings::PositionLeft);
	LoadTabs(cSettings::PositionRight);

	// set focus to left panel
	static_cast<cTreeWidget *>(qswLeft->currentWidget())->setFocus(Qt::OtherFocusReason);
} // cMainWindow

// load tabs from qsSettings
const void cMainWindow::LoadTabs(const cSettings::ePosition &epPosition)
{
	int iI;
	QStringList qslTabs;

	// get tabs
	qslTabs = csSettings.GetTabs(epPosition);

	if (qslTabs.isEmpty()) {
		// no tabs created yet -> create one default in settings file
		cSettings::sTabInfo stiTab;
		QPair<QString, cFileControl::sDrive> qpDrive;

		qpDrive = cfcFileControl->GetFirstDrive();

		stiTab.qsColumnSet = qsFULL;
		stiTab.qsPath = qpDrive.second.qsPath;
		stiTab.qsDrive = qpDrive.first;
		stiTab.ssSort.iSortedColumn = 1;
		stiTab.ssSort.soSortOrder = Qt::AscendingOrder;
		csSettings.CreateTab(epPosition, 0, stiTab);
		qslTabs.append("0");	// add created tab
	} // if

	// create tabs
	for (iI = 0; iI < qslTabs.count(); iI++) {
		cSettings::sTabInfo stiTabInfo;

		stiTabInfo = csSettings.GetTabInfo(epPosition, qslTabs.at(iI));
		if (epPosition == cSettings::PositionLeft) {
			cpLeft->AddTab(stiTabInfo, true);
		} else {
			cpRight->AddTab(stiTabInfo, true);
		} // if else
	} // for
} // LoadTabs

// left panel got focus
const void cMainWindow::on_cpLeft_GotFocus()
{
	cpSource = cpLeft;
	cpDestination = cpRight;
	SetSortByActions();
	ActualizeColumnSets();
} // on_cpLeft_GotFocus

// right panel got focus
const void cMainWindow::on_cpRight_GotFocus()
{
	cpSource = cpRight;
	cpDestination = cpLeft;
	SetSortByActions();
	ActualizeColumnSets();
} // on_cpRight_GotFocus

// about is selected
const void cMainWindow::on_qaAbout_triggered(bool checked /* false */)
{
	cAbout caAbout(this, qsAPPLICATION, qsVERSION);

	caAbout.exec();
} // on_qaAbout_triggered

// sort by action called
const void cMainWindow::on_qagSortBy_triggered(QAction *action) const
{
	int iSortBy;

	// find column to sort by
	for (iSortBy = 0; iSortBy < qagSortBy->actions().count(); iSortBy++) {
		if (qagSortBy->actions().at(iSortBy) == action) {
			break;
		} // if
	} // for

	cpSource->SortBy(iSortBy);
} // on_qagSortBy_triggered

// options are selected
const void cMainWindow::on_qaOptions_triggered(bool checked /* false */)
{
	// TODO on_qaOptions_triggered
	cOptionsDialog codOptions(this, &csSettings, cpPlugins->ccpContentPlugin);
	QFlags<cOptionsDialog::eToDo> qfToDo;

	qfToDo = static_cast<QFlags<cOptionsDialog::eToDo> >(codOptions.exec());

	if (qfToDo & cOptionsDialog::ReassignShortcuts) {
		retranslateUi(this);
		AssignShortcuts();
	} // if
	if (qfToDo & cOptionsDialog::ReloadPlugins) {
		cpPlugins->Unload();
		cpPlugins->Load();
	} // if
	if (qfToDo & cOptionsDialog::RefreshContent) {
		cpLeft->RefreshAllContents();
		cpRight->RefreshAllContents();
	} // if
	if (qfToDo & cOptionsDialog::RefreshHeader) {
		cpLeft->RefreshAllHeaders();
		cpRight->RefreshAllHeaders();
	} // if
	if (qfToDo & cOptionsDialog::RefreshTabs) {
		cpLeft->RefreshTabs();
		cpRight->RefreshTabs();
	} // if
	if (qfToDo & cOptionsDialog::RefreshFavouriteDirectories) {
		//ActualizeFavouriteDirectories();
	} // if
	if (qfToDo & cOptionsDialog::RefreshColumnSets) {
		ActualizeColumnSets();
	} // if
	if (qfToDo & cOptionsDialog::ShowHideDirectoryViewHeader) {
		cpLeft->ShowHideHeaders();
		cpRight->ShowHideHeaders();
	} // if
} // on_qaOptions_triggered

// selected column set from column set submenu
const void cMainWindow::on_qmColumnSets_triggered(QAction *action) const
{
	cpSource->SetColumnSet(action->text());
} // on_qmColumnSets_triggered

// context menu of left tab bar
const void cMainWindow::on_qtbLeft_customContextMenuRequested(const QPoint &pos) const
{
	// TODO on_qtbLeft_customContextMenuRequested
	//TabBarShowContextMenu(cSettings::PositionLeft, pos);
} // on_qtbLeft_customContextMenuRequested

// context menu of right tab bar
const void cMainWindow::on_qtbRight_customContextMenuRequested(const QPoint &pos) const
{
	// TODO on_qtbRight_customContextMenuRequested
	//TabBarShowContextMenu(cSettings::PositionRight, pos);
} // on_qtbRight_customContextMenuRequested

// save dir view settings
const void cMainWindow::SaveSettings() const
{
	// TODO SaveSettings
	//cpLeft->SaveSettings(cSettings::PositionLeft);
	//cpRight->SaveSettings(cSettings::PositionRight);
} // SaveSettings

// set sort by actions for sorting columns
const void cMainWindow::SetSortByActions() const
{
	int iI;
	QList<cSettings::sColumn> qlColumns;

	// clear previous actions
	for (iI = 0; iI < qagSortBy->actions().count(); iI++) {
		qagSortBy->actions().at(iI)->deleteLater();
	} // for
	qagSortBy->actions().clear();

	qlColumns = cpSource->GetColumns();

	for (iI = 0; iI < qlColumns.count(); iI++) {
		QAction *qaSortBy;

		qaSortBy = new QAction(qlColumns.at(iI).qsName, qagSortBy);

		// set shortcuts
		switch (iI) {
			case 0:
				qaSortBy->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::MainMenuCategory, qsSHORTCUT__MAIN_MENU__PANEL__SORT_BY_FIRST_COLUMN)));
				break;
			case 1:
				qaSortBy->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::MainMenuCategory, qsSHORTCUT__MAIN_MENU__PANEL__SORT_BY_SECOND_COLUMN)));
				break;
			case 2:
				qaSortBy->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::MainMenuCategory, qsSHORTCUT__MAIN_MENU__PANEL__SORT_BY_THIRD_COLUMN)));
				break;
			case 3:
				qaSortBy->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::MainMenuCategory, qsSHORTCUT__MAIN_MENU__PANEL__SORT_BY_FOURTH_COLUMN)));
		} // switch
	} // for

	qmPanel->insertActions(qaColumnsSeparator, qagSortBy->actions());
} // SetSortByActions