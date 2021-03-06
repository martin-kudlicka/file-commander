#include "Plugins/ListerPlugin.h"

#include <QtCore/QLibrary>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>

const DWORD dwPLUGIN_INTERFACE_VERSION_HI = 1;
const DWORD dwPLUGIN_INTERFACE_VERSION_LOW = 3;

// constructor
cListerPlugin::cListerPlugin(cSettings *csSettings)
{
	this->csSettings = csSettings;
} // cListerPlugin

// retrieve lister plugin info
const QHash<QString, cListerPlugin::sPluginInfo> &cListerPlugin::GetPluginsInfo() const
{
	return qhPlugins;
} // GetPluginsInfo

// loads lister plugins
const void cListerPlugin::Load()
{
	int iI;
	QList<cSettings::sPlugin> qlPlugins;

	// get plugin file list
	qlPlugins = csSettings->GetPlugins(cSettings::ListerPlugins);

	// enumerate plugins
	for (iI = 0; iI < qlPlugins.count(); iI++) {
		const cSettings::sPlugin *spPlugin;

		spPlugin = &qlPlugins.at(iI);
		if (spPlugin->bEnabled) {
			sPluginInfo spiPluginInfo;
			QLibrary qlLibrary;

			// load plugin
			qlLibrary.setFileName(spPlugin->qsName);
			qlLibrary.load();

			// fill plugin properties
			spiPluginInfo.tllListLoad = (tListLoad)qlLibrary.resolve("ListLoad");
			spiPluginInfo.tlcwListCloseWindow = (tListCloseWindow)qlLibrary.resolve("ListCloseWindow");
			spiPluginInfo.tlpListPrint = (tListPrint)qlLibrary.resolve("ListPrint");
			spiPluginInfo.tlstListSearchText = (tListSearchText)qlLibrary.resolve("ListSearchText");
			spiPluginInfo.tlscListSendCommand = (tListSendCommand)qlLibrary.resolve("ListSendCommand");
			spiPluginInfo.tlsdpListSetDefaultParams = (tListSetDefaultParams)qlLibrary.resolve("ListSetDefaultParams");

			// set default parameters
			if (spiPluginInfo.tlsdpListSetDefaultParams) {
				ListDefaultParamStruct ldpsParams;
				QString qsIniFile;

				qsIniFile = QFileInfo(spPlugin->qsName).path() + '/' + QFileInfo(spPlugin->qsName).completeBaseName() + ".ini";
				ldpsParams.PluginInterfaceVersionLow = dwPLUGIN_INTERFACE_VERSION_LOW;
				ldpsParams.PluginInterfaceVersionHi = dwPLUGIN_INTERFACE_VERSION_HI;
				strcpy(ldpsParams.DefaultIniName, QDir::toNativeSeparators(qsIniFile).toAscii().constData());

				spiPluginInfo.tlsdpListSetDefaultParams(&ldpsParams);
			} // if

			// add new plugin
			qhPlugins.insert(QFileInfo(spPlugin->qsName).fileName(), spiPluginInfo);
		} // if
	} // for
} // Load

// unloads lister plugins
const void cListerPlugin::Unload()
{
	qhPlugins.clear();
} // Unload
