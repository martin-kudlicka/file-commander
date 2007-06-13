#include "MainWindow.h"

#include "Common/System.h"
#include "FileOperation/FileRoutine.h"
#include "ListerMainWindow.h"
#include "FindFilesDialog.h"
#include "Panel/Process.h"
#include <QtGui/QKeyEvent>
#include <QtGui/QInputDialog>
#include <QtGui/QHeaderView>
#include "Common/About.h"
#include "ArchiveOperation.h"

// destructor
cMainWindow::~cMainWindow()
{
	if (csSettings.GetSaveSettingsOnQuit()) {
		SaveSettings();
	} // if

	delete qsLeftDrive;
	delete qsRightDrive;
	delete cfoFileOperation;
	delete qhblBackgroundOperations;
	cpLeft->deleteLater();
	cpRight->deleteLater();
	delete cpPlugins;
	qagSortBy->deleteLater();
	qagColumnSets->deleteLater();
	qtwLeftDrives->deleteLater();
	qtwRightDrives->deleteLater();
} // cMainWindow

// actualize column sets submenu
void cMainWindow::ActualizeColumnSets()
{
	int iI;
	QString qsActiveColumnSet;
	QStringList qslColumnSets;

	qsActiveColumnSet = cpActive->GetColumnSet();
	qagColumnSets->actions().clear();
	qmColumnSets.clear();
	qslColumnSets = csSettings.GetColumnSets();

	for (iI = 0; iI < qslColumnSets.count(); iI++) {
		QAction *qaColumnSet;

		qaColumnSet = qmColumnSets.addAction(qslColumnSets.at(iI));
		// put action to action group to prevent checking more than one action in a time
		qagColumnSets->addAction(qaColumnSet);
		qaColumnSet->setCheckable(true);
		if (qsActiveColumnSet == qslColumnSets.at(iI)) {
			qaColumnSet->setChecked(true);
		} // if
	} // for
} // ActualizeColumnSets

// drive lists actualization
void cMainWindow::ActualizeDrives()
{
	qmDrives = cFileRoutine::GetDrives();
	if (qtwLeftDrives->topLevelItemCount() != qmDrives.count()) {
		// assume drives have changed
		QString qsLeftDrive, qsRightDrive;

		qsLeftDrive = qcbLeftDrive->currentText();
		qsRightDrive = qcbRightDrive->currentText();

		qcbLeftDrive->blockSignals(true);
		qcbRightDrive->blockSignals(true);
		qtwLeftDrives->clear();
		qtwRightDrives->clear();

		QMapIterator<QString, cFileRoutine::sDriveInfo> qmiDrives(qmDrives);
		while (qmiDrives.hasNext()) {
			qmiDrives.next();
			qtwLeftDrives->addTopLevelItem(new QTreeWidgetItem(QStringList() << qmiDrives.key()));
			qtwRightDrives->addTopLevelItem(new QTreeWidgetItem(QStringList() << qmiDrives.key()));
		} // while

		// check for selected drive change, changes are handled in Panel class
		if (qtwLeftDrives->findItems(qcbLeftDrive->currentText(), Qt::MatchExactly).isEmpty()) {
			// selected drive changed
			qcbLeftDrive->blockSignals(false);
			qcbLeftDrive->setCurrentIndex(-1);
		} else {
			// selected drive not changed
			qcbLeftDrive->setCurrentIndex(qcbLeftDrive->findText(qsLeftDrive));
			qcbLeftDrive->blockSignals(false);
		} // if else
		if (qtwRightDrives->findItems(qcbRightDrive->currentText(), Qt::MatchExactly).isEmpty()) {
			// selected drive changed
			qcbRightDrive->blockSignals(false);
			qcbRightDrive->setCurrentIndex(-1);
		} else {
			// selected drive not changed
			qcbRightDrive->setCurrentIndex(qcbRightDrive->findText(qsRightDrive));
			qcbRightDrive->blockSignals(false);
		} // if else
	} // if
} // ActualizeDrives

// actualize favourite directories context menu
void cMainWindow::ActualizeFavouriteDirectories()
{
	qhFavouriteDirectories.clear();
	qmFavouriteDirectories.clear();

	FillFavouriteDirectories(&qmFavouriteDirectories, csSettings.GetFavouriteDirectories());
} // ActualizeFavouriteDirectories

// assign shortcuts
void cMainWindow::AssignShortcuts()
{
	// tabs
	qaTabBarDuplicateTab->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::PanelsCategory, qsSHORTCUT__PANELS__TABS__DUPLICATE_TAB)));
	qaTabBarCloseTab->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::PanelsCategory, qsSHORTCUT__PANELS__TABS__CLOSE_TAB)));
	qaTabBarCloseAllOtherTabs->setShortcut(QKeySequence(csSettings.GetShortcut(cSettings::PanelsCategory, qsSHORTCUT__PANELS__TABS__CLOSE_ALL_OTHER_TABS)));
	// directory view
	qsLeftDrive = new QShortcut(QKeySequence(csSettings.GetShortcut(cSettings::PanelsCategory, qsSHORTCUT__PANELS__DIRECTORY_VIEW__DRIVE_LEFT)), this);
	qsRightDrive = new QShortcut(QKeySequence(csSettings.GetShortcut(cSettings::PanelsCategory, qsSHORTCUT__PANELS__DIRECTORY_VIEW__DRIVE_RIGHT)), this);
	// main buttons
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

// create of main window
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
	cfoFileOperation = new cFileOperation(this, qhblBackgroundOperations, &csSettings);
	// panels
	qtwLeftDrives = new QTreeWidget(qcbLeftDrive);
	qtwLeftDrives->setRootIsDecorated(false);
	qtwLeftDrives->header()->hide();
	qcbLeftDrive->setModel(qtwLeftDrives->model());
	qcbLeftDrive->setView(qtwLeftDrives);
	cpLeft = new cPanel(this, qswLeft, qcbLeftDrive, qlLeftDriveInfo, &qtbLeft, qlLeftPath, qlLeftSelected, &csSettings, cpPlugins->ccpContentPlugin, cpPlugins->cppPackerPlugin, &qmDrives, qlGlobalPath, qcbCommand, cfoFileOperation, qleLeftQuickSearch);
	qtwRightDrives = new QTreeWidget(qcbRightDrive);
	qtwRightDrives->setRootIsDecorated(false);
	qtwRightDrives->header()->hide();
	qcbRightDrive->setModel(qtwRightDrives->model());
	qcbRightDrive->setView(qtwRightDrives);
	cpRight = new cPanel(this, qswRight, qcbRightDrive, qlRightDriveInfo, &qtbRight, qlRightPath, qlRightSelected, &csSettings, cpPlugins->ccpContentPlugin, cpPlugins->cppPackerPlugin,  &qmDrives, qlGlobalPath, qcbCommand, cfoFileOperation, qleRightQuickSearch);
	// quick searches
	qleLeftQuickSearch->hide();
	qleRightQuickSearch->hide();

	// create tab context menu
	qaTabBarDuplicateTab = qmTabBar.addAction(tr("&Duplicate tab"));
	this->addAction(qaTabBarDuplicateTab);
	qmTabBar.addSeparator();
	qaTabBarCloseTab = qmTabBar.addAction(tr("&Close tab"));
	this->addAction(qaTabBarCloseTab);
	qaTabBarCloseAllOtherTabs = qmTabBar.addAction(tr("Close &all other tabs"));
	this->addAction(qaTabBarCloseAllOtherTabs);
	// favourites context menu
	ActualizeFavouriteDirectories();
	qpbLeftFavourites->setMenu(&qmFavouriteDirectories);
	qpbRightFavourites->setMenu(&qmFavouriteDirectories);
	// column sets submenu
	qaColumnSet->setMenu(&qmColumnSets);

	// variables initialization
	qagSortBy = new QActionGroup(this);
	iTabBarIndex = -1;
	qcbCommand->installEventFilter(this);
	qagColumnSets = new QActionGroup(this);

	// shortcuts
	AssignShortcuts();

	// connections
	connect(cpLeft, SIGNAL(GotFocus()), SLOT(on_cpLeft_GotFocus()));
	connect(cpLeft, SIGNAL(Delete()), SLOT(on_cpPanel_Delete()));
	connect(cpRight, SIGNAL(GotFocus()), SLOT(on_cpRight_GotFocus()));
	connect(cpRight, SIGNAL(Delete()), SLOT(on_cpPanel_Delete()));
	connect(qsLeftDrive, SIGNAL(activated()), SLOT(on_qsLeftDrive_activated()));
	connect(qsRightDrive, SIGNAL(activated()), SLOT(on_qsRightDrive_activated()));
	connect(qaTabBarDuplicateTab, SIGNAL(triggered(bool)), SLOT(on_qaTabBarDuplicateTab_triggered(bool)));
	connect(qaTabBarCloseTab, SIGNAL(triggered(bool)), SLOT(on_qaTabBarCloseTab_triggered(bool)));
	connect(qaTabBarCloseAllOtherTabs, SIGNAL(triggered(bool)), SLOT(on_qaTabBarCloseAllOtherTabs_triggered(bool)));
	connect(qagSortBy, SIGNAL(triggered(QAction *)), SLOT(on_qagSortBy_triggered(QAction *)));
	connect(&qmFavouriteDirectories, SIGNAL(triggered(QAction *)), SLOT(on_qmFavouriteDirectories_triggered(QAction *)));
	connect(&qmColumnSets, SIGNAL(triggered(QAction *)), SLOT(on_qmColumnSets_triggered(QAction *)));

	ActualizeDrives();
	// show before change drive dialog can appear
	show();
	// load settings
	// left tabs
	LoadTabs(cSettings::PositionLeft);
	// right tabs
	LoadTabs(cSettings::PositionRight);

	// automatic actualizations
	connect(&qtTimer, SIGNAL(timeout()), SLOT(on_qtTimer_timeout()));
	qtTimer.start(iTIMER_INTERVAL);

	// set focus to left panel
	static_cast<cTreeWidget *>(qswLeft->currentWidget())->setFocus(Qt::OtherFocusReason);
} // cMainWindow

// event filter
bool cMainWindow::eventFilter(QObject *watched, QEvent *event)
{
	if (watched == qcbCommand) {
		if (event->type() == QEvent::KeyPress) {
			switch (static_cast<QKeyEvent *>(event)->key()) {
				int iCommandPos;

				case Qt::Key_Enter:
				case Qt::Key_Return:
					cProcess::Execute(qcbCommand->currentText(), qlGlobalPath->text());
					iCommandPos = qcbCommand->findText(qcbCommand->currentText());
					if (iCommandPos > 0) {
						qcbCommand->removeItem(iCommandPos);
					} // if
					if (iCommandPos != 0) {
						qcbCommand->insertItem(0, qcbCommand->currentText());
					} // if
					qcbCommand->setCurrentIndex(-1);
					qcbCommand->clearEditText();
					return true;
				case Qt::Key_Escape:
					qcbCommand->clearEditText();
					cpActive->SetFocus();
					return true;
			} // switch
		} // if
		return false;
	} else {
		return QMainWindow::eventFilter(watched, event);
	} // if else
} // eventFilter

// fill favourite directories context menu
void cMainWindow::FillFavouriteDirectories(QMenu *qmMenu, const QList<QPair<QString, cSettings::sFavouriteDirectory> > &qlFavouriteDirectories)
{
	int iI;

	for (iI = 0; iI < qlFavouriteDirectories.count(); iI++) {
		if (qlFavouriteDirectories.at(iI).second.bSubmenu) {
			QMenu *qmSubmenu;

			qmSubmenu = qmMenu->addMenu(qlFavouriteDirectories.at(iI).first);
			FillFavouriteDirectories(qmSubmenu, qlFavouriteDirectories.at(iI).second.qlChildFavourites);
		} else {
			QAction *qaFavouriteDirectory;
			cOptionsDialog::sFavouriteDirectory sfdFavouriteDirectory;

			qaFavouriteDirectory = qmMenu->addAction(qlFavouriteDirectories.at(iI).first);

			sfdFavouriteDirectory.qsSource = qlFavouriteDirectories.at(iI).second.qsSource;
			sfdFavouriteDirectory.bTarget = qlFavouriteDirectories.at(iI).second.bTarget;
			sfdFavouriteDirectory.qsTarget = qlFavouriteDirectories.at(iI).second.qsTarget;

			qhFavouriteDirectories.insert(qaFavouriteDirectory, sfdFavouriteDirectory);
		} // if else
	} // for
} // FillFavouriteDirectories

// load tabs from qsSettings
void cMainWindow::LoadTabs(const cSettings::ePosition &epPosition)
{
	int iI;
	QStringList qslTabs;

	// get tabs
	qslTabs = csSettings.GetTabs(epPosition);

	if (qslTabs.isEmpty()) {
		// no tabs created yet -> create one default in settings file
		cSettings::sTabInfo stiTab;

		stiTab.qsColumnSet = qsFULL;
		stiTab.qsPath = cSystem::GetFirstDrive();
		QMapIterator<QString, cFileRoutine::sDriveInfo> qmiDrives(qmDrives);
		while (qmiDrives.hasNext()) {
			qmiDrives.next();
			if (stiTab.qsPath.startsWith(qmiDrives.key())) {
				stiTab.qsDrive = qmiDrives.key();
				break;
			} // if
		} // while
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
void cMainWindow::on_cpLeft_GotFocus()
{
	cpActive = cpLeft;
	SetSortByActions();
	ActualizeColumnSets();
} // on_cpLeft_GotFocus

// delete marked files
void cMainWindow::on_cpPanel_Delete()
{
	qpbDelete->animateClick();
} // on_cpPanel_Delete

// right panel got focus
void cMainWindow::on_cpRight_GotFocus()
{
	cpActive = cpRight;
	SetSortByActions();
	ActualizeColumnSets();
} // on_cpRight_GotFocus

// about is selected
void cMainWindow::on_qaAbout_triggered(bool checked /* false */)
{
	cAbout caAbout(this, qsAPPLICATION, qsVERSION);

	caAbout.exec();
} // on_qaAbout_triggered

// branch view is selected
void cMainWindow::on_qaBranchView_triggered(bool checked /* false */)
{
	QFileInfoList qfilSource;

	qfilSource = cFileRoutine::GetDirectoryContent(cpActive->GetPath(), cpLeft->GetStandardFilters());
	qfilSource = cFileRoutine::GetSources(qfilSource);
	cpActive->RefreshContent(qfilSource);
} // on_qaBranchView_triggered

// compare directories is selected
void cMainWindow::on_qaCompareDirectories_triggered(bool checked /* false */)
{
	QHash<QTreeWidgetItem *, QFileInfo> qhFilesLeft, qhFilesRight;

	qhFilesLeft = cpLeft->GetDirContent();
	qhFilesRight = cpRight->GetDirContent();

	// mark first all files in the right panel, unmark directories
	QHashIterator<QTreeWidgetItem *, QFileInfo> qhiFileRight(qhFilesRight);
	while (qhiFileRight.hasNext()) {
		qhiFileRight.next();
		if (qhiFileRight.value().isFile()) {
			qhiFileRight.key()->setSelected(true);
		} else {
			qhiFileRight.key()->setSelected(false);
		} // if else
	} // while

	// go through left panel files and compare with right
	QHashIterator<QTreeWidgetItem *, QFileInfo> qhiFileLeft(qhFilesLeft);
	while (qhiFileLeft.hasNext()) {
		qhiFileLeft.next();

		if (qhiFileLeft.value().isFile()) {
			bool bFound;

			bFound = false;

			qhiFileRight.toFront();
			while (qhiFileRight.hasNext()) {
				qhiFileRight.next();

				if (qhiFileRight.value().isFile()) {
					if (qhiFileLeft.value().fileName() == qhiFileRight.value().fileName()) {
						bFound = true;

						if (qhiFileLeft.value().lastModified() == qhiFileRight.value().lastModified()) {
							// files are the same - clear mark of both files
							qhiFileLeft.key()->setSelected(false);
							qhiFileRight.key()->setSelected(false);
						} else {
							if (qhiFileLeft.value().lastModified() > qhiFileRight.value().lastModified()) {
								// newer file on the left panel
								qhiFileLeft.key()->setSelected(true);
								qhiFileRight.key()->setSelected(false);
							} else {
								// newer file on the right panel
								qhiFileLeft.key()->setSelected(false);
							} // if else
						} // if else

						break;
					} // if
				} // if
			} // while

			if (!bFound) {
				// file on the left is not on the right panel
				qhiFileLeft.key()->setSelected(true);
			} // if
		} else {
			// unmark directories on the left
			qhiFileLeft.key()->setSelected(false);
		} // if else
	} // while
} // on_qaCompareDirectories_triggered

// favourite directories are selected
void cMainWindow::on_qaFavouriteDirectories_triggered(bool checked /* false */)
{
	if (qhFavouriteDirectories.count() != 0) {
		if (cpActive == cpLeft) {
			qpbLeftFavourites->showMenu();
		} else {
			qpbRightFavourites->showMenu();
		} // if else
	} // if
} // on_qaFavouriteDirectories_triggered

// full screen mode is selected
void cMainWindow::on_qaFullScreen_triggered(bool checked /* false */)
{
	setWindowState(windowState() ^ Qt::WindowFullScreen);
} // on_qaFullScreen_triggered

// sort by action called
void cMainWindow::on_qagSortBy_triggered(QAction *action)
{
	int iSortBy;

	// find column to sort by
	for (iSortBy = 0; iSortBy < qagSortBy->actions().count(); iSortBy++) {
		if (qagSortBy->actions().at(iSortBy) == action) {
			break;
		} // if
	} // for

	cpActive->SortBy(iSortBy);
} // on_qagSortBy_triggered

// invert selection is selected
void cMainWindow::on_qaInvertSelection_triggered(bool checked /* false */)
{
	cPanel *cpSource;

	SetSourceAndDestinationPanel(&cpSource);
	cpSource->InvertSelection();
} // on_qaInvertSelection_triggered

// options are selected
void cMainWindow::on_qaOptions_triggered(bool checked /* false */)
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
} // on_qaOptions_triggered

///< refresh is selected
void cMainWindow::on_qaRefresh_triggered(bool checked /* false */)
{
	cPanel *cpSource;

	SetSourceAndDestinationPanel(&cpSource);
	cpSource->RefreshContent();
} // on_qaRefresh_triggered

// reverse order is selected
void cMainWindow::on_qaReverseOrder_triggered(bool checked /* false */)
{
	cpActive->ReverseOrder();
} // on_qaReverseOrder_triggered

// save position is selected
void cMainWindow::on_qaSavePosition_triggered(bool checked /* false */)
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
void cMainWindow::on_qaSaveSettings_triggered(bool checked /* false */)
{
	SaveSettings();
} // on_qaSaveSettings_triggered

// search selected
void cMainWindow::on_qaSearch_triggered(bool checked /* false */)
{
	cFindFilesDialog *cffdFind;
	cPanel *cpSource;

	SetSourceAndDestinationPanel(&cpSource);
	cffdFind = new cFindFilesDialog(this, cpSource, &csSettings, cpPlugins->clpListerPlugin);

	cffdFind->exec();
	cffdFind->deleteLater();
} // on_qaSearch_triggered

// select all selected
void cMainWindow::on_qaSelectAll_triggered(bool checked /* false */)
{
	cPanel *cpSource;

	SetSourceAndDestinationPanel(&cpSource);
	cpSource->SelectAll();
} // on_qaSelectAll_triggered

// select group selected
void cMainWindow::on_qaSelectGroup_triggered(bool checked /* false */)
{
	cPanel *cpSource;

	SetSourceAndDestinationPanel(&cpSource);
	cpSource->Select(cSelectFilesDialog::Select, cpPlugins->clpListerPlugin);
} // on_qaSelectGroup_triggered

// close all other tabs called
void cMainWindow::on_qaTabBarCloseAllOtherTabs_triggered(bool checked /* false */)
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
void cMainWindow::on_qaTabBarCloseTab_triggered(bool checked /* false */)
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
void cMainWindow::on_qaTabBarDuplicateTab_triggered(bool checked /* false */)
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

// unpack selected files chosen
void cMainWindow::on_qaUnpackSelectedFiles_triggered(bool checked /* false */)
{
	cArchiveOperation caoArchiveOperation(this, &csSettings);
	cPanel *cpDestination, *cpSource;

	SetSourceAndDestinationPanel(&cpSource, &cpDestination);
	caoArchiveOperation.UnpackSelectedFiles(cpSource->GetSelectedItemsFileList(QDir::Files), cpDestination->GetPath(), cpPlugins->cppPackerPlugin);
} // on_qaUnpackSelectedFiles_triggered

// unselect all selected
void cMainWindow::on_qaUnselectAll_triggered(bool checked /* false */)
{
	cPanel *cpSource;

	SetSourceAndDestinationPanel(&cpSource);
	cpSource->UnselectAll();
} // on_qaUnselectAll_triggered

// unselect group selected
void cMainWindow::on_qaUnselectGroup_triggered(bool checked /* false */)
{
	cPanel *cpSource;

	SetSourceAndDestinationPanel(&cpSource);
	cpSource->Select(cSelectFilesDialog::Unselect, cpPlugins->clpListerPlugin);
} // on_qaUnselectGroup_triggered

// selected column set from column set submenu
void cMainWindow::on_qmColumnSets_triggered(QAction *action)
{
	cpActive->SetColumnSet(action->text());
} // on_qmColumnSets_triggered

// selected favourite directory from from favourites context menu
void cMainWindow::on_qmFavouriteDirectories_triggered(QAction *action)
{
	cPanel *cpDestination, *cpSource;

	SetSourceAndDestinationPanel(&cpSource, &cpDestination);

	cpSource->ChangePath(qhFavouriteDirectories.value(action).qsSource);
	if (qhFavouriteDirectories.value(action).bTarget) {
		cpDestination->ChangePath(qhFavouriteDirectories.value(action).qsTarget);
	} // if
} // on_qmFavouriteDirectories_triggered

// copy button is clicked on
void cMainWindow::on_qpbCopy_clicked(bool checked /* false */)
{
	cPanel *cpDestination, *cpSource;
	cPanel::eLocation elSourceLocation;
	QFileInfoList qfilSource;
	QString qsDestination;

	SetSourceAndDestinationPanel(&cpSource, &cpDestination);
	elSourceLocation = cpSource->GetLocation();

	switch (elSourceLocation) {
		case cPanel::LocalDirectory:
			// copy from local directory to local directory
			qfilSource = cpSource->GetSelectedItemsFileList();
			qsDestination = cpDestination->GetPath();

			cfoFileOperation->Operate(cFileRoutine::CopyOperation, qfilSource, qsDestination);
			break;
		case cPanel::Archive:
			// extract from archive to local directory
			cArchiveOperation caoArchiveOperation(this, &csSettings);
			QList<tHeaderData> qlSourceAll, qlSourceSelected;
			cArchiveOperation::sArchive saSourceArchive;

			saSourceArchive = cpSource->GetArchiveInfo();
			qlSourceSelected = cpSource->GetSelectedItemsArchiveList();
			qsDestination = cpDestination->GetPath();

			caoArchiveOperation.Operate(cArchiveOperation::Extract, saSourceArchive, qlSourceSelected, qsDestination);
	} // switch
} // on_qpbCopy_clicked

// delete button is clicked on
void cMainWindow::on_qpbDelete_clicked(bool checked /* false */)
{
	cPanel *cpSource;
	QFileInfoList qfilSource;

	SetSourceAndDestinationPanel(&cpSource);
	qfilSource = cpSource->GetSelectedItemsFileList();
	cfoFileOperation->Operate(cFileRoutine::DeleteOperation, qfilSource);
} // on_qpbDelete_clicked

// edit button is clicked on
void cMainWindow::on_qpbEdit_clicked(bool checked /* false */)
{
	cPanel *cpSource;
	int iI;
	QFileInfoList qfilFiles;

	SetSourceAndDestinationPanel(&cpSource);
	qfilFiles = cpSource->GetSelectedItemsFileList();

	for (iI = 0; iI < qfilFiles.count(); iI++) {
		if (qfilFiles.at(iI).isFile()) {
			QString qsCommand;

			qsCommand = csSettings.GetExternalEditor();
			qsCommand = qsCommand.replace("%1", qfilFiles.at(iI).filePath());

			cProcess::Execute(qsCommand);
		} // if
	} // for
} // on_qpbEdit_clicked

// left root dir button is clicked on
void cMainWindow::on_qpbLeftRootDir_clicked(bool checked /* false */)
{
	cpLeft->GoToRootDir();
} // on_qpbLeftRootDir_clicked

// left up dir button is clicked on
void cMainWindow::on_qpbLeftUpDir_clicked(bool checked /* false */)
{
	cpLeft->GoToUpDir();
} // on_qpbLeftUpDir_clicked

// move button is clicked on
void cMainWindow::on_qpbMove_clicked(bool checked /* false */)
{
	cPanel *cpDestination, *cpSource;
	QFileInfoList qfilSource;
	QString qsDestination;

	SetSourceAndDestinationPanel(&cpSource, &cpDestination);
	qfilSource = cpSource->GetSelectedItemsFileList();
	qsDestination = cpDestination->GetPath();
	cfoFileOperation->Operate(cFileRoutine::MoveOperation, qfilSource, qsDestination);
} // on_qpbMove_clicked

// new directory button is clicked on
void cMainWindow::on_qpbNewDirectory_clicked(bool checked /* false */)
{
	if (cpActive->GetLocation() == cPanel::LocalDirectory) {
		QString qsName;

		qsName = QInputDialog::getText(this, tr("New directory"), tr("Enter name of a new directory:"));
		if (!qsName.isEmpty()) {
			QDir qdDir;
			QString qsNewDirectory;

			qsNewDirectory = cpActive->GetPath();
			qsNewDirectory += '/' + qsName;
			qdDir.mkpath(qsNewDirectory);

			// set focus to new directory
			cpActive->RefreshContent(QFileInfo(qsNewDirectory).fileName());
		} // if
	} // if
} // on_qpbNewDirectory_clicked

// right root dir button is clicked on
void cMainWindow::on_qpbRightRootDir_clicked(bool checked /* false */)
{
	cpRight->GoToRootDir();
} // on_qpbRightRootDir_clicked

// right up dir button is clicked on
void cMainWindow::on_qpbRightUpDir_clicked(bool checked /* false */)
{
	cpRight->GoToUpDir();
} // on_qpbRightUpDir_clicked

// view button is clicked on
void cMainWindow::on_qpbView_clicked(bool checked /* false */)
{
	cPanel *cpSource;
	int iI;
	QFileInfoList qfilFiles;

	SetSourceAndDestinationPanel(&cpSource);
	qfilFiles = cpSource->GetSelectedItemsFileList();

	for (iI = 0; iI < qfilFiles.count(); iI++) {
		if (qfilFiles.at(iI).isFile()) {
			if (csSettings.GetViewerType() == qsINTERNAL) {
				cListerMainWindow *clmwLister;

				clmwLister = new cListerMainWindow(&csSettings, cpPlugins->clpListerPlugin, qfilFiles.at(iI).filePath());
				clmwLister->show();
			} else {
				QString qsCommand;

				qsCommand = csSettings.GetExternalViewer();
				qsCommand = qsCommand.replace("%1", qfilFiles.at(iI).filePath());

				cProcess::Execute(qsCommand);
			} // if else
		} // if
	} // for
} // on_qpbView_clicked

// left drive shortcut activated
void cMainWindow::on_qsLeftDrive_activated()
{
	qcbLeftDrive->showPopup();
} // on_qsLeftDrive_activated

// right drive shortcut activated
void cMainWindow::on_qsRightDrive_activated()
{
	qcbRightDrive->showPopup();
} // on_qsRightDrive_activated

// context menu of left tab bar
void cMainWindow::on_qtbLeft_customContextMenuRequested(const QPoint &pos)
{
	TabBarShowContextMenu(cSettings::PositionLeft, pos);
} // on_qtbLeft_customContextMenuRequested

// context menu of right tab bar
void cMainWindow::on_qtbRight_customContextMenuRequested(const QPoint &pos)
{
	TabBarShowContextMenu(cSettings::PositionRight, pos);
} // on_qtbRight_customContextMenuRequested

///< timer's timeout
void cMainWindow::on_qtTimer_timeout()
{
	// check for new or removed drives
	ActualizeDrives();
} // on_qtTimer_timeout

// save dir view settings
void cMainWindow::SaveSettings()
{
	cpLeft->SaveSettings(cSettings::PositionLeft);
	cpRight->SaveSettings(cSettings::PositionRight);
} // SaveSettings

// set sort by actions for sorting columns
void cMainWindow::SetSortByActions()
{
	int iI;
	QList<cSettings::sColumn> *qlColumns;

	for (iI = 0; iI < qagSortBy->actions().count(); iI++) {
		qagSortBy->actions().at(iI)->deleteLater();
	} // for
	qagSortBy->actions().clear();

	qlColumns = cpActive->GetColumns();

	for (iI = 0; iI < qlColumns->count(); iI++) {
		QAction *qaSortBy;

		qaSortBy = new QAction(qlColumns->at(iI).qsName, qagSortBy);

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

// set focused panel as source, other as destination
void cMainWindow::SetSourceAndDestinationPanel(cPanel **cpSource, cPanel **cpDestination /* NULL */)
{
	if (cpLeft == cpActive) {
		*cpSource = cpLeft;
		if (cpDestination) {
			*cpDestination = cpRight;
		} // if
	} else {
		*cpSource = cpRight;
		if (cpDestination) {
			*cpDestination = cpLeft;
		} // if
	} // if else
} // SetSourceAndDestinationPanel

// show context menu for tab
void cMainWindow::TabBarShowContextMenu(const cSettings::ePosition &epTab, const QPoint &qpCursor)
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
