/// packer plugins

#ifndef PACKER_PLUGIN_H
#define PACKER_PLUGIN_H

#include "Settings.h"
#include "Plugins/WCXHead.h"
#include <QtCore/QHash>
#include <QtCore/QLibrary>

class cPackerPlugin
{
	public:
		typedef int (__stdcall *tCloseArchive)(HANDLE hArcData);										///< close archive
																														/**< \param hArcData handle representing archive
																															  \return error value */
		typedef int (__stdcall *tGetPackerCaps)();														///< plugin's capabilities
																														/**< \return plugin's capabilities */
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
#ifdef Q_WS_WIN
		typedef void (__stdcall *tSetProcessDataProc)(HANDLE hArcData, tProcessDataProc tpdpProcessDataProc);
																														///< set progress callback function
																														/**< \param hArcData handle representing archive
																															  \param tpdpProcessDataProc callback function */
#endif

		// file attributes
		static const int iREAD_ONLY = 0x1;																	///< read only file
		static const int iHIDDEN = 0x2;																		///< hidden file
		static const int iSYSTEM = 0x4;																		///< system file
		static const int iVOLUME_ID = 0x8;																	///< volume ID file
		static const int iDIRECTORY = 0x10;																	///< directory
		static const int iARCHIVE = 0x20;																	///< archive file
		static const int iANY = 0x3F;																			///< any file

		/// plugin info
		struct sPluginInfo {
			tCloseArchive tcaCloseArchive;																	///< CloseArchive function
			tGetPackerCaps tgpcGetPackerCaps;																///< GetPackerCaps function
			tOpenArchive toaOpenArchive;																		///< OpenArchive function
			tPackSetDefaultParams tpsdpPackSetDefaultParams;											///< PackSetDefaultParams function
			tProcessFile tpfProcessFile;																		///< ProcessFile function
			tReadHeader trhReadHeader;																			///< ReadHeader function
#ifdef Q_WS_WIN
			tSetProcessDataProc tspdpSetProcessDataProc;													///< SetProcessDataProc function
#endif
			int iCapabilities;																					///< plugin's capabilities
			QLibrary *qlLibrary;																					///< library identifying this plugin
		};

		cPackerPlugin(cSettings *csSettings);																///< constructor
																														/**< \param csSettings application's settings */
		~cPackerPlugin();																							///< destructor

		QHash<QString, sPluginInfo> *GetPluginsInfo();													///< retrieve packer plugins info
		const void Load();																						///< loads packer plugins
		const sPluginInfo LoadPlugin(const QString &qsName) const;									///< load packer plugin
																														/**< \param qsName file path to plugin
																															  \return plugin's description */
		const void cPackerPlugin::Unload();																	///< unloads packer plugins

	private:
		cSettings *csSettings;																					///< main settings "file"
		QHash<QString, sPluginInfo> qhPlugins;																///< table of plugins
																														/**< key is plugin name, value contains plugin's info */
}; // cPackerPlugin

#endif
