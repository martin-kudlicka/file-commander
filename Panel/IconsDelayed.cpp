#include "Panel/IconsDelayed.h"

#include <QtGui/QFileIconProvider> 

// constructor
cIconsDelayed::cIconsDelayed()
{
	qRegisterMetaType<sOutput>("cIconsDelayed::sOutput");
} // cIconsDelayed

// interrupt delayed icons processing before refresh dir view content
void cIconsDelayed::on_InterruptIconsDelayed()
{
	bStop = true;
} // on_InterruptIconsDelayed

// main thread
void cIconsDelayed::run()
{
	int iI;
	QFileIconProvider qfipIcon;

	for (iI = 0; iI < qlParameters.count() && !bStop; iI++) {
		qlParameters[iI].soOutput.qiIcon = qfipIcon.icon(qlParameters.at(iI).siInput.qsFilename);
		emit GotIcon(qlParameters.at(iI).soOutput);
	} // for
} // run

// start thread processing
void cIconsDelayed::Start(const QList<sParameters> &qlParameters)
{
	this->qlParameters = qlParameters;
	bStop = false;
	start();
} // Start
