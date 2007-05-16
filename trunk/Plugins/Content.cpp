#include "Plugins/Content.h"

#include <QtCore/QLibrary>
#include <QtCore/QStringList>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>

const DWORD dwPLUGIN_INTERFACE_VERSION_HI = 1;
const DWORD dwPLUGIN_INTERFACE_VERSION_LOW = 4;

// destructor
cContent::~cContent()
{
	Unload();
} // ~cContent

// constructor
cContent::cContent(cSettings *csSettings)
{
	this->csSettings = csSettings;
} // cContent

// get index of column in plugin
int cContent::GetFieldIndex(const QString &qsPlugin, const QString &qsColumn)
{
	int iI;

	for (iI = 0; iI < qhPlugins.value(qsPlugin).qlFields.count(); iI++) {
		if (qhPlugins.value(qsPlugin).qlFields.at(iI).qsName == qsColumn) {
			return iI;
		} // if
	} // for

	return -1;	// error
} // GetFieldIndex

// retrieve content plugin info
QHash<QString, cContent::sPluginInfo> cContent::GetPluginsInfo()
{
	return qhPlugins;
} // GetPluginsInfo

// returns plugin's value for specified column
QString cContent::GetPluginValue(const QString &qsFilename, const QString &qsPlugin, const QString &qsColumn, const QString &qsUnit, int *iFlag /* NULL */)
{
	char cFieldValue[uiMAX_CHAR];
	int iFieldIndex, iType, iUnitIndex;
	QString qsFieldValue;

	iFieldIndex = GetFieldIndex(qsPlugin, qsColumn);
	iUnitIndex = GetUnitIndex(qsUnit, qhPlugins.value(qsPlugin).qlFields.at(iFieldIndex).qsUnits);

	// get value
	if (iFlag == NULL) {
		iType = qhPlugins.value(qsPlugin).tcgvContentGetValue(qsFilename.toLocal8Bit().data(), iFieldIndex, iUnitIndex, cFieldValue, uiMAX_CHAR, 0);
	} else {
		iType = qhPlugins.value(qsPlugin).tcgvContentGetValue(qsFilename.toLocal8Bit().data(), iFieldIndex, iUnitIndex, cFieldValue, uiMAX_CHAR, CONTENT_DELAYIFSLOW);
		if (iType == ft_delayed) {
			iType = ft_string;
			*iFlag = ft_delayed;
		} else {
			// doesn't care what's here in iFlag
			*iFlag = ft_ondemand;
		} // if else
	} // if else
	qsFieldValue = ValidateFieldValue(cFieldValue, iType);

	return qsFieldValue;
} // GetPluginValue

// find index of unit
int cContent::GetUnitIndex(const QString &qsUnit, const QString &qsUnits)
{
	QStringList qslUnits;

	qslUnits = qsUnits.split('|');
	return qslUnits.indexOf(qsUnit);
} // GetUnitIndex

// loads content plugins
void cContent::Load()
{
	int iI;
	QList<cSettings::sPlugin> qlPlugins;

	// get plugin file list
	qlPlugins = csSettings->GetPlugins(cSettings::ContentPlugins);

	// enumerate plugins
	for (iI = 0; iI < qlPlugins.count(); iI++) {
		if (qlPlugins.at(iI).bEnabled) {
			int iField;
			QLibrary qlLibrary;
			sPluginInfo spiPluginInfo;
			tContentGetSupportedField tcgsfContentGetSupportedField;

			// load plugin
			qlLibrary.setFileName(qlPlugins.at(iI).qsName);
			qlLibrary.load();
			tcgsfContentGetSupportedField = (tContentGetSupportedField)qlLibrary.resolve("ContentGetSupportedField");

			// fill plugin properties
			spiPluginInfo.tcsdpContentSetDefaultParams = (tContentSetDefaultParams)qlLibrary.resolve("ContentSetDefaultParams");
			if (spiPluginInfo.tcsdpContentSetDefaultParams) {
				ContentDefaultParamStruct cdpsParam;
				QString qsIniFile;

				qsIniFile = QFileInfo(qlPlugins.at(iI).qsName).path() + '/' + QFileInfo(qlPlugins.at(iI).qsName).completeBaseName() + ".ini";
				cdpsParam.size = sizeof(cdpsParam);
				cdpsParam.PluginInterfaceVersionLow = dwPLUGIN_INTERFACE_VERSION_LOW;
				cdpsParam.PluginInterfaceVersionHi = dwPLUGIN_INTERFACE_VERSION_HI;
				strcpy(cdpsParam.DefaultIniName, QDir::toNativeSeparators(qsIniFile).toAscii().constData());

				spiPluginInfo.tcsdpContentSetDefaultParams(&cdpsParam);
			} // if
			spiPluginInfo.tcgvContentGetValue = (tContentGetValue)qlLibrary.resolve("ContentGetValue");
			spiPluginInfo.tcpuContentPluginUnloading = (tContentPluginUnloading)qlLibrary.resolve("ContentPluginUnloading");
			spiPluginInfo.tcsgvContentStopGetValue = (tContentStopGetValue)qlLibrary.resolve("ContentStopGetValue");

			// get fields
			iField = 0;
			while(true) {
				char cFieldName[uiMAX_CHAR], cUnits[uiMAX_CHAR];
				int iResult;

				iResult = tcgsfContentGetSupportedField(iField, cFieldName, cUnits, uiMAX_CHAR);
				if (iResult == ft_nomorefields) {
					break;
				} else {
					sField sfField;

					sfField.qsName = cFieldName;
					sfField.qsUnits = cUnits;
					sfField.iType = iResult;
					spiPluginInfo.qlFields.append(sfField);
				} // if else

				iField++;
			} // while
			// add new plugin
			qhPlugins.insert(QFileInfo(qlPlugins.at(iI).qsName).fileName(), spiPluginInfo);
		} // if
	} // for
} // Load

// checks if plugin qsName has been succesfully loaded
bool cContent::Loaded(const QString &qsName)
{
	return qhPlugins.contains(qsName);
} // Loaded

// unloads content plugins
void cContent::Unload()
{
	QHashIterator<QString, sPluginInfo> qhiPlugins(qhPlugins);

	while (qhiPlugins.hasNext()) {
		qhiPlugins.next();

		if (qhiPlugins.value().tcpuContentPluginUnloading) {
			qhiPlugins.value().tcpuContentPluginUnloading();
		} // if
	} // while

	qhPlugins.clear();
} // Unload

// "converts" plugin's returned value to QString
QString cContent::ValidateFieldValue(const char *cFieldValue, const int &iType)
{
	// TODO ValidateFieldValue other types
	QString qsValue;

	switch (iType) {
		case ft_numeric_32:
			return QVariant(*reinterpret_cast<int *>(const_cast<char *>(cFieldValue))).toString();
		case ft_numeric_64:
			return QVariant(*reinterpret_cast<__int64 *>(const_cast<char *>(cFieldValue))).toString();
		case ft_time:
			qsValue = csSettings->GetPluginTimeDisplay();
			qsValue.replace("%h", QString("%1").arg(reinterpret_cast<ptimeformat>(const_cast<char *>(cFieldValue))->wHour, 2, 10, QChar('0')));
			qsValue.replace("%m", QString("%1").arg(reinterpret_cast<ptimeformat>(const_cast<char *>(cFieldValue))->wMinute, 2, 10, QChar('0')));
			qsValue.replace("%s", QString("%1").arg(reinterpret_cast<ptimeformat>(const_cast<char *>(cFieldValue))->wSecond, 2, 10, QChar('0')));
			return qsValue;
		case ft_boolean:
			if (*cFieldValue) {
				return QT_TR_NOOP("Yes");
			} else {
				return QT_TR_NOOP("No");
			} // if else
		case ft_multiplechoice:
		case ft_string:
			return cFieldValue;
		default:
			return QString();
	} // switch
} // ValidateFieldValue
