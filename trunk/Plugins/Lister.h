/// lister plugins

#ifndef LISTER_H
#define LISTER_H

#include "Settings.h"
#include "Plugins/ListPlug.h"
#include <QHash>

class cLister
{
	public:
		typedef HWND (__stdcall *tListLoad)(HWND hwParentWin, char *cFileToLoad, int iShowFlags);
															///< plugin's ListLoad function
															/**< \param hwParentWin lister's window
																  \param cFileToLoad file content to show
																  \param iShowFlags lister's flags
																  \return plugin's window handle */

		/// plugin info
		struct sPluginInfo {
			tListLoad tllListLoad;					///< ListLoad function
		};

		cLister(cSettings *csSettings);			///< constructor
															/**< \param csSettings application's settings */

		void Load();									///< loads lister plugins

	private:
		cSettings *csSettings;						///< main settings "file"
		QHash<QString, sPluginInfo> qhPlugins;	///< table of plugins
															/**< key is plugin name, value contains plugin's info */
}; // cLister

#endif