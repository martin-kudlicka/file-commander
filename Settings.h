/// setting file accessing

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

const QString qsDATE = "Date";
const QString qsEXTENSION = "Extension";
const QString qsFULL = "Full";										///< full dir panel view (icon, filename, extension, date)
const QString qsICON = "Icon";
const QString qsIDENTIFIER = "Identifier";
const QString qsNAME = "Name";
const QString qsNO = "no";

class cSettings : private QObject
{
	public:
		/// plugin type
		enum ePlugin {
			ContentPlugins													///< content plugin type
		};
		/// choose of panel
		enum ePosition {
			PositionLeft,													///< left panel
			PositionRight													///< right panel
		};
		/// column in dir tree view
		struct sColumn {
			QString qsIdentifier;										///< column identifier
			QString qsName;												///< column name to show
			QString qsPlugin;												///< plugin filename or "no" if native
			QString qsUnit;												///< selected unit for column
		};
		/// plugin
		struct sPlugin {
			QString qsName;												///< plugin's file name
			bool bEnabled;													///< enable/disable flag
		};
		/// tab info
		struct sTabInfo {
			QString qsColumnSet;											///< column set defined in tab
			QString qsDrive;												///< drive
			QString qsPath;												///< path selected in tab
		};

		void CreateDefaultColumnSet();								///< create default (Full) column set
		void CreateTab(const ePosition &epPosition, const uint &uiIndex, const QString &qsColumnSet, const QString &qsPath);
																				///< create new tab in settings file
																				/**< \param epPosition left or right panel
																					  \param uiIndex index of tab in tab bar
																					  \param qsColumnSet name of column set for dir panel
																					  \param qsPath path to show */
		sColumn GetColumnInfo(const QString &qsColumnSet, const QString &qsColumn);
																				///< get some information about column
																				/**< \param qsColumnSet column's column set
																					  \param qsColumn column index
																					  \return column information */
		QStringList GetColumnsInSet(const QString &qsColumnSet);
																				///< get column names in column set
																				/**< \param qsColumnSet name of column set
																					  \return list of columns in column set */
		QList<sPlugin> GetPlugins(const ePlugin &epPlugin);	///< get plugin list
																				/**< \param epPlugin plugin type
																					  \return plugin list */
		sTabInfo GetTabInfo(const ePosition &epPosition, const QString &qsIndex);
																				///< get some information about tab
																				/**< \param epPosition left or right panel
																					  \param qsIndex index of tab
																					  \return information about panel's tab */
		QStringList GetTabs(const ePosition &epPosition);		///< get tab list for left or right panel
																				/**< \param epPosition left or right panel
																					  \return list of tabs in panel */

	private:
		QSettings qsSettings;											///< application's settings

		void CreateColumnSet(const QString &qsColumnSet, const QList<sColumn> &qlColumns);
																				///< create new column set
																				/**< \param qsColumnSet column set to create
																					  \param qlColumns columns in column set */
}; // cSettings

#endif
