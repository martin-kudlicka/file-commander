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
	cdmLeft.SetContentPlugins(&cpPlugins.ccContent);
	cdmLeft.SetPanel(cDirModel::PanelLeft);
	cdmLeft.SetSettings(&qsSettings);
	cdmRight.SetContentPlugins(&cpPlugins.ccContent);
	cdmRight.SetPanel(cDirModel::PanelRight);
	cdmRight.SetSettings(&qsSettings);

	// load settings
	// main window
	cdmLeft.SetPath(qsSettings.value(qsMAIN_WINDOW__LEFT_PANEL__PATH, cSystem::GetFirstDrive()).toString());
	cdmRight.SetPath(qsSettings.value(qsMAIN_WINDOW__RIGHT_PANEL__PATH, cSystem::GetFirstDrive()).toString());

	// setup GUI
	setupUi(this);
	qtvLeft->setModel(&cdmLeft);
	qtvRight->setModel(&cdmRight);
} // cMainWindow
