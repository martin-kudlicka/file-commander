/// setting file accessing

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QDateTime>

const QString qsANSI = "ANSI";
const QString qsASCII = "ASCII";
const QString qsASK = "Ask";
const QString qsBYTES = "Bytes";
const QString qsBYTES2 = "byte(s)";
const QString qsDATE = "Date";
const QString qsDYNAMIC = "Dynamic";
const QString qsEXTENSION = "Extension";
const QString qsFILES_AND_DIRECTORIES = "FilesAndDirectories";
const QString qsFULL = "Full";										///< full dir panel view (icon, filename, extension, date)
const QString qsFULL_SCREEN = "FullScreen";
const QString qsGIGABYTES = "Gigabytes";
const QString qsICON = "Icon";
const QString qsIDENTIFIER = "Identifier";
const QString qsKILOBYTES = "Kilobytes";
const QString qsMAXIMIZED = "Maximized";
const QString qsMEGABYTES = "Megabytes";
const QString qsMINUTES = "minute(s)";
const QString qsNAME = "Name";
const QString qsNO = "no";
const QString qsNO_TO_ALL = "NoToAll";
const QString qsNORMAL = "Normal";
const QString qsONLY_FILES = "OnlyFiles";
const QString qsOVERWRITE_ALL = "OverwriteAll";
const QString qsOVERWRITE_ALL_OLDER = "OverwriteAllOlder";
const QString qsSIZE = "Size";
const QString qsSKIP_ALL = "SkipAll";
const QString qsVARIABLE_CHAR_WIDTH = "VariableCharWidth";
const QString qsYES_TO_ALL = "YesToAll";
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
const QString qsSHORTCUT__MAIN_MENU__FILE__QUIT = "Quit";
const QString qsSHORTCUT__MAIN_MENU__MARK__SELECT_GROUP = "Select group...";
const QString qsSHORTCUT__MAIN_MENU__MARK__UNSELECT_GROUP = "Unselect group...";
const QString qsSHORTCUT__MAIN_MENU__MARK__SELECT_ALL = "Select all";
const QString qsSHORTCUT__MAIN_MENU__MARK__UNSELECT_ALL = "Unselect all";
const QString qsSHORTCUT__MAIN_MENU__MARK__INVERT_SELECTION = "Invert selection";
const QString qsSHORTCUT__MAIN_MENU__COMMANDS__SEARCH = "Search...";
const QString qsSHORTCUT__MAIN_MENU__SHOW__FULLSCREEN = "Fullscreen";
const QString qsSHORTCUT__MAIN_MENU__SHOW__REFRESH = "Refresh";
const QString qsSHORTCUT__MAIN_MENU__CONFIGURATION__OPTIONS = "Options";
const QString qsSHORTCUT__MAIN_MENU__CONFIGURATION__SAVE_POSITION = "Save position";
const QString qsSHORTCUT__MAIN_MENU__CONFIGURATION__SAVE_SETTINGS = "Save settings";
const QString qsSHORTCUT__OPERATIONS__DIALOG__ENQUEUE = "Enqueue";
const QString qsSHORTCUT__PANELS__DIRECTORY_VIEW__DRIVE_LEFT = "Drive left";
const QString qsSHORTCUT__PANELS__DIRECTORY_VIEW__DRIVE_RIGHT = "Drive right";
const QString qsSHORTCUT__PANELS__MAIN_BUTTON__VIEW = "View";
const QString qsSHORTCUT__PANELS__MAIN_BUTTON__EDIT = "Edit";
const QString qsSHORTCUT__PANELS__MAIN_BUTTON__COPY = "Copy";
const QString qsSHORTCUT__PANELS__MAIN_BUTTON__MOVE = "Move";
const QString qsSHORTCUT__PANELS__MAIN_BUTTON__NEW_DIRECTORY = "New directory";
const QString qsSHORTCUT__PANELS__MAIN_BUTTON__DELETE = "Delete";

class cSettings : private QObject
{
	public:
		/// plugin type
		enum ePlugin {
			ContentPlugins,												///< content plugin type
			ListerPlugins													///< lister plugin type
		};
		/// choose of panel
		enum ePosition {
			PositionLeft,													///< left panel
			PositionRight													///< right panel
		};
		/// shortcut category
		enum eShortcutCategory {
			MainMenuCategory,												///< main menu shortcut category
			PanelsCategory,												///< panels shortcut category
			OperationsCategory,											///< operations shortcut category
			ListerCategory													///< lister shortcut category
		};
		/// column in dir tree view
		struct sColumn {
			QString qsIdentifier;										///< column identifier
			QString qsName;												///< column name to show
			QString qsPlugin;												///< plugin filename or "no" if native
			QString qsUnit;												///< selected unit for column
			int iWidth;														///< column width
		};
		/// find settings
		struct sFindSettings {
			// general
			QString qsSearchFor;											///< search for this file(s)
			bool bSearchForRegularExpression;						///< search for is regular expression flag
			QString qsSearchIn;											///< search in this directory(s)
			int iSubdirectoryDepth;										///< number of subdirectories to go through
			bool bSearchForText;											///< search for text flag
			QString qsFullText;											///< text to search
			bool bFulTextWholeWords;									///< search whole words
			bool bFullTextCaseSensitive;								///< case sensitive search
			bool bFullTextNotContainingText;							///< search files not containing specified text
			bool bFullTextHex;											///< specified text is hex code flag
			bool bFullTextRegularExpression;							///< specified text is regular expression flag
			// advanced
			bool bDateTimeBetween;										///< search files between two dates/times flag
			QDateTime qdtFrom;											///< minimum date/time of file
			QDateTime qdtTo;												///< maximum date/time of file
			bool bNotOlderThan;											///< search file not older than flag
			int iNotOlderThanCount;										///< count of "something" to be not older than
			QString qsNotOlderThanType;								///< that's the "something" :)
			bool bFileSize;												///< file size to match in search flag
			QString qsFileSizeComparator;								///< file size comparator
			int iFileSizeValue;											///< file size value
			QString qsFileSizeType;										///< type of file size value
		};
		/// lister settings
		struct sLister {
			QString qsCharSet;											///< selected char set
			bool bWrapText;												///< word wrapping flag
			bool bFitImageToWindow;										///< image auto resize flag
		};
		/// plugin
		struct sPlugin {
			QString qsName;												///< plugin's file name
			bool bEnabled;													///< enable/disable flag
		};
		/// sort information
		struct sSort {
			int iSortedColumn;											///< column to sort by
			Qt::SortOrder soSortOrder;									///< sort order
		};
		/// tab info
		struct sTabInfo {
			QString qsColumnSet;											///< column set defined in tab
			QString qsDrive;												///< drive
			QString qsPath;												///< path selected in tab
			sSort ssSort;													///< sort information
		};
		/// main window state
		struct sMainWindowState {
			int iHeight;													///< height of main window
			int iWidth;														///< width of main window
			QString qsWindowState;										///< state of main window
		};

		void CreateColumnSet(const QString &qsColumnSet);		///< create new empty column set
																				/**< \param qsColumnSet column set to create */
		void CreateColumnSet(const QString &qsColumnSet, const QList<sColumn> &qlColumns);
																				///< create new column set
																				/**< \param qsColumnSet column set to create
																					  \param qlColumns columns in column set */
		void CreateDefaultColumnSet();								///< create default (Full) column set
		void CreateTab(const ePosition &epPosition, const uint &uiIndex, const sTabInfo &stiTab);
																				///< create new tab in settings file
																				/**< \param epPosition left or right panel
																					  \param uiIndex index of tab in tab bar
																					  \param stiTab tab information */
		QMap <QString, QString> GetAllSettings();					///< get all application's settings
																				/**< \return list of whole settings file */
		bool GetCloseTabOnDoubleClick();								///< close tab in tab bar on double click
																				/**< \return true if can close on double click */
		sColumn GetColumnInfo(const QString &qsColumnSet, const QString &qsColumn);
																				///< get some information about column
																				/**< \param qsColumnSet column's column set
																					  \param qsColumn column index
																					  \return column information */
		QStringList GetColumnSets();									///< get column sets
																				/**< \return list of column sets */
		QStringList GetColumnsInSet(const QString &qsColumnSet);
																				///< get column names in column set
																				/**< \param qsColumnSet name of column set
																					  \return list of columns in column set */
		bool GetConfirmCloseOfAllTabs();								///< confirm close of all tabs in tab bar
																				/**< return confirm close of all tabs flag */
		QString GetFileOverwrite();									///< find out file overwrite mode
																				/**< \return default file overwrite mode */
		QString GetFileSizeIn();										///< unit for files size
		sFindSettings GetFindSettings(const QString &qsName);	///< get find settings
																				/**< \param qsName name of find settings set
																					  \return find settings */
		sLister GetListerSettings();									///< lister settings
																				/**< \return lister settings */
																				/**< \return file size unit */
		QList<sPlugin> GetPlugins(const ePlugin &epPlugin);	///< get plugin list
																				/**< \param epPlugin plugin type
																					  \return plugin list */
		QString GetPluginTimeDisplay();								///< plugin time display format
																				/**< \return plugin time display format */
		QString GetReadonlyFileOverwrite();							///< find out readonly file overwrite mode
																				/**< \return default readonly file overwrite mode */
		QStringList GetSavedFinds();									///< list of saved find settings for find files dialog
																				/**< \return list of save find settings */
		bool GetSaveSettingsOnQuit();									///< save settings on quit option
																				/**< \return saves settings on quit when true */
		QString GetSelectionMode();									///< selection mode of files and directories
																				/**< \return selection mode */
		QString GetShortcut(const eShortcutCategory &escCategory, const QString &qsShortcut);
																				///< find shortcut
																				/**< \param escCategory shortcut category
																					  \param qsShortcut shortcut name
																					  \return shortcun key sequence */
		bool GetShowBracketsAroundDirectoryName();				///< square brackets around directory name
																				/**< \return show brackets around directory name flag */
		bool GetShowHiddenFiles();										///< show hidden files in dir view flag
																				/**< \return true if show hidden files */
		bool GetShowSystemFiles();										///< show system files in dir view flag
																				/**< \return true if show system files */
		bool GetShowTabBarWithOnlyOneTab();							///< show tab bar with only one tab flag
																				/**< \return true if show tab bar with only tab */
		sTabInfo GetTabInfo(const ePosition &epPosition, const QString &qsIndex);
																				///< get some information about tab
																				/**< \param epPosition left or right panel
																					  \param qsIndex index of tab
																					  \return information about panel's tab */
		QStringList GetTabs(const ePosition &epPosition);		///< get tab list for left or right panel
																				/**< \param epPosition left or right panel
																					  \return list of tabs in panel */
		sMainWindowState GetWindowState();							///< retrieve startup main window parameters
																				/**< \return startup main window parameters */
		void RemoveColumnSet(const QString &qsColumnSet);		///< remove column set
																				/**< \param qsColumnSet column set to remove */
		void RemoveFindSettings(const QString &qsFindSettings);
																				///< remove find settings
																				/**< \param qsFindSettings find settings name */
		void RestoreSettings(QMap <QString, QString> &qmSettings);
																				///< restore old application's settings
																				/**< \param qmSettings settings to restore */
		void SetCloseTabOnDoubleClick(const bool &bClose);		///< close tab on double click
																				/**< \param bClose can close tab on double click if true */
		void SetConfirmCloseOfAllTabs(const bool &bClose);		///< confirm close of all tabs
																				/**< \param bClose confirm close of all tabs flag */
		void SetFileOverwrite(const QString &qsMode);			///< set default overwrite behaviour
																				/**< \param qsMode overwrite mode */
		void SetFileSizeIn(const QString &qsSize);				///< set file size mode
																				/**< \param qsSize file size to show */
		void SetFindSettings(const QString &qsName, const sFindSettings &sfsFindSettings);
																				///< save find settings for find files dialog
																				/**< \param qsName name of find settings
																					  \param sfsFindSettings settings for find files dialog */
		void SetListerSettings(const sLister &slLister);		///< set default lister settings
																				/**< \param slLister lister settings */
		void SetPlugins(const ePlugin &epPlugin, const QList<sPlugin> &qlPlugins);
																				///< write plugins into settings file
																				/**< \param epPlugin plugins type
																					  \param qlPlugins plugin list */
		void SetPluginTimeDisplay(const QString &qsTime);		///< plugin time display format
																				/**< \param qsTime user defined time display format */
		void SetReadonlyFileOverwrite(const QString &qsMode);	///< set default readonly overwrite behaviour
																				/**< \param qsMode readonly overwrite mode */
		void SetSaveSettingsOnQuit(const bool &bSave);			///< save settings on quit option
																				/**< \param bSave saves settings on quit when true */
		void SetSelectionMode(const QString &qsMode);			///< set selection mode of files and directories
																				/**< \param qsMode selection mode */
		void SetShortcut(const eShortcutCategory &escCategory, const QString &qsName, const QString &qsShortcut);
																				///< set shortcut
																				/**< \param escCategory shortcut category
																					  \param qsName item name to set shortcut for
																					  \param qsShortcut shortcut to set */
		void SetShowBracketsAroundDirectoryName(const bool &bShowBrackets);
																				///< square brackets around directory name
																				/**< \param bShowBrackets show brackets around directory name flag */
		void SetShowHiddenFiles(const bool &bShowHidden);		///< set show hidden files in dir view
																				/**< \param bShowHidden show hidden files in dir view flag */
		void SetShowSystemFiles(const bool &bShowSystem);		///< set show system files in dir view
																				/**< \param bShowHidden show system files in dir view flag */
		void SetShowTabBarWithOnlyOneTab(const bool &bShow);	///< show tab bar with only one tab
																				/**< \param bShow show tab bar with only one tab flag */
		void SetTabs(const ePosition &epPosition, const QList<sTabInfo> &qlTabs);
																				///< save tab settings
																				/**< \param epPosition panel position
																					  \param qlTabs tab settings */
		void SetWindowState(const sMainWindowState &smwsState);
																				///< set startup main window state
																				/**< \param smwsState window state information */

	private:
		QSettings qsSettings;											///< application's settings
}; // cSettings

#endif
