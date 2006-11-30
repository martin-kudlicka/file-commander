/// content plugins

#ifndef CONTENT_H
#define CONTENT_H

#include <QHash>
#include "Settings.h"

class cContent
{
	public:
		cSettings *csSettings;							///< main settings "file"

		QString GetPluginValue(const QString qsPlugin, const QString qsColumn, const QString qsFilename);
																///< returns plugin's value for specified column
																/**< \param qsPlugin name of column's plugin
																	  \param qsColumn column to get value for
																	  \param qsFilename file name to probe */
		void Load();										///< loads content plugins
																/** fills qhPlugins */
		bool Loaded(const QString qsName);			///< checks if plugin qsName has been succesfully loaded
																/**< \param qsName plugin name */

	private:
		typedef int (__stdcall *tContentGetSupportedField)(int iFieldIndex, char *cFieldName, char *cUnits, int iMaxLen);
																///< plugin's ContentGetSupportedField function
																/**< \param iFieldIndex column index in plugin
																	  \param cFieldName returned column name
																	  \param cUnits units for column
																	  \param iMaxLen max size for returned parameters
																	  \return type of column */
		typedef int (__stdcall *tContentGetValue)(char *cFileName, int iFieldIndex, int iUnitIndex, void *vFieldValue, int iMaxLen, int iFlags);
																///< plugin's ContentGetValue function
																/**< \param cFileName file name to analyze
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

		QHash<QString, sPluginInfo> qhPlugins;		///< table of plugins
																/**< key is plugin name, value contains plugin's info */
}; // cContent

#endif