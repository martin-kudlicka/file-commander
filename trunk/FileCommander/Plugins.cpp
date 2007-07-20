#include "Plugins.h"

// constructor
cPlugins::cPlugins(cSettings *csSettings)
{
	this->csSettings = csSettings;
	ccpContentPlugin = new cContentPlugin(csSettings);
	clpListerPlugin = new cListerPlugin(csSettings);
	cppPackerPlugin = new cPackerPlugin(csSettings);
} // cPlugins

// destructor
cPlugins::~cPlugins()
{
	delete ccpContentPlugin;
	delete clpListerPlugin;
	delete cppPackerPlugin;
} // ~cPlugins

// loads all plugins
const void cPlugins::Load() const
{
	ccpContentPlugin->Load();
	clpListerPlugin->Load();
	cppPackerPlugin->Load();
} // Load

// unload all plugins
const void cPlugins::Unload() const
{
	ccpContentPlugin->Unload();
	clpListerPlugin->Unload();
} // Unload
