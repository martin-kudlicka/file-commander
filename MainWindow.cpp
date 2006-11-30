#include "MainWindow.h"

#include "Settings.h"
#include "Common/System.h"

// create of main window
cMainWindow::cMainWindow()
{
	// load plugins
	cpPlugins.SetSettings(&qsSettings);
	cpPlugins.Load();

	// set variables
	cdtLeft.etpPanel = cDirTree::TreePanelLeft;
	cdtLeft.ccContent = &cpPlugins.ccContent;
	cdtLeft.qsSettings = &qsSettings;
	cdtRight.etpPanel = cDirTree::TreePanelRight;
	cdtRight.ccContent = &cpPlugins.ccContent;
	cdtRight.qsSettings = &qsSettings;

	// load settings
	// main window
	cdtLeft.SetPath(qsSettings.value(qsMAIN_WINDOW__LEFT_PANEL__PATH, cSystem::GetFirstDrive()).toString());
	cdtRight.SetPath(qsSettings.value(qsMAIN_WINDOW__RIGHT_PANEL__PATH, cSystem::GetFirstDrive()).toString());

	// setup GUI
	setupUi(this);
	cdtLeft.qtwTreeWidget = qtwLeftPanel;
	cdtLeft.RefreshHeader();
	cdtRight.qtwTreeWidget = qtwRightPanel;
	cdtRight.RefreshHeader();
} // cMainWindow
