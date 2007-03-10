#include "Plugins/Lister.h"

#include <QLibrary>
#include <QFileInfo>

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

			// add new plugin
			qhPlugins.insert(QFileInfo(qlPlugins.at(iI).qsName).fileName(), spiPluginInfo);
		} // if
	} // for
} // Load