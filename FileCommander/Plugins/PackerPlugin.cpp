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

	qsCurrentPath = QDir::currentPath();
	QDir::setCurrent(QFileInfo(qsName).path());
#ifdef Q_WS_WIN
	spiPluginInfo.hmLibrary = LoadLibrary(reinterpret_cast<LPCWSTR>(qsName.unicode()));
	spiPluginInfo.qsLibrary = qsName;
#else
	spiPluginInfo.qlLibrary = new QLibrary();
	spiPluginInfo.qlLibrary->setFileName(qsName);
	spiPluginInfo.qlLibrary->load();
#endif

	// fill plugin properties
#ifdef Q_WS_WIN
	spiPluginInfo.tcaCloseArchive = (tCloseArchive)GetProcAddress(spiPluginInfo.hmLibrary,"CloseArchive");
	spiPluginInfo.tcpConfigurePacker = (tConfigurePacker)GetProcAddress(spiPluginInfo.hmLibrary,"ConfigurePacker");
	spiPluginInfo.tdfDeleteFiles = (tDeleteFiles)GetProcAddress(spiPluginInfo.hmLibrary,"DeleteFiles");
	spiPluginInfo.tgpcGetPackerCaps = (tGetPackerCaps)GetProcAddress(spiPluginInfo.hmLibrary,"GetPackerCaps");
	spiPluginInfo.toaOpenArchive = (tOpenArchive)GetProcAddress(spiPluginInfo.hmLibrary,"OpenArchive");
	spiPluginInfo.tpfPackFiles = (tPackFiles)GetProcAddress(spiPluginInfo.hmLibrary,"PackFiles");
	spiPluginInfo.tpsdpPackSetDefaultParams = (tPackSetDefaultParams)GetProcAddress(spiPluginInfo.hmLibrary,"PackSetDefaultParams");
	spiPluginInfo.tpfProcessFile = (tProcessFile)GetProcAddress(spiPluginInfo.hmLibrary,"ProcessFile");
	spiPluginInfo.trhReadHeader = (tReadHeader)GetProcAddress(spiPluginInfo.hmLibrary,"ReadHeader");
	spiPluginInfo.tspdpSetProcessDataProc = (tSetProcessDataProc)GetProcAddress(spiPluginInfo.hmLibrary, "SetProcessDataProc");
#else
	spiPluginInfo.tcaCloseArchive = (tCloseArchive)spiPluginInfo.qlLibrary->resolve("CloseArchive");
	spiPluginInfo.tcpConfigurePacker = (tConfigurePacker)spiPluginInfo.qlLibrary->resolve("ConfigurePacker");
	spiPluginInfo.tdfDeleteFiles = (tDeleteFiles)spiPluginInfo.qlLibrary->resolve("DeleteFiles");
	spiPluginInfo.tgpcGetPackerCaps = (tGetPackerCaps)spiPluginInfo.qlLibrary->resolve("GetPackerCaps");
	spiPluginInfo.toaOpenArchive = (tOpenArchive)spiPluginInfo.qlLibrary->resolve("OpenArchive");
	spiPluginInfo.tpfPackFiles = (tPackFiles)spiPluginInfo.qlLibrary->resolve("PackFiles");
	spiPluginInfo.tpsdpPackSetDefaultParams = (tPackSetDefaultParams)spiPluginInfo.qlLibrary->resolve("PackSetDefaultParams");
	spiPluginInfo.tpfProcessFile = (tProcessFile)spiPluginInfo.qlLibrary->resolve("ProcessFile");
	spiPluginInfo.trhReadHeader = (tReadHeader)spiPluginInfo.qlLibrary->resolve("ReadHeader");
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

#ifdef Q_WS_WIN
		FreeLibrary(qhiPlugin.value().hmLibrary);
#else
		qhiPlugin.value().qlLibrary->unload();
		qhiPlugin.value().qlLibrary->deleteLater();
#endif
	} // while

	qhPlugins.clear();
} // Unload
