#include "Settings.h"

#include <QtCore/QStringList>
#include <QtGui/QFont>

const QString qsASCENDING = "ascending";
const QString qsASK_TO_DELETE_NON_EMPTY_DIRECTORY = "AskToDeleteNonEmptyDirectory";
const QString qsBUFFER_SIZE = "BufferSize";
const QString qsCHAR_SET = "CharSet";
const QString qsCOLUMN_SET = "ColumnSet";
const QString qsCOMMAND_LINE = "CommandLine";
const QString qsDATE_TIME_BETWEEN = "DateTimeBetween";
const QString qsDATE_TIME_FROM = "DateTimeFrom";
const QString qsDATE_TIME_TO = "DateTimeTo";
const QString qsDEFAULT_PACKER_PLUGIN = "DefaultPackerPlugin";
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
const QString qsFILE_OPERATION_DESTINATION = "FileOperationDestination";
const QString qsFILE_OPERATION_FILTER = "FileOperationFilter";
const QString qsFILE_OVERWRITE = "FileOverwrite";
const QString qsFILE_SIZE = "FileSize";
const QString qsFILE_SIZE_COMPARATOR = "FileSizeComparator";
const QString qsFILE_SIZE_TYPE = "FileSizeType";
const QString qsFILE_SIZE_VALUE = "FileSizeValue";
const QString qsFIND_FILES_SEARCH_FOR = "FindFilesSearchFor";
const QString qsFIND_FILES_SEARCH_IN = "FindFilesSearchIn";
const QString qsFIT_IMAGE_TO_WINDOW = "FitImageToWindow";
const QString qsFONT = "Font";
const QString qsFULL_TEXT = "FullText";
const QString qsFULL_TEXT_CASE_SENSITIVE = "FullTextCaseSensitive";
const QString qsFULL_TEXT_HEX = "FullTextHex";
const QString qsFULL_TEXT_NOT_CONTAINING_TEXT = "FullTextNotContainingText";
const QString qsFULL_TEXT_REGULAR_EXPRESSION = "FullTextRegularExpression";
const QString qsFULL_TEXT_WHOLE_WORDS = "FullTextWholeWords";
const QString qsHEIGHT = "Height";
const QString qsHISTORY = "History";
const QString qsIDENTIFIER = "Identifier";
const QString qsINCLUDING_SUBDIRECTORIES = "IncludingSubdirectories";
const QString qsLISTER = "Lister";
const QString qsMAIN_MENU = "MainMenu";
const QString qsMOVE_TO_ARCHIVE = "MoveToArchive";
const QString qsNOT_OLDER_THAN = "NotOlderThan";
const QString qsNOT_OLDER_THAN_COUNT = "NotOlderThanCount";
const QString qsNOT_OLDER_THAN_TYPE = "NotOlderThanType";
const QString qsONE_ARCHIVE_PER_FILE_OR_DIRECTORY = "OneArchivePerFileOrDirectory";
const QString qsOPERATIONS = "Operations";
const QString qsPACK_FILES_DESTINATION = "PackFilesDestination";
const QString qsPACK_PATH_NAMES = "PackPathNames";
const QString qsPANELS = "Panels";
const QString qsPATH = "Path";
const QString qsPLUGIN = "Plugin";
const QString qsPOSITION = "Position";
const QString qsREADONLY_FILE_OVERWRITE = "ReadonlyFileOverwrite";
const QString qsSEARCH_FOR = "SearchFor";
const QString qsSEARCH_FOR_REGULAR_EXPRESSION = "SearchForRegularExpression";
const QString qsSEARCH_FOR_TEXT = "SearchForText";
const QString qsSEARCH_IN = "SearchIn";
const QString qsSELECT_FILES_FILTER = "SelectFilesFilter";
const QString qsSHOW = "Show";
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
const QString qsUNPACK_FILES_DESTINATION = "UnpackFilesDestination";
const QString qsUNPACK_FILES_FILTER = "UnpackFilesFilter";
const QString qsVIEWER_TYPE = "ViewerType";
const QString qsWIDTH = "Width";
const QString qsWINDOW_STATE = "WindowState";
const QString qsWRAP_TEXT = "WrapText";

// settings file
// ColumnSet
const QString qsCOLUMN_SET__ = qsCOLUMN_SET + "/";
// Display
const QString qsDISPLAY__CASE_SENSITIVE_SORTING = "Display/CaseSensitiveSorting";
const QString qsDISPLAY__DATE_TIME_DISPLAY = "Display/DateTimeDisplay";
const QString qsDISPLAY__FILE_SIZE_IN = "Display/FileSizeIn";
const QString qsDISPLAY__SAVE_SETTINGS_ON_QUIT = "Display/SaveSettingsOnQuit";
const QString qsDISPLAY__SELECTION_MODE = "Display/SelectionMode";
const QString qsDISPLAY__SHOW_BRACKETS_AROUND_DIRECTORY_NAME = "Display/ShowBracketsAroundDirectoryName";
const QString qsDISPLAY__SHOW_DIRECTORY_VIEW_HEADER = "Display/ShowDirectoryViewHeader"; 
const QString qsDISPLAY__SHOW_HIDDEN_FILES = "Display/ShowHiddenFiles";
const QString qsDISPLAY__SHOW_SYSTEM_FILES = "Display/ShowSystemFiles";
// FindFiles
const QString qsFIND_FILES__ = "FindFiles/";
// LeftPanel
const QString qsLEFT_PANEL__TABS__ = "LeftPanel/Tabs/";
// MainWindow
const QString qsMAIN_WINDOW = "MainWindow";
// Miscellaneous
const QString qsMISCELLANEOUS__MAXIMUM_HISTORY_DIRECTORY_LIST_SIZE = "Miscellaneous/MaximumHistoryDirectoryListSize";
// Operations
const QString qsOPERATIONS__ = "Operations/";
// Others/FavouriteDirectories
const QString qsOTHERS__FAVOURITE_DIRECTORIES = "Others/FavouriteDirectories";
// Others/Shortcuts
const QString qsOTHERS__SHORTCUTS__ = "Others/Shortcuts/";
// Plugins
const QString qsPLUGINS__CONTENT = "Plugins/Content";
const QString qsPLUGINS__DATE_TIME_DISPLAY = "Plugins/DateTimeDisplay";
const QString qsPLUGINS__LISTER = "Plugins/Lister";
const QString qsPLUGINS__LISTER__SETTINGS__ = "Plugins/Lister/Settings/";
const QString qsPLUGINS__PACKER = "Plugins/Packer";
const QString qsPLUGINS__PACKER__SETTINGS__ = "Plugins/Packer/Settings/";
const QString qsPLUGINS__TIME_DISPLAY = "Plugins/TimeDisplay";
// Quick search
const QString qsQUICK_SEARCH__ALT = "QuickSearch/Alt";
const QString qsQUICK_SEARCH__CTRL = "QuickSearch/Ctrl";
const QString qsQUICK_SEARCH__ENABLED = "QuickSearch/Enabled";
const QString qsQUICK_SEARCH__SHIFT = "QuickSearch/Shift";
const QString qsQUICK_SEARCH__SHOW_SEARCH_WINDOW = "QuickSearch/ShowSearchWindow";
// RightPanel
const QString qsRIGHT_PANEL__TABS__ = "RightPanel/Tabs/";
// Tabs
const QString qsTABS__CLOSE_TAB_ON_DOUBLE_CLICK = "Tabs/CloseTabOnDoubleClick";
const QString qsTABS__CONFIRM_CLOSE_OF_ALL_TABS = "Tabs/ConfirmCloseOfAllTabs";
const QString qsTABS__NEW_TAB_BY_SHORTCUT_IN_FOREGROUND = "Tabs/NewTabByShortcutInForeground";
const QString qsTABS__OPEN_NEW_TAB_NEXT_TO_CURRENT_TAB = "Tabs/OpenNewTabNextToCurrentTab";
const QString qsTABS__SHOW_DRIVE_LETTER = "Tabs/ShowDriveLetter";
const QString qsTABS__SHOW_TAB_BAR_WITH_ONLY_ONE_TAB = "Tabs/ShowTabBarWithOnlyOneTab";

// collect favourite directories from settings file
const QList<QPair<QString, cSettings::sFavouriteDirectory> > cSettings::CollectFavouriteDirectories()
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
const void cSettings::CreateColumnSet(const QString &qsColumnSet)
{
	qsSettings.beginGroup(qsCOLUMN_SET__ + qsColumnSet);
	// write something to create group
	qsSettings.setValue(".","");
	qsSettings.remove(".");
	qsSettings.endGroup();
} // CreateColumnSet

// create new column set
const void cSettings::CreateColumnSet(const QString &qsColumnSet, const QList<sColumn> &qlColumns)
{
	int iI;

	qsSettings.beginWriteArray(qsCOLUMN_SET__ + qsColumnSet);
	qsSettings.remove("");

	for (iI = 0; iI < qlColumns.count(); iI++) {
		const sColumn *scColumn;

		qsSettings.setArrayIndex(iI);

		scColumn = &qlColumns.at(iI);
		qsSettings.setValue(qsIDENTIFIER, scColumn->qsIdentifier);
		qsSettings.setValue(qsNAME, scColumn->qsName);
		qsSettings.setValue(qsPLUGIN, scColumn->qsPlugin);
		qsSettings.setValue(qsUNIT, scColumn->qsUnit);
		qsSettings.setValue(qsWIDTH, scColumn->iWidth);
	} // for

	qsSettings.endArray();
} // CreateColumnSet

// create default (Full) column set
const void cSettings::CreateDefaultColumnSet()
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
const void cSettings::CreateFavouriteDirectories(const QList<QPair<QString, cSettings::sFavouriteDirectory> > &qlFavouriteDirectories)
{
	int iI;

	for (iI = 0; iI < qlFavouriteDirectories.count(); iI++) {
		const QPair<QString, cSettings::sFavouriteDirectory> *qpFavourite;

		qpFavourite = &qlFavouriteDirectories.at(iI);
		qsSettings.beginGroup(QVariant(iI).toString());

		qsSettings.setValue(qsNAME, qpFavourite->first);
		qsSettings.setValue(qsSUBMENU, qpFavourite->second.bSubmenu);

		if (qpFavourite->second.bSubmenu) {
			CreateFavouriteDirectories(qpFavourite->second.qlChildFavourites);
		} else {
			qsSettings.setValue(qsSOURCE, qpFavourite->second.qsSource);
			qsSettings.setValue(qsTARGET_ENABLED, qpFavourite->second.bTarget);
			qsSettings.setValue(qsTARGET, qpFavourite->second.qsTarget);
		} // if else

		qsSettings.endGroup();
	} // for
} // CreateFavouriteDirectories

// create new tab in settings file
const void cSettings::CreateTab(const ePosition &epPosition, const uint &uiIndex, const sTabInfo &stiTab)
{
	int iI;

	if (epPosition == PositionLeft) {
		qsSettings.beginGroup(QString("%1%2").arg(qsLEFT_PANEL__TABS__).arg(uiIndex));
	} else {
		qsSettings.beginGroup(QString("%1%2").arg(qsRIGHT_PANEL__TABS__).arg(uiIndex));
	} // if else

	qsSettings.setValue(qsCOLUMN_SET, stiTab.qsColumnSet);
	qsSettings.setValue(qsDRIVE, stiTab.qsDrive);
	qsSettings.setValue(qsPATH, stiTab.qsPath);
	qsSettings.setValue(qsSORTED_COLUMN, stiTab.ssSort.iSortedColumn);
	qsSettings.setValue(qsSORT_ORDER, stiTab.ssSort.soSortOrder == Qt::AscendingOrder ? qsASCENDING : qsDESCENDING);

	// history
	qsSettings.setValue(qsHISTORY + '/' + qsPOSITION, stiTab.shHistory.iPosition);
	qsSettings.beginWriteArray(qsHISTORY);
	for (iI = 0; iI < stiTab.shHistory.qlLastPaths.count(); iI++) {
		const sLastPath *slpLastPath;

		qsSettings.setArrayIndex(iI);

		slpLastPath = &stiTab.shHistory.qlLastPaths.at(iI);

		qsSettings.setValue(qsSHOW, slpLastPath->qsShow);
		qsSettings.setValue(qsPATH, slpLastPath->qsPath);
	} // for
	qsSettings.endArray();

	qsSettings.endGroup();
} // CreateTab

// get all application's settings
const QMap <QString, QString> cSettings::GetAllSettings() const
{
	int iI;
	QMap <QString, QString> qlSettings;
	QStringList qslKeys;

	qslKeys = qsSettings.allKeys();
	for (iI = 0; iI < qslKeys.count(); iI++) {
		const QString *qsKey;

		qsKey = &qslKeys.at(iI);
		qlSettings.insert(*qsKey, qsSettings.value(*qsKey).toString());
	} // for

	return qlSettings;
} // GetAllSettings

// delete non empty directory
const bool cSettings::GetAskToDeleteNonEmptyDirectory() const
{
	return qsSettings.value(qsOPERATIONS__ + qsASK_TO_DELETE_NON_EMPTY_DIRECTORY, true).toBool();
} // GetAskToDeleteNonEmptyDirectory

// case sensitive sorting
const bool cSettings::GetCaseSensitiveSorting() const
{
	return qsSettings.value(qsDISPLAY__CASE_SENSITIVE_SORTING).toBool();
} // GetCaseSensitiveSorting

// close tab in tab bar on double click
const bool cSettings::GetCloseTabOnDoubleClick() const
{
	return qsSettings.value(qsTABS__CLOSE_TAB_ON_DOUBLE_CLICK, true).toBool();
} // GetCloseTabOnDoubleClick

// get some information about column
const cSettings::sColumn cSettings::GetColumnInfo(const QString &qsColumnSet, const QString &qsColumn)
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
const QStringList cSettings::GetColumnSets()
{
	QStringList qsColumnSets;

	qsSettings.beginGroup(qsCOLUMN_SET__);
	qsColumnSets = qsSettings.childGroups();
	qsSettings.endGroup();

	return qsColumnSets;
} // GetColumnSets

// get column names in column set
const QStringList cSettings::GetColumnsInSet(const QString &qsColumnSet)
{
	QStringList qslColumns;

	qsSettings.beginGroup(qsCOLUMN_SET__ + qsColumnSet);
	qslColumns = qsSettings.childGroups();
	qsSettings.endGroup();

	return qslColumns;
} // GetColumnsInSet

// get history of specified type
const QStringList cSettings::GetComboBoxHistory(const eHistoryType &ehtHistory)
{
	int iI;
	QStringList qslHistory;

	qsSettings.beginGroup(qsHISTORY);
	switch (ehtHistory) {
		case CommandLineHistory:
			qsSettings.beginGroup(qsCOMMAND_LINE);
			break;
		case FileOperationDestination:
			qsSettings.beginGroup(qsFILE_OPERATION_DESTINATION);
			break;
		case FileOperationFilter:
			qsSettings.beginGroup(qsFILE_OPERATION_FILTER);
			break;
		case FindFilesSearchFor:
			qsSettings.beginGroup(qsFIND_FILES_SEARCH_FOR);
			break;
		case FindFilesSearchIn:
			qsSettings.beginGroup(qsFIND_FILES_SEARCH_IN);
			break;
		case PackFilesDestination:
			qsSettings.beginGroup(qsPACK_FILES_DESTINATION);
			break;
		case SelectFilesFilter:
			qsSettings.beginGroup(qsSELECT_FILES_FILTER);
			break;
		case UnpackFilesFilter:
			qsSettings.beginGroup(qsUNPACK_FILES_FILTER);
			break;
		case UnpackFilesDestination:
			qsSettings.beginGroup(qsUNPACK_FILES_DESTINATION);
	} // switch

	iI = 1;
	while (true) {
		if (qsSettings.contains(QVariant(iI).toString())) {
			qslHistory.append(qsSettings.value(QVariant(iI).toString()).toString());
			iI++;
		} else {
			break;
		} // if else
	} // while

	qsSettings.endGroup();
	qsSettings.endGroup();

	return qslHistory;
} // GetComboBoxHistory

// confirm close of all tabs in tab bar
const bool cSettings::GetConfirmCloseOfAllTabs() const
{
	return qsSettings.value(qsTABS__CONFIRM_CLOSE_OF_ALL_TABS).toBool();
} // GetConfirmCloseOfAllTabs

// copy/move buffer size
const int cSettings::GetCopyMoveBufferSize() const
{
	return qsSettings.value(qsOPERATIONS__ + qsBUFFER_SIZE, 32).toInt();
} // GetCopyMoveBufferSize

// date/time display format
const QString cSettings::GetDateTimeDisplay() const
{
	return qsSettings.value(qsDISPLAY__DATE_TIME_DISPLAY, "%d.%o.%y %h:%m").toString();
} // GetDateTimeDisplay

#ifdef Q_WS_WIN
// delete files to recycle bin
const bool cSettings::GetDeleteToRecycleBin() const
{
	return qsSettings.value(qsOPERATIONS__ + qsDELETE_TO_RECYCLE_BIN, true).toBool();
} // GetDeleteToRecycleBin
#endif

// external editor
const QString cSettings::GetExternalEditor() const
{
	// TODO GetExternalEditor other OS than Windows
	return qsSettings.value(qsOPERATIONS__ + qsEXTERNAL_EDITOR, "notepad %1").toString();
} // GetExternalEditor

// external viewer
const QString cSettings::GetExternalViewer() const
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
const QString cSettings::GetFileOverwrite() const
{
	return qsSettings.value(qsOPERATIONS__ + qsFILE_OVERWRITE, qsASK).toString();
} // GetFileOverwrite

// unit for files size
const QString cSettings::GetFileSizeIn() const
{
	return qsSettings.value(qsDISPLAY__FILE_SIZE_IN, qsDYNAMIC).toString();
} // GetFileSizeIn

// get find settings
const cSettings::sFindSettings cSettings::GetFindSettings(const QString &qsName)
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
const QFont cSettings::GetListerFont() const
{
	QFont qfFont;

	qfFont.fromString(qsSettings.value(qsPLUGINS__LISTER__SETTINGS__ + qsFONT).toString());

	return qfFont;
} // GetListerFont

// lister settings
const cSettings::sLister cSettings::GetListerSettings()
{
	sLister slLister;

	qsSettings.beginGroup(qsPLUGINS__LISTER__SETTINGS__);
	slLister.qsCharSet = qsSettings.value(qsCHAR_SET, qsANSI).toString();
	slLister.bWrapText = qsSettings.value(qsWRAP_TEXT, true).toBool();
	slLister.bFitImageToWindow = qsSettings.value(qsFIT_IMAGE_TO_WINDOW).toBool();
	qsSettings.endGroup();

	return slLister;
} // GetListerSettings

// get maximum items in directory history list
const int cSettings::GetMaximumHistoryDirectoryListSize() const
{
	return qsSettings.value(qsMISCELLANEOUS__MAXIMUM_HISTORY_DIRECTORY_LIST_SIZE, 20).toInt();
} // GetMaximumHistoryDirectoryListSize

// new tab by shortcut in foreground
const bool cSettings::GetNewTabByShortcutInForeground() const
{
	return qsSettings.value(qsTABS__NEW_TAB_BY_SHORTCUT_IN_FOREGROUND, true).toBool();
} // GetNewTabByShortcutInForeground

// open new tab next to current tab
const bool cSettings::GetOpenNewTabNextToCurrentTab() const
{
	return qsSettings.value(qsTABS__OPEN_NEW_TAB_NEXT_TO_CURRENT_TAB).toBool();
} // GetOpenNewTabNextToCurrentTab

// default packer plugin to use for packing files
const cSettings::sDefaultPackerPlugin cSettings::GetPackerDefaultPlugin() const
{
	QStringList qslDefaultPackerPlugin;
	sDefaultPackerPlugin sdppPlugin;

	qslDefaultPackerPlugin = qsSettings.value(qsPLUGINS__PACKER__SETTINGS__ + qsDEFAULT_PACKER_PLUGIN).toString().split(';');
	if (qslDefaultPackerPlugin.count() == 2) {
		sdppPlugin.qsPlugin = qslDefaultPackerPlugin.at(0);
		sdppPlugin.qsExtension = qslDefaultPackerPlugin.at(1);
	} // if

	return sdppPlugin;
} // GetPackerDefaultPlugin

// pack including subdirectories
const bool cSettings::GetPackerIncludingSubdirectories() const
{
	return qsSettings.value(qsPLUGINS__PACKER__SETTINGS__ + qsINCLUDING_SUBDIRECTORIES, true).toBool();
} // GetPackerIncludingSubdirectories

// delete source files when packing
const bool cSettings::GetPackerMoveToArchive() const
{
	return qsSettings.value(qsPLUGINS__PACKER__SETTINGS__ + qsMOVE_TO_ARCHIVE).toBool();
} // GetPackerMoveToArchive

// pack each source file or directory to one archive
const bool cSettings::GetPackerOneArchivePerFileOrDirectory() const
{
	return qsSettings.value(qsPLUGINS__PACKER__SETTINGS__ + qsONE_ARCHIVE_PER_FILE_OR_DIRECTORY).toBool();
} // GetPackerOneArchivePerFileOrDirectory

// pack source files with full path
const bool cSettings::GetPackerPackPathNames() const
{
	return qsSettings.value(qsPLUGINS__PACKER__SETTINGS__ + qsPACK_PATH_NAMES, true).toBool();
} // GetPackerPackPathNames

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

// alt key activity in quick search
const bool cSettings::GetQuickSearchAlt() const
{
	return qsSettings.value(qsQUICK_SEARCH__ALT, true).toBool();
} // GetQuickSearchAlt

// ctrl key activity in quick search
const bool cSettings::GetQuickSearchCtrl() const
{
	return qsSettings.value(qsQUICK_SEARCH__CTRL, true).toBool();
} // GetQuickSearchCtrl

// enabled/disabled quick search
const bool cSettings::GetQuickSearchEnabled() const
{
	return qsSettings.value(qsQUICK_SEARCH__ENABLED, true).toBool();
} // GetQuickSearchEnabled

// quick search window visibility
const bool cSettings::GetQuickSearchShowSearchWindow() const
{
	return qsSettings.value(qsQUICK_SEARCH__SHOW_SEARCH_WINDOW, true).toBool();
} // GetQuickSearchShowSearchWindow

// find out readonly file overwrite mode
const QString cSettings::GetReadonlyFileOverwrite() const
{
	return qsSettings.value(qsOPERATIONS__ + qsREADONLY_FILE_OVERWRITE, qsASK).toString();
} // GetReadonlyFileOverwrite

// list of saved find settings for find files dialog
const QStringList cSettings::GetSavedFinds()
{
	QStringList qslSavedFinds;

	qsSettings.beginGroup(qsFIND_FILES__);
	qslSavedFinds = qsSettings.childGroups();
	qsSettings.endGroup();

	return qslSavedFinds;
} // GetSavedFinds

// save settings on quit option
const bool cSettings::GetSaveSettingsOnQuit() const
{
	return qsSettings.value(qsDISPLAY__SAVE_SETTINGS_ON_QUIT, true).toBool();
} // GetSaveSettingsOnQuit

// selection mode of files and directories
const QString cSettings::GetSelectionMode() const
{
	return qsSettings.value(qsDISPLAY__SELECTION_MODE, qsONLY_FILES).toString();
} // GetSelectionMode

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
				if (qsShortcut == qsSHORTCUT__MAIN_MENU__FILE__PACK) {
					return "Alt+F5";
				} // if
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
				if (qsShortcut == qsSHORTCUT__MAIN_MENU__COMMANDS__HISTORY_DIRECTORY_LIST) {
					return "Alt+Down";
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
				if (qsShortcut == qsSHORTCUT__PANELS__DIRECTORY_VIEW__HISTORY_BACK) {
					return "Alt+Left";
				} // if
				if (qsShortcut == qsSHORTCUT__PANELS__DIRECTORY_VIEW__HISTORY_FRONT) {
					return "Alt+Right";
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
const bool cSettings::GetShowBracketsAroundDirectoryName() const
{
	return qsSettings.value(qsDISPLAY__SHOW_BRACKETS_AROUND_DIRECTORY_NAME, true).toBool();
} // GetShowBracketsAroundDirectoryName

// get directory view header show flag
const bool cSettings::GetShowDirectoryViewHeader() const
{
	return qsSettings.value(qsDISPLAY__SHOW_DIRECTORY_VIEW_HEADER, true).toBool();
} // GetShowDirectoryViewHeader

// show drive letter in tab
const bool cSettings::GetShowDriveLetter() const
{
	return qsSettings.value(qsTABS__SHOW_DRIVE_LETTER).toBool();
} // GetShowDriveLetter

// show hidden files in dir view flag
const bool cSettings::GetShowHiddenFiles() const
{
	return qsSettings.value(qsDISPLAY__SHOW_HIDDEN_FILES).toBool();
} // GetShowHiddenFiles

// show system files in dir view flag
const bool cSettings::GetShowSystemFiles() const
{
	return qsSettings.value(qsDISPLAY__SHOW_SYSTEM_FILES).toBool();
} // GetShowSystemFiles

// show tab bar with only one tab flag
const bool cSettings::GetShowTabBarWithOnlyOneTab() const
{
	return qsSettings.value(qsTABS__SHOW_TAB_BAR_WITH_ONLY_ONE_TAB).toBool();
} // GetShowTabBarWithOnlyOneTab

// get some information about tab
const cSettings::sTabInfo cSettings::GetTabInfo(const ePosition &epPosition, const QString &qsIndex)
{
	int iCount, iI;
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

	// history
	stiTabInfo.shHistory.iPosition = qsSettings.value(qsHISTORY + '/' + qsPOSITION).toInt();
	iCount = qsSettings.beginReadArray(qsHISTORY);
	for (iI = 0; iI < iCount; iI++) {
		sLastPath slpLastPath;

		qsSettings.setArrayIndex(iI);

		slpLastPath.qsShow = qsSettings.value(qsSHOW).toString();
		slpLastPath.qsPath = qsSettings.value(qsPATH).toString();

		stiTabInfo.shHistory.qlLastPaths.append(slpLastPath);
	} // for
	qsSettings.endArray();

	qsSettings.endGroup();

	return stiTabInfo;
} // GetTabInfo

// get tab list for left or right panel
const QStringList cSettings::GetTabs(const ePosition &epPosition)
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

// treat archives like directories
const bool cSettings::GetTreatArchivesLikeDirectories() const
{
	return qsSettings.value(qsPLUGINS__PACKER__SETTINGS__ + qsTREAT_ARCHIVES_LIKE_DIRECTORIES, true).toBool();
} // GetTreatArchivesLikeDirectories

// viewer type
const QString cSettings::GetViewerType() const
{
	return qsSettings.value(qsOPERATIONS__ + qsVIEWER_TYPE, qsINTERNAL).toString();
} // GetViewerType

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

// remove column set
const void cSettings::RemoveColumnSet(const QString &qsColumnSet)
{
	qsSettings.remove(qsCOLUMN_SET__ + qsColumnSet);
} // RemoveColumnSet

// remove find settings
const void cSettings::RemoveFindSettings(const QString &qsFindSettings)
{
	qsSettings.remove(qsFIND_FILES__ + qsFindSettings);
} // RemoveFindSettings

// restore old application's settings
const void cSettings::RestoreSettings(QMap <QString, QString> &qmSettings)
{
	qsSettings.clear();

	QMapIterator<QString, QString> qmiSettings(qmSettings);
	while (qmiSettings.hasNext()) {
		qmiSettings.next();
		qsSettings.setValue(qmiSettings.key(), qmiSettings.value());
	} // while
} // RestoreSettings

// delete non empty directory
const void cSettings::SetAskToDeleteNonEmptyDirectory(const bool &bAsk)
{
	qsSettings.setValue(qsOPERATIONS__ + qsASK_TO_DELETE_NON_EMPTY_DIRECTORY, bAsk);
} // SetAskToDeleteNonEmptyDirectory

// case sensitive sorting
const void cSettings::SetCaseSensitiveSorting(const bool &bCaseSensitive)
{
	qsSettings.setValue(qsDISPLAY__CASE_SENSITIVE_SORTING, bCaseSensitive);
} // SetCaseSensitiveSorting

// close tab on double click
const void cSettings::SetCloseTabOnDoubleClick(const bool &bClose)
{
	qsSettings.setValue(qsTABS__CLOSE_TAB_ON_DOUBLE_CLICK, bClose);
} // SetCloseTabOnDoubleClick

// save combo box's history
const void cSettings::SetComboBoxHistory(const eHistoryType &ehtHistory, const QComboBox *qcbComboBox)
{
	int iI;

	qsSettings.beginGroup(qsHISTORY);
	switch (ehtHistory) {
		case CommandLineHistory:
			qsSettings.beginGroup(qsCOMMAND_LINE);
			break;
		case FileOperationDestination:
			qsSettings.beginGroup(qsFILE_OPERATION_DESTINATION);
			break;
		case FileOperationFilter:
			qsSettings.beginGroup(qsFILE_OPERATION_FILTER);
			break;
		case FindFilesSearchFor:
			qsSettings.beginGroup(qsFIND_FILES_SEARCH_FOR);
			break;
		case FindFilesSearchIn:
			qsSettings.beginGroup(qsFIND_FILES_SEARCH_IN);
			break;
		case PackFilesDestination:
			qsSettings.beginGroup(qsPACK_FILES_DESTINATION);
			break;
		case SelectFilesFilter:
			qsSettings.beginGroup(qsSELECT_FILES_FILTER);
			break;
		case UnpackFilesFilter:
			qsSettings.beginGroup(qsUNPACK_FILES_FILTER);
			break;
		case UnpackFilesDestination:
			qsSettings.beginGroup(qsUNPACK_FILES_DESTINATION);
	} // switch

	qsSettings.remove("");

	for (iI = 0; iI < qcbComboBox->count(); iI++) {
		qsSettings.setValue(QVariant(iI + 1).toString(), qcbComboBox->itemText(iI));
	} // for

	qsSettings.endGroup();
	qsSettings.endGroup();
} // SetComboBoxHistory

// confirm close of all tabs
const void cSettings::SetConfirmCloseOfAllTabs(const bool &bClose)
{
	qsSettings.setValue(qsTABS__CONFIRM_CLOSE_OF_ALL_TABS, bClose);
} // SetConfirmCloseOfAllTabs

// copy/move buffer size
const void cSettings::SetCopyMoveBufferSize(const int &iSize)
{
	qsSettings.setValue(qsOPERATIONS__ + qsBUFFER_SIZE, iSize);
} // SetCopyMoveBufferSize

// date/time display format
const void cSettings::SetDateTimeDisplay(const QString &qsDateTime)
{
	qsSettings.setValue(qsDISPLAY__DATE_TIME_DISPLAY, qsDateTime);
} // SetDateTimeDisplay

#ifdef Q_WS_WIN
// delete files to recycle bin
const void cSettings::SetDeleteToRecycleBin(const bool &bRecycleBin)
{
	qsSettings.setValue(qsOPERATIONS__ + qsDELETE_TO_RECYCLE_BIN, bRecycleBin);
} // SetDeleteToRecycleBin
#endif

// external editor
const void cSettings::SetExternalEditor(const QString &qsEditor)
{
	qsSettings.setValue(qsOPERATIONS__ + qsEXTERNAL_EDITOR, qsEditor);
} // SetExternalEditor

// external viewer
const void cSettings::SetExternalViewer(const QString &qsViewer)
{
	qsSettings.setValue(qsOPERATIONS__ + qsEXTERNAL_VIEWER, qsViewer);
} // SetExternalViewer

// set favourite directories
const void cSettings::SetFavouriteDirectories(QList<QPair<QString, cSettings::sFavouriteDirectory> > qlFavouriteDirectories)
{
	qsSettings.beginGroup(qsOTHERS__FAVOURITE_DIRECTORIES);

	// clear current favourite directories
	qsSettings.remove("");

	// set new one
	CreateFavouriteDirectories(qlFavouriteDirectories);

	qsSettings.endGroup();
} // SetFavouriteDirectories

// set default overwrite behaviour
const void cSettings::SetFileOverwrite(const QString &qsMode)
{
	qsSettings.setValue(qsOPERATIONS__ + qsFILE_OVERWRITE, qsMode);
} // SetFileOverwrite

// set file size mode
const void cSettings::SetFileSizeIn(const QString &qsSize)
{
	qsSettings.setValue(qsDISPLAY__FILE_SIZE_IN, qsSize);
} // SetFileSizeIn

// save find settings for find files dialog
const void cSettings::SetFindSettings(const QString &qsName, const sFindSettings &sfsFindSettings)
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
const void cSettings::SetListerFont(const QFont &qfFont)
{
	qsSettings.setValue(qsPLUGINS__LISTER__SETTINGS__ + qsFONT, qfFont.toString());
} // SetListerFont

// set default lister settings
const void cSettings::SetListerSettings(const sLister &slLister)
{
	qsSettings.beginGroup(qsPLUGINS__LISTER__SETTINGS__);
	qsSettings.setValue(qsCHAR_SET, slLister.qsCharSet);
	qsSettings.setValue(qsWRAP_TEXT, slLister.bWrapText);
	qsSettings.setValue(qsFIT_IMAGE_TO_WINDOW, slLister.bFitImageToWindow);
	qsSettings.endGroup();
} // SetListerSettings

// set maximum items in directory history list
const void cSettings::SetMaximumHistoryDirectoryListSize(const int &iSize)
{
	qsSettings.setValue(qsMISCELLANEOUS__MAXIMUM_HISTORY_DIRECTORY_LIST_SIZE, iSize);
} // SetMaximumHistoryDirectoryListSize

// new tab by shortcut in foreground
const void cSettings::SetNewTabByShortcutInForeground(const bool &bForeground)
{
	qsSettings.setValue(qsTABS__NEW_TAB_BY_SHORTCUT_IN_FOREGROUND, bForeground);
} // SetNewTabByShortcutInForeground

// open new tab next to current tab
const void cSettings::SetOpenNewTabNextToCurrentTab(const bool &bNextTo)
{
	qsSettings.setValue(qsTABS__OPEN_NEW_TAB_NEXT_TO_CURRENT_TAB, bNextTo);
} // SetOpenNewTabNextToCurrentTab

// default packer plugin to use for packing files
const void cSettings::SetPackerDefaultPlugin(const cSettings::sDefaultPackerPlugin &sdppPlugin)
{
	qsSettings.setValue(qsPLUGINS__PACKER__SETTINGS__ + qsDEFAULT_PACKER_PLUGIN, sdppPlugin.qsPlugin + ';' + sdppPlugin.qsExtension);
} // SetPackerDefaultPlugin

// pack including subdirectories
const void cSettings::SetPackerIncludingSubdirectories(const bool &bIncludingSubdirectories)
{
	qsSettings.setValue(qsPLUGINS__PACKER__SETTINGS__ + qsINCLUDING_SUBDIRECTORIES, bIncludingSubdirectories);
} // SetPackerIncludingSubdirectories

// delete source files when packing
const void cSettings::SetPackerMoveToArchive(const bool &bMoveToArchive)
{
	qsSettings.setValue(qsPLUGINS__PACKER__SETTINGS__ + qsMOVE_TO_ARCHIVE, bMoveToArchive);
} // SetPackerMoveToArchive

// pack each source file or directory to one archive
const void cSettings::SetPackerOneArchivePerFileOrDirectory(const bool &bOneArchivePerFileOrDirectory)
{
	qsSettings.setValue(qsPLUGINS__PACKER__SETTINGS__ + qsONE_ARCHIVE_PER_FILE_OR_DIRECTORY, bOneArchivePerFileOrDirectory);
} // SetPackerOneArchivePerFileOrDirectory

// pack source files with full path
const void cSettings::SetPackerPackPathNames(const bool &bPackPathNames)
{
	qsSettings.setValue(qsPLUGINS__PACKER__SETTINGS__ + qsPACK_PATH_NAMES, bPackPathNames);
} // SetPackerPackPathNames

// plugin date/time display format
const void cSettings::SetPluginDateTimeDisplay(const QString &qsDateTime)
{
	qsSettings.setValue(qsPLUGINS__DATE_TIME_DISPLAY, qsDateTime);
} // SetPluginDateTimeDisplay

// write plugins into settings file
const void cSettings::SetPlugins(const ePlugin &epPlugin, const QList<sPlugin> qlPlugins)
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
		const sPlugin *spPlugin;

		qsSettings.setArrayIndex(iI);
		spPlugin = &qlPlugins.at(iI);

		qsKey = qsNAME;
		qsValue = spPlugin->qsName;
		qsSettings.setValue(qsKey, qsValue);
		if (epPlugin == PackerPlugins) {
			qsKey = qsEXTENSIONS;
			qsValue = spPlugin->qsExtensions;
			qsSettings.setValue(qsKey, qsValue);
		} // if
		qsKey = qsENABLED;
		if (spPlugin->bEnabled) {
			qsValue = qsTRUE;
		} else {
			qsValue = qsFALSE;
		} // if else
		qsSettings.setValue(qsKey, qsValue);
	} // for

	qsSettings.endArray();
} // SetPlugins

///< plugin time display format
const void cSettings::SetPluginTimeDisplay(const QString &qsTime)
{
	qsSettings.setValue(qsPLUGINS__TIME_DISPLAY, qsTime);
} // SetPluginTimeDisplay

// alt key activity in quick search
const void cSettings::SetQuickSearchAlt(const bool &bAlt)
{
	qsSettings.setValue(qsQUICK_SEARCH__ALT, bAlt);
} // SetQuickSearchAlt

// ctrl key activity in quick search
const void cSettings::SetQuickSearchCtrl(const bool &bCtrl)
{
	qsSettings.setValue(qsQUICK_SEARCH__CTRL, bCtrl);
} // SetQuickSearchCtrl

// enabled/disabled quick search
const void cSettings::SetQuickSearchEnabled(const bool &bEnable)
{
	qsSettings.setValue(qsQUICK_SEARCH__ENABLED, bEnable);
} // SetQuickSearchEnabled

// quick search window visibility
const void cSettings::SetQuickSearchShowSearchWindow(const bool &bShow)
{
	qsSettings.setValue(qsQUICK_SEARCH__SHOW_SEARCH_WINDOW, bShow);
} // SetQuickSearchShowSearchWindow

// set default readonly overwrite behaviour
const void cSettings::SetReadonlyFileOverwrite(const QString &qsMode)
{
	qsSettings.setValue(qsOPERATIONS__ + qsREADONLY_FILE_OVERWRITE, qsMode);
} // SetReadonlyFileOverwrite

// save settings on quit option
const void cSettings::SetSaveSettingsOnQuit(const bool &bSave)
{
	qsSettings.setValue(qsDISPLAY__SAVE_SETTINGS_ON_QUIT, bSave);
} // SetSaveSettingsOnQuit

// set selection mode of files and directories
const void cSettings::SetSelectionMode(const QString &qsMode)
{
	qsSettings.setValue(qsDISPLAY__SELECTION_MODE, qsMode);
} // SetSelectionMode

// set shortcut
const void cSettings::SetShortcut(const eShortcutCategory &escCategory, const QString &qsName, const QString &qsShortcut)
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
const void cSettings::SetShowBracketsAroundDirectoryName(const bool &bShowBrackets)
{
	qsSettings.setValue(qsDISPLAY__SHOW_BRACKETS_AROUND_DIRECTORY_NAME, bShowBrackets);
} // SetShowBracketsAroundDirectoryName

// set directory view header show flag
const void cSettings::SetShowDirectoryViewHeader(const bool &bShow)
{
	qsSettings.setValue(qsDISPLAY__SHOW_DIRECTORY_VIEW_HEADER, bShow);
} // SetShowDirectoryViewHeader

// show drive letter in tab
const void cSettings::SetShowDriveLetter(const bool &bShow)
{
	qsSettings.setValue(qsTABS__SHOW_DRIVE_LETTER, bShow);
} // SetShowDriveLetter

// set show hidden files in dir view
const void cSettings::SetShowHiddenFiles(const bool &bShowHidden)
{
	qsSettings.setValue(qsDISPLAY__SHOW_HIDDEN_FILES, bShowHidden);
} // SetShowHiddenFiles

// set show system files in dir view
const void cSettings::SetShowSystemFiles(const bool &bShowSystem)
{
	qsSettings.setValue(qsDISPLAY__SHOW_SYSTEM_FILES, bShowSystem);
} // SetShowSystemFiles

// show tab bar with only one tab
const void cSettings::SetShowTabBarWithOnlyOneTab(const bool &bShow)
{
	qsSettings.setValue(qsTABS__SHOW_TAB_BAR_WITH_ONLY_ONE_TAB, bShow);
} // SetShowTabBarWithOnlyOneTab

// save tab settings
const void cSettings::SetTabs(const ePosition &epPosition, const QList<sTabInfo> &qlTabs)
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
const void cSettings::SetTreatArchivesLikeDirectories(const bool &bLikeDirectories)
{
	qsSettings.setValue(qsPLUGINS__PACKER__SETTINGS__ + qsTREAT_ARCHIVES_LIKE_DIRECTORIES, bLikeDirectories);
} // SetTreatArchivesLikeDirectories

// viewer type
const void cSettings::SetViewerType(const QString &qsType)
{
	qsSettings.setValue(qsOPERATIONS__ + qsVIEWER_TYPE, qsType);
} // SetViewerType

// set startup main window state
const void cSettings::SetWindowState(const sMainWindowState &smwsState)
{
	qsSettings.beginGroup(qsMAIN_WINDOW);
	if (smwsState.qsWindowState == qsNORMAL) {
		qsSettings.setValue(qsHEIGHT, smwsState.iHeight);
		qsSettings.setValue(qsWIDTH, smwsState.iWidth);
	} // if
	qsSettings.setValue(qsWINDOW_STATE, smwsState.qsWindowState);
	qsSettings.endGroup();
} // SetWindowState
