#include "MainWindow.h"

#include "Common/System.h"
#include "FileOperation/FileRoutine.h"
#include "NewDirectoryDialog.h"
#include "OptionsDialog.h"
#include "ListerMainWindow.h"
#include "FindFilesDialog.h"
#include "Panel/Process.h"
#include <QKeyEvent>

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
	delete cpLeft;
	delete cpRight;
	delete cpPlugins;
} // cMainWindow

// drive lists actualization
void cMainWindow::ActualizeDrives()
{
	qmDrives = cFileRoutine::GetDrives();
	if (qcbLeftDrive->count() != qmDrives.count()) {
		// assume drives have changed
		QString qsLeftDrive, qsRightDrive;

		qsLeftDrive = qcbLeftDrive->currentText();
		qsRightDrive = qcbRightDrive->currentText();

		qcbLeftDrive->blockSignals(true);
		qcbRightDrive->blockSignals(true);
		qcbLeftDrive->clear();
		qcbRightDrive->clear();

		QMapIterator<QString, cFileRoutine::sDriveInfo> qmiDrives(qmDrives);
		while (qmiDrives.hasNext()) {
			qmiDrives.next();
			qcbLeftDrive->addItem(qmiDrives.key());
			qcbRightDrive->addItem(qmiDrives.key());
		} // while

		// check for selected drive change, changes are handled in Panel class
		if (qcbLeftDrive->findText(qsLeftDrive) != -1) {
			// selected drive not changed
			qcbLeftDrive->setCurrentIndex(qcbLeftDrive->findText(qsLeftDrive));
			qcbLeftDrive->blockSignals(false);
		} else {
			qcbLeftDrive->blockSignals(false);
			qcbLeftDrive->setCurrentIndex(-1);
			// selected drive changed
		} // if else
		if (qcbRightDrive->findText(qsLeftDrive) != -1) {
			// selected drive not changed
			qcbRightDrive->setCurrentIndex(qcbRightDrive->findText(qsLeftDrive));
			qcbRightDrive->blockSignals(false);
		} else {
			// selected drive changed
			qcbRightDrive->blockSignals(false);
			qcbRightDrive->setCurrentIndex(-1);
		} // if else
	} // if
} // ActualizeDrives

// assign shortcuts
void cMainWindow::AssignShortcuts()
{
	// directory view
	qsLeftDrive = new QShortcut(QKeySequence(csSettings.GetShortcut(qsSHORTCUT__DIRECTORY_VIEW__DRIVE_LEFT)), this);
	qsRightDrive = new QShortcut(QKeySequence(csSettings.GetShortcut(qsSHORTCUT__DIRECTORY_VIEW__DRIVE_RIGHT)), this);
	// main buttons
	qpbView->setShortcut(QKeySequence(csSettings.GetShortcut(qsSHORTCUT__MAIN_BUTTON__VIEW)));
	qpbView->setText(csSettings.GetShortcut(qsSHORTCUT__MAIN_BUTTON__VIEW) + ' ' + qpbView->text());
	qpbEdit->setShortcut(QKeySequence(csSettings.GetShortcut(qsSHORTCUT__MAIN_BUTTON__EDIT)));
	qpbEdit->setText(csSettings.GetShortcut(qsSHORTCUT__MAIN_BUTTON__EDIT) + ' ' + qpbEdit->text());
	qpbCopy->setShortcut(QKeySequence(csSettings.GetShortcut(qsSHORTCUT__MAIN_BUTTON__COPY)));
	qpbCopy->setText(csSettings.GetShortcut(qsSHORTCUT__MAIN_BUTTON__COPY) + ' ' + qpbCopy->text());
	qpbMove->setShortcut(QKeySequence(csSettings.GetShortcut(qsSHORTCUT__MAIN_BUTTON__MOVE)));
	qpbMove->setText(csSettings.GetShortcut(qsSHORTCUT__MAIN_BUTTON__MOVE) + ' ' + qpbMove->text());
	qpbNewDirectory->setShortcut(QKeySequence(csSettings.GetShortcut(qsSHORTCUT__MAIN_BUTTON__NEW_DIRECTORY)));
	qpbNewDirectory->setText(csSettings.GetShortcut(qsSHORTCUT__MAIN_BUTTON__NEW_DIRECTORY) + ' ' + qpbNewDirectory->text());
	qpbDelete->setShortcut(QKeySequence(csSettings.GetShortcut(qsSHORTCUT__MAIN_BUTTON__DELETE)));
	qpbDelete->setText(csSettings.GetShortcut(qsSHORTCUT__MAIN_BUTTON__DELETE) + ' ' + qpbDelete->text());
	// main menu
	qaQuit->setShortcut(QKeySequence(csSettings.GetShortcut(qsSHORTCUT__MAIN_MENU__FILE__QUIT)));
	qaSelectGroup->setShortcut(QKeySequence(csSettings.GetShortcut(qsSHORTCUT__MAIN_MENU__MARK__SELECT_GROUP)));
	qaUnselectGroup->setShortcut(QKeySequence(csSettings.GetShortcut(qsSHORTCUT__MAIN_MENU__MARK__UNSELECT_GROUP)));
	qaSelectAll->setShortcut(QKeySequence(csSettings.GetShortcut(qsSHORTCUT__MAIN_MENU__MARK__SELECT_ALL)));
	qaUnselectAll->setShortcut(QKeySequence(csSettings.GetShortcut(qsSHORTCUT__MAIN_MENU__MARK__UNSELECT_ALL)));
	qaInvertSelection->setShortcut(QKeySequence(csSettings.GetShortcut(qsSHORTCUT__MAIN_MENU__MARK__INVERT_SELECTION)));
	qaSearch->setShortcut(QKeySequence(csSettings.GetShortcut(qsSHORTCUT__MAIN_MENU__COMMANDS__SEARCH)));
	qaFullScreen->setShortcut(QKeySequence(csSettings.GetShortcut(qsSHORTCUT__MAIN_MENU__SHOW__FULLSCREEN)));
	qaRefresh->setShortcut(QKeySequence(csSettings.GetShortcut(qsSHORTCUT__MAIN_MENU__SHOW__REFRESH)));
	qaOptions->setShortcut(QKeySequence(csSettings.GetShortcut(qsSHORTCUT__MAIN_MENU__CONFIGURATION__OPTIONS)));
	qaSavePosition->setShortcut(QKeySequence(csSettings.GetShortcut(qsSHORTCUT__MAIN_MENU__CONFIGURATION__SAVE_POSITION)));
	qaSaveSettings->setShortcut(QKeySequence(csSettings.GetShortcut(qsSHORTCUT__MAIN_MENU__CONFIGURATION__SAVE_SETTINGS)));
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
	qcbCommand->installEventFilter(this);
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
	// file operations class initizalization
	cfoFileOperation = new cFileOperation(this, qhblBackgroundOperations, &csSettings);
	// panels
	cpLeft = new cPanel(this, qswLeft, qcbLeftDrive, qlLeftDriveInfo, &qtbLeft, qlLeftPath, qlLeftSelected, &csSettings, cpPlugins->ccContent, &qmDrives, qlGlobalPath, qcbCommand, cfoFileOperation);
	cpRight = new cPanel(this, qswRight, qcbRightDrive, qlRightDriveInfo, &qtbRight, qlRightPath, qlRightSelected, &csSettings, cpPlugins->ccContent, &qmDrives, qlGlobalPath, qcbCommand, cfoFileOperation);

	ActualizeDrives();
	show();
	// load settings
	// left tabs
	LoadTabs(cSettings::PositionLeft);
	// right tabs
	LoadTabs(cSettings::PositionRight);

	connect(&qtTimer, SIGNAL(timeout()), SLOT(on_qtTimer_timeout()));
	qtTimer.start(iTIMER_INTERVAL);

	// shortcuts
	AssignShortcuts();
	connect(qsLeftDrive, SIGNAL(activated()), SLOT(on_qsLeftDrive_activated()));
	connect(qsRightDrive, SIGNAL(activated()), SLOT(on_qsRightDrive_activated()));

	// set focus to left panel
	static_cast<cTreeWidget *>(qswLeft->currentWidget())->setFocus(Qt::OtherFocusReason);
} // cMainWindow

// event filter
bool cMainWindow::eventFilter(QObject *watched, QEvent *event)
{
	if (watched == qcbCommand) {
		if (event->type() == QEvent::KeyPress) {
			switch (static_cast<QKeyEvent *>(event)->key()) {
				case Qt::Key_Enter:
				case Qt::Key_Return:	cProcess::Execute(qcbCommand->currentText(), qlGlobalPath->text());
											qcbCommand->insertItem(0, qcbCommand->currentText());
											qcbCommand->setCurrentIndex(-1);
											qcbCommand->setEditText("");
											return true;
			} // switch
		} // if
		return false;
	} else {
		return QMainWindow::eventFilter(watched, event);
	} // if else
} // eventFilter

// find active panel (left or right)
QStackedWidget *cMainWindow::GetActivePanel()
{
	if (qswLeft->currentWidget()->hasFocus()) {
		return qswLeft;
	} else {
		return qswRight;
	} // if else
} // GetActivePanel

// load tabs from qsSettings
void cMainWindow::LoadTabs(const cSettings::ePosition &epPosition)
{
	int iI;
	QStringList qslTabs;

	// get tabs
	qslTabs = csSettings.GetTabs(epPosition);

	if (qslTabs.count() == 0) {
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
			cpLeft->AddTab(stiTabInfo);
		} else {
			cpRight->AddTab(stiTabInfo);
		} // if else
	} // for
} // LoadTabs

// full screen mode is selected
void cMainWindow::on_qaFullScreen_triggered(bool checked /* false */)
{
	setWindowState(windowState() ^ Qt::WindowFullScreen);
} // on_qaFullScreen_triggered

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
	cOptionsDialog codOptions(this, &csSettings, cpPlugins->ccContent);

	codOptions.exec();
} // on_qaOptions_triggered

///< refresh is selected
void cMainWindow::on_qaRefresh_triggered(bool checked /* false */)
{
	cPanel *cpSource;

	SetSourceAndDestinationPanel(&cpSource);
	cpSource->Refresh();
} // on_qaRefresh_triggered

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
	cffdFind = new cFindFilesDialog(this, cpSource, &csSettings, cpPlugins->clLister);

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
	cpSource->Select(cSelectFilesDialog::Select, cpPlugins->clLister);
} // on_qaSelectGroup_triggered

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
	cpSource->Select(cSelectFilesDialog::Unselect, cpPlugins->clLister);
} // on_qaUnselectGroup_triggered

// copy button is clicked on
void cMainWindow::on_qpbCopy_clicked(bool checked /* false */)
{
	cPanel *cpDestination, *cpSource;
	QFileInfoList qfilSource;
	QString qsDestination;

	SetSourceAndDestinationPanel(&cpSource, &cpDestination);
	qfilSource = cpSource->GetSelectedItemsList();
	qsDestination = cpDestination->GetPath();
	cfoFileOperation->Operate(cFileRoutine::CopyOperation, qfilSource, qsDestination);
} // on_qpbCopy_clicked

// delete button is clicked on
void cMainWindow::on_qpbDelete_clicked(bool checked /* false */)
{
	cPanel *cpSource;
	QFileInfoList qfilSource;

	SetSourceAndDestinationPanel(&cpSource);
	qfilSource = cpSource->GetSelectedItemsList();
	cfoFileOperation->Operate(cFileRoutine::DeleteOperation, qfilSource);
} // on_qpbDelete_clicked

// edit button is clicked on
void cMainWindow::on_qpbEdit_clicked(bool checked /* false */)
{
	cPanel *cpSource;

	SetSourceAndDestinationPanel(&cpSource);
	cpSource->EditFile();
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
	qfilSource = cpSource->GetSelectedItemsList();
	qsDestination = cpDestination->GetPath();
	cfoFileOperation->Operate(cFileRoutine::MoveOperation, qfilSource, qsDestination);
} // on_qpbMove_clicked

// new directory button is clicked on
void cMainWindow::on_qpbNewDirectory_clicked(bool checked /* false */)
{
	cNewDirectoryDialog cnddNewDirectory(this);

	if (cnddNewDirectory.exec() == QDialog::Accepted) {
		QDir qdDir;
		QString qsNewDirectory;

		if (GetActivePanel() == qswLeft) {
			qsNewDirectory = cpLeft->GetPath();
		} else {
			qsNewDirectory = cpRight->GetPath();
		} // if else
		qsNewDirectory += '/' + cnddNewDirectory.qleDirectory->text();
		qdDir.mkpath(qsNewDirectory);
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
	qfilFiles = cpSource->GetSelectedItemsList();

	for (iI = 0; iI < qfilFiles.count(); iI++) {
		if (qfilFiles.at(iI).isFile()) {
			cListerMainWindow *clmwLister;

			clmwLister = new cListerMainWindow(&csSettings, cpPlugins->clLister, qfilFiles.at(iI).filePath());

			clmwLister->show();
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

// set focused panel as source, other as destination
void cMainWindow::SetSourceAndDestinationPanel(cPanel **cpSource, cPanel **cpDestination /* = NULL */)
{
	if (qswLeft == GetActivePanel()) {
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
	int iI, iTabIndex;
	QAction *qaChoice;
	QTabBar *qtbTabBar;

	// get the tab index clicked on
	if (epTab == cSettings::PositionLeft) {
		qtbTabBar = &qtbLeft;
	} else {
		qtbTabBar = &qtbRight;
	} // if else
	iTabIndex = -1;
	for (iI = 0; iI < qtbTabBar->count(); iI++) {
		if (qtbTabBar->tabRect(iI).contains(qpCursor)) {
			iTabIndex = iI;
			break;
		} // if
	} // for

	if (iTabIndex != -1) {
		cPanel *cpSource;
		QAction *qaTabBarCloseTab, *qaTabBarDuplicateTab;
		QMenu qmTabBar(this);

		// create tab context menu
		qaTabBarDuplicateTab = qmTabBar.addAction(tr("&Duplicate tab"));
		qaTabBarCloseTab = qmTabBar.addAction(tr("&Close tab"));

		// disable close tab if only one tab is available
		if (qtbTabBar->count() == 1) {
			qaTabBarCloseTab->setEnabled(false);
		} else {
			qaTabBarCloseTab->setEnabled(true);
		} // if else

		qaChoice = qmTabBar.exec(QCursor::pos());

		SetSourceAndDestinationPanel(&cpSource);

		if (qaChoice == qaTabBarDuplicateTab) {
			cpSource->DuplicateTab(iTabIndex);
		} else {
			if (qaChoice == qaTabBarCloseTab) {
				cpSource->CloseTab(iTabIndex);
			} // if
		} // if else
	} // if
} // TabBarShowContextMenu