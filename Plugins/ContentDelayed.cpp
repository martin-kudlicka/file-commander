#include "Plugins/ContentDelayed.h"

// constructor
cContentDelayed::cContentDelayed(cContent *ccContent)
{
	this->ccContent = ccContent;
	qRegisterMetaType<sOutput>("cContentDelayed::sOutput");
} // cContentDelayed

// interrupt delayed content processing before refresh dir view content
void cContentDelayed::on_InterruptContentDelayed()
{
	QHash<QString, cContent::sPluginInfo> qhPlugins;

	bStop = true;
	qhPlugins = ccContent->GetPluginsInfo();
	if (isRunning()) {
		if (qhPlugins.value(qsCurrentPlugin).tcsgvContentStopGetValue) {
			qhPlugins.value(qsCurrentPlugin).tcsgvContentStopGetValue(qsCurrentFile.toLocal8Bit().data());
		} // if
	} // if
} // on_InterruptContentDelayed

// main thread
void cContentDelayed::run()
{
	int iI;

	for (iI = 0; iI < qlParameters.count() && !bStop; iI++) {
		QString qsValue;
		sOutput soOutput;

		qsCurrentFile = qlParameters.at(iI).siInput.qsFilename;
		qsCurrentPlugin = qlParameters.at(iI).siInput.qsPlugin;

		soOutput = qlParameters.at(iI).soOutput;
		soOutput.qsValue = ccContent->GetPluginValue(qlParameters.at(iI).siInput.qsFilename, qlParameters.at(iI).siInput.qsPlugin, qlParameters.at(iI).siInput.qsColumn, qlParameters.at(iI).siInput.qsUnit);
		emit GotColumnValue(soOutput);
	} // for
} // run

// start thread processing
void cContentDelayed::Start(const QList<sParameters> &qlParameters)
{
	this->qlParameters = qlParameters;
	bStop = false;
	start();
} // Start
