#include "MainWindow.h"
#include "Common/System.h"

// create of main window
cMainWindow::cMainWindow()
{
	QVBoxLayout *qvblTabBar;

	// load plugins
	cpPlugins.csSettings = &csSettings;
	cpPlugins.Load();

	// setup GUI
	setupUi(this);
	// add left tab
	qvblTabBar = static_cast<QVBoxLayout *>(qswLeft->parentWidget()->layout());
	qvblTabBar->insertWidget(iTAB_POS, &qtbLeft);
	// add right tab
	qvblTabBar = static_cast<QVBoxLayout *>(qswRight->parentWidget()->layout());
	qvblTabBar->insertWidget(iTAB_POS, &qtbRight);
	// remove default widgets
	qswLeft->removeWidget(qswLeft->widget(0));
	qswRight->removeWidget(qswRight->widget(0));

	// set variables
	// left panel
	cpLeft = new cPanel(qswLeft);
	cpLeft->qcbDrive = qcbLeftDrive;
	cpLeft->qlDriveInfo = qlLeftDriveInfo;
	cpLeft->qtbTab = &qtbLeft;
	cpLeft->qlPath = qlLeftPath;
	cpLeft->qlSelected = qlLeftSelected;
	cpLeft->csSettings = &csSettings;
	cpLeft->ccContent = &cpPlugins.ccContent;
	// right panel
	cpRight = new cPanel(qswRight);
	cpRight->qcbDrive = qcbRightDrive;
	cpRight->qlDriveInfo = qlRightDriveInfo;
	cpRight->qtbTab = &qtbRight;
	cpRight->qlPath = qlRightPath;
	cpRight->qlSelected = qlRightSelected;
	cpRight->csSettings = &csSettings;
	cpRight->ccContent = &cpPlugins.ccContent;

	// load settings
	// left tabs
	LoadTabs(cSettings::PositionLeft);
	// right tabs
	LoadTabs(cSettings::PositionRight);
} // cMainWindow

// load tabs from qsSettings
void cMainWindow::LoadTabs(const cSettings::ePosition epPosition)
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