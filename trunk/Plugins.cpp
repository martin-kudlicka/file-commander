#include "Plugins.h"

// constructor
cPlugins::cPlugins(cSettings *csSettings)
{
	this->csSettings = csSettings;
	ccContent = new cContent(csSettings);
	clLister = new cLister(csSettings);
} // cPlugins

// destructor
cPlugins::~cPlugins()
{
	delete ccContent;
	delete clLister;
} // ~cPlugins

// loads all plugins
void cPlugins::Load()
{
	ccContent->Load();
	clLister->Load();
} // Load