/// setting file accessing

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

const QString qsDATE = "Date";
const QString qsEXTENSION = "Extension";
const QString qsFALSE = "false";
const QString qsFULL = "Full";										///< full dir panel view (icon, filename, extension, date)
const QString qsICON = "Icon";
const QString qsIDENTIFIER = "Identifier";
const QString qsNAME = "Name";
const QString qsNO = "no";
const QString qsTRUE = "true";

class cSettings : private QObject
{
	public:
		/// keys for values
		enum eKey {
			ShowHiddenFiles,												///< visibility of hidden files in panels
			ShowSystemFiles												///< visibility of system files in panels
		};
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
			int iWidth;														///< column width
		};
		/// plugin
		struct sPlugin {
			QString qsName;												///< plugin's file name
			bool bEnabled;													///< enable/disable flag
		};
		/// sort information
		struct sSort {
			int iSortedColumn;											///< column to sort by
			Qt::SortOrder soSortOrder;									///< sort order
		};
		/// tab info
		struct sTabInfo {
			QString qsColumnSet;											///< column set defined in tab
			QString qsDrive;												///< drive
			QString qsPath;												///< path selected in tab
			sSort ssSort;													///< sort information
		};

		void CreateColumnSet(const QString &qsColumnSet);		///< create new empty column set
																				/**< \param qsColumnSet column set to create */
		void CreateColumnSet(const QString &qsColumnSet, const QList<sColumn> &qlColumns);
																				///< create new column set
																				/**< \param qsColumnSet column set to create
																					  \param qlColumns columns in column set */
		void CreateDefaultColumnSet();								///< create default (Full) column set
		void CreateTab(const ePosition &epPosition, const uint &uiIndex, const sTabInfo &stiTab);
																				///< create new tab in settings file
																				/**< \param epPosition left or right panel
																					  \param uiIndex index of tab in tab bar
																					  \param stiTab tab information */
		QMap <QString, QString> GetAllSettings();				///< get all application's settings
																				/**< \return list of whole settings file */
		sColumn GetColumnInfo(const QString &qsColumnSet, const QString &qsColumn);
																				///< get some information about column
																				/**< \param qsColumnSet column's column set
																					  \param qsColumn column index
																					  \return column information */
		QStringList GetColumnSets();									///< get column sets
																				/**< \return list of column sets */
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
		QString GetValue(const eKey &ekKey);						///< find out value for specified key
																				/**< \param ekKey key to find value for
																					  \return value for the key */
		void RemoveColumnSet(const QString &qsColumnSet);		///< remove column set
																				/**< \param qsColumnSet column set to remove */
		void RestoreSettings(QMap <QString, QString> &qmSettings);
																				///< restore old application's settings
																				/**< \param qmSettings settings to restore */
		void SetPlugins(const ePlugin &epPlugin, const QList<sPlugin> &qlPlugins);
																				///< write plugins into settings file
																				/**< \param epPlugin plugins type
																					  \param qlPlugins plugin list */
		void SetValue(const eKey &ekKey, const QString &qsValue);
																				///< set key - value pair
																				/**< \param ekKey key
																					  \param qsValue value */

	private:
		QSettings qsSettings;											///< application's settings
}; // cSettings

#endif
