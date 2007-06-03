#include "Plugins.h"

// constructor
cPlugins::cPlugins(cSettings *csSettings)
{
	this->csSettings = csSettings;
	ccContent = new cContent(csSettings);
	clLister = new cLister(csSettings);
	cpPacker = new cPacker(csSettings);
} // cPlugins

// destructor
cPlugins::~cPlugins()
{
	delete ccContent;
	delete clLister;
	delete cpPacker;
} // ~cPlugins

// loads all plugins
void cPlugins::Load()
{
	ccContent->Load();
	clLister->Load();
	cpPacker->Load();
} // Load

// unload all plugins
void cPlugins::Unload()
{
	ccContent->Unload();
} // Unload
