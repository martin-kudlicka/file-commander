#include "MainWindow.h"

#include <QtGui/QHeaderView>
#include <QtGui/QDirModel>
#include "OptionsDialog.h"

const QString qsFULL_SCREEN = "FullScreen";
const QString qsMAXIMIZED = "Maximized";

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
} // AssignShortcuts

// creates main window
cMainWindow::cMainWindow()
{
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

	// file operations class initizalization
	//cfoFileOperation = new cFileOperation(this, qhblBackgroundOperations, &csSettings);

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

	// create panels
	// TODO cMainWindow create panels

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

	// variables initialization
	qcDirModel.setModel(new QDirModel(&qcDirModel));
	qcbCommand->setCompleter(&qcDirModel);

	// shortcuts
	AssignShortcuts();

	// connections

	//ActualizeDrives();
	// show before change drive dialog can appear
	show();
	// load settings
	// left tabs
	//LoadTabs(cSettings::PositionLeft);
	// right tabs
	//LoadTabs(cSettings::PositionRight);

	// automatic actualizations
	//connect(&qtTimer, SIGNAL(timeout()), SLOT(on_qtTimer_timeout()));
	//qtTimer.start(iTIMER_INTERVAL);

	// set focus to left panel
	//static_cast<cTreeWidget *>(qswLeft->currentWidget())->setFocus(Qt::OtherFocusReason);
} // cMainWindow

// options are selected
const void cMainWindow::on_qaOptions_triggered(bool checked /* false */)
{
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
		//cpLeft->RefreshAllContents();
		//cpRight->RefreshAllContents();
	} // if
	if (qfToDo & cOptionsDialog::RefreshHeader) {
		//cpLeft->RefreshAllHeaders();
		//cpRight->RefreshAllHeaders();
	} // if
	if (qfToDo & cOptionsDialog::RefreshTabs) {
		//cpLeft->RefreshTabs();
		//cpRight->RefreshTabs();
	} // if
	if (qfToDo & cOptionsDialog::RefreshFavouriteDirectories) {
		//ActualizeFavouriteDirectories();
	} // if
	if (qfToDo & cOptionsDialog::RefreshColumnSets) {
		//ActualizeColumnSets();
	} // if
	if (qfToDo & cOptionsDialog::ShowHideDirectoryViewHeader) {
		//cpLeft->ShowHideHeaders();
		//cpRight->ShowHideHeaders();
	} // if
} // on_qaOptions_triggered

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
