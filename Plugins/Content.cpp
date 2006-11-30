#include "Content.h"

#include "Settings.h"
#include <QLibrary>
#include "Plugins/ContPlug.h"
#include <QStringList>
#include <QFileInfo>

// loads content plugins
void cContent::Load()
{
	int iI;
	QStringList qslKeys;

	// get files
	qsSettings->beginGroup(qsPLUGINS__CONTENT);
	qslKeys = qsSettings->childKeys();
	qsSettings->endGroup();

	// enumerate plugins
	for (iI = 0; iI < qslKeys.count(); iI++) {
		int iField;
		QFileInfo qfiFile;
		QLibrary qlLibrary;
		sPluginInfo spiPluginInfo;
		tContentGetSupportedField tcgsfContentGetSupportedField;

		// load plugin
		qlLibrary.setFileName(qslKeys.at(iI));
		qlLibrary.load();
		tcgsfContentGetSupportedField = (tContentGetSupportedField)qlLibrary.resolve("ContentGetSupportedField");

		// fill plugin properties
		spiPluginInfo.tcgvContentGetValue = (tContentGetValue)qlLibrary.resolve("ContentGetValue");
		// get fields
		iField = 0;
		while(true) {
			char cFieldName[MAX_PATH], cUnits[MAX_PATH];
			int iResult;

			iResult = tcgsfContentGetSupportedField(iField, cFieldName, cUnits, MAX_PATH);
			if (iResult == ft_nomorefields) {
				break;
			} else {
				sField sfField;

				sfField.qsName = cFieldName;
				sfField.qsUnits = cUnits;
				spiPluginInfo.qlFields.append(sfField);
			} // if else

			iField++;
		} // while
		// add new plugin
		qfiFile.setFile(qslKeys.at(iI));
		qhPlugins.insert(qfiFile.fileName(), spiPluginInfo);
	} // for
} // Load

// checks if plugin qsName has been succesfully loaded
bool cContent::Loaded(QString qsName)
{
	return qhPlugins.contains(qsName);
} // Loaded

// sets main settings "file"
void cContent::SetSettings(QSettings *qsSettings)
{
	this->qsSettings = qsSettings;
} // SetSettings