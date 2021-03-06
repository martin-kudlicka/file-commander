/// lister plugins

#ifndef LISTER_PLUGIN_H
#define LISTER_PLUGIN_H

#include "Settings.h"
#include "Plugins/ListPlug.h"
#include <QtCore/QHash>

class cListerPlugin
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
		typedef int (__stdcall *tListPrint)(HWND hwListWin, char *cFileToPrint, char *cDefPrinter, int iPrintFlags, RECT *rMargins);
																						///< print file shown in plugin's window
																						/**< \param hwListWin plugin's window
																							  \param cFileToPrint file name content to be printed
																							  \param cDefPrinter default printer to use
																							  \param iPrintFlags currently not used
																							  \param rMargins margins of print area
																							  \return ok or error */
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
			tListPrint tlpListPrint;											///< ListPrint function
			tListSearchText tlstListSearchText;								///< ListSearchText function
			tListSendCommand tlscListSendCommand;							///< ListSendCommand function
			tListSetDefaultParams tlsdpListSetDefaultParams;			///< ListSetDefaultParams function
		};

		cListerPlugin(cSettings *csSettings);								///< constructor
																						/**< \param csSettings application's settings */

		const QHash<QString, sPluginInfo> &GetPluginsInfo() const;	///< retrieve lister plugins info
		const void Load();														///< loads lister plugins
		const void Unload();														///< unloads lister plugins

	private:
		cSettings *csSettings;													///< main settings "file"
		QHash<QString, sPluginInfo> qhPlugins;								///< table of plugins
																						/**< key is plugin name, value contains plugin's info */
}; // cListerPlugin

#endif
