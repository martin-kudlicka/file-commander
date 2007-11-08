#include "MainWindow.h"

#include <QtGui/QHeaderView>
#include <QtGui/QDirModel>
#include "Common/About.h"
#include "Panel/TreeWidget.h"
#include "FileControl/Process.h"
#include <QtGui/QKeyEvent>
#include "FindFilesDialog.h"

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
	delete cfcFileControl;
	qtwLeftDrives->deleteLater();
	qtwRightDrives->deleteLater();
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
		const QString *qsColumnSet;

		qsColumnSet = &qslColumnSets.at(iI);

		qaColumnSet = qmColumnSets.addAction(*qsColumnSet);
		// put action to action group to prevent checking more than one action in a time
		qagColumnSets->addAction(qaColumnSet);
		qaColumnSet->setCheckable(true);
		if (qsActiveColumnSet == *qsColumnSet) {
			qaColumnSet->setChecked(true);
		} // if
	} // for
} // ActualizeColumnSets

// actualize favourite directories context menu
const void cMainWindow::ActualizeFavouriteDirectories()
{
	qhFavouriteDirectories.clear();
	qmFavouriteDirectories.clear();

	FillFavouriteDirectories(&qmFavouriteDirectories, csSettings.GetFavouriteDirectories());
} // ActualizeFavouriteDirectories

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
	qsHistoryBack = new QShortcut(QKeySequence(csSettings.GetShortcut(cSettings::PanelsCategory, qsSHORTCUT__PANELS__DIRECTORY_VIEW__HISTORY_BACK)), this);
	qsHistoryFront = new QShortcut(QKeySequence(csSettings.GetShortcut(cSettings::PanelsCategory, qsSHORTCUT__PANELS__DIRECTORY_VIEW__HISTORY_FRONT)), this);
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
	qaPack->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::MainMenuCategory, qsSHORTCUT__MAIN_MENU__FILE__PACK)));
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
	qaHistoryDirectoryList->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::MainMenuCategory, qsSHORTCUT__MAIN_MENU__COMMANDS__HISTORY_DIRECTORY_LIST)));
	qaBranchView->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::MainMenuCategory, qsSHORTCUT__MAIN_MENU__COMMANDS__BRANCH_VIEW)));
	qaTargetAsSource->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::MainMenuCategory, qsSHORTCUT__MAIN_MENU__COMMANDS__TARGET_AS_SOURCE)));
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
	cSettings::sMainWindowState smwsState;
	QPalette qpLineEdit;
	QVBoxLayout *qvblTabBar;

	// load plugins
	cpPlugins = new cPlugins(&csSettings);
	cpPlugins->Load();

	// setup GUI
	setupUi(this);

	// path's line edits palette change
	qpLineEdit = qleLeftPath->palette();
	qpLineEdit.setColor(QPalette::Base, qlLeftDriveInfo->palette().color(QPalette::Window));
	qleLeftPath->setPalette(qpLineEdit);
	qleRightPath->setPalette(qpLineEdit);

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
	cfcFileControl = new cFileControl(this, qhblBackgroundOperations, &csSettings, cpPlugins->ccpContentPlugin, cpPlugins->clpListerPlugin, cpPlugins->cppPackerPlugin);

	// create panels
	cpLeft = new cPanel(this, qswLeft, qcbLeftDrive, qlLeftDriveInfo, &qtbLeft, qleLeftPath, qlLeftSelected, &csSettings, cpPlugins->ccpContentPlugin, qlGlobalPath, qcbCommand, cfcFileControl, qleLeftQuickSearch);
	cpRight = new cPanel(this, qswRight, qcbRightDrive, qlRightDriveInfo, &qtbRight, qleRightPath, qlRightSelected, &csSettings, cpPlugins->ccpContentPlugin, qlGlobalPath, qcbCommand, cfcFileControl, qleRightQuickSearch);

	// quick searches
	qleLeftQuickSearch->hide();
	qleRightQuickSearch->hide();	

	// create tab bar context menu
	qaTabBarDuplicateTab = qmTabBar.addAction(tr("&Duplicate tab"));
	addAction(qaTabBarDuplicateTab);
	qmTabBar.addSeparator();
	qaTabBarCloseTab = qmTabBar.addAction(tr("&Close tab"));
	addAction(qaTabBarCloseTab);
	qaTabBarCloseAllOtherTabs = qmTabBar.addAction(tr("Close &all other tabs"));
	addAction(qaTabBarCloseAllOtherTabs);

	// favourites context menu
	ActualizeFavouriteDirectories();
	qpbLeftFavourites->setMenu(&qmFavouriteDirectories);
	qpbRightFavourites->setMenu(&qmFavouriteDirectories);

	// history directory list context menu
	qpbLeftHistory->setMenu(&qmLeftHistoryDirectoryList);
	qpbRightHistory->setMenu(&qmRightHistoryDirectoryList);

	// column sets submenu
	qaColumnSet->setMenu(&qmColumnSets);

	// variables initialization
	iTabBarIndex = -1;
	qagSortBy = new QActionGroup(this);
	qcbCommand->installEventFilter(this);
	qagColumnSets = new QActionGroup(this);
	qcDirModel.setModel(new QDirModel(&qcDirModel));
	qcbCommand->setCompleter(&qcDirModel);

	// shortcuts
	AssignShortcuts();

	// connections
	connect(cpLeft, SIGNAL(GotFocus(const cPanel *)), SLOT(on_cPanel_GotFocus(const cPanel *)));
	connect(cpLeft, SIGNAL(Delete()), SLOT(on_cPanel_Delete()));
	connect(cpLeft, SIGNAL(FileSystemUnaccessible(cPanel *)), SLOT(on_cPanel_FileSystemUnaccessible(cPanel *)));
	connect(cpLeft, SIGNAL(Copy()), SLOT(on_cPanel_Copy()));
	connect(cpRight, SIGNAL(GotFocus(const cPanel *)), SLOT(on_cPanel_GotFocus(const cPanel *)));
	connect(cpRight, SIGNAL(Delete()), SLOT(on_cPanel_Delete()));
	connect(cpRight, SIGNAL(FileSystemUnaccessible(cPanel *)), SLOT(on_cPanel_FileSystemUnaccessible(cPanel *)));
	connect(cpRight, SIGNAL(Copy()), SLOT(on_cPanel_Copy()));
	connect(qsLeftDrive, SIGNAL(activated()), SLOT(on_qsLeftDrive_activated()));
	connect(qsRightDrive, SIGNAL(activated()), SLOT(on_qsRightDrive_activated()));
	connect(qsHistoryBack, SIGNAL(activated()), SLOT(on_qsHistoryBack_activated()));
	connect(qsHistoryFront, SIGNAL(activated()), SLOT(on_qsHistoryFront_activated()));
	connect(qaTabBarDuplicateTab, SIGNAL(triggered(bool)), SLOT(on_qaTabBarDuplicateTab_triggered(bool)));
	connect(qaTabBarCloseTab, SIGNAL(triggered(bool)), SLOT(on_qaTabBarCloseTab_triggered(bool)));
	connect(qaTabBarCloseAllOtherTabs, SIGNAL(triggered(bool)), SLOT(on_qaTabBarCloseAllOtherTabs_triggered(bool)));
	connect(qagSortBy, SIGNAL(triggered(QAction *)), SLOT(on_qagSortBy_triggered(QAction *)));
	connect(&qmFavouriteDirectories, SIGNAL(triggered(QAction *)), SLOT(on_qmFavouriteDirectories_triggered(QAction *)));
	connect(&qmLeftHistoryDirectoryList, SIGNAL(aboutToShow()), SLOT(on_qmLeftHistoryDirectoryList_aboutToShow()));
	connect(&qmRightHistoryDirectoryList, SIGNAL(aboutToShow()), SLOT(on_qmRightHistoryDirectoryList_aboutToShow()));
	connect(&qmLeftHistoryDirectoryList, SIGNAL(triggered(QAction *)), SLOT(on_qmLeftHistoryDirectoryList_triggered(QAction *)));
	connect(&qmRightHistoryDirectoryList, SIGNAL(triggered(QAction *)), SLOT(on_qmRightHistoryDirectoryList_triggered(QAction *)));
	connect(&qmColumnSets, SIGNAL(triggered(QAction *)), SLOT(on_qmColumnSets_triggered(QAction *)));

	// show before change drive dialog can appear
	show();

	// load tabs
	LoadTabs(cSettings::PositionLeft);
	LoadTabs(cSettings::PositionRight);

	// set focus to left panel
	static_cast<cTreeWidget *>(qswLeft->currentWidget())->setFocus(Qt::OtherFocusReason);
} // cMainWindow

// drive changed in panel
const void cMainWindow::DriveIndexChanged(cPanel *cpPanel, const QString &qsDrive) const
{
	bool bChangeFileSystem;
	cFileSystem *cfsFileSystem;
	cPanel *cpOther;
	QComboBox *qcbDrive;

	if (qswLeft->currentIndex() == -1) {
		// tabs are not yet initialized
		return;
	} // if

	if (cpPanel == cpLeft) {
		cpOther = cpRight;
		qcbDrive = qcbLeftDrive;
	} else {
		cpOther = cpLeft;
		qcbDrive = qcbRightDrive;
	} // if else

	// check if drive change is neccessary
	if (qsDrive != cpOther->GetFileSystem()->GetDrive() && qsDrive == cpPanel->GetFileSystem()->GetDrive()) {
		return;
	} // if

	cfsFileSystem = cpPanel->GetFileSystem();
	if (qsDrive == cpOther->GetFileSystem()->GetDrive()) {
		bChangeFileSystem = cfcFileControl->ChangeFileSystem(&cfsFileSystem, qcbDrive, qsDrive, cpOther->GetFileSystem()->GetPath());
	} else {
		bChangeFileSystem = cfcFileControl->ChangeFileSystem(&cfsFileSystem, qcbDrive, qsDrive);
	} // if else

	if (bChangeFileSystem) {
		// TODO DriveIndexChanged connect new file system
	} // if
} // DriveIndexChanged

// event filter
bool cMainWindow::eventFilter(QObject *watched, QEvent *event)
{
	if (watched == qcbCommand) {
		if (event->type() == QEvent::KeyPress) {
			cProcess cpProcess;
			QString qsCommand;

			switch (static_cast<QKeyEvent *>(event)->key()) {
				int iCommandPos;

				case Qt::Key_Enter:
				case Qt::Key_Return:
					cpProcess.StartDetached(qcbCommand->currentText(), qlGlobalPath->text());

					qsCommand = qcbCommand->currentText();
					iCommandPos = qcbCommand->findText(qsCommand);
					if (iCommandPos > 0) {
						qcbCommand->removeItem(iCommandPos);
					} // if
					if (iCommandPos != 0) {
						qcbCommand->insertItem(0, qsCommand);
					} // if
					qcbCommand->setCurrentIndex(-1);
					qcbCommand->clearEditText();
					return true;
				case Qt::Key_Escape:
					qcbCommand->clearEditText();
					cpSource->SetFocus();
					return true;
			} // switch
		} // if
		return false;
	} else {
		return QMainWindow::eventFilter(watched, event);
	} // if else
} // eventFilter

// fill favourite directories context menu
const void cMainWindow::FillFavouriteDirectories(QMenu *qmMenu, const QList<QPair<QString, cSettings::sFavouriteDirectory> > &qlFavouriteDirectories)
{
	int iI;

	for (iI = 0; iI < qlFavouriteDirectories.count(); iI++) {
		const QPair<QString, cSettings::sFavouriteDirectory> *qpFavourite;

		qpFavourite = &qlFavouriteDirectories.at(iI);
		if (qpFavourite->second.bSubmenu) {
			QMenu *qmSubmenu;

			qmSubmenu = qmMenu->addMenu(qpFavourite->first);
			FillFavouriteDirectories(qmSubmenu, qpFavourite->second.qlChildFavourites);
		} else {
			QAction *qaFavouriteDirectory;
			cOptionsDialog::sFavouriteDirectory sfdFavouriteDirectory;

			qaFavouriteDirectory = qmMenu->addAction(qpFavourite->first);

			sfdFavouriteDirectory.qsSource = qpFavourite->second.qsSource;
			sfdFavouriteDirectory.bTarget = qpFavourite->second.bTarget;
			sfdFavouriteDirectory.qsTarget = qpFavourite->second.qsTarget;

			qhFavouriteDirectories.insert(qaFavouriteDirectory, sfdFavouriteDirectory);
		} // if else
	} // for
} // FillFavouriteDirectories

// fill history directory list for specified panel
const void cMainWindow::FillHistoryDirectoryList(const cSettings::ePosition &epPosition)
{
	cPanel *cpPanel;
	int iI;
	QMenu *qmHistoryDirectoryList;
	cPanel::sHistoryDirectoryList shdlList;

	if (epPosition == cSettings::PositionLeft) {
		cpPanel = cpLeft;
		qmHistoryDirectoryList = &qmLeftHistoryDirectoryList;
	} else {
		cpPanel = cpRight;
		qmHistoryDirectoryList = &qmRightHistoryDirectoryList;
	} // if else

	shdlList = cpPanel->GetHistoryDirectoryList();

	qmHistoryDirectoryList->clear();
	for (iI = 0; iI < shdlList.qslDirectories.count(); iI++) {
		QAction *qaDirectory;

		qaDirectory = qmHistoryDirectoryList->addAction(shdlList.qslDirectories.at(iI));
		qaDirectory->setData(iI);

		if (iI == shdlList.iPosition) {
			qaDirectory->setCheckable(true);
			qaDirectory->setChecked(true);
		} // if
	} // for
} // FillHistoryDirectoryList

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

// copy marked files
const void cMainWindow::on_cPanel_Copy() const
{
	qpbCopy->animateClick();
} // on_cPanel_Copy

// delete marked files
const void cMainWindow::on_cPanel_Delete() const
{
	qpbDelete->animateClick();
} // on_cPanel_Delete

// panel's file system unaccessible
const void cMainWindow::on_cPanel_FileSystemUnaccessible(cPanel *cpPanel) const
{
	cFileSystem *cfsFileSystem;
	QComboBox *qcbDrive;

	if (cpPanel == cpLeft) {
		qcbDrive = qcbLeftDrive;
	} else {
		qcbDrive = qcbRightDrive;
	} // if else

	cfsFileSystem = cpPanel->GetFileSystem();
	if (cfcFileControl->ChangeFileSystem(&cfsFileSystem, qcbDrive, cpPanel->GetFileSystem()->GetDrive())) {
		// TODO on_cPanel_FileSystemUnaccessible connect new file system
	} // if
} // on_cPanel_FileSystemUnaccessible

// panel got focus
const void cMainWindow::on_cPanel_GotFocus(const cPanel *cpPanel)
{
	if (cpPanel == cpLeft) {
		cpSource = cpLeft;
		cpDestination = cpRight;
	} else {
		cpSource = cpRight;
		cpDestination = cpLeft;
	} // if else
	SetSortByActions();
	ActualizeColumnSets();
} // on_cPanel_GotFocus

// about is selected
const void cMainWindow::on_qaAbout_triggered(bool checked /* false */)
{
	cAbout caAbout(this, qsAPPLICATION, qsVERSION);

	caAbout.exec();
} // on_qaAbout_triggered

// branch view is selected
const void cMainWindow::on_qaBranchView_triggered(bool checked /* false */) const
{
	cpSource->BranchView();
} // on_qaBranchView_triggered

// compare directories is selected
const void cMainWindow::on_qaCompareDirectories_triggered(bool checked /* false */) const
{
	cfcFileControl->CompareDirectories(cpLeft->GetFileSystem(), cpRight->GetFileSystem());
} // on_qaCompareDirectories_triggered

// favourite directories are selected
const void cMainWindow::on_qaFavouriteDirectories_triggered(bool checked /* false */) const
{
	if (!qhFavouriteDirectories.isEmpty()) {
		if (cpSource == cpLeft) {
			qpbLeftFavourites->showMenu();
		} else {
			qpbRightFavourites->showMenu();
		} // if else
	} // if
} // on_qaFavouriteDirectories_triggered

// full screen mode is selected
const void cMainWindow::on_qaFullScreen_triggered(bool checked /* false */)
{
	setWindowState(windowState() ^ Qt::WindowFullScreen);
} // on_qaFullScreen_triggered

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

// history directory list is selected
const void cMainWindow::on_qaHistoryDirectoryList_triggered(bool checked /* false */) const
{
	if (cpSource == cpLeft) {
		qpbLeftHistory->showMenu();
	} else {
		qpbRightHistory->showMenu();
	} // if else
} // on_qaHistoryDirectoryList_triggered

// invert selection is selected
const void cMainWindow::on_qaInvertSelection_triggered(bool checked /* false */) const
{
	cpSource->InvertSelection();
} // on_qaInvertSelection_triggered

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
		ActualizeFavouriteDirectories();
	} // if
	if (qfToDo & cOptionsDialog::RefreshColumnSets) {
		ActualizeColumnSets();
	} // if
	if (qfToDo & cOptionsDialog::ShowHideDirectoryViewHeader) {
		cpLeft->ShowHideHeaders();
		cpRight->ShowHideHeaders();
	} // if
} // on_qaOptions_triggered

///< refresh is selected
const void cMainWindow::on_qaRefresh_triggered(bool checked /* false */) const
{
	cpSource->RefreshContent();
} // on_qaRefresh_triggered

// reverse order is selected
const void cMainWindow::on_qaReverseOrder_triggered(bool checked /* false */) const
{
	cpSource->ReverseOrder();
} // on_qaReverseOrder_triggered

// save position is selected
const void cMainWindow::on_qaSavePosition_triggered(bool checked /* false */)
{
	cSettings::sMainWindowState smwsState;

	smwsState.iHeight = height();
	smwsState.iWidth = width();

	if (!windowState()) {
		smwsState.qsWindowState = qsNORMAL;
	} else {
		if (windowState() & Qt::WindowMaximized) {
			smwsState.qsWindowState = qsMAXIMIZED;
		} else {
			smwsState.qsWindowState = qsFULL_SCREEN;
		} // if else
	} // if else

	csSettings.SetWindowState(smwsState);
} // on_qaSavePosition_triggered

// save settings is selected
const void cMainWindow::on_qaSaveSettings_triggered(bool checked /* false */) const
{
	SaveSettings();
} // on_qaSaveSettings_triggered

// search selected
const void cMainWindow::on_qaSearch_triggered(bool checked /* false */)
{
	cFindFilesDialog cffdFind(this, cpSource, cfcFileControl, &csSettings, cpPlugins->clpListerPlugin);

	cffdFind.exec();
} // on_qaSearch_triggered

// select all selected
const void cMainWindow::on_qaSelectAll_triggered(bool checked /* false */) const
{
	cpSource->SelectAll();
} // on_qaSelectAll_triggered

// select group selected
const void cMainWindow::on_qaSelectGroup_triggered(bool checked /* false */) const
{
	cpSource->Select(cSelectFilesDialog::Select, cpPlugins->clpListerPlugin);
} // on_qaSelectGroup_triggered

// close all other tabs called
const void cMainWindow::on_qaTabBarCloseAllOtherTabs_triggered(bool checked /* false */)
{
	if (iTabBarIndex == -1) {
		// called by shortcut
		if (qswLeft->currentWidget()->hasFocus()) {
			cpTabBarAction = cpLeft;
			iTabBarIndex = qtbLeft.currentIndex();
		} else {
			cpTabBarAction = cpRight;
			iTabBarIndex = qtbRight.currentIndex();
		} // if else
	} // if

	cpTabBarAction->CloseAllOtherTabs(iTabBarIndex);
	iTabBarIndex = -1;
} // on_qaTabBarCloseAllOtherTabs_triggered

// close tab called
const void cMainWindow::on_qaTabBarCloseTab_triggered(bool checked /* false */)
{
	if (iTabBarIndex == -1) {
		// called by shortcut
		if (qswLeft->currentWidget()->hasFocus()) {
			cpTabBarAction = cpLeft;
			iTabBarIndex = qtbLeft.currentIndex();
		} else {
			cpTabBarAction = cpRight;
			iTabBarIndex = qtbRight.currentIndex();
		} // if else
	} // if

	cpTabBarAction->CloseTab(iTabBarIndex);
	iTabBarIndex = -1;
} // on_qaTabBarCloseTab_triggered

// duplicate tab called
const void cMainWindow::on_qaTabBarDuplicateTab_triggered(bool checked /* false */)
{
	bool bForeground;
	int iNewTab;

	if (iTabBarIndex == -1) {
		// called by shortcut
		if (qswLeft->currentWidget()->hasFocus()) {
			cpTabBarAction = cpLeft;
			iTabBarIndex = qtbLeft.currentIndex();
		} else {
			cpTabBarAction = cpRight;
			iTabBarIndex = qtbRight.currentIndex();
		} // if else

		if (csSettings.GetNewTabByShortcutInForeground()) {
			bForeground = true;
		} else {
			bForeground = false;
		} // if else
	} else {
		bForeground = false;
	} // if else

	iNewTab = cpTabBarAction->DuplicateTab(iTabBarIndex);
	iTabBarIndex = -1;

	if (bForeground) {
		if (cpTabBarAction == cpLeft) {
			qtbLeft.setCurrentIndex(iNewTab);
		} else {
			qtbRight.setCurrentIndex(iNewTab);
		} // if else
	} // if
} // on_qaTabBarDuplicateTab_triggered

// set target panel as source
const void cMainWindow::on_qaTargetAsSource_triggered(bool checked /* false */) const
{
	cpDestination->SetPath(cpSource->GetFileSystem()->GetPath());
} // on_qaTargetAsSource_triggered

// unpack selected files chosen
const void cMainWindow::on_qaUnpackSelectedFiles_triggered(bool checked /* false */) const
{
	cfcFileControl->UnpackSelectedFiles(cpSource->GetFileSystem(), cpSource->GetSelectedFiles(), cpDestination->GetPath());
} // on_qaUnpackSelectedFiles_triggered

// unselect all selected
const void cMainWindow::on_qaUnselectAll_triggered(bool checked /* false */) const
{
	cpSource->UnselectAll();
} // on_qaUnselectAll_triggered

// unselect group selected
const void cMainWindow::on_qaUnselectGroup_triggered(bool checked /* false */) const
{
	cpSource->Select(cSelectFilesDialog::Unselect, cpPlugins->clpListerPlugin);
} // on_qaUnselectGroup_triggered

// selected drive in left panel changes
const void cMainWindow::on_qcbLeftDrive_currentIndexChanged(int index) const
{
	DriveIndexChanged(cpLeft, qcbLeftDrive->currentText());
} // on_qcbLeftDrive_currentIndexChanged

// selected drive in right panel changes
const void cMainWindow::on_qcbRightDrive_currentIndexChanged(int index) const
{
	DriveIndexChanged(cpRight, qcbRightDrive->currentText());
} // on_qcbRightDrive_currentIndexChanged

// selected column set from column set submenu
const void cMainWindow::on_qmColumnSets_triggered(QAction *action) const
{
	cpSource->SetColumnSet(action->text());
} // on_qmColumnSets_triggered

// selected favourite directory from favourites context menu
const void cMainWindow::on_qmFavouriteDirectories_triggered(QAction *action) const
{
	cPanel *cpFavouriteDestination, *cpFavouriteSource;

	if (epFavourite == cSettings::PositionLeft) {
		cpFavouriteSource = cpLeft;
		cpFavouriteDestination = cpRight;
	} else {
		cpFavouriteSource = cpRight;
		cpFavouriteDestination = cpLeft;
	} // if else

	cpFavouriteSource->SetPath(qhFavouriteDirectories.value(action).qsSource);
	if (qhFavouriteDirectories.value(action).bTarget) {
		cpFavouriteDestination->SetPath(qhFavouriteDirectories.value(action).qsTarget);
	} // if
} // on_qmFavouriteDirectories_triggered

// history directory list is about to show
const void cMainWindow::on_qmLeftHistoryDirectoryList_aboutToShow()
{
	FillHistoryDirectoryList(cSettings::PositionLeft);
} // on_qmHistoryDirectoryList_aboutToShow

// selected directory from left panel's history list
const void cMainWindow::on_qmLeftHistoryDirectoryList_triggered(QAction *action) const
{
	if (!action->isChecked()) {
		cpLeft->SetHistoryDirectory(action->data().toInt());
	} // if
} // on_qmLeftHistoryDirectoryList_triggered

// right history directory list is about to show
const void cMainWindow::on_qmRightHistoryDirectoryList_aboutToShow()
{
	FillHistoryDirectoryList(cSettings::PositionRight);
} // on_qmRightHistoryDirectoryList_aboutToShow

// selected directory from right panel's history list
const void cMainWindow::on_qmRightHistoryDirectoryList_triggered(QAction *action) const
{
	if (!action->isChecked()) {
		cpRight->SetHistoryDirectory(action->data().toInt());
	} // if
} // on_qmRightHistoryDirectoryList_triggered

// copy button is clicked on
const void cMainWindow::on_qpbCopy_clicked(bool checked /* false */) const
{
	cfcFileControl->Operation(cFileOperationDialog::CopyOperation, cpSource->GetFileSystem(), cpSource->GetSelectedFiles(), cpDestination->GetFileSystem());
} // on_qpbCopy_clicked

// delete button is clicked on
const void cMainWindow::on_qpbDelete_clicked(bool checked /* false */) const
{
	cfcFileControl->Operation(cFileOperationDialog::DeleteOperation, cpSource->GetFileSystem(), cpSource->GetSelectedFiles());
} // on_qpbDelete_clicked

// edit button is clicked on
const void cMainWindow::on_qpbEdit_clicked(bool checked /* false */) const
{
	cfcFileControl->Edit(cpSource->GetFileSystem(), cpSource->GetSelectedFiles());
} // on_qpbEdit_clicked

// left favourite button is clicked on
const void cMainWindow::on_qpbLeftFavourites_clicked(bool checked /* false */)
{
	epFavourite = cSettings::PositionLeft;
} // on_qpbLeftFavourites_clicked

// left root dir button is clicked on
const void cMainWindow::on_qpbLeftRootDir_clicked(bool checked /* false */) const
{
	cpLeft->GetFileSystem()->GoToRootDir();
} // on_qpbLeftRootDir_clicked

// left up dir button is clicked on
const void cMainWindow::on_qpbLeftUpDir_clicked(bool checked /* false */) const
{
	cpLeft->GetFileSystem()->GoToUpDir();
} // on_qpbLeftUpDir_clicked

// copy button is clicked on
const void cMainWindow::on_qpbMove_clicked(bool checked /* false */) const
{
	cfcFileControl->Operation(cFileOperationDialog::MoveOperation, cpSource->GetFileSystem(), cpSource->GetSelectedFiles(), cpDestination->GetFileSystem());
} // on_qpbMove_clicked

// new directory button is clicked on
const void cMainWindow::on_qpbNewDirectory_clicked(bool checked /* false */) const
{
	QString qsName;

	qsName = cfcFileControl->CreateDirectory(cpSource->GetFileSystem());

	if (!qsName.isEmpty()) {
		cpSource->RefreshContent();
		qsName = QDir::toNativeSeparators(qsName);
		cpSource->GoToFile(qsName.left(qsName.indexOf(QDir::separator())));
	} // if
} // on_qpbNewDirectory_clicked

// right favourite button is clicked on
const void cMainWindow::on_qpbRightFavourites_clicked(bool checked /* false */)
{
	epFavourite = cSettings::PositionRight;
} // on_qpbRightFavourites_clicked

// right root dir button is clicked on
const void cMainWindow::on_qpbRightRootDir_clicked(bool checked /* false */) const
{
	cpRight->GetFileSystem()->GoToRootDir();
} // on_qpbRightRootDir_clicked

// right up dir button is clicked on
const void cMainWindow::on_qpbRightUpDir_clicked(bool checked /* false */) const
{
	cpRight->GetFileSystem()->GoToUpDir();
} // on_qpbRightUpDir_clicked

// terminal button is clicked on
const void cMainWindow::on_qpbTerminal_clicked(bool checked /* false */) const
{
	cfcFileControl->StartTerminal(cpSource->GetPath(true));
} // on_qpbTerminal_clicked

// view button is clicked on
const void cMainWindow::on_qpbView_clicked(bool checked /* false */) const
{
	cfcFileControl->View(cpSource->GetFileSystem(), cpSource->GetSelectedFiles());
} // on_qpbView_clicked

// history back shortcut activated
const void cMainWindow::on_qsHistoryBack_activated() const
{
	cpSource->HistoryGoBack();
} // on_qsHistoryBack_activated

// history front shortcut activated
const void cMainWindow::on_qsHistoryFront_activated() const
{
	cpSource->HistoryGoFront();
} // on_qsHistoryFront_activated

// left drive shortcut activated
const void cMainWindow::on_qsLeftDrive_activated() const
{
	qcbLeftDrive->showPopup();
} // on_qsLeftDrive_activated

// right drive shortcut activated
const void cMainWindow::on_qsRightDrive_activated() const
{
	qcbRightDrive->showPopup();
} // on_qsRightDrive_activated

// context menu of left tab bar
const void cMainWindow::on_qtbLeft_customContextMenuRequested(const QPoint &pos)
{
	TabBarShowContextMenu(cSettings::PositionLeft, pos);
} // on_qtbLeft_customContextMenuRequested

// context menu of right tab bar
const void cMainWindow::on_qtbRight_customContextMenuRequested(const QPoint &pos)
{
	TabBarShowContextMenu(cSettings::PositionRight, pos);
} // on_qtbRight_customContextMenuRequested

// save dir view settings
const void cMainWindow::SaveSettings() const
{
	cpLeft->SaveSettings(cSettings::PositionLeft);
	cpRight->SaveSettings(cSettings::PositionRight);
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

// show context menu for tab
const void cMainWindow::TabBarShowContextMenu(const cSettings::ePosition &epTab, const QPoint &qpCursor)
{
	// source panel
	if (epTab == cSettings::PositionLeft) {
		cpTabBarAction = cpLeft;
	} else {
		cpTabBarAction = cpRight;
	} // if else

	iTabBarIndex = cpTabBarAction->GetTabIndex(qpCursor);

	if (iTabBarIndex != -1) {
		QTabBar *qtbTabBar;

		// disable close tab if only one tab is available
		if (epTab == cSettings::PositionLeft) {
			qtbTabBar = &qtbLeft;
		} else {
			qtbTabBar = &qtbRight;
		} // if else
		if (qtbTabBar->count() == 1) {
			qaTabBarCloseTab->setEnabled(false);
			qaTabBarCloseAllOtherTabs->setEnabled(false);
		} else {
			qaTabBarCloseTab->setEnabled(true);
			qaTabBarCloseAllOtherTabs->setEnabled(true);
		} // if else

		qmTabBar.popup(QCursor::pos());
	} // if
} // TabBarShowContextMenu
