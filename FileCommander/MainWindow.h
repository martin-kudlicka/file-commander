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
const QString qsVERSION = "0.0.1.3 rev 589";

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
		cSettings::ePosition epFavourite;										///< favourite for specified panel's position
		int iTabBarIndex;																///< selected tab for actions on tab bar
		QAction *qaTabBarCloseAllOtherTabs;										///< close all other tabs
		QAction *qaTabBarCloseTab;													///< close current tab
		QAction *qaTabBarDuplicateTab;											///< duplicate current tab
		QActionGroup *qagColumnSets;												///< column sets action group
		QActionGroup *qagSortBy;													///< sort by actions
		QCompleter qcDirModel;														///< completer based on dir model
		QHash<QAction *, cOptionsDialog::sFavouriteDirectory> qhFavouriteDirectories;
																							///< favourite directories table
		QHBoxLayout *qhblBackgroundOperations;									///< layout for background operations
		QMenu qmColumnSets;															///< column sets submenu
		QMenu qmFavouriteDirectories;												///< favourite directories context menu
		QMenu qmLeftHistoryDirectoryList;										///< history directory list menu for left panel
		QMenu qmRightHistoryDirectoryList;										///< history directory list menu for right panel
		QMenu qmTabBar;																///< tab bar context menu
		QShortcut *qsHistoryBack;													///< go back in history directory list
		QShortcut *qsHistoryFront;													///< go front in history directory list
		QShortcut *qsLeftDrive;														///< left drive combo box shortcut
		QShortcut *qsRightDrive;													///< right drive combo box shortcut
		QTabBar qtbLeft;																///< left's panel tab bar
		QTabBar qtbRight;																///< right's panel tab bar
		QTreeView *qtwLeftDrives;													///< drives list for left drive combo box
		QTreeView *qtwRightDrives;													///< drives list for right drive combo box

		const void ActualizeColumnSets();										///< actualize column sets submenu
		const void ActualizeFavouriteDirectories();							///< actualize favourite directories context menu
		const void AssignShortcuts();												///< assign shortcuts
		const void DriveIndexChanged(cPanel *cpPanel, const QString &qsDrive) const;
																							///< drive changed in panel
																							/**< \param cpPanel panel that drive changes in
																								  \param qsDrive drive to change to */
		bool eventFilter(QObject *watched, QEvent *event);					///< event filter
																							/**< \param watched filtered object
																								  \param event event description
																								  \return true if event is handled */
		const void FillFavouriteDirectories(QMenu *qmMenu, const QList<QPair<QString, cSettings::sFavouriteDirectory> > &qlFavouriteDirectories);
																							///< fill favourite directories context menu
																							/**< \param qmMenu menu to fill in
																								  \param qlFavouriteDirectories favourites to fill */
		const void FillHistoryDirectoryList(const cSettings::ePosition &epPosition);
																							///< fill history directory list for specified panel
																							/**< \param epPosition panel position */
		const void LoadTabs(const cSettings::ePosition &epPosition);	///< load tabs from qsSettings
																							/**< \param epPosition tabs for left or right panel */
		const void SaveSettings() const;											///< save dir view settings
		const void SetSortByActions() const;									///< set sort by actions for sorting columns
		const void TabBarShowContextMenu(const cSettings::ePosition &epTab, const QPoint &qpCursor);
																							///< show context menu for tab
																							/**< \param epTab left or right tab
																								  \param qpCursor cursor position on tab */

	private slots:
		const void on_cPanel_Copy() const;										///< copy marked files
		const void on_cPanel_Delete() const;									///< delete marked files
		const void on_cPanel_FileSystemUnaccessible(cPanel *cpPanel) const;
																							///< panel's file system unaccessible
																							/**< \param cpPanel panel with unaccessible file system */
		const void on_cPanel_GotFocus(const cPanel *cpPanel);				///< panel got focus
																							/**< \param cpPanel panel that got the focus */
		const void on_qaAbout_triggered(bool checked = false);			///< about is selected
																							/**< \param checked true if menu item is checkable and checked */
		const void on_qaBranchView_triggered(bool checked = false) const;
																							///< branch view is selected
																							/**< \param checked true if menu item is checkable and checked */
		const void on_qaCompareDirectories_triggered(bool checked = false) const;
																							///< compare directories is selected
																							/**< \param checked true if menu item is checkable and checked */
		const void on_qaFavouriteDirectories_triggered(bool checked = false) const;
																							///< favourite directories are selected
																							/**< \param checked true if menu item is checkable and checked */
		const void on_qaFullScreen_triggered(bool checked = false);		///< full screen mode is selected
																							/**< \param checked true if menu item is checkable and checked */
		const void on_qagSortBy_triggered(QAction *action) const;		///< sort by action called
																							/**< \param action column to sort by */
		const void on_qaHistoryDirectoryList_triggered(bool checked = false) const;
																							///< history directory list is selected
																							/**< \param checked true if menu item is checkable and checked */
		const void on_qaInvertSelection_triggered(bool checked = false) const;
																							///< invert selection is selected
																							/**< \param checked true if menu item is checkable and checked */
		const void on_qaOptions_triggered(bool checked = false);			///< options are selected
																							/**< \param checked true if menu item is checkable and checked */
		const void on_qaPack_triggered(bool checked = false) const;		///< pack is selected
																							/**< \param checked true if menu item is checkable and checked */
		const void on_qaRefresh_triggered(bool checked = false) const;	///< refresh is selected
																							/**< \param checked true if menu item is checkable and checked */
		const void on_qaReverseOrder_triggered(bool checked = false) const;
																							///< reverse order is selected
																							/**< \param checked true if menu item is checkable and checked */
		const void on_qaSavePosition_triggered(bool checked = false);	///< save position is selected
																							/**< \param checked true if menu item is checkable and checked */
		const void on_qaSaveSettings_triggered(bool checked = false) const;
																							///< save settings is selected
																							/**< \param checked true if menu item is checkable and checked */
		const void on_qaSearch_triggered(bool checked = false);			///< search selected
																							/**< \param checked true if menu item is checkable and checked */
		const void on_qaSelectAll_triggered(bool checked = false) const;
																							///< select all selected
																							/**< \param checked true if menu item is checkable and checked */
		const void on_qaSelectGroup_triggered(bool checked = false) const;
																							///< select group selected
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
		const void on_qaTargetAsSource_triggered(bool checked = false) const;
																							///< set target panel as source
																							/**< \param checked true if menu item is checkable and checked */
		const void on_qaUnpackSelectedFiles_triggered(bool checked = false) const;
																							///< unpack selected files chosen
																							/**< \param checked true if menu item is checkable and checked */
		const void on_qaUnselectAll_triggered(bool checked = false) const;
																							///< unselect all selected
																							/**< \param checked true if menu item is checkable and checked */
		const void on_qaUnselectGroup_triggered(bool checked = false) const;
																							///< unselect group selected
																							/**< \param checked true if menu item is checkable and checked */
		const void on_qcbLeftDrive_currentIndexChanged(int index) const;
																							///< selected drive in left panel changes
																							/**< \param index index of new drive */
		const void on_qcbRightDrive_currentIndexChanged(int index) const;
																							///< selected drive in right panel changes
																							/**< \param index index of new drive */
		const void on_qmColumnSets_triggered(QAction *action) const;	///< selected column set from column set submenu
																							/**< \param action column set */
		const void on_qmFavouriteDirectories_triggered(QAction *action) const;
																							///< selected favourite directory from favourites context menu
																							/**< \param action favourite directory */
		const void on_qmLeftHistoryDirectoryList_aboutToShow();			///< left history directory list is about to show
		const void on_qmLeftHistoryDirectoryList_triggered(QAction *action) const;
																							///< selected directory from left panel's history list
																							/**< \param action directory information */
		const void on_qmRightHistoryDirectoryList_aboutToShow();			///< right history directory list is about to show
		const void on_qmRightHistoryDirectoryList_triggered(QAction *action) const;
																							///< selected directory from right panel's history list
																							/**< \param action directory information */
		const void on_qpbCopy_clicked(bool checked = false) const;		///< copy button is clicked on
																							/**< \param checked true if button is checkable and checked */
		const void on_qpbDelete_clicked(bool checked = false) const;	///< delete button is clicked on
																							/**< \param checked true if button is checkable and checked */
		const void on_qpbEdit_clicked(bool checked = false) const;		///< edit button is clicked on
																							/**< \param checked true if button is checkable and checked */
		const void on_qpbLeftFavourites_clicked(bool checked = false);	///< left favourite button is clicked on
																							/**< \param checked true if button is checkable and checked */
		const void on_qpbLeftRootDir_clicked(bool checked = false) const;
																							///< left root dir button is clicked on
																							/**< \param checked true if button is checkable and checked */
		const void on_qpbLeftUpDir_clicked(bool checked = false) const;
																							///< left up dir button is clicked on
																							/**< \param checked true if button is checkable and checked */
		const void on_qpbMove_clicked(bool checked = false) const;		///< copy button is clicked on
																							/**< \param checked true if button is checkable and checked */
		const void on_qpbNewDirectory_clicked(bool checked = false) const;
																							///< new directory button is clicked on
																							/**< \param checked true if button is checkable and checked */
		const void on_qpbRightFavourites_clicked(bool checked = false);
																							///< right favourite button is clicked on
																							/**< \param checked true if button is checkable and checked */
		const void on_qpbRightRootDir_clicked(bool checked = false) const;
																							///< right root dir button is clicked on
																							/**< \param checked true if button is checkable and checked */
		const void on_qpbRightUpDir_clicked(bool checked = false) const;
																							///< right up dir button is clicked on
																							/**< \param checked true if button is checkable and checked */
		const void on_qpbTerminal_clicked(bool checked = false) const;	///< terminal button is clicked on
																							/**< \param checked true if button is checkable and checked */
		const void on_qpbView_clicked(bool checked = false) const;		///< view button is clicked on
																							/**< \param checked true if button is checkable and checked */
		const void on_qsHistoryBack_activated() const;						///< history back shortcut activated
		const void on_qsHistoryFront_activated() const;						///< history front shortcut activated
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
