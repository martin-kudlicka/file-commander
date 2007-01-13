#include "MainWindow.h"

#include "Common/System.h"

// create of main window
cMainWindow::cMainWindow()
{
	QVBoxLayout *qvblTabBar;

	// load plugins
	cpPlugins = new cPlugins(&csSettings);
	cpPlugins->Load();

	// setup GUI
	setupUi(this);
	// add left tab
	qvblTabBar = static_cast<QVBoxLayout *>(qswLeft->parentWidget()->layout());
	qvblTabBar->insertWidget(iTAB_POS, &qtbLeft);
	qtbLeft.setFocusPolicy(Qt::NoFocus);
	// add right tab
	qvblTabBar = static_cast<QVBoxLayout *>(qswRight->parentWidget()->layout());
	qvblTabBar->insertWidget(iTAB_POS, &qtbRight);
	qtbRight.setFocusPolicy(Qt::NoFocus);
	// remove default widgets
	qswLeft->removeWidget(qswLeft->widget(0));
	qswRight->removeWidget(qswRight->widget(0));
	// panels
	cpLeft = new cPanel(qswLeft, qcbLeftDrive, qlLeftDriveInfo, &qtbLeft, qlLeftPath, qlLeftSelected, &csSettings, cpPlugins->ccContent);
	cpRight = new cPanel(qswRight, qcbRightDrive, qlRightDriveInfo, &qtbRight, qlRightPath, qlRightSelected, &csSettings, cpPlugins->ccContent);

	// load settings
	// left tabs
	LoadTabs(cSettings::PositionLeft);
	// right tabs
	LoadTabs(cSettings::PositionRight);

	// initialize variables
	// background operations layout
	qhblBackgroundOperations = new QHBoxLayout();
	static_cast<QVBoxLayout *>(qpbCopy->parentWidget()->layout()->parentWidget()->layout())->insertLayout(0, qhblBackgroundOperations);
	cfoFileOperation = new cFileOperation(this, qhblBackgroundOperations);
} // cMainWindow

// destructor
cMainWindow::~cMainWindow()
{
	delete cfoFileOperation;
	delete qhblBackgroundOperations;
	delete cpLeft;
	delete cpRight;
	delete cpPlugins;
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
		csSettings.CreateTab(epPosition, 0, qsFULL, cSystem::GetFirstDrive());
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

// delete button is clicked on
void cMainWindow::on_qpbDelete_clicked(bool checked /* false */)
{
	cPanel *cpSource;

	SetSourceAndDestinationPanel(&cpSource);
	cfoFileOperation->Operate(cFileRoutine::DeleteOperation, cpSource);
} // on_qpbDelete_clicked

// copy button is clicked on
void cMainWindow::on_qpbCopy_clicked(bool checked /* false */)
{
	cPanel *cpDestination, *cpSource;

	SetSourceAndDestinationPanel(&cpSource, &cpDestination);
	cfoFileOperation->Operate(cFileRoutine::CopyOperation, cpSource, cpDestination);
} // on_qpbCopy_clicked

// move button is clicked on
void cMainWindow::on_qpbMove_clicked(bool checked /* false */)
{
	cPanel *cpDestination, *cpSource;

	SetSourceAndDestinationPanel(&cpSource, &cpDestination);
	cfoFileOperation->Operate(cFileRoutine::MoveOperation, cpSource, cpDestination);
} // on_qpbMove_clicked

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