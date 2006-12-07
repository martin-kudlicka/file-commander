#include "Plugins.h"

// constructor
cPlugins::cPlugins(cSettings *csSettings)
{
	this->csSettings = csSettings;
	ccContent = new cContent(csSettings);
} // cPlugins

// destructor
cPlugins::~cPlugins()
{
	delete ccContent;
} // ~cPlugins

// loads all plugins
void cPlugins::Load()
{
	ccContent->Load();
} // Load