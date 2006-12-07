/// plugins handler

#ifndef PLUGINS_H
#define PLUGINS_H

#include "Plugins/Content.h"
#include "Settings.h"

class cPlugins
{
	public:
		cContent	*ccContent;					///< content plugins

		cPlugins(cSettings *csSettings);	///< constructor
													/**< \param csSettings application's settings */
		~cPlugins();							///< destructor

		void Load();							///< loads all plugins

	private:
		cSettings *csSettings;				///< main settings
}; // cPlugins

#endif