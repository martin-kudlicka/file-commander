#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "ui_MainWindow.h"

#include "Plugins.h"
#include <QtGui/QTreeView>
#include <QtGui/QCompleter>
#include <QtGui/QShortcut>
#include "FileControl.h"
#include "Panel.h"
#include "OptionsDialog.h"

const QString qsAPPLICATION = "File Commander";
const QString qsVERSION = "0.0.0.4 rev 395";

/// application's main window

class cMainWindow : public QMainWindow, private Ui::qmwMainWindow
{
	Q_OBJECT

	public:
		cMainWindow();																	///< creates main window
																							/**< load plugins, prepare panels, load settings, setup GUI */
		~cMainWindow();																///< destructor

	private:
		static const int iTAB_POS = 1;											///< position of TabBar in layout

		cFileControl *cfcFileControl;												///< file operations control
		cPanel *cpDestination;														///< last nonactive panel
		cPanel *cpLeft;																///< left directory view panel
		cPanel *cpRight;																///< right directory view panel
		cPanel *cpSource;																///< last active panel
		cPanel *cpTabBarAction;														///< selected tab bar for action with tabs
		cPlugins *cpPlugins;															///< application's plugins
		cSettings csSettings;														///< accessing application's settings
		int iTabBarIndex;																///< selected tab for actions on tab bar
		QAction *qaTabBarCloseAllOtherTabs;										///< close all other tabs
		QAction *qaTabBarCloseTab;													///< close current tab
		QAction *qaTabBarDuplicateTab;											///< duplicate current tab
		QActionGroup *qagColumnSets;												///< column sets action group
		QActionGroup *qagSortBy;													///< sort by actions
		QCompleter qcDirModel;														///< completer based on dir model
		QHash<QAction *, cOptionsDialog::sFavouriteDirectory> qhFavouriteDirectories;
																							///< favourite directories table
		QMenu qmFavouriteDirectories;												///< favourite directories context menu
		QHBoxLayout *qhblBackgroundOperations;									///< layout for background operations
		QMenu qmColumnSets;															///< column sets submenu
		QMenu qmTabBar;																///< tab bar context menu
		QShortcut *qsLeftDrive;														///< left drive combo box shortcut
		QShortcut *qsRightDrive;													///< right drive combo box shortcut
		QTabBar qtbLeft;																///< left's panel tab bar
		QTabBar qtbRight;																///< right's panel tab bar
		QTreeView *qtwLeftDrives;													///< drives list for left drive combo box
		QTreeView *qtwRightDrives;													///< drives list for right drive combo box

		const void ActualizeColumnSets();										///< actualize column sets submenu
		const void ActualizeFavouriteDirectories();							///< actualize favourite directories context menu
		const void AssignShortcuts();												///< assign shortcuts
		const void FillFavouriteDirectories(QMenu *qmMenu, QList<QPair<QString, cSettings::sFavouriteDirectory> > &qlFavouriteDirectories);
																							///< fill favourite directories context menu
																							/**< \param qmMenu menu to fill in
																								  \param qlFavouriteDirectories favourites to fill */
		const void LoadTabs(const cSettings::ePosition &epPosition);	///< load tabs from qsSettings
																							/**< \param epPosition tabs for left or right panel */
		const void SaveSettings() const;											///< save dir view settings
		const void SetSortByActions() const;									///< set sort by actions for sorting columns
		const void TabBarShowContextMenu(const cSettings::ePosition &epTab, const QPoint &qpCursor);
																							///< show context menu for tab
																							/**< \param epTab left or right tab
																								  \param qpCursor cursor position on tab */

	private slots:
		const void on_cpLeft_GotFocus();											///< left panel got focus
		const void on_cpRight_GotFocus();										///< right panel got focus
		const void on_qaAbout_triggered(bool checked = false);			///< about is selected
																							/**< \param checked true if menu item is checkable and checked */
		const void on_qaFavouriteDirectories_triggered(bool checked = false) const;
																							///< favourite directories are selected
																							/**< \param checked true if menu item is checkable and checked */
		const void on_qagSortBy_triggered(QAction *action) const;		///< sort by action called
																							/**< \param action column to sort by */
		const void on_qaOptions_triggered(bool checked = false);			///< options are selected
																							/**< \param checked true if menu item is checkable and checked */
		const void on_qaTabBarCloseAllOtherTabs_triggered(bool checked = false);
																							///< close all other tabs called
																							/**< \param checked true if menu item is checkable and checked */
		const void on_qaTabBarCloseTab_triggered(bool checked = false);
																							///< close tab called
																							/**< \param checked true if menu item is checkable and checked */
		const void on_qaTabBarDuplicateTab_triggered(bool checked = false);
																							///< duplicate tab called
																							/**< \param checked true if menu item is checkable and checked */
		const void on_qmColumnSets_triggered(QAction *action) const;	///< selected column set from column set submenu
																							/**< \param action column set */
		const void on_qmFavouriteDirectories_triggered(QAction *action) const;
																							///< selected favourite directory from favourites context menu
																							/**< \param action favourite directory */
		const void on_qsLeftDrive_activated() const;							///< left drive shortcut activated
		const void on_qsRightDrive_activated() const;						///< right drive shortcut activated
		const void on_qtbLeft_customContextMenuRequested(const QPoint &pos);
																							///< context menu of left tab bar
																							/**< \param pos cursor position */
		const void on_qtbRight_customContextMenuRequested(const QPoint &pos);
																							///< context menu of right tab bar
																							/**< \param pos cursor position */
}; // cMainWindow

#endif