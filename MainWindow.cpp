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
	delete qsLeftDrive;
	delete qsRightDrive;
	delete cfoFileOperation;
	delete qhblBackgroundOperations;
	delete cpLeft;
	delete cpRight;
	delete cpPlugins;
	ccbCommand->deleteLater();
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
	qvblTabBar = static_cast<QVBoxLayout *>(qswLeft->parentWidget()->layout());
	qvblTabBar->insertWidget(iTAB_POS, &qtbLeft);
	qtbLeft.setFocusPolicy(Qt::NoFocus);
	// add right tab
	qvblTabBar = static_cast<QVBoxLayout *>(qswRight->parentWidget()->layout());
	qvblTabBar->insertWidget(iTAB_POS, &qtbRight);
	qtbRight.setFocusPolicy(Qt::NoFocus);
	// add command combo box
	ccbCommand = new cComboBox();
	ccbCommand->setEditable(true);
	ccbCommand->setFocusPolicy(Qt::NoFocus);
	connect(ccbCommand, SIGNAL(KeyPressed(QKeyEvent *)), SLOT(on_ccbCommand_KeyPressed(QKeyEvent *)));
	static_cast<QHBoxLayout *>(centralwidget->layout()->itemAt(1))->addWidget(ccbCommand);
	// remove default widgets
	qswLeft->removeWidget(qswLeft->widget(0));
	qswRight->removeWidget(qswRight->widget(0));
	// panels
	cpLeft = new cPanel(this, qswLeft, qcbLeftDrive, qlLeftDriveInfo, &qtbLeft, qlLeftPath, qlLeftSelected, &csSettings, cpPlugins->ccContent, &qmDrives, qlGlobalPath, ccbCommand);
	cpRight = new cPanel(this, qswRight, qcbRightDrive, qlRightDriveInfo, &qtbRight, qlRightPath, qlRightSelected, &csSettings, cpPlugins->ccContent, &qmDrives, qlGlobalPath, ccbCommand);

	ActualizeDrives();
	// load settings
	// left tabs
	LoadTabs(cSettings::PositionLeft);
	// right tabs
	LoadTabs(cSettings::PositionRight);

	// initialize variables
	// background operations layout
	qhblBackgroundOperations = new QHBoxLayout(qpbCopy->parentWidget()->layout()->parentWidget());
	static_cast<QVBoxLayout *>(qpbCopy->parentWidget()->layout()->parentWidget()->layout())->insertLayout(0, qhblBackgroundOperations);
	cfoFileOperation = new cFileOperation(this, qhblBackgroundOperations, &csSettings);

	connect(&qtTimer, SIGNAL(timeout()), SLOT(on_qtTimer_timeout()));
	qtTimer.start(iTIMER_INTERVAL);

	// shortcuts
	qsLeftDrive = new QShortcut(QKeySequence("Alt+F1"), this);
	connect(qsLeftDrive, SIGNAL(activated()), SLOT(on_qsLeftDrive_activated()));
	qsRightDrive = new QShortcut(QKeySequence("Alt+F2"), this);
	connect(qsRightDrive, SIGNAL(activated()), SLOT(on_qsRightDrive_activated()));
} // cMainWindow

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

// command confirmed
void cMainWindow::on_ccbCommand_KeyPressed(QKeyEvent *qkeEvent)
{
	if (qkeEvent->key() == Qt::Key_Enter) {
		cProcess::Execute(ccbCommand->currentText(), qlGlobalPath->text());
	} // if
} // on_ccbCommand_KeyPressed

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

// search selected
void cMainWindow::on_qaSearch_triggered(bool checked /* false */)
{
	cFindFilesDialog *cffdFind;
	cPanel *cpSource;
	QFileInfoList qfilDirectories;

	SetSourceAndDestinationPanel(&cpSource);
	qfilDirectories = cpSource->GetSelectedDirItemsList();
	cffdFind = new cFindFilesDialog(this, cpSource, qfilDirectories, &csSettings, cpPlugins->clLister);

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

// unselect all selected
void cMainWindow::on_qaUnselectAll_triggered(bool checked /* false */)
{
	cPanel *cpSource;

	SetSourceAndDestinationPanel(&cpSource);
	cpSource->UnselectAll();
} // on_qaUnselectAll_triggered

// copy button is clicked on
void cMainWindow::on_qpbCopy_clicked(bool checked /* false */)
{
	cPanel *cpDestination, *cpSource;

	SetSourceAndDestinationPanel(&cpSource, &cpDestination);
	cfoFileOperation->Operate(cFileRoutine::CopyOperation, cpSource, cpDestination);
} // on_qpbCopy_clicked

// delete button is clicked on
void cMainWindow::on_qpbDelete_clicked(bool checked /* false */)
{
	cPanel *cpSource;

	SetSourceAndDestinationPanel(&cpSource);
	cfoFileOperation->Operate(cFileRoutine::DeleteOperation, cpSource);
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

	SetSourceAndDestinationPanel(&cpSource, &cpDestination);
	cfoFileOperation->Operate(cFileRoutine::MoveOperation, cpSource, cpDestination);
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
	cPanel *cSource;
	int iI;
	QFileInfoList qfilFiles;

	SetSourceAndDestinationPanel(&cSource);
	qfilFiles = cSource->GetSelectedItemsList();

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

///< timer's timeout
void cMainWindow::on_qtTimer_timeout()
{
	// check for new or removed drives
	ActualizeDrives();
} // on_qtTimer_timeout

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