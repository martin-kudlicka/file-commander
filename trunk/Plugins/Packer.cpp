#include "Plugins/Packer.h"

#include <QtCore/QLibrary>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>

const DWORD dwPLUGIN_INTERFACE_VERSION_HI = 2;
const DWORD dwPLUGIN_INTERFACE_VERSION_LOW = 1;

// constructor
cPacker::cPacker(cSettings *csSettings)
{
	this->csSettings = csSettings;
} // cPacker

// loads packer plugins
void cPacker::Load()
{
	int iI;
	QList<cSettings::sPlugin> qlPlugins;

	// get plugin file list
	qlPlugins = csSettings->GetPlugins(cSettings::PackerPlugins);

	// enumerate plugins
	for (iI = 0; iI < qlPlugins.count(); iI++) {
		if (qlPlugins.at(iI).bEnabled) {
			sPluginInfo spiPluginInfo;
			QLibrary qlLibrary;

			// load plugin
			qlLibrary.setFileName(qlPlugins.at(iI).qsName);
			qlLibrary.load();

			// fill plugin properties
			spiPluginInfo.tcaCloseArchive = static_cast<tCloseArchive>(qlLibrary.resolve("CloseArchive"));
			spiPluginInfo.toaOpenArchive = static_cast<tOpenArchive>(qlLibrary.resolve("OpenArchive"));
			spiPluginInfo.tpfProcessFile = static_cast<tProcessFile>(qlLibrary.resolve("ProcessFile"));
			spiPluginInfo.trhReadHeader = static_cast<tReadHeader>(qlLibrary.resolve("ReadHeader"));

			spiPluginInfo.tpsdpPackSetDefaultParams = static_cast<tPackSetDefaultParams>(qlLibrary.resolve("PackSetDefaultParams"));
			// set default parameters
			if (spiPluginInfo.tpsdpPackSetDefaultParams) {
				PackDefaultParamStruct pdpsParams;
				QString qsIniFile;

				qsIniFile = QFileInfo(qlPlugins.at(iI).qsName).path() + '/' + QFileInfo(qlPlugins.at(iI).qsName).completeBaseName() + ".ini";
				pdpsParams.PluginInterfaceVersionLow = dwPLUGIN_INTERFACE_VERSION_LOW;
				pdpsParams.PluginInterfaceVersionHi = dwPLUGIN_INTERFACE_VERSION_HI;
				strcpy(pdpsParams.DefaultIniName, QDir::toNativeSeparators(qsIniFile).toAscii().constData());

				spiPluginInfo.tpsdpPackSetDefaultParams(&pdpsParams);
			} // if

			// add new plugin
			qhPlugins.insert(QFileInfo(qlPlugins.at(iI).qsName).fileName(), spiPluginInfo);
		} // if
	} // for
} // Load