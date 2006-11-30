/// plugins handler

#ifndef PLUGINS_H
#define PLUGINS_H

#include "Plugins/Content.h"
#include "Settings.h"

class cPlugins
{
	public:
		cContent	ccContent;		///< content plugins
		cSettings *csSettings;	///< main settings

		void Load();				///< loads all plugins
}; // cPlugins

#endif