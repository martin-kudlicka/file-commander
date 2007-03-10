/// lister's main window

#ifndef LISTER_MAIN_WINDOW_H
#define LISTER_MAIN_WINDOW_H

#include "ui_ListerMainWindow.h"

#include "Plugins/Lister.h"

class cListerMainWindow : public QMainWindow, private Ui::qmwLister
{
	Q_OBJECT

	public:
		cListerMainWindow(cSettings *csSettings, cLister *clLister, const QString &qsFile);	///< creates lister window
																														/**< \param csSettings application's settings
																															  \param clLister lister plugins
																															  \param qsFile file content to show */


	private:
		cLister *clLister;															///< lister plugin's class
		cSettings *csSettings;														///< application's main settings file
		QString qsFile;																///< file content to show
}; // cListerMainWindow

#endif