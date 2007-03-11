#include "Settings.h"

#include <QStringList>
#include "FileOperation/FileRoutine.h"

// general
const QChar qcPATH_SEPARATOR = '|';	///< some substitution needed beacuse '/' is group separator in QSettings
const QString qsASCENDING = "ascending";
const QString qsCHAR_SET = "CharSet";
const QString qsCOLUMN_SET = "ColumnSet";
const QString qsDESCENDING = "descending";
const QString qsDISABLED = "Disabled";
const QString qsDRIVE = "Drive";
const QString qsENABLED = "Enabled";
const QString qsFILE_OVERWRITE = "FileOverwrite";
const QString qsFIT_IMAGE_TO_WINDOW = "FitImageToWindow";
const QString qsHEIGHT = "Height";
const QString qsPATH = "Path";
const QString qsPLUGIN = "Plugin";
const QString qsREADONLY_FILE_OVERWRITE = "ReadonlyFileOverwrite";
const QString qsSORT_ORDER = "SortOrder";
const QString qsSORTED_COLUMN = "SortColumn";
const QString qsUNIT = "Unit";
const QString qsWIDTH = "Width";
const QString qsWINDOW_STATE = "WindowState";
const QString qsWRAP_TEXT = "WrapText";
// settings file
// ColumnSet/
const QString qsCOLUMN_SET__ = qsCOLUMN_SET + "/";
// Display
const QString qsDISPLAY__FILE_SIZE_IN = "Display/FileSizeIn";
const QString qsDISPLAY__SHOW_HIDDEN_FILES = "Display/ShowHiddenFiles";
const QString qsDISPLAY__SHOW_SYSTEM_FILES = "Display/ShowSystemFiles";
// LeftPanel
// LeftPanel/Tabs/
const QString qsLEFT_PANEL__TABS__ = "LeftPanel/Tabs/";
// MainWindow/
const QString qsMAIN_WINDOW = "MainWindow";
// Others/
const QString qsOTHERS__ = "Others/";
// RightPanel
// RightPanel/Tabs/
const QString qsRIGHT_PANEL__TABS__ = "RightPanel/Tabs/";
// Plugins
const QString qsPLUGINS__LISTER__SETTINGS = "Plugins/Lister/Settings";
const QString qsPLUGINS__TIME_DISPLAY = "Plugins/TimeDisplay";
// Plugins/Content
const QString qsPLUGINS__CONTENT = "Plugins/Content";
const QString qsPLUGINS__LISTER = "Plugins/Lister";

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

	qsSettings.beginGroup(qsCOLUMN_SET__ + qsColumnSet);
	qsSettings.remove("");
	for (iI = 0; iI < qlColumns.count(); iI++) {
		qsSettings.beginGroup(QVariant(iI).toString());
		qsSettings.setValue(qsIDENTIFIER, qlColumns.at(iI).qsIdentifier);
		qsSettings.setValue(qsNAME, qlColumns.at(iI).qsName);
		qsSettings.setValue(qsPLUGIN, qlColumns.at(iI).qsPlugin);
		qsSettings.setValue(qsUNIT, qlColumns.at(iI).qsUnit);
		qsSettings.setValue(qsWIDTH, qlColumns.at(iI).iWidth);
		qsSettings.endGroup();
	} // for
	qsSettings.endGroup();
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
	scColumn.qsIdentifier = qsDATE;
	scColumn.qsName = tr("Date");
	scColumn.qsPlugin = qsNO;
	scColumn.iWidth = 50;
	qlColumns.append(scColumn);

	CreateColumnSet(qsFULL, qlColumns);
} // CreateDefaultColumnSet

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

// find out file overwrite mode
QString cSettings::GetFileOverwrite()
{
	return qsSettings.value(qsOTHERS__ + qsFILE_OVERWRITE, qsASK).toString();
} // GetFileOverwrite

// unit for files size
QString cSettings::GetFileSizeIn()
{
	return qsSettings.value(qsDISPLAY__FILE_SIZE_IN, qsDYNAMIC).toString();
} // GetFileSizeIn

// lister settings
cSettings::sLister cSettings::GetListerSettings()
{
	sLister slLister;

	qsSettings.beginGroup(qsPLUGINS__LISTER__SETTINGS);
	slLister.qsCharSet = qsSettings.value(qsCHAR_SET, qsANSI).toString();
	slLister.qsWrapText = qsSettings.value(qsWRAP_TEXT, qsTRUE).toString();
	slLister.qsFitImageToWindow = qsSettings.value(qsFIT_IMAGE_TO_WINDOW, qsFALSE).toString();
	qsSettings.endGroup();

	return slLister;
} // GetListerSettings

///< get plugin list
QList<cSettings::sPlugin> cSettings::GetPlugins(const ePlugin &epPlugin)
{
	int iI;
	QList<sPlugin> qlPlugins;
	QStringList qslPlugins;

	switch (epPlugin) {
		// TODO GetPlugins - other plugin types
		case ContentPlugins:	qsSettings.beginGroup(qsPLUGINS__CONTENT);
									break;
		case ListerPlugins:	qsSettings.beginGroup(qsPLUGINS__LISTER);
	} // switch
	qslPlugins = qsSettings.childKeys();

	for (iI = 0; iI < qslPlugins.count(); iI++) {
		sPlugin spPlugin;

		spPlugin.qsName = qslPlugins.at(iI);
		spPlugin.qsName.replace(qcPATH_SEPARATOR, '/');
		if (qsSettings.value(qslPlugins.at(iI)).toString() == qsENABLED) {
			spPlugin.bEnabled = true;
		} else {
			spPlugin.bEnabled = false;
		} // if

		qlPlugins.append(spPlugin);
	} // for
	qsSettings.endGroup();

	return qlPlugins;
} // GetPlugins

// plugin time display format
QString cSettings::GetPluginTimeDisplay()
{
	return qsSettings.value(qsPLUGINS__TIME_DISPLAY, "%h:%m:%s").toString();
} // GetPluginTimeDisplay

// find out readonly file overwrite mode
QString cSettings::GetReadonlyFileOverwrite()
{
	return qsSettings.value(qsOTHERS__ + qsREADONLY_FILE_OVERWRITE, qsASK).toString();
} // GetReadonlyFileOverwrite

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

// find out value for specified key
QString cSettings::GetValue(const eKey &ekKey)
{
	QString qsResult;

	switch (ekKey) {
		case ShowHiddenFiles:	qsResult = qsSettings.value(qsDISPLAY__SHOW_HIDDEN_FILES, qsFALSE).toString();
										break;
		case ShowSystemFiles:	qsResult = qsSettings.value(qsDISPLAY__SHOW_SYSTEM_FILES, qsFALSE).toString();
										break;
	} // switch

	return qsResult;
} // GetValue

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

// set default overwrite behaviour
void cSettings::SetFileOverwrite(const QString &qsMode)
{
	qsSettings.setValue(qsOTHERS__ + qsFILE_OVERWRITE, qsMode);
} // SetFileOverwrite

// set file size mode
void cSettings::SetFileSizeIn(const QString &qsSize)
{
	qsSettings.setValue(qsDISPLAY__FILE_SIZE_IN, qsSize);
} // SetFileSizeIn

// set default lister settings
void cSettings::SetListerSettings(const sLister &slLister)
{
	qsSettings.beginGroup(qsPLUGINS__LISTER__SETTINGS);
	qsSettings.setValue(qsCHAR_SET, slLister.qsCharSet);
	qsSettings.setValue(qsWRAP_TEXT, slLister.qsWrapText);
	qsSettings.setValue(qsFIT_IMAGE_TO_WINDOW, slLister.qsFitImageToWindow);
	qsSettings.endGroup();
} // SetListerSettings

// write plugins into settings file
void cSettings::SetPlugins(const ePlugin &epPlugin, const QList<sPlugin> &qlPlugins)
{
	int iI;

	switch (epPlugin) {
		// TODO GetPlugins - other plugin types
		case ContentPlugins:	qsSettings.beginGroup(qsPLUGINS__CONTENT);
									break;
		case ListerPlugins:	qsSettings.beginGroup(qsPLUGINS__LISTER);
	} // switch

	qsSettings.remove("");

	for (iI = 0; iI < qlPlugins.count(); iI++) {
		QString qsKey, qsValue;

		qsKey = qlPlugins.at(iI).qsName;
		qsKey.replace('/', qcPATH_SEPARATOR);
		if (qlPlugins.at(iI).bEnabled) {
			qsValue = qsENABLED;
		} else {
			qsValue = qsDISABLED;
		} // if else

		qsSettings.setValue(qsKey, qsValue);
	} // for

	qsSettings.endGroup();
} // SetPlugins

///< plugin time display format
void cSettings::SetPluginTimeDisplay(const QString &qsTime)
{
	qsSettings.setValue(qsPLUGINS__TIME_DISPLAY, qsTime);
} // SetPluginTimeDisplay

// set default readonly overwrite behaviour
void cSettings::SetReadonlyFileOverwrite(const QString &qsMode)
{
	qsSettings.setValue(qsOTHERS__ + qsREADONLY_FILE_OVERWRITE, qsMode);
} // SetReadonlyFileOverwrite

// set key - value pair
void cSettings::SetValue(const eKey &ekKey, const QString &qsValue)
{
	switch (ekKey) {
		case ShowHiddenFiles:	qsSettings.setValue(qsDISPLAY__SHOW_HIDDEN_FILES, qsValue);
										break;
		case ShowSystemFiles:	qsSettings.setValue(qsDISPLAY__SHOW_SYSTEM_FILES, qsValue);
										break;
	} // switch
} // SetValue

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