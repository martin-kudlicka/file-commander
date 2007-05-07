/// plugins handler

#ifndef PLUGINS_H
#define PLUGINS_H

#include "Plugins/Content.h"
#include "Settings.h"
#include "Plugins/Lister.h"

class cPlugins
{
	public:
		cContent	*ccContent;					///< content plugins
		cLister *clLister;					///< lister plugins

		cPlugins(cSettings *csSettings);	///< constructor
													/**< \param csSettings application's settings */
		~cPlugins();							///< destructor

		void Load();							///< loads all plugins
		void Unload();							///< unload all plugins

	private:
		cSettings *csSettings;				///< main settings
}; // cPlugins

#endif