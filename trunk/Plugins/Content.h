/// content plugins

#ifndef CONTENT_H
#define CONTENT_H

#include <QHash>
#include "Settings.h"

class cContent
{
	public:
		cContent(cSettings *csSettings);				///< constructor
																/**< \param csSettings application's settings */

		QString GetPluginValue(const QString qsFilename, const QString qsPlugin, const QString qsColumn, const QString qsUnit);
																///< returns plugin's value for specified column
																/**< \param qsFilename file name to probe
																	  \param qsPlugin name of column's plugin
																	  \param qsColumn column to get value for
																	  \param qsUnit unit for column value
																	  \return plugin's column value for specified file */
		void Load();										///< loads content plugins
																/** fills qhPlugins */
		bool Loaded(const QString qsName);			///< checks if plugin qsName has been succesfully loaded
																/**< \param qsName plugin name
																	  \return true if plugin was succesfully loaded and can be used */

	private:
		typedef int (__stdcall *tContentGetSupportedField)(int iFieldIndex, char *cFieldName, char *cUnits, int iMaxLen);
																///< plugin's ContentGetSupportedField function
																/**< \param iFieldIndex column index in plugin
																	  \param cFieldName returned column name
																	  \param cUnits units for column
																	  \param iMaxLen max size for returned parameters
																	  \return type of column */
		typedef int (__stdcall *tContentGetValue)(char *cFilename, int iFieldIndex, int iUnitIndex, void *vFieldValue, int iMaxLen, int iFlags);
																///< plugin's ContentGetValue function
																/**< \param cFilename file name to analyze
																	  \param iFieldIndex column index to fill
																	  \param iUnitIndex unit to use
																	  \param vFieldValue returned column value
																	  \param iMaxLen max size for returned parameters
																	  \param iFlags
																	  \return */

		static const uint uiMAX_CHAR = 272;			///< maximum length of char field for plugins

		/// column info
		struct sField {
			QString qsName;								///< displayed column name
			QString qsUnits;								///< units for column
			int iType;										///< type of field
		};
		/// plugin info
		struct sPluginInfo {
			QList<sField> qlFields;						///< plugin's columns
			tContentGetValue tcgvContentGetValue;	///< pointer to plugin's ContentGetValue function
		};

		cSettings *csSettings;							///< main settings "file"
		QHash<QString, sPluginInfo> qhPlugins;		///< table of plugins
																/**< key is plugin name, value contains plugin's info */

		int GetFieldIndex(const QString qsPlugin, const QString qsColumn);
																///< get index of column in plugin
																/**< \param qsPlugin plugin filename
																	  \param qsColumn plugin's column name
																	  \return index of column */
		QString ValidateFieldValue(const char *cFieldValue, const int iType);
																///< "converts" plugin's returned value to QString
																/**< \param cFieldValue value returned by plugin
																	  \param iType type of value
																	  \return QString presentation of value */
}; // cContent

#endif