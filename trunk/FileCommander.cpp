#include <QApplication>
#include "MainWindow.h"
#include "Common/System.h"

int main(int argc, char **argv)
{
	cMainWindow *cmwMainWindow;
	QApplication qaApplication(argc, argv);
	
	qaApplication.addLibraryPath(cSystem::GetIsshouCommonPath());

#ifdef Q_WS_X11
		// change style on X11 - Motif is not very nice
		qaApplication.setStyle("plastique");
#endif

	// set QSettings location
	QCoreApplication::setOrganizationName(qsCOMPANY);
 	QCoreApplication::setApplicationName(qsAPPLICATION);

	cmwMainWindow = new cMainWindow();
	cmwMainWindow->show();

	return qaApplication.exec();
} // main
