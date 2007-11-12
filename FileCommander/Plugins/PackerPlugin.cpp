#include "Plugins/PackerPlugin.h"

#include <QtCore/QFileInfo>
#include <QtCore/QDir>

const DWORD dwPLUGIN_INTERFACE_VERSION_HI = 2;
const DWORD dwPLUGIN_INTERFACE_VERSION_LOW = 1;

// destructor
cPackerPlugin::~cPackerPlugin()
{
	Unload();
} // ~cPackerPlugin

// constructor
cPackerPlugin::cPackerPlugin(cSettings *csSettings)
{
	this->csSettings = csSettings;
} // cPackerPlugin

// retrieve packer plugin info
QHash<QString, cPackerPlugin::sPluginInfo> *cPackerPlugin::GetPluginsInfo()
{
	return &qhPlugins;
} // GetPluginsInfo

// loads packer plugins
const void cPackerPlugin::Load()
{
	int iI;
	QList<cSettings::sPlugin> qlPlugins;

	// get plugin file list
	qlPlugins = csSettings->GetPlugins(cSettings::PackerPlugins);

	// enumerate plugins
	for (iI = 0; iI < qlPlugins.count(); iI++) {
		const cSettings::sPlugin *spPlugin;

		spPlugin = &qlPlugins.at(iI);
		if (spPlugin->bEnabled) {
			sPluginInfo spiPluginInfo;

			// load plugin
			spiPluginInfo = LoadPlugin(spPlugin->qsName);

			// add new plugin
			qhPlugins.insert(QFileInfo(spPlugin->qsName).fileName(), spiPluginInfo);
		} // if
	} // for
} // Load

// load packer plugin
const cPackerPlugin::sPluginInfo cPackerPlugin::LoadPlugin(const QString &qsName) const
{
	QString qsCurrentPath;
	sPluginInfo spiPluginInfo;

	spiPluginInfo.qlLibrary = new QLibrary();

	qsCurrentPath = QDir::currentPath();
	QDir::setCurrent(QFileInfo(qsName).path());
	spiPluginInfo.qlLibrary->setFileName(qsName);
	spiPluginInfo.qlLibrary->load();

	// fill plugin properties
	spiPluginInfo.tcaCloseArchive = (tCloseArchive)spiPluginInfo.qlLibrary->resolve("CloseArchive");
	spiPluginInfo.tcpConfigurePacker = (tConfigurePacker)spiPluginInfo.qlLibrary->resolve("ConfigurePacker");
	spiPluginInfo.tdfDeleteFiles = (tDeleteFiles)spiPluginInfo.qlLibrary->resolve("DeleteFiles");
	spiPluginInfo.tgpcGetPackerCaps = (tGetPackerCaps)spiPluginInfo.qlLibrary->resolve("GetPackerCaps");
	spiPluginInfo.toaOpenArchive = (tOpenArchive)spiPluginInfo.qlLibrary->resolve("OpenArchive");
	spiPluginInfo.tpfPackFiles = (tPackFiles)spiPluginInfo.qlLibrary->resolve("PackFiles");
	spiPluginInfo.tpsdpPackSetDefaultParams = (tPackSetDefaultParams)spiPluginInfo.qlLibrary->resolve("PackSetDefaultParams");
	spiPluginInfo.tpfProcessFile = (tProcessFile)spiPluginInfo.qlLibrary->resolve("ProcessFile");
	spiPluginInfo.trhReadHeader = (tReadHeader)spiPluginInfo.qlLibrary->resolve("ReadHeader");
#ifdef Q_WS_WIN
	spiPluginInfo.tspdpSetProcessDataProc = (tSetProcessDataProc)spiPluginInfo.qlLibrary->resolve("SetProcessDataProc");
#endif

	// get plugin's capabilites
	if (spiPluginInfo.tgpcGetPackerCaps) {
		spiPluginInfo.iCapabilities = spiPluginInfo.tgpcGetPackerCaps();
	} else {
		spiPluginInfo.iCapabilities = 0;
	} // if else
	// set default parameters
	if (spiPluginInfo.tpsdpPackSetDefaultParams) {
		PackDefaultParamStruct pdpsParams;
		QString qsIniFile;

		qsIniFile = QFileInfo(qsName).path() + '/' + QFileInfo(qsName).completeBaseName() + ".ini";
		pdpsParams.PluginInterfaceVersionLow = dwPLUGIN_INTERFACE_VERSION_LOW;
		pdpsParams.PluginInterfaceVersionHi = dwPLUGIN_INTERFACE_VERSION_HI;
		strcpy(pdpsParams.DefaultIniName, QDir::toNativeSeparators(qsIniFile).toAscii().constData());

		spiPluginInfo.tpsdpPackSetDefaultParams(&pdpsParams);
	} // if

	QDir::setCurrent(qsCurrentPath);

	return spiPluginInfo;
} // LoadPlugin

// unloads packer plugins
const void cPackerPlugin::Unload()
{
	QHashIterator<QString, sPluginInfo> qhiPlugin(qhPlugins);
	while (qhiPlugin.hasNext()) {
		qhiPlugin.next();

		qhiPlugin.value().qlLibrary->unload();
		qhiPlugin.value().qlLibrary->deleteLater();
	} // while

	qhPlugins.clear();
} // Unload
