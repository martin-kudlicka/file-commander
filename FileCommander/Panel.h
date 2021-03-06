/// dir panel

#ifndef PANEL_H
#define PANEL_H

#include <QtGui/QStackedWidget>
#include "Settings.h"
#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include "Plugins/ContentPlugin.h"
#include "FileSystem.h"
#include "FileControl.h"
#include "Plugins/ContentPluginDelayed.h"
#include "Panel/SelectFilesDialog.h"
#include "Panel/TreeWidget.h"

class cPanel : public QObject
{
	Q_OBJECT

	public:
		static const qint64 qi64_GIGABYTE = 1073741824;				///< 1 gigabyte in bytes
		static const qint64 qi64_KILOBYTE = 1024;						///< 1 kilobyte in bytes
		static const qint64 qi64_MEGABYTE = 1048576;					///< 1 megabyte in bytes

		/// history directory list to show
		struct sHistoryDirectoryList {
			QStringList qslDirectories;									///< list of directories to choose from
			int iPosition;														///< actual directory position in the list
		};

		cPanel(QMainWindow *qmwParent, QStackedWidget *qswDirs, QComboBox *qcbDrive, QLabel *qlDriveInfo, QTabBar *qtbTab, QLineEdit *qlePath, QLabel *qlSelected, cSettings *csSettings, cContentPlugin *ccpContentPlugin, QLabel *qlGlobalPath, QComboBox *qcbCommand, cFileControl *cfcFileControl, QLineEdit *qleQuickSearch);
																					///< constructor
																					/**< \param qmwParent parent window for dialogs
																						  \param qswDirs panel for cTreeWidget
																						  \param qcbDrive drive combo box
																						  \param qlDriveInfo drive name and it's space information
																						  \param qtbTab panel's tab bar
																						  \param qlePath current path
																						  \param qlSelected information about selected directories and files
																						  \param csSettings application's settings
																						  \param ccpContentPlugin application's content plugins
																						  \param qlGlobalPath path visible in the bottom of main window
																						  \param qcbCommand command combo box
																						  \param cfcFileControl file operations control
																						  \param qleQuickSearch quick search window */
		~cPanel();																		///< destructor

		const int AddTab(const cSettings::sTabInfo &stiTabInfo, const bool &bStartUp = false, cFileSystem *cfsFileSystem = NULL);
																					///< add new tab with dir view
																					/**< \param stiTabInfo new tab description
																						  \param bStartUp true if tab is added on application startup
																						  \param cfsFileSystem file system for panel
																						  \return new tab index */
		const void BranchView();											///< branch view for current directory
		const void CloseAllOtherTabs(const int &iTabIndex);		///< close all other tabs than selected
																					/**< \param iTabIndex selected tab */
		const void CloseTab(const int &iTabIndex);					///< close tab
																					/**< \param iTabIndex tab to close */
		const int DuplicateTab(const int &iTabIndex);				///< create new tab by duplicate one
																					/**< \param iTabIndex tab to duplicate
																						  \return new tab index */
		const QList<cSettings::sColumn> GetColumns() const;		///< columns for current dir view
																					/**< \return columns for current dir view */
		const QString GetColumnSet() const;								///< column set for current directory view
																					/**< \return column set for current directory view */
		cFileSystem *GetFileSystem();										///< file system for active directory view
																					/**< \return file system for active directory view */
		const sHistoryDirectoryList GetHistoryDirectoryList() const;
																					///< retreive history directory list
																					/**< \return history directory list */
		const QString GetPath(const bool &bLocal = false) const;	///< path in current directory view
																					/**< \param bLocal only local path if true
																						  \return path for current directory view */
		const QList<QTreeWidgetItem *> GetSelectedFiles() const;	///< selected files from current directory view
																					/**< \return selected files from current directory view */
		const int GetTabIndex(const QPoint &qpPos) const;			///< find out tab index in tab bar
																					/**< \param qpPos cursor position in tab bar
																						  \return tab index */
		const cSettings::sTabInfo GetTabSettings(int iTabIndex = -1) const;
																					///< tab settings for specified tab
																					/**< \param iTabIndex tab settings for this tab (default is current tab) */
		const bool GoToFile(const QString qsGoTo) const;			///< set cursor onto specified file
																					/**< \param qsGoTo file to set cursor on
																						  \return true if file found */
		const void HistoryGoBack();										///< go back in history directory list
		const void HistoryGoFront();										///< go front in history directory list
		const void InvertSelection() const;								///< invert selection of files
		const void RefreshAllContents();									///< refresh all dir view contents
		const void RefreshAllHeaders();									///< refresh all dir view headers
		const void RefreshContent(const bool &bRefresh = true);	///< refresh current dir content
																					/**< \param bRefresh reload files from file system when true */
		const void RefreshTabs() const;									///< refresh tabs
		const void ReverseOrder() const;									///< reverse sort order
		const void SaveSettings(const cSettings::ePosition &epPosition) const;
																					///< save panel settings
																					/**< \param epPosition panel's position */
		const void Select(const cSelectFilesDialog::eSelectType &estType, cListerPlugin *clpListerPlugin) const;
																					///< select or unselect some files
																					/**< \param estType select/unselect mode
																						  \param clpListerPlugin lister plugins */
		const void SelectAll() const;										///< select all files
		const void SetColumnSet(const QString &qsColumnSet);		///< selected another column set for actual directory view
																					/**< \param qsColumnSet new column set */
		const void SetFocus() const;										///< set focus to current directory view
		const void SetHistoryDirectory(const int &iPosition);		///< set path by directory from history list
																					/**< \param iPosition directory position in history list */
		const void SetPath(const QString &qsPath);					///< set new path for current dir view on selected drive
																					/**< \param qsPath new path */
		const void SetTabIndex(const int &iTabIndex) const;		///< switch tabs
																					/**< \param iTabIndex new tab index */
		const void ShowHideHeaders() const;								///< show or hide headers in all tabs
		const void SortBy(const int &iColumn) const;					///< sort by specified column
																					/**< \param iColumn column position to sort by */
		const void UnselectAll() const;									///< unselect all files

	private:
		static const int iDATE_TIME_ROLE = Qt::UserRole;			///< position to store date/time in tree widget item
		static const int iSIZE_ROLE = Qt::UserRole;					///< position to store file size in tree widget item
		static const int iTIMER_INTERVAL = 1000;						///< timer interval (ms)

		/// quick search direction in directory view
		enum eQuickSearchDirection {
			SearchUp,															///< search up
			SearchDown															///< search down
		};

		/// strings for widgets
		struct sWidgets {
			QString qsPath;													///< path on drive
			QString qsSelected;												///< selected item info
		};
		/// tab information
		struct sTab {
			bool bValid;														///< tab content is validated flag
			QList<cSettings::sColumn> qlColumns;						///< columns in specified tab
			QString qsColumnSet;												///< column set for tab
			sWidgets swWidgets;												///< to remember displayed strings
			cFileSystem *cfsFileSystem;									///< attached file system handle
			cSettings::sHistory shHistory;								///< directory history information
			QString qsDirectoryIn;											///< name of last visited directory
		};

		cContentPlugin *ccpContentPlugin;								///< access to content plugins
		cFileControl *cfcFileControl;										///< file operations control
		cSettings *csSettings;												///< main settings
		QComboBox *qcbCommand;												///< command combo box
		QComboBox *qcbDrive;													///< drive combo box
		QLabel *qlDriveInfo;													///< drive info - label, used/free place
		QLabel *qlGlobalPath;												///< path visible in the bottom of main window
		QLabel *qlSelected;													///< selected items
		QLineEdit *qlePath;													///< path
		QLineEdit *qleQuickSearch;											///< quick search window
		QList<sTab> qlTabs;													///< tabs for dir view
		QMainWindow *qmwParent;												///< parent window for dialogs
		QStackedWidget *qswDirs;											///< directory views
		static QStackedWidget *qswLastActive;							///< last active panel
		QTabBar *qtbTab;														///< tabs for dir views
		QTreeWidgetItem *qtwiLastMovedOver;								///< last item moved over when dragging
		static cSettings::sSort ssSort;									///< sort information

		const void ActualizeDrives() const;								///< drive list actualization
		const void ActualizeVolumeInfo() const;						///< actualize volume information - disk name and space
		const void ActualizeWidgets() const;							///< actualize widgets with info about current directory view
		const void AddHistory(const int &iIndex);						///< add current path to last paths history
																					/**< \param iIndex tab index to add history */
		const void CloseTab(const QMouseEvent *qmeEvent);			///< close tab
																					/**< \param qeEvent description of tab to close */
		const void ConnectFileSystem(const cFileSystem *cfsFileSystem) const;
																					///< connect new file system's signals/slots
																					/**< \param cfsFileSystem file system to connect */
		bool eventFilter(QObject *watched, QEvent *event);			///< event filter
																					/**< \param watched filtered object
																						  \param event event description
																						  \return true if event is handled */
		const QString GetDateTimeString(const QDateTime &qdtDateTime) const;
																					///< convert QDateTime to user defined format
																					/**< \param qdtDateTime date/time to convert
																						  \return user defined date/time format */
		const int GetNativeColumnIndex(const QString &qsColumn, const int &iTabIndex) const;
																					///< find index of native column
																					/**< \param qsColumn native column name
																						  \param iTabIndex tab index
																						  \return index of native column */
		const QString GetSizeString(const qint64 &qi64Size) const;
																					///< "convert" size to string according to setting in options
																					/**< \param qi64Size size
																						  \return file size in string with suffix */
		const void HideOrShowTabBar() const;							///< hide or show tab bar as set in options
		const bool QuickSearch(const QString &qsNextChar, const eQuickSearchDirection &eqsdDirection) const;
																					///< search if quick searched file exists in current dir view
																					/**< \param qsNextChar next filename character to search with
																						  \param eqsdDirection direction of search
																						  \return true if file found */
		const void RefreshContent(const int &iIndex, const bool &bRefresh = true);
																					///< refresh dir content
																					/**< \param iIndex index of dir view
																						  \param bRefresh reload files from file system when true */
		const void RefreshHeader(const int &iIndex, const bool &bContent = false);
																					///< refresh column's header
																					/**< \param iIndex index of dir view
																						  \param bContent refresh content too flag */
		const void SetTabText(const int &iTabIndex) const;			///< set text in tab bar
																					/**< \param iTabIndex tab bar index to set text in */
		const void ShowHideHeader(const int &iTabIndex) const;	///< show or hide header for specified tab
																					/**< \param iTabIndex tab index to show/hide header */
		const void Sort(const int &iIndex, QList<QTreeWidgetItem *> &qlToSort) const;
																					///< sort dir content
																					/**< \param iIndex index of dir view
																						  \param qlToSort files to sort */
		void timerEvent(QTimerEvent *event);							///< automatic actualizations
		static const bool TreeSortByQDateTime(const QTreeWidgetItem *qtwiItem1, const QTreeWidgetItem *qtwiItem2);
																					///< compare items by QDateTime
																					/**< \param qtwiItem1 1st item
																						  \param qtwiItem2 2nd item */
		static const bool TreeSortByQInt64(const QTreeWidgetItem *qtwiItem1, const QTreeWidgetItem *qtwiItem2);
																					///< compare items by qint64
																					/**< \param qtwiItem1 1st item
																						  \param qtwiItem2 2nd item */
		static const bool TreeSortByString(const QTreeWidgetItem *qtwiItem1, const QTreeWidgetItem *qtwiItem2);
																					///< compare items by text
																					/**< \param qtwiItem1 1st item
																						  \param qtwiItem2 2nd item */

	signals:
		void Copy() const;													///< copy marked files
		void Delete() const;													///< delete marked files
		void GotFocus(const cPanel *cpPanel) const;					///< panel got focus
																					/**< \param cpPanel pointer to this panel */
		void FileSystemUnaccessible(cPanel *cpPanel) const;		///< file system unaccessible
																					/**< \param cpPanel pointer to this panel */

	private slots:
		const void on_cfsFileSystem_ContentChanged(const cFileSystem *cfsFileSystem);
																					///< directory content changed for filesystem
																					/**< \param cfsFileSystem filesystem identifier */
		const void on_cfsFileSystem_GotColumnValue(const cContentPluginDelayed::sOutput &soOutput) const;
																					///< got column value from content plugin
																					/**< \param soOutput information to update dir view */
		const void on_cfsFileSystem_Unaccessible(const cFileSystem *cfsFileSystem);
																					///< file system unacessible
																					/**< \param cfsFileSystem filesystem identifier */
		const void on_ctwTree_itemActivated(QTreeWidgetItem *item, int column) const;
																					///< double click in tree view
																					/**< \param item item clicked on
																						  \param column column in item clicked in */
		const void on_ctwTree_customContextMenuRequested(const QPoint &pos) const;
																					///< show tree view context menu
																					/**< \param pos position of context menu */
		const void on_ctwTree_DragEvent();								///< start dragging of selected objects
		const void on_ctwTree_DropEvent(const cTreeWidget::eDropAction &edaAction, const QList<QUrl> &qlUrls, const QString &qsSourceFileSystem, QTreeWidgetItem *qtwiDroppedOn) const;
																					///< drop event occured
																					/**< \param edaAction action to do in this event with source
																						  \param qlUrls source objects location
																						  \param qsSourceFileSystem address of source file system in memory
																						  \param qtwiDroppedOn item dropped on */
		const void on_ctwTree_GotFocus();								///< dir view got focus
		const void on_ctwTree_itemSelectionChanged();				///< changed selected items in current directory view
		const void on_ctwTree_KeyPressed(QKeyEvent *qkeEvent);	///< space pressed in dir view
																					/**< \param qkeEvent key event description */
		const void on_ctwTree_MoveEvent(QTreeWidgetItem *qtwiMovedOver);
																					///< dragging items
																					/**< \param qtwiMovedOver dragging over qtwiMovedOver item */
		const void on_qhvTreeHeader_sectionClicked(int logicalIndex);
																					///< click on header in tree (dir) view
																					/**< \param logicalIndex column index clicked on */
		const void on_qtbTab_currentChanged(int index);				///< tab bar's index changed
																					/**< \param index new tab bar index */
}; // cPanel

#endif
