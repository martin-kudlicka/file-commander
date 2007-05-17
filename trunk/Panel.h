/// dir panel

#ifndef PANEL_H
#define PANEL_H

#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include <QtGui/QStackedWidget>
#include "Settings.h"
#include <QtCore/QHash>
#include "Plugins/Content.h"
#include <QtCore/QFileInfo>
#include <QtGui/QFileIconProvider>
#include "Panel/ShellMenu.h"
#include <QtCore/QFileSystemWatcher>
#include "FileOperation/FileRoutine.h"
#include "Panel/TreeWidget.h"
#include <QtCore/QTimer>
#include "Plugins/ContentDelayed.h"
#include <QtGui/QMainWindow>
#include "Panel/SelectFilesDialog.h"
#include "FileOperation.h"

class cPanel : public QObject
{
	Q_OBJECT

	public:
		static const qint64 qi64_GIGABYTE = 1073741824;						///< 1 gigabyte in bytes
		static const qint64 qi64_KILOBYTE = 1024;								///< 1 kilobyte in bytes
		static const qint64 qi64_MEGABYTE = 1048576;							///< 1 megabyte in bytes

		cPanel(QMainWindow *qmwParent, QStackedWidget *qswPanel, QComboBox *qcbDrive, QLabel *qlDriveInfo, QTabBar *qtbTab, QLabel *qlPath, QLabel *qlSelected, cSettings *csSettings, cContent *ccContent, QMap<QString, cFileRoutine::sDriveInfo> *qmDrives, QLabel *qlGlobalPath, QComboBox *qcbCommand, cFileOperation *cfoFileOperation, QLineEdit *qleQuickSearch);
																							///< constructor
																							/**< \param qmwParent parent window for dialogs
																								  \param qswPanel panel for cTreeWidget
																								  \param qcbDrive drive combo box
																								  \param qlDriveInfo drive name and it's space information
																								  \param qtbTab panel's tab bar
																								  \param qlPath current path
																								  \param qlSelected information about selected directories and files
																								  \param csSettings application's settings
																								  \param ccContent application'c content plugins
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
		void CloseAllOtherTabs(const int &iTabIndex);						///< close all other tabs than selected
																							/**< \param iTabIndex selected tab */
		void CloseTab(const int &iTabIndex);									///< close tab
																							/**< \param iTabIndex tab to close */
		void CloseTab(const QMouseEvent *qmeEvent);							///< close tab
																							/**< \param qeEvent description of tab to close */
		int DuplicateTab(const int &iTabIndex);								///< create new tab by duplicate one
																							/**< \param iTabIndex tab to duplicate
																								  \return new tab index */
		void FeedToPanel(QFileInfoList &qfilFiles);							///< show custom list of files in current dir view
																							/**< \param qfilFiles custom list of files */
		QList<cSettings::sColumn> *GetColumns();								///< columns for current dir view
																							/**< \return columns for current dir view */
		QHash<QTreeWidgetItem *, QFileInfo> *GetDirContent();				///< get content of the directory view
																							/**< \return content of the directory view */
		QString GetPath();															///< get path for current dir
																							/**< \return current dir view path */
		QFileInfoList GetSelectedDirItemsList();								///< get dir infos of selected items
																							/**< \return dir info list about selected directories */
		QFileInfoList GetSelectedItemsList();									///< get file infos of selected items
																							/**< \return file info list about selected files */
		QDir::Filters GetStandardFilters();										///< filters for file query
																							/**< \return filter for file query */
		int GetTabIndex(const QPoint &qpPos);									///< find out tab index in tab bar
																							/**< \param qpPos cursor position in tab bar
																								  \return tab index */
		void GoToFile(const QString &qsFile);									///< show dir view with specified file
																							/**< \param qsFile file to find */
		void GoToRootDir();															///< go to root directory
		void GoToUpDir();																///< go up one level (directory)
		void InvertSelection();														///< invert selection of files
		bool IsActive();																///< active panel flag
																							/**< \return true if panel is active */
		void RefreshContent();														///< refresh current dir view
		void RefreshContent(QFileInfoList &qfilFiles);						///< refresh current dir view with custom files
																							/**< \param qfilFiles custom list of files */
		void RefreshAllContents();													///< refresh all dir views
		void RefreshAllHeaders();													///< refresh all dir view headers
		void RefreshTabs();															///< refresh tabs
		void ReverseOrder();															///< reverse sort order
		void SaveSettings(const cSettings::ePosition &epPosition);		///< save panel settings
																							/**< \param epPosition panel's position */
		void Select(const cSelectFilesDialog::eSelectType &estType, cLister *clLister);
																							///< select or unselect some files
																							/**< \param estType select/unselect mode
																								  \param clLister lister plugins */
		void SelectAll();																///< select all files
		void SetFocus();																///< set focus to current directory view
		void SortBy(const int &iColumn);											///< sort by specified column
																							/**< \param iColumn column position to sort by */
		void UnselectAll();															///< unselect all files

	private:
		static const int iTIMER_INTERVAL = 1000;								///< timer interval

		///< quick search direction in directory view
		enum eQuickSearchDirection {
			SearchUp,																	///< search up
			SearchDown																	///< search down
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
			QHash<QTreeWidgetItem *, QFileInfo> *qhFiles;					///< info about files listed in dir panel
			QList<cSettings::sColumn> *qlColumns;								///< columns in specified tab
			QString qsColumnSet;														///< column set for tab
			sWidgets *swWidgets;														///< to remember displayed strings
		};

		cContent *ccContent;															///< access to content plugins
		cContentDelayed *ccdContentDelayed;										///< thread to get delayed content plugins values
		cFileOperation *cfoFileOperation;										///< handling file operations
		cSettings *csSettings;														///< main settings
		cShellMenu *csmMenu;															///< right click "native" shell menu
		QComboBox *qcbCommand;														///< command combo box
		QComboBox *qcbDrive;															///< drive
		QFileIconProvider qfipIcon;												///< icons
		QFileSystemWatcher qfswWatcher;											///< watching directory changes
		QHash<uint, sTab> qhTabs;													///< tabs in current panel
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
		static cSettings::sSort ssSort;											///< sort information
		static QStackedWidget *qswLastActive;									///< last active panel

		void ActualizeVolumeInfo();												///< actualize volume information - disk name and space
		void ActualizeWidgets();													///< actualize widgets with info about current directory view
		QString DateTime(const QDateTime &qdtDateTime);						///< convert QDateTime to user defined format
																							/**< \param qdtDateTime date/time to convert
																								  \return formatted date/time string */
		bool eventFilter(QObject *watched, QEvent *event);					///< event filter
																							/**< \param watched filtered object
																								  \param event event description
																								  \return true if event is handled */
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
																							///< refresh dir content
																							/**< \param iIndex index of dir view
																								  \param qfilFiles custom list of files */
		void RefreshHeader(const int &iIndex, const bool &bContent = false);
																							///< refresh column's header
																							/**< \param iIndex index of dir view
																								  \param bContent refresh content too flag */
		void SetPath(const QString &qsPath);									///< set new path for current dir view
																							/**< \param qsPath new path */
		void SetTabText(const int &iTabIndex);									///< set text in tab bar
																							/**< \param iTabIndex tab bar index to set text in */
		void Sort(const int &iIndex);												///< sort dir content
																							/**< \param iIndex index of dir view */
		static bool TreeSortByString(const QTreeWidgetItem *qtwiItem1, const QTreeWidgetItem *qtwiItem2);
																							///< compare items by text
																							/**< \param qtwiItem1 1st item
																								  \param qtwiItem2 2nd item */

	signals:
		void GotFocus();																///< panel got focus
		void InterruptContentDelayed();											///< interrupt delayed content processing before refresh dir view content

	private slots:
		void on_ccdContentDelayed_GotColumnValue(const cContentDelayed::sOutput &soOutput);
																							///< got golumn value from plugin
																							/**< \param soOutput information to update dir view */
		void on_ctwTree_customContextMenuRequested(const QPoint &pos);	///< show tree view context menu
																							/**< \param pos position of context menu */
		void on_ctwTree_DragEvent(cTreeWidget *ctwSource);					///< start dragging of selected objects
																							/**< \param ctwSource source dir view */
		void on_ctwTree_DropEvent(const cTreeWidget::eDropAction &edaAction, const QList<QUrl> &clUrls);
																							///< drop event occured
																							/**< \param edaAction action to do in this event with source
																								  \param clUrls source objects location */
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
