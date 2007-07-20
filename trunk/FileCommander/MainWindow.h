#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "ui_MainWindow.h"

#include "Plugins.h"
#include <QtGui/QTreeView>
#include <QtGui/QCompleter>
#include <QtGui/QShortcut>

const QString qsAPPLICATION = "File Commander";
const QString qsVERSION = "0.0.0.4 rev 395";

/// application's main window

class cMainWindow : public QMainWindow, private Ui::qmwMainWindow
{
	Q_OBJECT

	public:
		cMainWindow();															///< creates main window
																					/**< load plugins, prepare panels, load settings, setup GUI */

	private:
		static const int iTAB_POS = 1;									///< position of TabBar in layout

		cPlugins *cpPlugins;													///< application's plugins
		cSettings csSettings;												///< accessing application's settings
		QAction *qaTabBarCloseAllOtherTabs;								///< close all other tabs
		QAction *qaTabBarCloseTab;											///< close current tab
		QAction *qaTabBarDuplicateTab;									///< duplicate current tab
		QCompleter qcDirModel;												///< completer based on dir model
		QHBoxLayout *qhblBackgroundOperations;							///< layout for background operations
		QMenu qmTabBar;														///< tab bar context menu
		QShortcut *qsLeftDrive;												///< left drive combo box shortcut
		QShortcut *qsRightDrive;											///< right drive combo box shortcut
		QTabBar qtbLeft;														///< left's panel tab bar
		QTabBar qtbRight;														///< right's panel tab bar
		QTreeView *qtwLeftDrives;											///< drives list for left drive combo box
		QTreeView *qtwRightDrives;											///< drives list for right drive combo box

		const void AssignShortcuts();										///< assign shortcuts

	private slots:
		const void on_qaAbout_triggered(bool checked = false);	///< about is selected
																					/**< \param checked true if menu item is checkable and checked */
		const void on_qaOptions_triggered(bool checked = false);	///< options are selected
																					/**< \param checked true if menu item is checkable and checked */
		const void on_qtbLeft_customContextMenuRequested(const QPoint &pos) const;
																					///< context menu of left tab bar
																					/**< \param pos cursor position */
		const void on_qtbRight_customContextMenuRequested(const QPoint &pos) const;
																					///< context menu of right tab bar
																					/**< \param pos cursor position */
}; // cMainWindow

#endif
