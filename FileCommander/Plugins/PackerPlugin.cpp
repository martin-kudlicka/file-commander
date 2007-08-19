#include "Plugins/PackerPlugin.h"

#include <QtCore/QLibrary>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>

const DWORD dwPLUGIN_INTERFACE_VERSION_HI = 2;
const DWORD dwPLUGIN_INTERFACE_VERSION_LOW = 1;

// constructor
cPackerPlugin::cPackerPlugin(cSettings *csSettings)
{
	this->csSettings = csSettings;
} // cPackerPlugin

// retrieve packer plugin info
const QHash<QString, cPackerPlugin::sPluginInfo> &cPackerPlugin::GetPluginsInfo() const
{
	return qhPlugins;
} // GetPluginsInfo

// loads packer plugins
const void cPackerPlugin::Load()
{
	int iI;
	QList<cSettings::sPlugin> qlPlugins;
	QString qsCurrentPath;

	qsCurrentPath = QDir::currentPath();

	// get plugin file list
	qlPlugins = csSettings->GetPlugins(cSettings::PackerPlugins);

	// enumerate plugins
	for (iI = 0; iI < qlPlugins.count(); iI++) {
		const cSettings::sPlugin *spPlugin;

		spPlugin = &qlPlugins.at(iI);
		if (spPlugin->bEnabled) {
			sPluginInfo spiPluginInfo;
			QLibrary qlLibrary;

			// load plugin
			QDir::setCurrent(QFileInfo(spPlugin->qsName).path());
			qlLibrary.setFileName(spPlugin->qsName);
			qlLibrary.load();

			// fill plugin properties
			spiPluginInfo.tcaCloseArchive = (tCloseArchive)qlLibrary.resolve("CloseArchive");
			spiPluginInfo.toaOpenArchive = (tOpenArchive)qlLibrary.resolve("OpenArchive");
			spiPluginInfo.tpfProcessFile = (tProcessFile)qlLibrary.resolve("ProcessFile");
			spiPluginInfo.trhReadHeader = (tReadHeader)qlLibrary.resolve("ReadHeader");
#ifdef Q_WS_WIN
			spiPluginInfo.tspdpSetProcessDataProc = (tSetProcessDataProc)qlLibrary.resolve("SetProcessDataProc");
#endif

			spiPluginInfo.tpsdpPackSetDefaultParams = (tPackSetDefaultParams)qlLibrary.resolve("PackSetDefaultParams");
			// set default parameters
			if (spiPluginInfo.tpsdpPackSetDefaultParams) {
				PackDefaultParamStruct pdpsParams;
				QString qsIniFile;

				qsIniFile = QFileInfo(spPlugin->qsName).path() + '/' + QFileInfo(spPlugin->qsName).completeBaseName() + ".ini";
				pdpsParams.PluginInterfaceVersionLow = dwPLUGIN_INTERFACE_VERSION_LOW;
				pdpsParams.PluginInterfaceVersionHi = dwPLUGIN_INTERFACE_VERSION_HI;
				strcpy(pdpsParams.DefaultIniName, QDir::toNativeSeparators(qsIniFile).toAscii().constData());

				spiPluginInfo.tpsdpPackSetDefaultParams(&pdpsParams);
			} // if

			// add new plugin
			qhPlugins.insert(QFileInfo(spPlugin->qsName).fileName(), spiPluginInfo);
		} // if
	} // for

	QDir::setCurrent(qsCurrentPath);
} // Load
