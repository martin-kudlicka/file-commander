/// dir panel

#ifndef PANEL_H
#define PANEL_H

#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include <QtGui/QStackedWidget>
#include "Settings.h"
#include <QtCore/QHash>
#include "Plugins/ContentPlugin.h"
#include <QtCore/QFileInfo>
#include <QtCore/QFileSystemWatcher>
#include "FileOperation/FileRoutine.h"
#include "Panel/TreeWidget.h"
#include <QtCore/QTimer>
#include "Plugins/ContentPluginDelayed.h"
#include <QtGui/QMainWindow>
#include "Panel/SelectFilesDialog.h"
#include "FileOperation.h"
#include "Plugins/PackerPlugin.h"
#include "ArchiveOperation.h"

class cPanel : public QObject
{
	Q_OBJECT

	public:
		static const qint64 qi64_GIGABYTE = 1073741824;						///< 1 gigabyte in bytes
		static const qint64 qi64_KILOBYTE = 1024;								///< 1 kilobyte in bytes
		static const qint64 qi64_MEGABYTE = 1048576;							///< 1 megabyte in bytes

		/// location of the directory view
		enum eLocation {
			LocalDirectory,															///< standard directory
			Archive																		///< archive contents
		};

		/// history directory list to show
		struct sHistoryDirectoryList {
			QStringList qslDirectories;											///< list of directories to choose from
			int iPosition;																///< actual directory position in the list
		};

		cPanel(QMainWindow *qmwParent, QStackedWidget *qswPanel, QComboBox *qcbDrive, QLabel *qlDriveInfo, QTabBar *qtbTab, QLabel *qlPath, QLabel *qlSelected, cSettings *csSettings, cContentPlugin *ccpContentPlugin, cPackerPlugin *cppPackerPlugin, QMap<QString, cFileRoutine::sDriveInfo> *qmDrives, QLabel *qlGlobalPath, QComboBox *qcbCommand, cFileOperation *cfoFileOperation, QLineEdit *qleQuickSearch);
																							///< constructor
																							/**< \param qmwParent parent window for dialogs
																								  \param qswPanel panel for cTreeWidget
																								  \param qcbDrive drive combo box
																								  \param qlDriveInfo drive name and it's space information
																								  \param qtbTab panel's tab bar
																								  \param qlPath current path
																								  \param qlSelected information about selected directories and files
																								  \param csSettings application's settings
																								  \param ccpContentPlugin application's content plugins
																								  \param cppPackerPlugin application's packer plugins
																								  \param qmDrives information about system drives
																								  \param qlGlobalPath path visible in the bottom of main window
																								  \param qcbCommand command combo box
																								  \param cfoFileOperation handling file operations
																								  \param qleQuickSearch quick search window */
		~cPanel();																		///< destructor

		int AddTab(const cSettings::sTabInfo &stiTabInfo, const bool &bStartUp = false);
																							///< add new tab with dir view
																							/**< \param stiTabInfo new tab description
																								  \param bStartUp true if tab is added on application startup
																								  \return new tab index */
		void ChangePath(const QString &qsPath);								///< change current directory view path even to another drive
																							/**< \param qsPath new path */
		void CloseAllOtherTabs(const int &iTabIndex);						///< close all other tabs than selected
																							/**< \param iTabIndex selected tab */
		void CloseTab(const int &iTabIndex);									///< close tab
																							/**< \param iTabIndex tab to close */
		void CloseTab(const QMouseEvent *qmeEvent);							///< close tab
																							/**< \param qeEvent description of tab to close */
		int DuplicateTab(const int &iTabIndex);								///< create new tab by duplicate one
																							/**< \param iTabIndex tab to duplicate
																								  \return new tab index */
		void FeedToPanel(const QFileInfoList &qfilFiles);					///< show custom list of files in current dir view
																							/**< \param qfilFiles custom list of files */
		cArchiveOperation::sArchive GetArchiveInfo();						///< information about displayed archive in directory view
																							/**< \return information about displayed archive in directory view */
		QList<cSettings::sColumn> *GetColumns();								///< columns for current dir view
																							/**< \return columns for current dir view */
		QString GetColumnSet();														///< column set for current directory view
																							/**< \return column set for current directory view */
		QHash<QTreeWidgetItem *, QFileInfo> GetDirContent();				///< get content of the directory view
																							/**< \return content of the directory view */
		sHistoryDirectoryList GetHistoryDirectoryList();					///< retreive history directory list
																							/**< \return history directory list */
		eLocation GetLocation();													///< location of current tab directory view
																							/**< \return location of current tab directory view */
		QString GetPath();															///< get path for current dir
																							/**< \return current dir view path */
		QList<tHeaderData> GetSelectedItemsArchiveList();					///< collect selected archive files
																							/**< \return selected archive files */
		QFileInfoList GetSelectedItemsFileList(const QDir::Filters &qfType = QDir::Dirs | QDir::Files);
																							///< get file infos of selected items
																							/**< \param qfType type of files to retreive info about
																								  \return file info list about selected files */
		QDir::Filters GetStandardFilters();										///< filters for file query
																							/**< \return filter for file query */
		int GetTabIndex(const QPoint &qpPos);									///< find out tab index in tab bar
																							/**< \param qpPos cursor position in tab bar
																								  \return tab index */
		void GoToFile(const QString &qsFile);									///< show dir view with specified file
																							/**< \param qsFile file to find */
		void GoToRootDir();															///< go to root directory
		void GoToUpDir();																///< go up one level (directory)
		void HistoryGoBack();														///< go back in history directory list
		void HistoryGoFront();														///< go front in history directory list
		void InvertSelection();														///< invert selection of files
		bool IsActive();																///< active panel flag
																							/**< \return true if panel is active */
		void RefreshContent(const QString &qsFocusTo = QString());		///< refresh current dir view
																							/**< \param qsFocusTo set focus to this file */
		void RefreshContent(const QFileInfoList &qfilFiles);				///< refresh current dir view with custom files
																							/**< \param qfilFiles custom list of files */
		void RefreshAllContents();													///< refresh all dir views
		void RefreshAllHeaders();													///< refresh all dir view headers
		void RefreshTabs();															///< refresh tabs
		void ReverseOrder();															///< reverse sort order
		void SaveSettings(const cSettings::ePosition &epPosition);		///< save panel settings
																							/**< \param epPosition panel's position */
		void Select(const cSelectFilesDialog::eSelectType &estType, cListerPlugin *clpListerPlugin);
																							///< select or unselect some files
																							/**< \param estType select/unselect mode
																								  \param clpListerPlugin lister plugins */
		void SelectAll();																///< select all files
		void SetColumnSet(const QString &qsColumnSet);						///< selected another column set for actual directory view
																							/**< \param qsColumnSet new column set */
		void SetFocus();																///< set focus to current directory view
		void SetHistoryDirectory(const int &iPosition);						///< set path by directory from history list
																							/**< \param iPosition directory position in history list */
		void SortBy(const int &iColumn);											///< sort by specified column
																							/**< \param iColumn column position to sort by */
		void UnselectAll();															///< unselect all files

	private:
		static const int iTIMER_INTERVAL = 1000;								///< timer interval

		/// quick search direction in directory view
		enum eQuickSearchDirection {
			SearchUp,																	///< search up
			SearchDown																	///< search down
		};

		/// last visited path
		struct sLastPath {
			QString qsLocalDirectory;												///< path in local directory
			QString qsArchive;														///< path in archive
			QString qsPathInArchive;												///< path in archive
			QString qsShow;															///< path to show in history view
			eLocation elLocation;													///< location of the directory view
		};
		/// directory history information
		struct sHistory {
			QList<sLastPath> qlLastPaths;											///< list of last visited paths
			int iPosition;																///< current position in last paths list
		};
		/// listed local directory information
		struct sLocalDirectory {
			QHash<QTreeWidgetItem *, QFileInfo> qhFiles;						///< info about files listed in dir panel
			QString qsPath;															///< path in local directory
		};
		/// strings for widgets
		struct sWidgets {
			QString qsDrive;															///< selected drive
			QString qsPath;															///< path on drive
			QString qsSelected;														///< selected item info
		};
		/// tab information
		struct sTab {
			bool bValid;																///< tab content is validated flag
			QList<cSettings::sColumn> *qlColumns;								///< columns in specified tab
			QString qsColumnSet;														///< column set for tab
			sWidgets *swWidgets;														///< to remember displayed strings
			eLocation elLocation;													///< location of the directory view
			sLocalDirectory sldLocalDirectory;									///< local directory information
			cArchiveOperation::sArchive saArchive;								///< archive information
			sHistory shHistory;														///< directory history information
		};

		bool bNewDirectory;															///< creating new directory
		cContentPlugin *ccpContentPlugin;										///< access to content plugins
		cContentPluginDelayed *ccpdContentPluginDelayed;					///< thread to get delayed content plugins values
		cFileOperation *cfoFileOperation;										///< handling file operations
		cPackerPlugin *cppPackerPlugin;											///< access to packer plugins
		cSettings *csSettings;														///< main settings
		QComboBox *qcbCommand;														///< command combo box
		QComboBox *qcbDrive;															///< drive
		QFileSystemWatcher qfswWatcher;											///< watching directory changes
		QHash<int, sTab> qhTabs;													///< tabs in current panel
		QHash<QString, QString> qhLastPaths;									///< last visited paths for drives
		QLabel *qlGlobalPath;														///< path visible in the bottom of main window
		QLabel *qlPath;																///< path
		QLabel *qlDriveInfo;															///< drive info - label, used/free place
		QLabel *qlSelected;															///< selected items
		QLineEdit *qleQuickSearch;													///< quick search window
		QMainWindow *qmwParent;														///< parent window for dialogs
		QMap<QString, cFileRoutine::sDriveInfo> *qmDrives;					///< drives in system
		QStackedWidget *qswDir;														///< directory view
		QTabBar *qtbTab;																///< tabs for dir view
		QTimer qtTimer;																///< timer for requesting changeable informations
		QTreeWidgetItem *qtwiLastMovedOver;										///< last item moved over when dragging
		static cSettings::sSort ssSort;											///< sort information
		static QStackedWidget *qswLastActive;									///< last active panel

		void ActualizeVolumeInfo();												///< actualize volume information - disk name and space
		void ActualizeWidgets();													///< actualize widgets with info about current directory view
		void AddHistory(const int &iIndex);										///< add current path to last paths history
																							/**< \param iIndex tab index to add history */
		QString DateTime(const QDateTime &qdtDateTime);						///< convert QDateTime to user defined format
																							/**< \param qdtDateTime date/time to convert
																								  \return formatted date/time string */
		bool DirInAnotherTab(const int &iIndex, const QString &qsDirectory);
																							///< check if directory is displayed in another tab too
																							/**< \param iIndex tab index to ignore
																								  \param qsDirectory directory (full path) name
																								  \return true if in another tab too */
		bool eventFilter(QObject *watched, QEvent *event);					///< event filter
																							/**< \param watched filtered object
																								  \param event event description
																								  \return true if event is handled */
		void FillDirViewItem(const int &iIndex, const eLocation &elType, QTreeWidgetItem *qtwiFile, const void *vData, QList<cContentPluginDelayed::sParameters> *qlParameters);
																							///< fill directory view item accodring to content of vData
																							/**< \param iIndex directory view tab index
																								  \param elType type of item
																								  \param qtwiItem item to fill
																								  \param vData data to fill by
																								  \param qlParameters parameters for delayed content plugins */
		int GetNativeColumnIndex(const QString &qsColumn, const int &iTabIndex);
																							///< find index of native column
																							/**< \param qsColumn native column name
																								  \param iTabIndex tab index
																								  \return index of native column */
		QStringList GetSelectedItemsStringList();								///< get selected files and directories from current dir view
																							/**< \return names of selected items in string list */
		QString GetSizeString(const qint64 &qi64Size);						///< "convert" size to string according to setting in options
																							/**< \param qi64Size size
																								  \return file size in string with suffix */
		void HideOrShowTabBar();													///< hide or show tab bar as set in options
		bool IsRootDirectory(const QString &qsDirectory);					///< find out if qsDirectory points to the root
																							/**< \param qsDirectory directory to test
																								  \return true if qsDirectory is root directory */
		bool OpenArchive(const QFileInfo &qfiFile);							///< try to open archive
																							/**< \param qfiFile archive filename */
#ifdef Q_WS_WIN
		bool PathExists(const QString &qsPath);								///< check if path is valid
																							/**< \param qsPath path to test
																								  \return true if path exists and is accessible */
#endif
		bool QuickSearch(const QString &qsNextChar, const eQuickSearchDirection &eqsdDirection);
																							///< search if quick searched file exists in current dir view
																							/**< \param qsNextChar next filename character to search with
																								  \param eqsdDirection direction of search
																								  \return true if file found */
		void RefreshContent(const int &iIndex, QFileInfoList qfilFiles = QFileInfoList());
																							///< refresh dir content with local directory files
																							/**< \param iIndex index of dir view
																								  \param qfilFiles custom list of files */
		void RefreshHeader(const int &iIndex, const bool &bContent = false);
																							///< refresh column's header
																							/**< \param iIndex index of dir view
																								  \param bContent refresh content too flag */
		void SetPath(const QString &qsPath);									///< set new path for current dir view on selected drive
																							/**< \param qsPath new path */
		void SetTabText(const int &iTabIndex);									///< set text in tab bar
																							/**< \param iTabIndex tab bar index to set text in */
		void Sort(const int &iIndex);												///< sort dir content
																							/**< \param iIndex index of dir view */
		static bool TreeSortByQDateTime(const QTreeWidgetItem *qtwiItem1, const QTreeWidgetItem *qtwiItem2);
																							///< compare items by QDateTime
																							/**< \param qtwiItem1 1st item
																								  \param qtwiItem2 2nd item */
		static bool TreeSortByQInt64(const QTreeWidgetItem *qtwiItem1, const QTreeWidgetItem *qtwiItem2);
																							///< compare items by qint64
																							/**< \param qtwiItem1 1st item
																								  \param qtwiItem2 2nd item */
		static bool TreeSortByString(const QTreeWidgetItem *qtwiItem1, const QTreeWidgetItem *qtwiItem2);
																							///< compare items by text
																							/**< \param qtwiItem1 1st item
																								  \param qtwiItem2 2nd item */

	signals:
		void CopyArchiveFiles();													///< copy of archive files called
		void Delete();																	///< delete marked files
		void GotFocus();																///< panel got focus
		void InterruptContentDelayed();											///< interrupt delayed content processing before refresh dir view content

	private slots:
		void on_ccpdContentPluginDelayed_GotColumnValue(const cContentPluginDelayed::sOutput &soOutput);
																							///< got golumn value from plugin
																							/**< \param soOutput information to update dir view */
		void on_ctwTree_customContextMenuRequested(const QPoint &pos);	///< show tree view context menu
																							/**< \param pos position of context menu */
		void on_ctwTree_DragEvent();												///< start dragging of selected objects
		void on_ctwTree_DropEvent(const cTreeWidget::eDropAction &edaAction, const QList<QUrl> &clUrls, const QString &qsArchiveInformation, const QString &qsArchiveFiles, QTreeWidgetItem *qtwiDroppedOn);
																							///< drop event occured
																							/**< \param edaAction action to do in this event with source
																								  \param clUrls source objects location
																								  \param qsArchiveInformation address of archive information structure
																								  \param qsArchiveFiles files to extract from archive
																								  \param qtwiDroppedOn item dropped on */
		void on_ctwTree_GotFocus();												///< dir view got focus
		void on_ctwTree_itemActivated(QTreeWidgetItem *item, int column);
																							///< double click in tree view
																							/**< \param item item clicked on
																								  \param column column in item clicked in */
		void on_ctwTree_itemSelectionChanged(const cTreeWidget *ctwTree);
																							///< changed selected items in directory view
																							/**< \param ctwTree changed directory view */
		void on_ctwTree_KeyPressed(QKeyEvent *qkeEvent, QTreeWidgetItem *qtwiItem);
																							///< space pressed in dir view
																							/**< \param qkeEvent key event description
																								  \param qtwiItem item space pressed on */
		void on_ctwTree_MoveEvent(QTreeWidgetItem *qtwiMovedOver);		///< dragging items
																							/**< \param qtwiMovedOver dragging over qtwiMovedOver item */
		void on_qcbDrive_activated(int index);									///< drive selected
																							/**< \param index index of selected drive */
		void on_qcbDrive_currentIndexChanged(int index);					///< selected drive changes
																							/**< \param index index of selected drive in combo box */
		void on_qfswWatcher_directoryChanged(const QString &path);		///< detect directory modifications
																							/**< \param path directory to watch */
		void on_qhvTreeHeader_sectionClicked(int logicalIndex);			///< click on header in tree (dir) view
																							/**< \param logicalIndex column index clicked on */
		void on_qtbTab_currentChanged(int index);								///< tab bar's index changed
																							/**< \param index new tab bar index */
		void on_qtTimer_timeout();													///< timer's timeout
}; // cPanel

#endif
