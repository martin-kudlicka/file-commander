#include "Plugins/ContentPluginDelayed.h"

// constructor
cContentPluginDelayed::cContentPluginDelayed(cContentPlugin *ccpContentPlugin)
{
	this->ccpContentPlugin = ccpContentPlugin;
	qRegisterMetaType<sOutput>("cContentPluginDelayed::sOutput");
} // cContentPluginDelayed

// interrupt delayed content processing before refresh dir view content
const void cContentPluginDelayed::on_InterruptContentDelayed()
{
	QHash<QString, cContentPlugin::sPluginInfo> qhPlugins;

	bStop = true;
	qhPlugins = ccpContentPlugin->GetPluginsInfo();
	if (isRunning()) {
		cContentPlugin::sPluginInfo *spiPluginInfo;

		spiPluginInfo = &qhPlugins[qsCurrentPlugin];
		if (spiPluginInfo->tcsgvContentStopGetValue) {
			spiPluginInfo->tcsgvContentStopGetValue(qsCurrentFile.toLocal8Bit().data());
		} // if
	} // if
} // on_InterruptContentDelayed

// main thread
void cContentPluginDelayed::run()
{
	int iI;

	for (iI = 0; iI < qlParameters.count() && !bStop; iI++) {
		sParameters *spParameters;

		spParameters = &qlParameters[iI];
		qsCurrentFile = spParameters->siInput.qsFilename;
		qsCurrentPlugin = spParameters->siInput.qsPlugin;

		spParameters->soOutput.qsValue = ccpContentPlugin->GetPluginValue(spParameters->siInput.qsFilename, spParameters->siInput.qsPlugin, spParameters->siInput.qsColumn, spParameters->siInput.qsUnit);
		emit GotColumnValue(spParameters->soOutput);
	} // for
} // run

// start thread processing
const void cContentPluginDelayed::Start(const QList<sParameters> &qlParameters)
{
	this->qlParameters = qlParameters;
	bStop = false;
	start();
} // Start
