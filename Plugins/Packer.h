/// packer plugins

#ifndef PACKER_H
#define PACKER_H

#include "Settings.h"
#include "Plugins/WCXHead.h"
#include <QtCore/QHash>

class cPacker
{
	public:
		typedef int (__stdcall *tCloseArchive)(HANDLE hArcData);										///< close archive
																														/**< \param hArcData handle representing archive
																															  \return error value */
		typedef HANDLE (__stdcall *tOpenArchive)(tOpenArchiveData *toadArchiveData);			///< open archive
																														/**< \param toadArchiveData archive open mode
																															  \return handle representing archive */
		typedef void (__stdcall *tPackSetDefaultParams)(PackDefaultParamStruct *pdpsParams);
																														///< set parameters for plugin
																														/**< \param pdpsParams plugin's parameters */
		typedef int (__stdcall *tProcessFile)(HANDLE hArcData, int iOperation, char *cDestPath, char *cDestName);
																														///< unpack or test file in archive
																														/**< \param hArcData handle representing archive
																															  \param iOperation operation type
																															  \param cDestPath destination filepath
																															  \param cDestName destination filename or filepath
																															  \return error value */
		typedef int (__stdcall *tReadHeader)(HANDLE hArcData, tHeaderData *thdHeaderData);	///< read header to find out files in archive
																														/**< \param hArcData handle representing archive
																															  \param thdHeaderData information about next file
																															  \return error value */

		/// plugin info
		struct sPluginInfo {
			tCloseArchive tcaCloseArchive;																	///< CloseArchive function
			tOpenArchive toaOpenArchive;																		///< OpenArchive function
			tPackSetDefaultParams tpsdpPackSetDefaultParams;											///< PackSetDefaultParams function
			tProcessFile tpfProcessFile;																		///< ProcessFile function
			tReadHeader trhReadHeader;																			///< ReadHeader function
		};

		cPacker(cSettings *csSettings);																		///< constructor
																														/**< \param csSettings application's settings */

		//QHash<QString, sPluginInfo> GetPluginsInfo();													///< retrieve packer plugins info
		void Load();																								///< loads packer plugins

	private:
		cSettings *csSettings;																					///< main settings "file"
		QHash<QString, sPluginInfo> qhPlugins;																///< table of plugins
																														/**< key is plugin name, value contains plugin's info */
}; // cPacker

#endif