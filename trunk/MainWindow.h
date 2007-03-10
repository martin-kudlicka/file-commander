#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "ui_MainWindow.h"

#include "Plugins.h"
#include "Panel.h"
#include "Settings.h"
#include "FileOperation.h"
#include <QTimer>
#include <QShortcut>

const QString qsAPPLICATION = "File Commander";
const QString qsVERSION = "0.0.0.1";

/// application's main window

class cMainWindow : public QMainWindow, private Ui::qmwMainWindow
{
	Q_OBJECT

	public:
		cMainWindow();													///< creates main window
																			/**< load plugins, prepare panels, load settings, setup GUI */
		~cMainWindow();												///< destructor

	private:
		static const int iTAB_POS = 1;							///< position of TabBar in layout
		static const int iTIMER_INTERVAL = 1000;				///< timer interval

		cFileOperation *cfoFileOperation;						///< handling file operations
		cPanel *cpLeft;												///< left dir panel
		cPanel *cpRight;												///< right dir panel
		cPlugins *cpPlugins;											///< application's plugins
		cSettings csSettings;										///< accessing application's settings
		QMap<QString, cFileRoutine::sDriveInfo> qmDrives;	///< drives in system
		QHBoxLayout *qhblBackgroundOperations;					///< layout for background operations
		QShortcut *qsFullScreen;									///< full screen window mode switch
		QShortcut *qsLeftDrive;										///< left drive combo box shortcut
		QShortcut *qsRightDrive;									///< right drive combo box shortcut
		QTabBar qtbLeft;												///< left's panel tab bar
		QTabBar qtbRight;												///< right's panel tab bar
		QTimer qtTimer;												///< timer for requesting changeable informations

		void ActualizeDrives();										///< drive lists actualization
		QStackedWidget *GetActivePanel();						///< find active panel (left or right)
																			/**< \return pointer to active panel */
		void LoadTabs(const cSettings::ePosition &epPosition);
																			///< load tabs from qsSettings
																			/**< \param epPos means left or right TabBar */
		void resizeEvent(QResizeEvent *event);					///< size of main window changed
																			/**< \param event olasize information */
		void SetSourceAndDestinationPanel(cPanel **cpSource, cPanel **cpDestination = NULL);
																			///< set focused panel as source, other as destination
																			/**< \param cpSource source panel
																				  \param cpDestination destination panel */

	private slots:
		void on_qaOptions_triggered(bool checked = false);	///< options are selected
																			/**< \param checked true if menu item is checkable and checked */
		void on_qaSavePosition_triggered(bool checked = false);
																			///< save position is selected
																			/**< \param checked true if menu item is checkable and checked */
		void on_qpbCopy_clicked(bool checked = false);		///< copy button is clicked on
																			/**< \param checked true if button is checkable and checked */
		void on_qpbDelete_clicked(bool checked = false);	///< delete button is clicked on
																			/**< \param checked true if button is checkable and checked */
		void on_qpbEdit_clicked(bool checked = false);		///< edit button is clicked on
																			/**< \param checked true if button is checkable and checked */
		void on_qpbLeftRootDir_clicked(bool checked = false);
																			///< left root dir button is clicked on
																			/**< \param checked true if button is checkable and checked */
		void on_qpbLeftUpDir_clicked(bool checked = false);
																			///< left up dir button is clicked on
																			/**< \param checked true if button is checkable and checked */
		void on_qpbMove_clicked(bool checked = false);		///< copy button is clicked on
																			/**< \param checked true if button is checkable and checked */
		void on_qpbNewDirectory_clicked(bool checked = false);
																			///< new directory button is clicked on
																			/**< \param checked true if button is checkable and checked */
		void on_qpbRightRootDir_clicked(bool checked = false);
																			///< right root dir button is clicked on
																			/**< \param checked true if button is checkable and checked */
		void on_qpbRightUpDir_clicked(bool checked = false);
																			///< right up dir button is clicked on
																			/**< \param checked true if button is checkable and checked */
		void on_qpbView_clicked(bool checked = false);		///< view button is clicked on
																			/**< \param checked true if button is checkable and checked */
		void on_qsFullScreen_activated();						///< full screen mode switched
		void on_qsLeftDrive_activated();							///< left drive shortcut activated
		void on_qsRightDrive_activated();						///< right drive shortcut activated
		void on_qtTimer_timeout();									///< timer's timeout
}; // cMainWindow

#endif
