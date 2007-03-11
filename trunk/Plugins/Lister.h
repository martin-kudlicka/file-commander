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
		typedef int (__stdcall *tListSearchText)(HWND hwListWin, char *cSearchString, int iSearchParameter);
																						///< search text in plugin
																						/**< \param hwListWin plugin's window
																							  \param cSearchString string to be searched
																							  \param iSearchParameter search flags
																							  \return ok or error */
		typedef int (__stdcall *tListSendCommand)(HWND hwListWin, int iCommand, int iParameter);
																						///< send command to lister plugin
																						/**< \param hwListWin plugin's window
																							  \param iCommand command to send
																							  \param iParameter command's parameter
																							  \return error or ok */
		typedef void (__stdcall *tListSetDefaultParams)(ListDefaultParamStruct* ldpsParams);
																						///< set default parameters for plugin
																						/**< \param ldpsParams default parameters */ 

		/// plugin info
		struct sPluginInfo {
			tListCloseWindow tlcwListCloseWindow;							///< ListCloseWindow function
			tListLoad tllListLoad;												///< ListLoad function
			tListSearchText tlstListSearchText;								///< ListSearchText function
			tListSendCommand tlscListSendCommand;							///< ListSendCommand function
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