/// setting file accessing

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtCore/QSettings>

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
		/// plugin type
		enum ePlugin {
			ContentPlugins,										///< content plugin type
			ListerPlugins,											///< lister plugin type
			PackerPlugins											///< packer plugin type
		};
		/// shortcut category
		enum eShortcutCategory {
			MainMenuCategory,										///< main menu shortcut category
			PanelsCategory,										///< panels shortcut category
			OperationsCategory,									///< operations shortcut category
			ListerCategory											///< lister shortcut category
		};

		/// main window state
		struct sMainWindowState {
			int iHeight;											///< height of main window
			int iWidth;												///< width of main window
			QString qsWindowState;								///< state of main window
		};
		/// plugin
		struct sPlugin {
			QString qsName;										///< plugin's file name
			QString qsExtensions;								///< packer plugin's extensions
			bool bEnabled;											///< enable/disable flag
		};

		const QString GetPluginDateTimeDisplay() const;	///< plugin date/time display format
																		/**< \return plugin date/time display format */
		const QList<sPlugin> GetPlugins(const ePlugin &epPlugin);
																		///< get plugin list
																		/**< \param epPlugin plugin type
																					  \return plugin list */
		const QString GetPluginTimeDisplay() const;		///< plugin time display format
																		/**< \return plugin time display format */
		const QString GetShortcut(const eShortcutCategory &escCategory, const QString &qsShortcut) const;
																		///< find shortcut
																		/**< \param escCategory shortcut category
																			  \param qsShortcut shortcut name
																			  \return shortcun key sequence */
		const sMainWindowState GetWindowState();			///< retrieve startup main window parameters
																		/**< \return startup main window parameters */

	private:
		QSettings qsSettings;									///< application's settings
}; // cSettings

#endif
