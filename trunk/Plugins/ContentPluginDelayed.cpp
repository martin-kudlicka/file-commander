#include "Plugins/ContentPluginDelayed.h"

// constructor
cContentPluginDelayed::cContentPluginDelayed(cContentPlugin *ccpContentPlugin)
{
	this->ccpContentPlugin = ccpContentPlugin;
	qRegisterMetaType<sOutput>("cContentPluginDelayed::sOutput");
} // cContentPluginDelayed

// interrupt delayed content processing before refresh dir view content
void cContentPluginDelayed::on_InterruptContentDelayed()
{
	QHash<QString, cContentPlugin::sPluginInfo> qhPlugins;

	bStop = true;
	qhPlugins = ccpContentPlugin->GetPluginsInfo();
	if (isRunning()) {
		if (qhPlugins.value(qsCurrentPlugin).tcsgvContentStopGetValue) {
			qhPlugins.value(qsCurrentPlugin).tcsgvContentStopGetValue(qsCurrentFile.toLocal8Bit().data());
		} // if
	} // if
} // on_InterruptContentDelayed

// main thread
void cContentPluginDelayed::run()
{
	int iI;

	for (iI = 0; iI < qlParameters.count() && !bStop; iI++) {
		QString qsValue;
		sOutput soOutput;

		qsCurrentFile = qlParameters.at(iI).siInput.qsFilename;
		qsCurrentPlugin = qlParameters.at(iI).siInput.qsPlugin;

		soOutput = qlParameters.at(iI).soOutput;
		soOutput.qsValue = ccpContentPlugin->GetPluginValue(qlParameters.at(iI).siInput.qsFilename, qlParameters.at(iI).siInput.qsPlugin, qlParameters.at(iI).siInput.qsColumn, qlParameters.at(iI).siInput.qsUnit);
		emit GotColumnValue(soOutput);
	} // for
} // run

// start thread processing
void cContentPluginDelayed::Start(const QList<sParameters> &qlParameters)
{
	this->qlParameters = qlParameters;
	bStop = false;
	start();
} // Start
