#include "Settings.h"

#include <QStringList>

// general
const QChar qcPATH_SEPARATOR = '|';	///< some substitution needed beacuse '/' is group separator in QSettings
const QString qsCOLUMN_SET = "ColumnSet";
const QString qsDRIVE = "Drive";
const QString qsENABLED = "Enabled";
const QString qsPATH = "Path";
const QString qsPLUGIN = "Plugin";
const QString qsUNIT = "Unit";
// LeftPanel
// LeftPanel/Tabs/
const QString qsLEFT_PANEL__TABS__ = "LeftPanel/Tabs/";
// RightPanel
// RightPanel/Tabs/
const QString qsRIGHT_PANEL__TABS__ = "RightPanel/Tabs/";
// ColumnSet/
const QString qsCOLUMN_SET__ = qsCOLUMN_SET + "/";
// Plugins
// Plugins/Content
const QString qsPLUGINS__CONTENT = "Plugins/Content";

// create new column set
void cSettings::CreateColumnSet(const QString &qsColumnSet, const QList<sColumn> &qlColumns)
{
	int iI;

	qsSettings.beginGroup(qsCOLUMN_SET + "/" + qsColumnSet);
	for (iI = 0; iI < qlColumns.count(); iI++) {
		qsSettings.beginGroup(QString("%1").arg(iI));
		qsSettings.setValue(qsIDENTIFIER, qlColumns.at(iI).qsIdentifier);
		qsSettings.setValue(qsNAME, qlColumns.at(iI).qsName);
		qsSettings.setValue(qsPLUGIN, qlColumns.at(iI).qsPlugin);
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
	qlColumns.append(scColumn);
	// name
	scColumn.qsIdentifier = qsNAME;
	scColumn.qsName = tr("Name");
	scColumn.qsPlugin = qsNO;
	qlColumns.append(scColumn);
	// extension
	scColumn.qsIdentifier = qsEXTENSION;
	scColumn.qsName = tr("Extension");
	scColumn.qsPlugin = qsNO;
	qlColumns.append(scColumn);
	// date
	scColumn.qsIdentifier = qsDATE;
	scColumn.qsName = tr("Date");
	scColumn.qsPlugin = qsNO;
	qlColumns.append(scColumn);

	CreateColumnSet(qsFULL, qlColumns);
} // CreateDefaultColumnSet

// create new tab in settings file
void cSettings::CreateTab(const ePosition &epPosition, const uint &uiIndex, const QString &qsColumnSet, const QString &qsPath)
{
	if (epPosition == PositionLeft) {
		qsSettings.beginGroup(QString("%1%2").arg(qsLEFT_PANEL__TABS__).arg(uiIndex));
	} else {
		qsSettings.beginGroup(QString("%1%2").arg(qsRIGHT_PANEL__TABS__).arg(uiIndex));
	} // if else

	qsSettings.setValue(qsCOLUMN_SET, qsColumnSet);
	qsSettings.setValue(qsPATH, qsPath);

	qsSettings.endGroup();
} // CreateTab

// get some information about column
cSettings::sColumn cSettings::GetColumnInfo(const QString &qsColumnSet, const QString &qsColumn)
{
	sColumn scColumn;

	qsSettings.beginGroup(qsCOLUMN_SET__ + qsColumnSet + "/" + qsColumn);
	scColumn.qsIdentifier = qsSettings.value(qsIDENTIFIER).toString();
	scColumn.qsName = qsSettings.value(qsNAME).toString();
	scColumn.qsPlugin = qsSettings.value(qsPLUGIN).toString();
	scColumn.qsUnit = qsSettings.value(qsUNIT).toString();
	qsSettings.endGroup();

	return scColumn;
} // GetColumnInfo

// get column names in column set
QStringList cSettings::GetColumnsInSet(const QString &qsColumnSet)
{
	QStringList qslColumns;

	qsSettings.beginGroup(qsCOLUMN_SET__ + qsColumnSet);
	qslColumns = qsSettings.childGroups();
	qsSettings.endGroup();

	return qslColumns;
} // GetColumnsInSet

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

	qsSettings.endGroup();

	return stiTabInfo;
} // GetTabInfo

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