#include "Settings.h"

#include <QStringList>
#include "FileOperation/FileRoutine.h"

// general
const QChar qcPATH_SEPARATOR = '|';	///< some substitution needed beacuse '/' is group separator in QSettings
const QString qsASCENDING = "ascending";
const QString qsCOLUMN_SET = "ColumnSet";
const QString qsDESCENDING = "descending";
const QString qsDISABLED = "Disabled";
const QString qsDRIVE = "Drive";
const QString qsENABLED = "Enabled";
const QString qsFILE_OVERWRITE = "FileOverwrite";
const QString qsPATH = "Path";
const QString qsPLUGIN = "Plugin";
const QString qsSORT_ORDER = "SortOrder";
const QString qsSORTED_COLUMN = "SortColumn";
const QString qsUNIT = "Unit";
const QString qsWIDTH = "Width";
// settings file
// ColumnSet/
const QString qsCOLUMN_SET__ = qsCOLUMN_SET + "/";
// Display
const QString qsDISPLAY__SHOW_HIDDEN_FILES = "Display/ShowHiddenFiles";
const QString qsDISPLAY__SHOW_SYSTEM_FILES = "Display/ShowSystemFiles";
// LeftPanel
// LeftPanel/Tabs/
const QString qsLEFT_PANEL__TABS__ = "LeftPanel/Tabs/";
// RightPanel
// RightPanel/Tabs/
const QString qsRIGHT_PANEL__TABS__ = "RightPanel/Tabs/";
// Plugins
// Plugins/Content
const QString qsPLUGINS__CONTENT = "Plugins/Content";
// Others/
const QString qsOTHERS__ = "Others/";

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
		qsSettings.beginGroup(QString("%1").arg(iI));
		qsSettings.setValue(qsIDENTIFIER, qlColumns.at(iI).qsIdentifier);
		qsSettings.setValue(qsNAME, qlColumns.at(iI).qsName);
		qsSettings.setValue(qsPLUGIN, qlColumns.at(iI).qsPlugin);
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

// write plugins into settings file
void cSettings::SetPlugins(const ePlugin &epPlugin, const QList<sPlugin> &qlPlugins)
{
	int iI;

	switch (epPlugin) {
		// TODO GetPlugins - other plugin types
		case ContentPlugins:	qsSettings.beginGroup(qsPLUGINS__CONTENT);
									break;
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