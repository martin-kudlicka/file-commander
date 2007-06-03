#include "Plugins/Lister.h"

#include <QtCore/QLibrary>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>

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
			spiPluginInfo.tllListLoad = static_cast<tListLoad>(qlLibrary.resolve("ListLoad"));
			spiPluginInfo.tlcwListCloseWindow = static_cast<tListCloseWindow>(qlLibrary.resolve("ListCloseWindow"));
			spiPluginInfo.tlpListPrint = static_cast<tListPrint>(qlLibrary.resolve("ListPrint"));
			spiPluginInfo.tlstListSearchText = static_cast<tListSearchText>(qlLibrary.resolve("ListSearchText"));
			spiPluginInfo.tlscListSendCommand = static_cast<tListSendCommand>(qlLibrary.resolve("ListSendCommand"));
			spiPluginInfo.tlsdpListSetDefaultParams = static_cast<tListSetDefaultParams>(qlLibrary.resolve("ListSetDefaultParams"));

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