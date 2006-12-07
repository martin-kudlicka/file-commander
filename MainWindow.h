#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "ui_MainWindow.h"

#include "Plugins/Plugins.h"
#include "Panel.h"
#include "Settings.h"
#include "FileOperation.h"

const QString qsAPPLICATION = "File Commander";
const QString qsVERSION = "0.0.0.1";

/// application's main window

class cMainWindow : public QMainWindow, private Ui::qmwMainWindow
{
	Q_OBJECT

	public:
		cMainWindow();											///< creates main window
																	/**< load plugins, prepare panels, load settings, setup GUI */
		~cMainWindow();										///< destructor

	private:
		static const int iTAB_POS = 1;					///< position of TabBar in layout

		cPanel *cpLeft;										///< left dir panel
		cPanel *cpRight;										///< right dir panel
		cPlugins *cpPlugins;									///< application's plugins
		cSettings csSettings;								///< accessing application's settings
		QTabBar qtbLeft;										///< left's panel tab bar
		QTabBar qtbRight;										///< right's panel tab bar

		cFileOperation cfoFileOperation;					///< handling file operations

		QStackedWidget *GetActivePanel();				///< find active panel (left or right)
																	/**< \return pointer to active panel */
		void LoadTabs(const cSettings::ePosition epPosition);
																	///< load tabs from qsSettings
																	/**< \param epPos means left or right TabBar */
		void SetSourceAndDestinationPanel(cPanel **cpSource, cPanel **cpDestination = NULL);
																	///< set focused panel as source, other as destination
																	/**< \param cpSource source panel
																		  \param cpDestination destination panel */

	private slots:
		void on_qpbCopy_clicked(bool checked = false);	
																	///< copy button is clicked on
																	/**< \param checked true if button is checkable and checked */
}; // cMainWindow

#endif
