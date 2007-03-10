#include "ListerMainWindow.h"

// creates lister window
cListerMainWindow::cListerMainWindow(cSettings *csSettings, cLister *clLister, const QString &qsFile)
{
	// setup GUI
	setupUi(this);

	this->csSettings = csSettings;
	this->clLister = clLister;
	this->qsFile = qsFile;
} // cListerMainWindow