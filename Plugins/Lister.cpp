#include "Plugins/Lister.h"

#include <QLibrary>
#include <QFileInfo>
#include <QDir>

const DWORD dwPLUGIN_INTERFACE_VERSION_HI = 1;
const DWORD dwPLUGIN_INTERFACE_VERSION_LOW = 3;

// constructor
cLister::cLister(cSettings *csSettings)
{
	this->csSettings = csSettings;
} // cLister

// retrieve lister plugin info
QHash<QString, cLister::sPluginInfo> cLister::GetPluginsInfo()
{
	return qhPlugins;
} // GetPluginsInfo

// loads lister plugins
void cLister::Load()
{
	int iI;
	QList<cSettings::sPlugin> qlPlugins;

	// get plugin file list
	qlPlugins = csSettings->GetPlugins(cSettings::ListerPlugins);

	// enumerate plugins
	for (iI = 0; iI < qlPlugins.count(); iI++) {
		if (qlPlugins.at(iI).bEnabled) {
			sPluginInfo spiPluginInfo;
			QLibrary qlLibrary;

			// load plugin
			qlLibrary.setFileName(qlPlugins.at(iI).qsName);
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

				qsIniFile = QFileInfo(qlPlugins.at(iI).qsName).path() + '/' + QFileInfo(qlPlugins.at(iI).qsName).completeBaseName() + ".ini";
				ldpsParams.PluginInterfaceVersionLow = dwPLUGIN_INTERFACE_VERSION_LOW;
				ldpsParams.PluginInterfaceVersionHi = dwPLUGIN_INTERFACE_VERSION_HI;
				strcpy(ldpsParams.DefaultIniName, QDir::toNativeSeparators(qsIniFile).toAscii().constData());

				spiPluginInfo.tlsdpListSetDefaultParams(&ldpsParams);
			} // if

			// add new plugin
			qhPlugins.insert(QFileInfo(qlPlugins.at(iI).qsName).fileName(), spiPluginInfo);
		} // if
	} // for
} // Load

// unloads lister plugins
void cLister::Unload()
{
	qhPlugins.clear();
} // Unload