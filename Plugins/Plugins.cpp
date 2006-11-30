#include "Plugins.h"

// loads all plugins
void cPlugins::Load()
{
	ccContent.SetSettings(qsSettings);
	ccContent.Load();
} // Load

// sets main settings "file"
void cPlugins::SetSettings(QSettings *qsSettings)
{
	this->qsSettings = qsSettings;
} // SetSettings