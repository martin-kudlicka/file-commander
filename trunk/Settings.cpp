#include "Settings.h"

#include <QStringList>

// general
const QString qsCOLUMN_SET = "ColumnSet";
const QString qsPATH = "Path";
const QString qsPLUGIN = "Plugin";
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
void cSettings::CreateColumnSet(const QString qsColumnSet, const QList<sColumn> qlColumns)
{
	int iI;

	qsSettings.beginGroup(qsCOLUMN_SET + "/" + qsColumnSet);
	for (iI = 0; iI < qlColumns.count(); iI++) {
		qsSettings.beginGroup(QString("%1").arg(iI));
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
	scColumn.qsName = qsICON;
	scColumn.qsPlugin = qsNO;
	qlColumns.append(scColumn);
	// name
	scColumn.qsName = qsNAME;
	scColumn.qsPlugin = qsNO;
	qlColumns.append(scColumn);
	// extension
	scColumn.qsName = qsEXTENSION;
	scColumn.qsPlugin = qsNO;
	qlColumns.append(scColumn);
	// date
	scColumn.qsName = qsDATE;
	scColumn.qsPlugin = qsNO;
	qlColumns.append(scColumn);

	CreateColumnSet(qsFULL, qlColumns);
} // CreateDefaultColumnSet

// create new tab in settings file
void cSettings::CreateTab(const ePosition epPosition, const uint uiIndex, const QString qsColumnSet, const QString qsPath)
{
	if (epPosition == PositionLeft) {
		qsSettings.beginGroup(QString("%1%2").arg(qsLEFT_PANEL__TABS__).arg(uiIndex));
	} else {
		qsSettings.beginGroup(QString("%1%2").arg(qsRIGHT_PANEL__TABS__).arg(uiIndex));
	} // if else

	QString a = qsSettings.group();
	qsSettings.setValue(qsCOLUMN_SET, qsColumnSet);
	qsSettings.setValue(qsPATH, qsPath);

	qsSettings.endGroup();
} // CreateTab

// get some information about column
cSettings::sColumn cSettings::GetColumnInfo(const QString qsColumnSet, const QString qsColumn)
{
	sColumn scColumn;

	qsSettings.beginGroup(qsCOLUMN_SET__ + qsColumnSet + "/" + qsColumn);
	scColumn.qsName = qsSettings.value(qsNAME).toString();
	scColumn.qsPlugin = qsSettings.value(qsPLUGIN).toString();
	qsSettings.endGroup();

	return scColumn;
} // GetColumnInfo

// get column names in column set
QStringList cSettings::GetColumnsInSet(const QString qsColumnSet)
{
	QStringList qslColumns;

	qsSettings.beginGroup(qsCOLUMN_SET__ + qsColumnSet);
	qslColumns = qsSettings.childGroups();
	qsSettings.endGroup();

	return qslColumns;
} // GetColumnsInSet

///< get plugin list
QStringList cSettings::GetPlugins(const ePlugin epPlugin)
{
	QStringList qslPlugins;

	switch (epPlugin) {
		case ContentPlugins:	qsSettings.beginGroup(qsPLUGINS__CONTENT);
									break;
	} // switch
	qslPlugins = qsSettings.childKeys();
	qsSettings.endGroup();

	return qslPlugins;
} // GetPlugins

///< get some information about tab
cSettings::sTabInfo cSettings::GetTabInfo(const ePosition epPosition, const QString qsIndex)
{
	sTabInfo stiTabInfo;

	if (epPosition == PositionLeft) {
		qsSettings.beginGroup(qsLEFT_PANEL__TABS__ + qsIndex);
	} else {
		qsSettings.beginGroup(qsRIGHT_PANEL__TABS__ + qsIndex);
	} // if else

	stiTabInfo.qsColumnSet = qsSettings.value(qsCOLUMN_SET).toString();
	stiTabInfo.qsPath = qsSettings.value(qsPATH).toString();

	qsSettings.endGroup();

	return stiTabInfo;
} // GetTabInfo

// get tab list for left or right panel
QStringList cSettings::GetTabs(const ePosition epPosition)
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