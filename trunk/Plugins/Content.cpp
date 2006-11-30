#include "Content.h"

#include "Settings.h"
#include <QLibrary>
#include "Plugins/ContPlug.h"
#include <QStringList>
#include <QFileInfo>

// returns plugin's value for specified column
QString cContent::GetPluginValue(const QString qsPlugin, const QString qsColumn, const QString qsFilename)
{
	// TODO GetPluginValue
	/*char cFieldValue[uiMAX_CHAR];
	int iFieldIndex, iUnitIndex;
	sPluginInfo spiPluginInfo;

	// find plugin
	spiPluginInfo = qhPlugins.value(qsPlugin);

	// search for column index
	iFieldIndex = 0;
	while(true) {
		if (spiPluginInfo.qlFields.at(iFieldIndex).qsName == qsColumn) {
			break;
		} // if
		iFieldIndex++;
	} // while

	// get value
	//spiPluginInfo.tcgvContentGetValue(qsFilename, iFieldIndex, iUnitIndex, cFieldValue, uiMAX_CHAR, */

	return "";
} // GetPluginValue

// loads content plugins
void cContent::Load()
{
	int iI;
	QStringList qslPlugins;

	// get plugin file list
	qslPlugins = csSettings->GetPlugins(cSettings::ContentPlugins);

	// enumerate plugins
	for (iI = 0; iI < qslPlugins.count(); iI++) {
		int iField;
		QFileInfo qfiFile;
		QLibrary qlLibrary;
		sPluginInfo spiPluginInfo;
		tContentGetSupportedField tcgsfContentGetSupportedField;

		// load plugin
		qlLibrary.setFileName(qslPlugins.at(iI));
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
		qfiFile.setFile(qslPlugins.at(iI));
		qhPlugins.insert(qfiFile.fileName(), spiPluginInfo);
	} // for
} // Load

// checks if plugin qsName has been succesfully loaded
bool cContent::Loaded(const QString qsName)
{
	return qhPlugins.contains(qsName);
} // Loaded