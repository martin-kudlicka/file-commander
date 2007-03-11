/// lister plugins

#ifndef LISTER_H
#define LISTER_H

#include "Settings.h"
#include "Plugins/ListPlug.h"
#include <QHash>

class cLister
{
	public:
		typedef void (__stdcall *tListCloseWindow)(HWND hwListWin);	///< close plugin's window function
																						/**< \param hwListWin plugin's window handle */
		typedef HWND (__stdcall *tListLoad)(HWND hwParentWin, char *cFileToLoad, int iShowFlags);
																						///< plugin's ListLoad function
																						/**< \param hwParentWin lister's window
																							  \param cFileToLoad file content to show
																							  \param iShowFlags lister's flags
																							  \return plugin's window handle */
		typedef void (__stdcall *tListSetDefaultParams)(ListDefaultParamStruct* ldpsParams);
																						///< set default parameters for plugin
																						/**< \param ldpsParams default parameters */ 

		/// plugin info
		struct sPluginInfo {
			tListCloseWindow tlcwListCloseWindow;							///< ListCloseWindow function
			tListLoad tllListLoad;												///< ListLoad function
			tListSetDefaultParams tlsdpListSetDefaultParams;			///< ListSetDefaultParams function
		};

		cLister(cSettings *csSettings);										///< constructor
																						/**< \param csSettings application's settings */

		QHash<QString, sPluginInfo> GetPluginsInfo();					///< retrieve lister plugin info
		void Load();																///< loads lister plugins

	private:
		cSettings *csSettings;													///< main settings "file"
		QHash<QString, sPluginInfo> qhPlugins;								///< table of plugins
																						/**< key is plugin name, value contains plugin's info */
}; // cLister

#endif