/// plugins handler

#ifndef PLUGINS_H
#define PLUGINS_H

#include "Plugins/ContentPlugin.h"
#include "Settings.h"
#include "Plugins/ListerPlugin.h"
#include "Plugins/PackerPlugin.h"

class cPlugins
{
	public:
		cContentPlugin	*ccpContentPlugin;	///< content plugins
		cListerPlugin *clpListerPlugin;		///< lister plugins
		cPackerPlugin *cppPackerPlugin;		///< packer plugins

		cPlugins(cSettings *csSettings);		///< constructor
														/**< \param csSettings application's settings */
		~cPlugins();								///< destructor

		const void Load() const;				///< loads all plugins
		const void Unload() const;				///< unload all plugins

	private:
		cSettings *csSettings;					///< main settings
}; // cPlugins

#endif
