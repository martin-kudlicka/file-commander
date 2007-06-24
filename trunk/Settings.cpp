#include "Settings.h"

#include <QtCore/QStringList>
#include "FileOperation/FileRoutine.h"
#include <QtGui/QFont>

// general
const QString qsASCENDING = "ascending";
const QString qsASK_TO_DELETE_NON_EMPTY_DIRECTORY = "AskToDeleteNonEmptyDirectory";
const QString qsBUFFER_SIZE = "BufferSize";
const QString qsCHAR_SET = "CharSet";
const QString qsCOLUMN_SET = "ColumnSet";
const QString qsDATE_TIME_BETWEEN = "DateTimeBetween";
const QString qsDATE_TIME_FROM = "DateTimeFrom";
const QString qsDATE_TIME_TO = "DateTimeTo";
#ifdef Q_WS_WIN
const QString qsDELETE_TO_RECYCLE_BIN = "DeleteToRecycleBin";
#endif
const QString qsDESCENDING = "descending";
const QString qsDRIVE = "Drive";
const QString qsENABLED = "Enabled";
const QString qsEXTENSIONS = "Extensions";
const QString qsEXTERNAL_EDITOR = "ExternalEditor";
const QString qsEXTERNAL_VIEWER = "ExternalViewer";
const QString qsFALSE = "false";
const QString qsFILE_OVERWRITE = "FileOverwrite";
const QString qsFILE_SIZE = "FileSize";
const QString qsFILE_SIZE_COMPARATOR = "FileSizeComparator";
const QString qsFILE_SIZE_TYPE = "FileSizeType";
const QString qsFILE_SIZE_VALUE = "FileSizeValue";
const QString qsFIT_IMAGE_TO_WINDOW = "FitImageToWindow";
const QString qsFONT = "Font";
const QString qsFULL_TEXT = "FullText";
const QString qsHEIGHT = "Height";
const QString qsLISTER = "Lister";
const QString qsMAIN_MENU = "MainMenu";
const QString qsNOT_OLDER_THAN = "NotOlderThan";
const QString qsNOT_OLDER_THAN_COUNT = "NotOlderThanCount";
const QString qsNOT_OLDER_THAN_TYPE = "NotOlderThanType";
const QString qsOPERATIONS = "Operations";
const QString qsPANELS = "Panels";
const QString qsPATH = "Path";
const QString qsPLUGIN = "Plugin";
const QString qsREADONLY_FILE_OVERWRITE = "ReadonlyFileOverwrite";
const QString qsSEARCH_FOR = "SearchFor";
const QString qsSEARCH_FOR_REGULAR_EXPRESSION = "SearchForRegularExpression";
const QString qsSEARCH_FOR_TEXT = "SearchForText";
const QString qsFULL_TEXT_CASE_SENSITIVE = "FullTextCaseSensitive";
const QString qsFULL_TEXT_HEX = "FullTextHex";
const QString qsFULL_TEXT_NOT_CONTAINING_TEXT = "FullTextNotContainingText";
const QString qsFULL_TEXT_REGULAR_EXPRESSION = "FullTextRegularExpression";
const QString qsFULL_TEXT_WHOLE_WORDS = "FullTextWholeWords";
const QString qsSEARCH_IN = "SearchIn";
const QString qsSORT_ORDER = "SortOrder";
const QString qsSORTED_COLUMN = "SortColumn";
const QString qsSOURCE = "Source";
const QString qsSUBDIRECTORY_DEPTH = "SubdirectoryDepth";
const QString qsSUBMENU = "Submenu";
const QString qsTARGET = "Target";
const QString qsTARGET_ENABLED = "TargetEnabled";
const QString qsTREAT_ARCHIVES_LIKE_DIRECTORIES = "TreatArchivesLikeDirectories";
const QString qsTRUE = "true";
const QString qsUNIT = "Unit";
const QString qsVIEWER_TYPE = "ViewerType";
const QString qsWIDTH = "Width";
const QString qsWINDOW_STATE = "WindowState";
const QString qsWRAP_TEXT = "WrapText";
// settings file
// ColumnSet/
const QString qsCOLUMN_SET__ = qsCOLUMN_SET + "/";
// Display
const QString qsDISPLAY__CASE_SENSITIVE_SORTING = "Display/CaseSensitiveSorting";
const QString qsDISPLAY__DATE_TIME_DISPLAY = "Display/DateTimeDisplay";
const QString qsDISPLAY__FILE_SIZE_IN = "Display/FileSizeIn";
const QString qsDISPLAY__SAVE_SETTINGS_ON_QUIT = "Display/SaveSettingsOnQuit";
const QString qsDISPLAY__SELECTION_MODE = "Display/SelectionMode";
const QString qsDISPLAY__SHOW_BRACKETS_AROUND_DIRECTORY_NAME = "Display/ShowBracketsAroundDirectoryName";
const QString qsDISPLAY__SHOW_HIDDEN_FILES = "Display/ShowHiddenFiles";
const QString qsDISPLAY__SHOW_SYSTEM_FILES = "Display/ShowSystemFiles";
// Tabs
const QString qsTABS__CLOSE_TAB_ON_DOUBLE_CLICK = "Tabs/CloseTabOnDoubleClick";
const QString qsTABS__CONFIRM_CLOSE_OF_ALL_TABS = "Tabs/ConfirmCloseOfAllTabs";
const QString qsTABS__NEW_TAB_BY_SHORTCUT_IN_FOREGROUND = "Tabs/NewTabByShortcutInForeground";
const QString qsTABS__OPEN_NEW_TAB_NEXT_TO_CURRENT_TAB = "Tabs/OpenNewTabNextToCurrentTab";
const QString qsTABS__SHOW_DRIVE_LETTER = "Tabs/ShowDriveLetter";
const QString qsTABS__SHOW_TAB_BAR_WITH_ONLY_ONE_TAB = "Tabs/ShowTabBarWithOnlyOneTab";
// Quick search
const QString qsQUICK_SEARCH__ALT = "QuickSearch/Alt";
const QString qsQUICK_SEARCH__CTRL = "QuickSearch/Ctrl";
const QString qsQUICK_SEARCH__ENABLED = "QuickSearch/Enabled";
const QString qsQUICK_SEARCH__SHIFT = "QuickSearch/Shift";
const QString qsQUICK_SEARCH__SHOW_SEARCH_WINDOW = "QuickSearch/ShowSearchWindow";

// FindFiles
const QString qsFIND_FILES__ = "FindFiles/";
// LeftPanel
// LeftPanel/Tabs/
const QString qsLEFT_PANEL__TABS__ = "LeftPanel/Tabs/";
// MainWindow/
const QString qsMAIN_WINDOW = "MainWindow";
// Operations/
const QString qsOPERATIONS__ = "Operations/";
// Others/FavouriteDirectories
const QString qsOTHERS__FAVOURITE_DIRECTORIES = "Others/FavouriteDirectories";
// Others/Shortcuts
const QString qsOTHERS__SHORTCUTS__ = "Others/Shortcuts/";
// Plugins
const QString qsPLUGINS__LISTER__SETTINGS__ = "Plugins/Lister/Settings/";
const QString qsPLUGINS__PACKER__SETTINGS__ = "Plugins/Packer/Settings/";
const QString qsPLUGINS__DATE_TIME_DISPLAY = "Plugins/DateTimeDisplay";
const QString qsPLUGINS__TIME_DISPLAY = "Plugins/TimeDisplay";
// Plugins/Content
const QString qsPLUGINS__CONTENT = "Plugins/Content";
const QString qsPLUGINS__LISTER = "Plugins/Lister";
const QString qsPLUGINS__PACKER = "Plugins/Packer";
// RightPanel
// RightPanel/Tabs/
const QString qsRIGHT_PANEL__TABS__ = "RightPanel/Tabs/";

// collect favourite directories from settings file
QList<QPair<QString, cSettings::sFavouriteDirectory> > cSettings::CollectFavouriteDirectories()
{
	int iI;
	QList<QPair<QString, cSettings::sFavouriteDirectory> > qlFavouriteDirectories;
	QStringList qslFavouriteDirectories;

	qslFavouriteDirectories = qsSettings.childGroups();
	for (iI = 0; iI < qslFavouriteDirectories.count(); iI++) {
		sFavouriteDirectory sfdFavouriteDirectory;

		qsSettings.beginGroup(QVariant(iI).toString());

		sfdFavouriteDirectory.bSubmenu = qsSettings.value(qsSUBMENU).toBool();
		if (sfdFavouriteDirectory.bSubmenu) {
			sfdFavouriteDirectory.qlChildFavourites = CollectFavouriteDirectories();
		} else {
			sfdFavouriteDirectory.qsSource = qsSettings.value(qsSOURCE).toString();
			sfdFavouriteDirectory.bTarget = qsSettings.value(qsTARGET_ENABLED).toBool();
			sfdFavouriteDirectory.qsTarget = qsSettings.value(qsTARGET).toString();
		} // if else

		qlFavouriteDirectories.append(QPair<QString, cSettings::sFavouriteDirectory>(qsSettings.value(qsNAME).toString(), sfdFavouriteDirectory));

		qsSettings.endGroup();
	} // for

	return qlFavouriteDirectories;
} // CollectFavouriteDirectories

// create new empty column set
void cSettings::CreateColumnSet(const QString &qsColumnSet)
{
	qsSettings.beginGroup(qsCOLUMN_SET__ + qsColumnSet);
	// write something to create group
	qsSettings.setValue(".","");
	qsSettings.remove(".");
	qsSettings.endGroup();
} // CreateColumnSet

// create new column set
void cSettings::CreateColumnSet(const QString &qsColumnSet, const QList<sColumn> &qlColumns)
{
	int iI;

	qsSettings.beginWriteArray(qsCOLUMN_SET__ + qsColumnSet);
	qsSettings.remove("");

	for (iI = 0; iI < qlColumns.count(); iI++) {
		qsSettings.setArrayIndex(iI);

		qsSettings.setValue(qsIDENTIFIER, qlColumns.at(iI).qsIdentifier);
		qsSettings.setValue(qsNAME, qlColumns.at(iI).qsName);
		qsSettings.setValue(qsPLUGIN, qlColumns.at(iI).qsPlugin);
		qsSettings.setValue(qsUNIT, qlColumns.at(iI).qsUnit);
		qsSettings.setValue(qsWIDTH, qlColumns.at(iI).iWidth);
	} // for

	qsSettings.endArray();
} // CreateColumnSet

// create default (Full) column set
void cSettings::CreateDefaultColumnSet()
{
	QList<sColumn> qlColumns;
	sColumn scColumn;

	// icon
	scColumn.qsIdentifier = qsICON;
	scColumn.qsName = tr("Icon");
	scColumn.qsPlugin = qsNO;
	scColumn.iWidth = 20;
	qlColumns.append(scColumn);
	// name
	scColumn.qsIdentifier = qsNAME;
	scColumn.qsName = tr("Name");
	scColumn.qsPlugin = qsNO;
	scColumn.iWidth = 0;
	qlColumns.append(scColumn);
	// extension
	scColumn.qsIdentifier = qsEXTENSION;
	scColumn.qsName = tr("Extension");
	scColumn.qsPlugin = qsNO;
	scColumn.iWidth = 30;
	qlColumns.append(scColumn);
	// size
	scColumn.qsIdentifier = qsSIZE;
	scColumn.qsName = tr("Size");
	scColumn.qsPlugin = qsNO;
	scColumn.iWidth = 50;
	qlColumns.append(scColumn);
	// date
	scColumn.qsIdentifier = qsDATE_TIME;
	scColumn.qsName = tr("Date");
	scColumn.qsPlugin = qsNO;
	scColumn.iWidth = 99;
	qlColumns.append(scColumn);
#ifdef Q_WS_WIN
	// attributes
	scColumn.qsIdentifier = qsATTRIBUTES;
	scColumn.qsName = tr("Attributes");
	scColumn.qsPlugin = qsNO;
	scColumn.iWidth = 40;
	qlColumns.append(scColumn);
#endif

	CreateColumnSet(qsFULL, qlColumns);
} // CreateDefaultColumnSet

// create favourite directories
void cSettings::CreateFavouriteDirectories(const QList<QPair<QString, cSettings::sFavouriteDirectory> > &qlFavouriteDirectories)
{
	int iI;

	for (iI = 0; iI < qlFavouriteDirectories.count(); iI++) {
		qsSettings.beginGroup(QVariant(iI).toString());

		qsSettings.setValue(qsNAME, qlFavouriteDirectories.at(iI).first);
		qsSettings.setValue(qsSUBMENU, qlFavouriteDirectories.at(iI).second.bSubmenu);

		if (qlFavouriteDirectories.at(iI).second.bSubmenu) {
			CreateFavouriteDirectories(qlFavouriteDirectories.at(iI).second.qlChildFavourites);
		} else {
			qsSettings.setValue(qsSOURCE, qlFavouriteDirectories.at(iI).second.qsSource);
			qsSettings.setValue(qsTARGET_ENABLED, qlFavouriteDirectories.at(iI).second.bTarget);
			qsSettings.setValue(qsTARGET, qlFavouriteDirectories.at(iI).second.qsTarget);
		} // if else

		qsSettings.endGroup();
	} // for
} // CreateFavouriteDirectories

// create new tab in settings file
void cSettings::CreateTab(const ePosition &epPosition, const uint &uiIndex, const sTabInfo &stiTab)
{
	QMap<QString, cFileRoutine::sDriveInfo> qmDrives;

	if (epPosition == PositionLeft) {
		qsSettings.beginGroup(QString("%1%2").arg(qsLEFT_PANEL__TABS__).arg(uiIndex));
	} else {
		qsSettings.beginGroup(QString("%1%2").arg(qsRIGHT_PANEL__TABS__).arg(uiIndex));
	} // if else

	qmDrives = cFileRoutine::GetDrives();

	qsSettings.setValue(qsCOLUMN_SET, stiTab.qsColumnSet);
	qsSettings.setValue(qsDRIVE, stiTab.qsDrive);
	qsSettings.setValue(qsPATH, stiTab.qsPath);
	qsSettings.setValue(qsSORTED_COLUMN, stiTab.ssSort.iSortedColumn);
	qsSettings.setValue(qsSORT_ORDER, stiTab.ssSort.soSortOrder == Qt::AscendingOrder ? qsASCENDING : qsDESCENDING);

	qsSettings.endGroup();
} // CreateTab

// get all application's settings
QMap <QString, QString> cSettings::GetAllSettings()
{
	int iI;
	QMap <QString, QString> qlSettings;
	QStringList qslKeys;

	qslKeys = qsSettings.allKeys();
	for (iI = 0; iI < qslKeys.count(); iI++) {
		qlSettings.insert(qslKeys.at(iI), qsSettings.value(qslKeys.at(iI)).toString());
	} // for

	return qlSettings;
} // GetAllSettings

// delete non empty directory
bool cSettings::GetAskToDeleteNonEmptyDirectory()
{
	return qsSettings.value(qsOPERATIONS__ + qsASK_TO_DELETE_NON_EMPTY_DIRECTORY, true).toBool();
} // GetAskToDeleteNonEmptyDirectory

// case sensitive sorting
bool cSettings::GetCaseSensitiveSorting()
{
	return qsSettings.value(qsDISPLAY__CASE_SENSITIVE_SORTING).toBool();
} // GetCaseSensitiveSorting

// close tab in tab bar on double click
bool cSettings::GetCloseTabOnDoubleClick()
{
	return qsSettings.value(qsTABS__CLOSE_TAB_ON_DOUBLE_CLICK, true).toBool();
} // GetCloseTabOnDoubleClick

// confirm close of all tabs in tab bar
bool cSettings::GetConfirmCloseOfAllTabs()
{
	return qsSettings.value(qsTABS__CONFIRM_CLOSE_OF_ALL_TABS).toBool();
} // GetConfirmCloseOfAllTabs

// get some information about column
cSettings::sColumn cSettings::GetColumnInfo(const QString &qsColumnSet, const QString &qsColumn)
{
	sColumn scColumn;

	qsSettings.beginGroup(qsCOLUMN_SET__ + qsColumnSet + "/" + qsColumn);
	scColumn.qsIdentifier = qsSettings.value(qsIDENTIFIER).toString();
	scColumn.qsName = qsSettings.value(qsNAME).toString();
	scColumn.qsPlugin = qsSettings.value(qsPLUGIN).toString();
	scColumn.qsUnit = qsSettings.value(qsUNIT).toString();
	scColumn.iWidth = qsSettings.value(qsWIDTH).toInt();
	qsSettings.endGroup();

	return scColumn;
} // GetColumnInfo

// get column sets
QStringList cSettings::GetColumnSets()
{
	QStringList qsColumnSets;

	qsSettings.beginGroup(qsCOLUMN_SET__);
	qsColumnSets = qsSettings.childGroups();
	qsSettings.endGroup();

	return qsColumnSets;
} // GetColumnSets

// get column names in column set
QStringList cSettings::GetColumnsInSet(const QString &qsColumnSet)
{
	QStringList qslColumns;

	qsSettings.beginGroup(qsCOLUMN_SET__ + qsColumnSet);
	qslColumns = qsSettings.childGroups();
	qsSettings.endGroup();

	return qslColumns;
} // GetColumnsInSet

// copy/move buffer size
int cSettings::GetCopyMoveBufferSize()
{
	return qsSettings.value(qsOPERATIONS__ + qsBUFFER_SIZE, 32).toInt();
} // GetCopyMoveBufferSize

// date/time display format
QString cSettings::GetDateTimeDisplay()
{
	return qsSettings.value(qsDISPLAY__DATE_TIME_DISPLAY, "%d.%o.%y %h:%m").toString();
} // GetDateTimeDisplay

#ifdef Q_WS_WIN
// delete files to recycle bin
bool cSettings::GetDeleteToRecycleBin()
{
	return qsSettings.value(qsOPERATIONS__ + qsDELETE_TO_RECYCLE_BIN, true).toBool();
} // GetDeleteToRecycleBin
#endif

// external editor
QString cSettings::GetExternalEditor()
{
	// TODO GetExternalEditor other OS than Windows
	return qsSettings.value(qsOPERATIONS__ + qsEXTERNAL_EDITOR, "notepad %1").toString();
} // GetExternalEditor

// external viewer
QString cSettings::GetExternalViewer()
{
	return qsSettings.value(qsOPERATIONS__ + qsEXTERNAL_VIEWER).toString();
} // GetExternalViewer

// get favourite directories
QList<QPair<QString, cSettings::sFavouriteDirectory> > cSettings::GetFavouriteDirectories()
{
	QList<QPair<QString, cSettings::sFavouriteDirectory> > qlFavouriteDirectories;

	qsSettings.beginGroup(qsOTHERS__FAVOURITE_DIRECTORIES);

	// collect them
	qlFavouriteDirectories = CollectFavouriteDirectories();

	qsSettings.endGroup();

	return qlFavouriteDirectories;
} // GetFavouriteDirectories

// find out file overwrite mode
QString cSettings::GetFileOverwrite()
{
	return qsSettings.value(qsOPERATIONS__ + qsFILE_OVERWRITE, qsASK).toString();
} // GetFileOverwrite

// unit for files size
QString cSettings::GetFileSizeIn()
{
	return qsSettings.value(qsDISPLAY__FILE_SIZE_IN, qsDYNAMIC).toString();
} // GetFileSizeIn

// get find settings
cSettings::sFindSettings cSettings::GetFindSettings(const QString &qsName)
{
	sFindSettings sfsFindSettings;

	qsSettings.beginGroup(qsFIND_FILES__ + qsName);
	// general
	sfsFindSettings.qsSearchFor = qsSettings.value(qsSEARCH_FOR).toString();
	sfsFindSettings.bSearchForRegularExpression = qsSettings.value(qsSEARCH_FOR_REGULAR_EXPRESSION).toBool();
	sfsFindSettings.qsSearchIn = qsSettings.value(qsSEARCH_IN).toString();
	sfsFindSettings.iSubdirectoryDepth = qsSettings.value(qsSUBDIRECTORY_DEPTH).toInt();
	sfsFindSettings.bSearchForText = qsSettings.value(qsSEARCH_FOR_TEXT).toBool();
	if (sfsFindSettings.bSearchForText) {
		sfsFindSettings.qsFullText = qsSettings.value(qsFULL_TEXT).toString();
		sfsFindSettings.bFulTextWholeWords = qsSettings.value(qsFULL_TEXT_WHOLE_WORDS).toBool();
		sfsFindSettings.bFullTextCaseSensitive = qsSettings.value(qsFULL_TEXT_CASE_SENSITIVE).toBool();
		sfsFindSettings.bFullTextNotContainingText = qsSettings.value(qsFULL_TEXT_NOT_CONTAINING_TEXT).toBool();
		sfsFindSettings.bFullTextHex = qsSettings.value(qsFULL_TEXT_HEX).toBool();
		sfsFindSettings.bFullTextRegularExpression = qsSettings.value(qsFULL_TEXT_REGULAR_EXPRESSION).toBool();
	} // if
	// advanced
	sfsFindSettings.bDateTimeBetween = qsSettings.value(qsDATE_TIME_BETWEEN).toBool();
	if (sfsFindSettings.bDateTimeBetween) {
		sfsFindSettings.qdtFrom = qsSettings.value(qsDATE_TIME_FROM).toDateTime();
		sfsFindSettings.qdtTo = qsSettings.value(qsDATE_TIME_TO).toDateTime();
	} // if
	sfsFindSettings.bNotOlderThan = qsSettings.value(qsNOT_OLDER_THAN).toBool();
	if (sfsFindSettings.bNotOlderThan) {
		sfsFindSettings.iNotOlderThanCount = qsSettings.value(qsNOT_OLDER_THAN_COUNT).toInt();
	} // if
	sfsFindSettings.qsNotOlderThanType = qsSettings.value(qsNOT_OLDER_THAN_TYPE, qsMINUTES).toString();
	sfsFindSettings.bFileSize = qsSettings.value(qsFILE_SIZE).toBool();
	if (sfsFindSettings.bFileSize) {
		sfsFindSettings.iFileSizeValue = qsSettings.value(qsFILE_SIZE_VALUE).toInt();
	} // if
	sfsFindSettings.qsFileSizeComparator = qsSettings.value(qsFILE_SIZE_COMPARATOR, "=").toString();
	sfsFindSettings.qsFileSizeType = qsSettings.value(qsFILE_SIZE_TYPE, qsBYTES2).toString();
	qsSettings.endGroup();

	return sfsFindSettings;
} // GetFindSettings

// font used in lister
QFont cSettings::GetListerFont()
{
	QFont qfFont;

	qfFont.fromString(qsSettings.value(qsPLUGINS__LISTER__SETTINGS__ + qsFONT).toString());

	return qfFont;
} // GetListerFont

// lister settings
cSettings::sLister cSettings::GetListerSettings()
{
	sLister slLister;

	qsSettings.beginGroup(qsPLUGINS__LISTER__SETTINGS__);
	slLister.qsCharSet = qsSettings.value(qsCHAR_SET, qsANSI).toString();
	slLister.bWrapText = qsSettings.value(qsWRAP_TEXT, true).toBool();
	slLister.bFitImageToWindow = qsSettings.value(qsFIT_IMAGE_TO_WINDOW).toBool();
	qsSettings.endGroup();

	return slLister;
} // GetListerSettings

// new tab by shortcut in foreground
bool cSettings::GetNewTabByShortcutInForeground()
{
	return qsSettings.value(qsTABS__NEW_TAB_BY_SHORTCUT_IN_FOREGROUND, true).toBool();
} // GetNewTabByShortcutInForeground

// open new tab next to current tab
bool cSettings::GetOpenNewTabNextToCurrentTab()
{
	return qsSettings.value(qsTABS__OPEN_NEW_TAB_NEXT_TO_CURRENT_TAB).toBool();
} // GetOpenNewTabNextToCurrentTab

// plugin date/time display format
QString cSettings::GetPluginDateTimeDisplay()
{
	return qsSettings.value(qsPLUGINS__DATE_TIME_DISPLAY, "%d.%o.%y %h:%m").toString();
} // GetPluginDateTimeDisplay

///< get plugin list
QList<cSettings::sPlugin> cSettings::GetPlugins(const ePlugin &epPlugin)
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
QString cSettings::GetPluginTimeDisplay()
{
	return qsSettings.value(qsPLUGINS__TIME_DISPLAY, "%h:%m:%s").toString();
} // GetPluginTimeDisplay

// alt key activity in quick search
bool cSettings::GetQuickSearchAlt()
{
	return qsSettings.value(qsQUICK_SEARCH__ALT, true).toBool();
} // GetQuickSearchAlt

// ctrl key activity in quick search
bool cSettings::GetQuickSearchCtrl()
{
	return qsSettings.value(qsQUICK_SEARCH__CTRL, true).toBool();
} // GetQuickSearchCtrl

// enabled/disabled quick search
bool cSettings::GetQuickSearchEnabled()
{
	return qsSettings.value(qsQUICK_SEARCH__ENABLED, true).toBool();
} // GetQuickSearchEnabled

// shift key activity in quick search
bool cSettings::GetQuickSearchShift()
{
	return qsSettings.value(qsQUICK_SEARCH__SHIFT).toBool();
} // GetQuickSearchShift

// quick search window visibility
bool cSettings::GetQuickSearchShowSearchWindow()
{
	return qsSettings.value(qsQUICK_SEARCH__SHOW_SEARCH_WINDOW, true).toBool();
} // GetQuickSearchShowSearchWindow

// find out readonly file overwrite mode
QString cSettings::GetReadonlyFileOverwrite()
{
	return qsSettings.value(qsOPERATIONS__ + qsREADONLY_FILE_OVERWRITE, qsASK).toString();
} // GetReadonlyFileOverwrite

// list of saved find settings for find files dialog
QStringList cSettings::GetSavedFinds()
{
	QStringList qslSavedFinds;

	qsSettings.beginGroup(qsFIND_FILES__);
	qslSavedFinds = qsSettings.childGroups();
	qsSettings.endGroup();

	return qslSavedFinds;
} // GetSavedFinds

// save settings on quit option
bool cSettings::GetSaveSettingsOnQuit()
{
	return qsSettings.value(qsDISPLAY__SAVE_SETTINGS_ON_QUIT, true).toBool();
} // GetSaveSettingsOnQuit

// selection mode of files and directories
QString cSettings::GetSelectionMode()
{
	return qsSettings.value(qsDISPLAY__SELECTION_MODE, qsONLY_FILES).toString();
} // GetSelectionMode

// find shortcut
QString cSettings::GetShortcut(const eShortcutCategory &escCategory, const QString &qsShortcut)
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

// square brackets around directory name
bool cSettings::GetShowBracketsAroundDirectoryName()
{
	return qsSettings.value(qsDISPLAY__SHOW_BRACKETS_AROUND_DIRECTORY_NAME, true).toBool();
} // GetShowBracketsAroundDirectoryName

// show drive letter in tab
bool cSettings::GetShowDriveLetter()
{
	return qsSettings.value(qsTABS__SHOW_DRIVE_LETTER).toBool();
} // GetShowDriveLetter

// show hidden files in dir view flag
bool cSettings::GetShowHiddenFiles()
{
	return qsSettings.value(qsDISPLAY__SHOW_HIDDEN_FILES).toBool();
} // GetShowHiddenFiles

// show system files in dir view flag
bool cSettings::GetShowSystemFiles()
{
	return qsSettings.value(qsDISPLAY__SHOW_SYSTEM_FILES).toBool();
} // GetShowSystemFiles

// show tab bar with only one tab flag
bool cSettings::GetShowTabBarWithOnlyOneTab()
{
	return qsSettings.value(qsTABS__SHOW_TAB_BAR_WITH_ONLY_ONE_TAB).toBool();
} // GetShowTabBarWithOnlyOneTab

///< get some information about tab
cSettings::sTabInfo cSettings::GetTabInfo(const ePosition &epPosition, const QString &qsIndex)
{
	sTabInfo stiTabInfo;

	if (epPosition == PositionLeft) {
		qsSettings.beginGroup(qsLEFT_PANEL__TABS__ + qsIndex);
	} else {
		qsSettings.beginGroup(qsRIGHT_PANEL__TABS__ + qsIndex);
	} // if else

	stiTabInfo.qsColumnSet = qsSettings.value(qsCOLUMN_SET).toString();
	stiTabInfo.qsDrive = qsSettings.value(qsDRIVE).toString();
	stiTabInfo.qsPath = qsSettings.value(qsPATH).toString();
	stiTabInfo.ssSort.iSortedColumn = qsSettings.value(qsSORTED_COLUMN).toInt();
	stiTabInfo.ssSort.soSortOrder = qsSettings.value(qsSORT_ORDER).toString() == qsASCENDING ? Qt::AscendingOrder : Qt::DescendingOrder;

	qsSettings.endGroup();

	return stiTabInfo;
} // GetTabInfo

// treat archives like directories
bool cSettings::GetTreatArchivesLikeDirectories()
{
	return qsSettings.value(qsPLUGINS__PACKER__SETTINGS__ + qsTREAT_ARCHIVES_LIKE_DIRECTORIES, true).toBool();
} // GetTreatArchivesLikeDirectories

// viewer type
QString cSettings::GetViewerType()
{
	return qsSettings.value(qsOPERATIONS__ + qsVIEWER_TYPE, qsINTERNAL).toString();
} // GetViewerType

// retrieve startup main window parameters
cSettings::sMainWindowState cSettings::GetWindowState()
{
	sMainWindowState smwsState;

	qsSettings.beginGroup(qsMAIN_WINDOW);
	smwsState.iHeight = qsSettings.value(qsHEIGHT, 0).toInt();
	smwsState.iWidth = qsSettings.value(qsWIDTH, 0).toInt();
	smwsState.qsWindowState = qsSettings.value(qsWINDOW_STATE, qsNORMAL).toString();
	qsSettings.endGroup();

	return smwsState;
} // GetWindowState

// get tab list for left or right panel
QStringList cSettings::GetTabs(const ePosition &epPosition)
{
	QStringList qslTabs;

	if (epPosition == PositionLeft) {
		qsSettings.beginGroup(qsLEFT_PANEL__TABS__);
	} else {
		qsSettings.beginGroup(qsRIGHT_PANEL__TABS__);
	} // if else
	qslTabs = qsSettings.childGroups();
	qsSettings.endGroup();

	return qslTabs;
} // GetTabs

// remove column set
void cSettings::RemoveColumnSet(const QString &qsColumnSet)
{
	qsSettings.remove(qsCOLUMN_SET__ + qsColumnSet);
} // RemoveColumnSet

// remove find settings
void cSettings::RemoveFindSettings(const QString &qsFindSettings)
{
	qsSettings.remove(qsFIND_FILES__ + qsFindSettings);
} // RemoveFindSettings

// restore old application's settings
void cSettings::RestoreSettings(QMap <QString, QString> &qmSettings)
{
	qsSettings.clear();

	QMapIterator<QString, QString> qmiSettings(qmSettings);
	while (qmiSettings.hasNext()) {
		qmiSettings.next();
		qsSettings.setValue(qmiSettings.key(), qmiSettings.value());
	} // while
} // RestoreSettings

// delete non empty directory
void cSettings::SetAskToDeleteNonEmptyDirectory(const bool &bAsk)
{
	qsSettings.setValue(qsOPERATIONS__ + qsASK_TO_DELETE_NON_EMPTY_DIRECTORY, bAsk);
} // SetAskToDeleteNonEmptyDirectory

// case sensitive sorting
void cSettings::SetCaseSensitiveSorting(const bool &bCaseSensitive)
{
	qsSettings.setValue(qsDISPLAY__CASE_SENSITIVE_SORTING, bCaseSensitive);
} // SetCaseSensitiveSorting

// close tab on double click
void cSettings::SetCloseTabOnDoubleClick(const bool &bClose)
{
	qsSettings.setValue(qsTABS__CLOSE_TAB_ON_DOUBLE_CLICK, bClose);
} // SetCloseTabOnDoubleClick

// confirm close of all tabs
void cSettings::SetConfirmCloseOfAllTabs(const bool &bClose)
{
	qsSettings.setValue(qsTABS__CONFIRM_CLOSE_OF_ALL_TABS, bClose);
} // SetConfirmCloseOfAllTabs

// copy/move buffer size
void cSettings::SetCopyMoveBufferSize(const int &iSize)
{
	qsSettings.setValue(qsOPERATIONS__ + qsBUFFER_SIZE, iSize);
} // SetCopyMoveBufferSize

// date/time display format
void cSettings::SetDateTimeDisplay(const QString &qsDateTime)
{
	qsSettings.setValue(qsDISPLAY__DATE_TIME_DISPLAY, qsDateTime);
} // SetDateTimeDisplay

#ifdef Q_WS_WIN
// delete files to recycle bin
void cSettings::SetDeleteToRecycleBin(const bool &bRecycleBin)
{
	qsSettings.setValue(qsOPERATIONS__ + qsDELETE_TO_RECYCLE_BIN, bRecycleBin);
} // SetDeleteToRecycleBin
#endif

// external editor
void cSettings::SetExternalEditor(const QString &qsEditor)
{
	qsSettings.setValue(qsOPERATIONS__ + qsEXTERNAL_EDITOR, qsEditor);
} // SetExternalEditor

// external viewer
void cSettings::SetExternalViewer(const QString &qsViewer)
{
	qsSettings.setValue(qsOPERATIONS__ + qsEXTERNAL_VIEWER, qsViewer);
} // SetExternalViewer

// set favourite directories
void cSettings::SetFavouriteDirectories(const QList<QPair<QString, cSettings::sFavouriteDirectory> > &qlFavouriteDirectories)
{
	qsSettings.beginGroup(qsOTHERS__FAVOURITE_DIRECTORIES);

	// clear current favourite directories
	qsSettings.remove("");

	// set new one
	CreateFavouriteDirectories(qlFavouriteDirectories);

	qsSettings.endGroup();
} // SetFavouriteDirectories

// set default overwrite behaviour
void cSettings::SetFileOverwrite(const QString &qsMode)
{
	qsSettings.setValue(qsOPERATIONS__ + qsFILE_OVERWRITE, qsMode);
} // SetFileOverwrite

// set file size mode
void cSettings::SetFileSizeIn(const QString &qsSize)
{
	qsSettings.setValue(qsDISPLAY__FILE_SIZE_IN, qsSize);
} // SetFileSizeIn

// save find settings for find files dialog
void cSettings::SetFindSettings(const QString &qsName, const sFindSettings &sfsFindSettings)
{
	qsSettings.beginGroup(qsFIND_FILES__ + qsName);
	// general
	qsSettings.setValue(qsSEARCH_FOR, sfsFindSettings.qsSearchFor);
	qsSettings.setValue(qsSEARCH_FOR_REGULAR_EXPRESSION, sfsFindSettings.bSearchForRegularExpression);
	qsSettings.setValue(qsSEARCH_IN, sfsFindSettings.qsSearchIn);
	qsSettings.setValue(qsSUBDIRECTORY_DEPTH, sfsFindSettings.iSubdirectoryDepth);
	qsSettings.setValue(qsSEARCH_FOR_TEXT, sfsFindSettings.bSearchForText);
	if (sfsFindSettings.bSearchForText) {
		qsSettings.setValue(qsFULL_TEXT, sfsFindSettings.qsFullText);
		qsSettings.setValue(qsFULL_TEXT_WHOLE_WORDS, sfsFindSettings.bFulTextWholeWords);
		qsSettings.setValue(qsFULL_TEXT_CASE_SENSITIVE, sfsFindSettings.bFullTextCaseSensitive);
		qsSettings.setValue(qsFULL_TEXT_NOT_CONTAINING_TEXT, sfsFindSettings.bFullTextNotContainingText);
		qsSettings.setValue(qsFULL_TEXT_HEX, sfsFindSettings.bFullTextHex);
		qsSettings.setValue(qsFULL_TEXT_REGULAR_EXPRESSION, sfsFindSettings.bFullTextRegularExpression);
	} // if
	// advanced
	qsSettings.setValue(qsDATE_TIME_BETWEEN, sfsFindSettings.bDateTimeBetween);
	if (sfsFindSettings.bDateTimeBetween) {
		qsSettings.setValue(qsDATE_TIME_FROM, sfsFindSettings.qdtFrom);
		qsSettings.setValue(qsDATE_TIME_TO, sfsFindSettings.qdtTo);
	} // if
	qsSettings.setValue(qsNOT_OLDER_THAN, sfsFindSettings.bNotOlderThan);
	if (sfsFindSettings.bNotOlderThan) {
		qsSettings.setValue(qsNOT_OLDER_THAN_COUNT, sfsFindSettings.iNotOlderThanCount);
		qsSettings.setValue(qsNOT_OLDER_THAN_TYPE, sfsFindSettings.qsNotOlderThanType);
	} // if
	qsSettings.setValue(qsFILE_SIZE, sfsFindSettings.bFileSize);
	if (sfsFindSettings.bFileSize) {
		qsSettings.setValue(qsFILE_SIZE_COMPARATOR, sfsFindSettings.qsFileSizeComparator);
		qsSettings.setValue(qsFILE_SIZE_VALUE, sfsFindSettings.iFileSizeValue);
		qsSettings.setValue(qsFILE_SIZE_TYPE, sfsFindSettings.qsFileSizeType);
	} // if
	qsSettings.endGroup();
} // SetFindSettings

// font used in lister
void cSettings::SetListerFont(const QFont &qfFont)
{
	qsSettings.setValue(qsPLUGINS__LISTER__SETTINGS__ + qsFONT, qfFont.toString());
} // SetListerFont

// set default lister settings
void cSettings::SetListerSettings(const sLister &slLister)
{
	qsSettings.beginGroup(qsPLUGINS__LISTER__SETTINGS__);
	qsSettings.setValue(qsCHAR_SET, slLister.qsCharSet);
	qsSettings.setValue(qsWRAP_TEXT, slLister.bWrapText);
	qsSettings.setValue(qsFIT_IMAGE_TO_WINDOW, slLister.bFitImageToWindow);
	qsSettings.endGroup();
} // SetListerSettings

// new tab by shortcut in foreground
void cSettings::SetNewTabByShortcutInForeground(const bool &bForeground)
{
	qsSettings.setValue(qsTABS__NEW_TAB_BY_SHORTCUT_IN_FOREGROUND, bForeground);
} // SetNewTabByShortcutInForeground

// open new tab next to current tab
void cSettings::SetOpenNewTabNextToCurrentTab(const bool &bNextTo)
{
	qsSettings.setValue(qsTABS__OPEN_NEW_TAB_NEXT_TO_CURRENT_TAB, bNextTo);
} // SetOpenNewTabNextToCurrentTab

// plugin date/time display format
void cSettings::SetPluginDateTimeDisplay(const QString &qsDateTime)
{
	qsSettings.setValue(qsPLUGINS__DATE_TIME_DISPLAY, qsDateTime);
} // SetPluginDateTimeDisplay

// write plugins into settings file
void cSettings::SetPlugins(const ePlugin &epPlugin, const QList<sPlugin> &qlPlugins)
{
	int iI;

	switch (epPlugin) {
		// TODO GetPlugins - other plugin types
		case ContentPlugins:
			qsSettings.beginWriteArray(qsPLUGINS__CONTENT);
			break;
		case ListerPlugins:
			qsSettings.beginWriteArray(qsPLUGINS__LISTER);
			break;
		case PackerPlugins:
			qsSettings.beginWriteArray(qsPLUGINS__PACKER);
	} // switch

	qsSettings.remove("");

	for (iI = 0; iI < qlPlugins.count(); iI++) {
		QString qsKey, qsValue;

		qsSettings.setArrayIndex(iI);

		qsKey = qsNAME;
		qsValue = qlPlugins.at(iI).qsName;
		qsSettings.setValue(qsKey, qsValue);
		if (epPlugin == PackerPlugins) {
			qsKey = qsEXTENSIONS;
			qsValue = qlPlugins.at(iI).qsExtensions;
			qsSettings.setValue(qsKey, qsValue);
		} // if
		qsKey = qsENABLED;
		if (qlPlugins.at(iI).bEnabled) {
			qsValue = qsTRUE;
		} else {
			qsValue = qsFALSE;
		} // if else
		qsSettings.setValue(qsKey, qsValue);
	} // for

	qsSettings.endArray();
} // SetPlugins

///< plugin time display format
void cSettings::SetPluginTimeDisplay(const QString &qsTime)
{
	qsSettings.setValue(qsPLUGINS__TIME_DISPLAY, qsTime);
} // SetPluginTimeDisplay

// alt key activity in quick search
void cSettings::SetQuickSearchAlt(const bool &bAlt)
{
	qsSettings.setValue(qsQUICK_SEARCH__ALT, bAlt);
} // SetQuickSearchAlt

// ctrl key activity in quick search
void cSettings::SetQuickSearchCtrl(const bool &bCtrl)
{
	qsSettings.setValue(qsQUICK_SEARCH__CTRL, bCtrl);
} // SetQuickSearchCtrl

// enabled/disabled quick search
void cSettings::SetQuickSearchEnabled(const bool &bEnable)
{
	qsSettings.setValue(qsQUICK_SEARCH__ENABLED, bEnable);
} // SetQuickSearchEnabled

// shift key activity in quick search
void cSettings::SetQuickSearchShift(const bool &bShift)
{
	qsSettings.setValue(qsQUICK_SEARCH__SHIFT, bShift);
} // SetQuickSearchShift

// quick search window visibility
void cSettings::SetQuickSearchShowSearchWindow(const bool &bShow)
{
	qsSettings.setValue(qsQUICK_SEARCH__SHOW_SEARCH_WINDOW, bShow);
} // SetQuickSearchShowSearchWindow

// set default readonly overwrite behaviour
void cSettings::SetReadonlyFileOverwrite(const QString &qsMode)
{
	qsSettings.setValue(qsOPERATIONS__ + qsREADONLY_FILE_OVERWRITE, qsMode);
} // SetReadonlyFileOverwrite

// save settings on quit option
void cSettings::SetSaveSettingsOnQuit(const bool &bSave)
{
	qsSettings.setValue(qsDISPLAY__SAVE_SETTINGS_ON_QUIT, bSave);
} // SetSaveSettingsOnQuit

// set selection mode of files and directories
void cSettings::SetSelectionMode(const QString &qsMode)
{
	qsSettings.setValue(qsDISPLAY__SELECTION_MODE, qsMode);
} // SetSelectionMode

// set shortcut
void cSettings::SetShortcut(const eShortcutCategory &escCategory, const QString &qsName, const QString &qsShortcut)
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

	qsSettings.setValue(qsOTHERS__SHORTCUTS__ + qsCategory + '/' + qsName, qsShortcut);
} // SetShortcut

// square brackets around directory name
void cSettings::SetShowBracketsAroundDirectoryName(const bool &bShowBrackets)
{
	qsSettings.setValue(qsDISPLAY__SHOW_BRACKETS_AROUND_DIRECTORY_NAME, bShowBrackets);
} // SetShowBracketsAroundDirectoryName

// show drive letter in tab
void cSettings::SetShowDriveLetter(const bool &bShow)
{
	qsSettings.setValue(qsTABS__SHOW_DRIVE_LETTER, bShow);
} // SetShowDriveLetter

// set show hidden files in dir view
void cSettings::SetShowHiddenFiles(const bool &bShowHidden)
{
	qsSettings.setValue(qsDISPLAY__SHOW_HIDDEN_FILES, bShowHidden);
} // SetShowHiddenFiles

// set show system files in dir view
void cSettings::SetShowSystemFiles(const bool &bShowSystem)
{
	qsSettings.setValue(qsDISPLAY__SHOW_SYSTEM_FILES, bShowSystem);
} // SetShowSystemFiles

// show tab bar with only one tab
void cSettings::SetShowTabBarWithOnlyOneTab(const bool &bShow)
{
	qsSettings.setValue(qsTABS__SHOW_TAB_BAR_WITH_ONLY_ONE_TAB, bShow);
} // SetShowTabBarWithOnlyOneTab

// save tab settings
void cSettings::SetTabs(const ePosition &epPosition, const QList<sTabInfo> &qlTabs)
{
	int iI;

	// remove old tabs
	if (epPosition == PositionLeft) {
		qsSettings.beginGroup(qsLEFT_PANEL__TABS__);
	} else {
		qsSettings.beginGroup(qsRIGHT_PANEL__TABS__);
	} // if else
	qsSettings.remove("");
	qsSettings.endGroup();

	// create new tabs
	for (iI = 0; iI < qlTabs.count(); iI++) {
		CreateTab(epPosition, iI, qlTabs.at(iI));
	} // for
} // SetTabs

// treat archives like directories
void cSettings::SetTreatArchivesLikeDirectories(const bool &bLikeDirectories)
{
	qsSettings.setValue(qsPLUGINS__PACKER__SETTINGS__ + qsTREAT_ARCHIVES_LIKE_DIRECTORIES, bLikeDirectories);
} // SetTreatArchivesLikeDirectories

// viewer type
void cSettings::SetViewerType(const QString &qsType)
{
	qsSettings.setValue(qsOPERATIONS__ + qsVIEWER_TYPE, qsType);
} // SetViewerType

// set startup main window state
void cSettings::SetWindowState(const sMainWindowState &smwsState)
{
	qsSettings.beginGroup(qsMAIN_WINDOW);
	if (smwsState.qsWindowState == qsNORMAL) {
		qsSettings.setValue(qsHEIGHT, smwsState.iHeight);
		qsSettings.setValue(qsWIDTH, smwsState.iWidth);
	} // if
	qsSettings.setValue(qsWINDOW_STATE, smwsState.qsWindowState);
	qsSettings.endGroup();
} // SetWindowState
