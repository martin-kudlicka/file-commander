/// setting file accessing

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtCore/QSettings>
#include <QtCore/QPair>

const QString qsANSI = "ANSI";
const QString qsASK = "Ask";
#ifdef Q_WS_WIN
const QString qsATTRIBUTES = "Attributes";
#endif
const QString qsBYTES = "Bytes";
const QString qsDATE_TIME = "DateTime";
const QString qsDYNAMIC = "Dynamic";
const QString qsEXTENSION = "Extension";
const QString qsFULL = "Full";					///< full dir panel view (icon, filename, extension, date)
const QString qsGIGABYTES = "Gigabytes";
const QString qsICON = "Icon";
const QString qsINTERNAL = "Internal";
const QString qsKILOBYTES = "Kilobytes";
const QString qsMEGABYTES = "Megabytes";
const QString qsNAME = "Name";
const QString qsNAME_WITH_EXTENSION = "NameWithExtension";
const QString qsNO = "no";
const QString qsONLY_FILES = "OnlyFiles";
const QString qsSIZE = "Size";

// shortcuts
const QString qsSHORTCUT__LISTER__FILE__OPEN = "Open...";
const QString qsSHORTCUT__LISTER__FILE__SAVE_AS = "Save as...";
const QString qsSHORTCUT__LISTER__FILE__PRINT = "Print...";
const QString qsSHORTCUT__LISTER__FILE__PRINT_SETUP = "Print setup...";
const QString qsSHORTCUT__LISTER__FILE__CLOSE = "Close";
const QString qsSHORTCUT__LISTER__EDIT__COPY_AS_TEXT = "Copy as text";
const QString qsSHORTCUT__LISTER__EDIT__SELECT_ALL = "Select all";
const QString qsSHORTCUT__LISTER__EDIT__FIND = "Find...";
const QString qsSHORTCUT__LISTER__EDIT__FIND_NEXT = "Find next";
const QString qsSHORTCUT__LISTER__OPTIONS__TEXT = "Text";
const QString qsSHORTCUT__LISTER__OPTIONS__BINARY = "Binary";
const QString qsSHORTCUT__LISTER__OPTIONS__HEX = "Hex";
const QString qsSHORTCUT__LISTER__OPTIONS__MULTIMEDIA = "Multimedia";
const QString qsSHORTCUT__LISTER__OPTIONS__ANSI = "ANSI (Windows char set)";
const QString qsSHORTCUT__LISTER__OPTIONS__ASCII = "ASCII (DOS char set)";
const QString qsSHORTCUT__LISTER__OPTIONS__VARIABLE_CHAR_WIDTH = "Variable char width";
const QString qsSHORTCUT__LISTER__OPTIONS__WRAP_TEXT = "Wrap text";
const QString qsSHORTCUT__LISTER__OPTIONS__FIT_IMAGE_TO_WINDOW = "Fit image to window";
const QString qsSHORTCUT__MAIN_MENU__FILE__UNPACK_SELECTED_FILES = "Unpack selected files...";
const QString qsSHORTCUT__MAIN_MENU__FILE__QUIT = "Quit";
const QString qsSHORTCUT__MAIN_MENU__MARK__SELECT_GROUP = "Select group...";
const QString qsSHORTCUT__MAIN_MENU__MARK__UNSELECT_GROUP = "Unselect group...";
const QString qsSHORTCUT__MAIN_MENU__MARK__SELECT_ALL = "Select all";
const QString qsSHORTCUT__MAIN_MENU__MARK__UNSELECT_ALL = "Unselect all";
const QString qsSHORTCUT__MAIN_MENU__MARK__INVERT_SELECTION = "Invert selection";
const QString qsSHORTCUT__MAIN_MENU__MARK__COMPARE_DIRECTORIES = "Compare directories";
const QString qsSHORTCUT__MAIN_MENU__COMMANDS__SEARCH = "Search...";
const QString qsSHORTCUT__MAIN_MENU__COMMANDS__FAVOURITE_DIRECTORIES = "Favourite directories";
const QString qsSHORTCUT__MAIN_MENU__COMMANDS__HISTORY_DIRECTORY_LIST = "History directory list";
const QString qsSHORTCUT__MAIN_MENU__COMMANDS__BRANCH_VIEW = "Branch view";
const QString qsSHORTCUT__MAIN_MENU__COMMANDS__FULLSCREEN = "Fullscreen";
const QString qsSHORTCUT__MAIN_MENU__PANEL__SORT_BY_FIRST_COLUMN = "Sort by first column";
const QString qsSHORTCUT__MAIN_MENU__PANEL__SORT_BY_SECOND_COLUMN = "Sort by second column";
const QString qsSHORTCUT__MAIN_MENU__PANEL__SORT_BY_THIRD_COLUMN = "Sort by third column";
const QString qsSHORTCUT__MAIN_MENU__PANEL__SORT_BY_FOURTH_COLUMN = "Sort by fourth column";
const QString qsSHORTCUT__MAIN_MENU__PANEL__REVERSE_ORDER = "Reverse order";
const QString qsSHORTCUT__MAIN_MENU__PANEL__REFRESH = "Refresh";
const QString qsSHORTCUT__MAIN_MENU__CONFIGURATION__OPTIONS = "Options...";
const QString qsSHORTCUT__MAIN_MENU__CONFIGURATION__SAVE_POSITION = "Save position";
const QString qsSHORTCUT__MAIN_MENU__CONFIGURATION__SAVE_SETTINGS = "Save settings";
const QString qsSHORTCUT__OPERATIONS__DIALOG__ENQUEUE = "Enqueue";
const QString qsSHORTCUT__PANELS__DIRECTORY_VIEW__DRIVE_LEFT = "Drive left";
const QString qsSHORTCUT__PANELS__DIRECTORY_VIEW__DRIVE_RIGHT = "Drive right";
const QString qsSHORTCUT__PANELS__DIRECTORY_VIEW__HISTORY_BACK = "History back";
const QString qsSHORTCUT__PANELS__DIRECTORY_VIEW__HISTORY_FRONT = "History front";
const QString qsSHORTCUT__PANELS__MAIN_BUTTON__TERMINAL = "Terminal";
const QString qsSHORTCUT__PANELS__MAIN_BUTTON__VIEW = "View";
const QString qsSHORTCUT__PANELS__MAIN_BUTTON__EDIT = "Edit";
const QString qsSHORTCUT__PANELS__MAIN_BUTTON__COPY = "Copy";
const QString qsSHORTCUT__PANELS__MAIN_BUTTON__MOVE = "Move";
const QString qsSHORTCUT__PANELS__MAIN_BUTTON__NEW_DIRECTORY = "New directory";
const QString qsSHORTCUT__PANELS__MAIN_BUTTON__DELETE = "Delete";
const QString qsSHORTCUT__PANELS__TABS__DUPLICATE_TAB = "Duplicate tab";
const QString qsSHORTCUT__PANELS__TABS__CLOSE_TAB = "Close tab";
const QString qsSHORTCUT__PANELS__TABS__CLOSE_ALL_OTHER_TABS = "Close all other tabs";

class cSettings : private QObject
{
	public:
		/// panel's position
		enum ePosition {
			PositionLeft,																	///< left panel
			PositionRight																	///< right panel
		};
		/// plugin type
		enum ePlugin {
			ContentPlugins,																///< content plugin type
			ListerPlugins,																	///< lister plugin type
			PackerPlugins																	///< packer plugin type
		};
		/// shortcut category
		enum eShortcutCategory {
			MainMenuCategory,																///< main menu shortcut category
			PanelsCategory,																///< panels shortcut category
			OperationsCategory,															///< operations shortcut category
			ListerCategory																	///< lister shortcut category
		};

		struct sColumn {
			QString qsIdentifier;														///< column identifier
			QString qsName;																///< column name to show
			QString qsPlugin;																///< plugin filename or "no" if native
			QString qsUnit;																///< selected unit for column
			int iWidth;																		///< column width
		};
		/// favourite directory
		struct sFavouriteDirectory {
			bool bSubmenu;																	///< true if submenu
			QString qsSource;																///< path for source panel
			bool bTarget;																	///< set target path too
			QString qsTarget;																///< path for destination panel
			QList<QPair<QString, sFavouriteDirectory> > qlChildFavourites;	///< can contain children favourites if bSubmenu is true
		};
		/// lister settings
		struct sLister {
			QString qsCharSet;															///< selected char set
			bool bWrapText;																///< word wrapping flag
			bool bFitImageToWindow;														///< image auto resize flag
		};
		/// main window state
		struct sMainWindowState {
			int iHeight;																	///< height of main window
			int iWidth;																		///< width of main window
			QString qsWindowState;														///< state of main window
		};
		/// plugin
		struct sPlugin {
			QString qsName;																///< plugin's file name
			QString qsExtensions;														///< packer plugin's extensions
			bool bEnabled;																	///< enable/disable flag
		};
		/// sort information
		struct sSort {
			int iSortedColumn;															///< column to sort by
			Qt::SortOrder soSortOrder;													///< sort order
			bool bCaseSensitive;															///< case sensitive sorting
		};
		/// tab info
		struct sTabInfo {
			QString qsColumnSet;															///< column set defined in tab
			QString qsDrive;																///< drive
			QString qsPath;																///< path selected in tab
			sSort ssSort;																	///< sort information
		};

		const void CreateColumnSet(const QString &qsColumnSet);				///< create new empty column set
																								/**< \param qsColumnSet column set to create */
		const void CreateColumnSet(const QString &qsColumnSet, QList<sColumn> &qlColumns);
																								///< create new column set
																								/**< \param qsColumnSet column set to create
																									  \param qlColumns columns in column set */
		const void CreateDefaultColumnSet();										///< create default (Full) column set
		const void CreateTab(const ePosition &epPosition, const uint &uiIndex, sTabInfo &stiTab);
																								///< create new tab in settings file
																								/**< \param epPosition left or right panel
																									  \param uiIndex index of tab in tab bar
																									  \param stiTab tab information */
		const QMap <QString, QString> GetAllSettings() const;					///< get all application's settings
																								/**< \return list of whole settings file */
		const bool GetAskToDeleteNonEmptyDirectory() const;					///< delete non empty directory
																								/**< return delete non empty directory flag */
		const bool GetCaseSensitiveSorting() const;								///< case sensitive sorting
																								/**< \return true if sorting is case sensitive */
		const bool GetCloseTabOnDoubleClick() const;								///< close tab in tab bar on double click
																								/**< \return true if can close on double click */
		const sColumn GetColumnInfo(const QString &qsColumnSet, const QString &qsColumn);
																								///< get some information about column
																								/**< \param qsColumnSet column's column set
																									  \param qsColumn column index
																									  \return column information */
		const QStringList GetColumnSets();											///< get column sets
																								/**< \return list of column sets */
		const QStringList GetColumnsInSet(const QString &qsColumnSet);		///< get column names in column set
																								/**< \param qsColumnSet name of column set
																									  \return list of columns in column set */
		const bool GetConfirmCloseOfAllTabs() const;								///< confirm close of all tabs in tab bar
																								/**< return confirm close of all tabs flag */
		const int GetCopyMoveBufferSize() const;									///< copy/move buffer size
																								/**< \return copy/move buffer size in kilobytes */
		const QString GetDateTimeDisplay() const;									///< date/time display format
																								/**< \return date/time display format */
#ifdef Q_WS_WIN
		const bool GetDeleteToRecycleBin() const;									///< delete files to recycle bin
																								/**< \return true if delete to recycle bin */
#endif
		const QString GetExternalEditor() const;									///< external editor
																								/**< \return external editor command line */
		const QString GetExternalViewer() const;									///< external viewer
																								/**< \return external viewer command line */
		QList<QPair<QString, cSettings::sFavouriteDirectory> > GetFavouriteDirectories();
																								///< get favourite directories
																								/**< \return list of favourite directories */
		const QString GetFileOverwrite() const;									///< find out file overwrite mode
																								/**< \return default file overwrite mode */
		const QString GetFileSizeIn() const;										///< unit for files size
																								/**< \return file size unit */
		const QFont GetListerFont() const;											///< font used in lister
																								/**< \return user defined font for lister */
		const sLister GetListerSettings();											///< lister settings
																								/**< \return lister settings */
		const int GetMaximumHistoryDirectoryListSize() const;					///< get maximum items in directory history list
																								/**< \return maximum items in directory history list */
		const bool GetNewTabByShortcutInForeground() const;					///< new tab by shortcut in foreground
																								/** \return new tab by shortcut in foreground flag */
		const bool GetOpenNewTabNextToCurrentTab() const;						///< open new tab next to current tab
																								/**< \return open new tab next to current tab flag */
		const QString GetPluginDateTimeDisplay() const;							///< plugin date/time display format
																								/**< \return plugin date/time display format */
		const QList<sPlugin> GetPlugins(const ePlugin &epPlugin);			///< get plugin list
																								/**< \param epPlugin plugin type
																									  \return plugin list */
		const QString GetPluginTimeDisplay() const;								///< plugin time display format
																								/**< \return plugin time display format */
		const bool GetQuickSearchAlt() const;										///< alt key activity in quick search
																								/**< \return alt key activity flag */
		const bool GetQuickSearchCtrl() const;										///< ctrl key activity in quick search
																								/**< \return ctrl key activity flag */
		const bool GetQuickSearchEnabled() const;									///< enabled/disabled quick search
																								/**< \return enable/disable quick search flag */
		const bool GetQuickSearchShift() const;									///< shift key activity in quick search
																								/**< \return shift key activity flag */
		const bool GetQuickSearchShowSearchWindow() const;						///< quick search window visibility
																								/**< \return quick search window visibility flag */
		const QString GetReadonlyFileOverwrite() const;							///< find out readonly file overwrite mode
																								/**< \return default readonly file overwrite mode */
		const bool GetSaveSettingsOnQuit() const;									///< save settings on quit option
																								/**< \return saves settings on quit when true */
		const QString GetSelectionMode() const;									///< selection mode of files and directories
																								/**< \return selection mode */
		const QString GetShortcut(const eShortcutCategory &escCategory, const QString &qsShortcut) const;
																								///< find shortcut
																								/**< \param escCategory shortcut category
																									  \param qsShortcut shortcut name
																									  \return shortcun key sequence */
		const bool GetShowBracketsAroundDirectoryName() const;				///< square brackets around directory name
																								/**< \return show brackets around directory name flag */
		const bool GetShowDirectoryViewHeader() const;							///< get directory view header show flag
																								/**< \return directory view header show flag */ 
		const bool GetShowDriveLetter() const;										///< show drive letter in tab
																								/**< \return show drive letter in tab flag */
		const bool GetShowHiddenFiles() const;										///< show hidden files in dir view flag
																								/**< \return true if show hidden files */
		const bool GetShowSystemFiles() const;										///< show system files in dir view flag
																								/**< \return true if show system files */
		const bool GetShowTabBarWithOnlyOneTab() const;							///< show tab bar with only one tab flag
																								/**< \return true if show tab bar with only tab */
		const sTabInfo GetTabInfo(const ePosition &epPosition, const QString &qsIndex);
																								///< get some information about tab
																								/**< \param epPosition left or right panel
																									  \param qsIndex index of tab
																									  \return information about panel's tab */
		const QStringList GetTabs(const ePosition &epPosition);				///< get tab list for left or right panel
																								/**< \param epPosition left or right panel
																									  \return list of tabs in panel */
		const bool GetTreatArchivesLikeDirectories() const;					///< treat archives like directories
																								/**< \return browse through archives like by directories if true */
		const QString GetViewerType() const;										///< viewer type
																								/**< \return internal or external viewer */
		const sMainWindowState GetWindowState();									///< retrieve startup main window parameters
																								/**< \return startup main window parameters */
		const void RemoveColumnSet(const QString &qsColumnSet);				///< remove column set
																								/**< \param qsColumnSet column set to remove */
		const void RestoreSettings(QMap <QString, QString> &qmSettings);	///< restore old application's settings
																								/**< \param qmSettings settings to restore */
		const void SetAskToDeleteNonEmptyDirectory(const bool &bAsk);		///< delete non empty directory
																								/**< \param bAsk delete non empty directory flag */
		const void SetCaseSensitiveSorting(const bool &bCaseSensitive);	///< case sensitive sorting
																								/**< \param bCaseSensitive true if sorting is case sensitive */
		const void SetCloseTabOnDoubleClick(const bool &bClose);				///< close tab on double click
																								/**< \param bClose can close tab on double click if true */
		const void SetConfirmCloseOfAllTabs(const bool &bClose);				///< confirm close of all tabs
																								/**< \param bClose confirm close of all tabs flag */
		const void SetCopyMoveBufferSize(const int &iSize);					///< copy/move buffer size
																								/**< \param iSize copy/move buffer size in kilobytes */
		const void SetDateTimeDisplay(const QString &qsDateTime);			///< date/time display format
																								/**< \param qsDateTime user defined date/time display format */
#ifdef Q_WS_WIN
		const void SetDeleteToRecycleBin(const bool &bRecycleBin);			///< delete files to recycle bin
																								/**< \param bRecycleBin true if delete to recycle bin */
#endif
		const void SetExternalEditor(const QString &qsEditor);				///< external editor
																								/**< \param qsEditor external editor command line */
		const void SetExternalViewer(const QString &qsViewer);				///< external viewer
																								/**< \param qsViewer external viewer command line */
		const void SetFavouriteDirectories(QList<QPair<QString, cSettings::sFavouriteDirectory> > qlFavouriteDirectories);
																								///< set favourite directories
																								/**< \param qlFavouriteDirectories list of favourite directories */
		const void SetFileOverwrite(const QString &qsMode);					///< set default overwrite behaviour
																								/**< \param qsMode overwrite mode */
		const void SetFileSizeIn(const QString &qsSize);						///< set file size mode
																								/**< \param qsSize file size to show */
		const void SetListerFont(const QFont &qfFont);							///< font used in lister
																								/**< \param qfFont user defined font for lister */
		const void SetListerSettings(const sLister &slLister);				///< set default lister settings
																								/**< \param slLister lister settings */
		const void SetMaximumHistoryDirectoryListSize(const int &iSize);	///< set maximum items in directory history list
																								/**< \param iSize maximum items in directory history list */
		const void SetNewTabByShortcutInForeground(const bool &bForeground);
																								///< new tab by shortcut in foreground
																								/**< \param bForeground new tab by shortcut in foreground flag */
		const void SetOpenNewTabNextToCurrentTab(const bool &bNextTo);		///< open new tab next to current tab
																								/**< \param bNextTo open new tab next to current tab flag */
		const void SetPluginDateTimeDisplay(const QString &qsDateTime);	///< plugin date/time display format
																								/**< \param qsDateTime user defined date/time display format */
		const void SetPlugins(const ePlugin &epPlugin, QList<sPlugin> qlPlugins);
																								///< write plugins into settings file
																								/**< \param epPlugin plugins type
																									  \param qlPlugins plugin list */
		const void SetPluginTimeDisplay(const QString &qsTime);				///< plugin time display format
																								/**< \param qsTime user defined time display format */
		const void SetQuickSearchAlt(const bool &bAlt);							///< alt key activity in quick search
																								/**< \param bAlt alt key activity flag */
		const void SetQuickSearchCtrl(const bool &bCtrl);						///< ctrl key activity in quick search
																								/**< \param bCtrl ctrl key activity flag */
		const void SetQuickSearchEnabled(const bool &bEnable);				///< enabled/disabled quick search
																								/**< \param bEnable enable/disable quick search flag */
		const void SetQuickSearchShift(const bool &bShift);					///< shift key activity in quick search
																								/**< \param bShift shift key activity flag */
		const void SetQuickSearchShowSearchWindow(const bool &bShow);		///< quick search window visibility
																								/**< \param bShow quick search window visibility flag */
		const void SetReadonlyFileOverwrite(const QString &qsMode);			///< set default readonly overwrite behaviour
																								/**< \param qsMode readonly overwrite mode */
		const void SetSaveSettingsOnQuit(const bool &bSave);					///< save settings on quit option
																								/**< \param bSave saves settings on quit when true */
		const void SetSelectionMode(const QString &qsMode);					///< set selection mode of files and directories
																								/**< \param qsMode selection mode */
		const void SetShortcut(const eShortcutCategory &escCategory, const QString &qsName, const QString &qsShortcut);
																								///< set shortcut
																								/**< \param escCategory shortcut category
																									  \param qsName item name to set shortcut for
																									  \param qsShortcut shortcut to set */
		const void SetShowBracketsAroundDirectoryName(const bool &bShowBrackets);
																								///< square brackets around directory name
																								/**< \param bShowBrackets show brackets around directory name flag */
		const void SetShowDirectoryViewHeader(const bool &bShow);			///< set directory view header show flag
																								/**< \param bShow directory view header show flag */
		const void SetShowDriveLetter(const bool &bShow);						///< show drive letter in tab
																								/**< \param bShow show drive letter in tab flag */
		const void SetShowHiddenFiles(const bool &bShowHidden);				///< set show hidden files in dir view
																								/**< \param bShowHidden show hidden files in dir view flag */
		const void SetShowSystemFiles(const bool &bShowSystem);				///< set show system files in dir view
																								/**< \param bShowHidden show system files in dir view flag */
		const void SetShowTabBarWithOnlyOneTab(const bool &bShow);			///< show tab bar with only one tab
																								/**< \param bShow show tab bar with only one tab flag */
		const void SetTreatArchivesLikeDirectories(const bool &bLikeDirectories);
																								///< treat archives like directories
																								/**< \param bLikeDirectories browse through archives like by directories if true */
		const void SetViewerType(const QString &qsType);						///< viewer type
																								/**< \param qsType internal or external viewer */

	private:
		QSettings qsSettings;															///< application's settings

		const QList<QPair<QString, cSettings::sFavouriteDirectory> > CollectFavouriteDirectories();
																								///< collect favourite directories from settings file
																								/**< \return list of favourite directories */
		const void CreateFavouriteDirectories(QList<QPair<QString, cSettings::sFavouriteDirectory> > &qlFavouriteDirectories);
																								///< create favourite directories
																								/**< \param qlFavouriteDirectories list of favourite directories */
}; // cSettings

#endif
