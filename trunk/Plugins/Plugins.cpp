#include "Plugins.h"

// loads all plugins
void cPlugins::Load()
{
	ccContent.csSettings = csSettings;
	ccContent.Load();
} // Load