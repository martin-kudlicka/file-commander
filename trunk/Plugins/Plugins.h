/// plugins handler

#ifndef PLUGINS_H
#define PLUGINS_H

#include "Plugins/Content.h"
#include <QSettings>

class cPlugins
{
	public:
		cContent	ccContent;								///< content plugins

		void Load();										///< loads all plugins
		void SetSettings(QSettings *qsSettings);	///< sets main settings "file"
																/**< \param qsSettings settings "file" */

	private:
		QSettings *qsSettings;							///< main settings "file"
}; // cPlugins

#endif