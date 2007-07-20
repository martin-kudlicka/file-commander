#include "Settings.h"

const QString qsENABLED = "Enabled";
const QString qsEXTENSIONS = "Extensions";
const QString qsHEIGHT = "Height";
const QString qsLISTER = "Lister";
const QString qsMAIN_MENU = "MainMenu";
const QString qsNAME = "Name";
const QString qsNORMAL = "Normal";
const QString qsOPERATIONS = "Operations";
const QString qsPANELS = "Panels";
const QString qsTRUE = "true";
const QString qsWIDTH = "Width";
const QString qsWINDOW_STATE = "WindowState";

// settings file
// MainWindow/
const QString qsMAIN_WINDOW = "MainWindow";
// Others/Shortcuts
const QString qsOTHERS__SHORTCUTS__ = "Others/Shortcuts/";
// Plugins
const QString qsPLUGINS__CONTENT = "Plugins/Content";
const QString qsPLUGINS__DATE_TIME_DISPLAY = "Plugins/DateTimeDisplay";
const QString qsPLUGINS__LISTER = "Plugins/Lister";
const QString qsPLUGINS__PACKER = "Plugins/Packer";
const QString qsPLUGINS__TIME_DISPLAY = "Plugins/TimeDisplay";

// plugin date/time display format
const QString cSettings::GetPluginDateTimeDisplay() const
{
	return qsSettings.value(qsPLUGINS__DATE_TIME_DISPLAY, "%d.%o.%y %h:%m").toString();
} // GetPluginDateTimeDisplay

// get plugin list
const QList<cSettings::sPlugin> cSettings::GetPlugins(const ePlugin &epPlugin)
{
	int iCount, iI;
	QList<sPlugin> qlPlugins;

	switch (epPlugin) {
		// TODO GetPlugins - other plugin types
		case ContentPlugins:
			iCount = qsSettings.beginReadArray(qsPLUGINS__CONTENT);
			break;
		case ListerPlugins:
			iCount = qsSettings.beginReadArray(qsPLUGINS__LISTER);
			break;
		case PackerPlugins:
			iCount = qsSettings.beginReadArray(qsPLUGINS__PACKER);
			break;
		default:
			iCount = 0;
	} // switch

	for (iI = 0; iI < iCount; iI++) {
		sPlugin spPlugin;

		qsSettings.setArrayIndex(iI);

		spPlugin.qsName = qsSettings.value(qsNAME).toString();
		if (epPlugin == PackerPlugins) {
			spPlugin.qsExtensions = qsSettings.value(qsEXTENSIONS).toString();
		} // if
		if (qsSettings.value(qsENABLED).toString() == qsTRUE) {
			spPlugin.bEnabled = true;
		} else {
			spPlugin.bEnabled = false;
		} // if

		qlPlugins.append(spPlugin);
	} // for

	qsSettings.endArray();

	return qlPlugins;
} // GetPlugins

// plugin time display format
const QString cSettings::GetPluginTimeDisplay() const
{
	return qsSettings.value(qsPLUGINS__TIME_DISPLAY, "%h:%m:%s").toString();
} // GetPluginTimeDisplay

// find shortcut
const QString cSettings::GetShortcut(const eShortcutCategory &escCategory, const QString &qsShortcut) const
{
	QString qsCategory;

	switch (escCategory) {
		case MainMenuCategory:
			qsCategory = qsMAIN_MENU;
			break;
		case PanelsCategory:
			qsCategory = qsPANELS;
			break;
		case OperationsCategory:
			qsCategory = qsOPERATIONS;
			break;
		case ListerCategory:
			qsCategory = qsLISTER;
	} // switch

	if (qsSettings.contains(qsOTHERS__SHORTCUTS__ + qsCategory + '/' + qsShortcut)) {
		// if shortcut is defined return it
		return qsSettings.value(qsOTHERS__SHORTCUTS__ + qsCategory + '/' + qsShortcut).toString();
	} else {
		// else return default

		switch (escCategory) {
			case MainMenuCategory:
				if (qsShortcut == qsSHORTCUT__MAIN_MENU__FILE__UNPACK_SELECTED_FILES) {
					return "Alt+F9";
				} // if
				if (qsShortcut == qsSHORTCUT__MAIN_MENU__FILE__QUIT) {
					return "Alt+F4";
				} // if
				if (qsShortcut == qsSHORTCUT__MAIN_MENU__MARK__SELECT_GROUP) {
					return "+";
				} // if
				if (qsShortcut == qsSHORTCUT__MAIN_MENU__MARK__UNSELECT_GROUP) {
					return "-";
				} // if
				if (qsShortcut == qsSHORTCUT__MAIN_MENU__MARK__SELECT_ALL) {
					return "Ctrl++";
				} // if
				if (qsShortcut == qsSHORTCUT__MAIN_MENU__MARK__UNSELECT_ALL) {
					return "Ctrl+-";
				} // if
				if (qsShortcut == qsSHORTCUT__MAIN_MENU__MARK__INVERT_SELECTION) {
					return "*";
				} // if
				if (qsShortcut == qsSHORTCUT__MAIN_MENU__MARK__COMPARE_DIRECTORIES) {
					return "Shift+F2";
				} // if
				if (qsShortcut == qsSHORTCUT__MAIN_MENU__COMMANDS__SEARCH) {
					return "Alt+F7";
				} // if
				if (qsShortcut == qsSHORTCUT__MAIN_MENU__COMMANDS__FAVOURITE_DIRECTORIES) {
					return "Ctrl+D";
				} // if
				if (qsShortcut == qsSHORTCUT__MAIN_MENU__COMMANDS__BRANCH_VIEW) {
					return "Ctrl+B";
				} // if
				if (qsShortcut == qsSHORTCUT__MAIN_MENU__COMMANDS__FULLSCREEN) {
					return "F11";
				} // if
				if (qsShortcut == qsSHORTCUT__MAIN_MENU__PANEL__SORT_BY_FIRST_COLUMN) {
					return "Ctrl+F3";
				} // if
				if (qsShortcut == qsSHORTCUT__MAIN_MENU__PANEL__SORT_BY_SECOND_COLUMN) {
					return "Ctrl+F4";
				} // if
				if (qsShortcut == qsSHORTCUT__MAIN_MENU__PANEL__SORT_BY_THIRD_COLUMN) {
					return "Ctrl+F5";
				} // if
				if (qsShortcut == qsSHORTCUT__MAIN_MENU__PANEL__SORT_BY_FOURTH_COLUMN) {
					return "Ctrl+F6";
				} // if
				if (qsShortcut == qsSHORTCUT__MAIN_MENU__PANEL__REFRESH) {
					return "Ctrl+R";
				} // if
				break;
			case PanelsCategory:	
				if (qsShortcut == qsSHORTCUT__PANELS__TABS__DUPLICATE_TAB) {
					return "Ctrl+T";
				} // if
				if (qsShortcut == qsSHORTCUT__PANELS__TABS__CLOSE_TAB) {
					return "Ctrl+W";
				} // if
				if (qsShortcut == qsSHORTCUT__PANELS__MAIN_BUTTON__TERMINAL) {
					return "F2";
				} // if
				if (qsShortcut == qsSHORTCUT__PANELS__MAIN_BUTTON__VIEW) {
					return "F3";
				} // if
				if (qsShortcut == qsSHORTCUT__PANELS__MAIN_BUTTON__EDIT) {
					return "F4";
				} // if
				if (qsShortcut == qsSHORTCUT__PANELS__MAIN_BUTTON__COPY) {
					return "F5";
				} // if
				if (qsShortcut == qsSHORTCUT__PANELS__MAIN_BUTTON__MOVE) {
					return "F6";
				} // if
				if (qsShortcut == qsSHORTCUT__PANELS__MAIN_BUTTON__NEW_DIRECTORY) {
					return "F7";
				} // if
				if (qsShortcut == qsSHORTCUT__PANELS__MAIN_BUTTON__DELETE) {
					return "F8";
				} // if
				if (qsShortcut == qsSHORTCUT__PANELS__DIRECTORY_VIEW__DRIVE_LEFT) {
					return "Alt+F1";
				} // if
				if (qsShortcut == qsSHORTCUT__PANELS__DIRECTORY_VIEW__DRIVE_RIGHT) {
					return "Alt+F2";
				} // if
				break;
			case OperationsCategory:
				if (qsShortcut == qsSHORTCUT__OPERATIONS__DIALOG__ENQUEUE) {
					return "F2";
				} // if
				break;
			case ListerCategory:
				if (qsShortcut == qsSHORTCUT__LISTER__FILE__PRINT) {
					return "Ctrl+P";
				} // if
				if (qsShortcut == qsSHORTCUT__LISTER__FILE__CLOSE) {
					return "Esc";
				} // if
				if (qsShortcut == qsSHORTCUT__LISTER__EDIT__COPY_AS_TEXT) {
					return "Ctrl+C";
				} // if
				if (qsShortcut == qsSHORTCUT__LISTER__EDIT__SELECT_ALL) {
					return "Ctrl+A";
				} // if
				if (qsShortcut == qsSHORTCUT__LISTER__EDIT__FIND) {
					return "F7";
				} // if
				if (qsShortcut == qsSHORTCUT__LISTER__EDIT__FIND_NEXT) {
					return "F3";
				} // if
				if (qsShortcut == qsSHORTCUT__LISTER__OPTIONS__TEXT) {
					return "1";
				} // if
				if (qsShortcut == qsSHORTCUT__LISTER__OPTIONS__BINARY) {
					return "2";
				} // if
				if (qsShortcut == qsSHORTCUT__LISTER__OPTIONS__HEX) {
					return "3";
				} // if
				if (qsShortcut == qsSHORTCUT__LISTER__OPTIONS__MULTIMEDIA) {
					return "4";
				} // if
				if (qsShortcut == qsSHORTCUT__LISTER__OPTIONS__ANSI) {
					return "A";
				} // if
				if (qsShortcut == qsSHORTCUT__LISTER__OPTIONS__ASCII) {
					return "S";
				} // if
				if (qsShortcut == qsSHORTCUT__LISTER__OPTIONS__VARIABLE_CHAR_WIDTH) {
					return "V";
				} // if
				if (qsShortcut == qsSHORTCUT__LISTER__OPTIONS__WRAP_TEXT) {
					return "W";
				} // if
				if (qsShortcut == qsSHORTCUT__LISTER__OPTIONS__FIT_IMAGE_TO_WINDOW) {
					return "F";
				} // if
		} // switch

		return QString();
	} // if else
} // GetShortcut

// retrieve startup main window parameters
const cSettings::sMainWindowState cSettings::GetWindowState()
{
	sMainWindowState smwsState;

	qsSettings.beginGroup(qsMAIN_WINDOW);
	smwsState.iHeight = qsSettings.value(qsHEIGHT, 0).toInt();
	smwsState.iWidth = qsSettings.value(qsWIDTH, 0).toInt();
	smwsState.qsWindowState = qsSettings.value(qsWINDOW_STATE, qsNORMAL).toString();
	qsSettings.endGroup();

	return smwsState;
} // GetWindowState