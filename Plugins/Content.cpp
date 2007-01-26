#include "Content.h"

#include <QLibrary>
#include "Plugins/ContPlug.h"
#include <QStringList>
#include <QFileInfo>

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

// returns plugin's value for specified column
QString cContent::GetPluginValue(const QString &qsFilename, const QString &qsPlugin, const QString &qsColumn, const QString &qsUnit)
{
	char cFieldValue[uiMAX_CHAR];
	int iFieldIndex, iType, iUnitIndex;
	QString qsFieldValue;

	iFieldIndex = GetFieldIndex(qsPlugin, qsColumn);
	iUnitIndex = qhPlugins.value(qsPlugin).qlFields.at(iFieldIndex).qsUnits.toInt();

	// get value
	iType = qhPlugins.value(qsPlugin).tcgvContentGetValue(qsFilename.toAscii().data(), iFieldIndex, iUnitIndex, cFieldValue, uiMAX_CHAR, 0);
	qsFieldValue = ValidateFieldValue(cFieldValue, iType);

	return qsFieldValue;
} // GetPluginValue

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
			QFileInfo qfiFile;
			QLibrary qlLibrary;
			sPluginInfo spiPluginInfo;
			tContentGetSupportedField tcgsfContentGetSupportedField;

			// load plugin
			qlLibrary.setFileName(qlPlugins.at(iI).qsName);
			qlLibrary.load();
			tcgsfContentGetSupportedField = (tContentGetSupportedField)qlLibrary.resolve("ContentGetSupportedField");

			// fill plugin properties
			spiPluginInfo.tcgvContentGetValue = (tContentGetValue)qlLibrary.resolve("ContentGetValue");
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
			qfiFile.setFile(qlPlugins.at(iI).qsName);
			qhPlugins.insert(qfiFile.fileName(), spiPluginInfo);
		} // if
	} // for
} // Load

// checks if plugin qsName has been succesfully loaded
bool cContent::Loaded(const QString &qsName)
{
	return qhPlugins.contains(qsName);
} // Loaded

// "converts" plugin's returned value to QString
QString cContent::ValidateFieldValue(const char *cFieldValue, const int &iType)
{
	// TODO ValidateFieldValue other types
	switch (iType) {
		case ft_numeric_32:	return QString("%1").arg(static_cast<int>(*cFieldValue));
		default:					return "";
	} // switch
} // ValidateFieldValue