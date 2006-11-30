#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "ui_MainWindow.h"

#include <QSettings>
#include "DirModel.h"
#include "Plugins/Plugins.h"

const QString qsAPPLICATION = "File Commander";
const QString qsVERSION = "0.0.0.1";

/// application's main window

class cMainWindow : public QMainWindow, private Ui::qmwMainWindow
{
	Q_OBJECT

	public:
		cMainWindow();				///< creates main window
										/**< load plugins, prepare panels, load settings, setup GUI */

	private:
		cDirModel cdmLeft;		///< left dir panel
		cDirModel cdmRight;		///< right dir panel
		cPlugins	cpPlugins;		///< application's plugins
		QSettings qsSettings;	///< application's settings
}; // cMainWindow

#endif
