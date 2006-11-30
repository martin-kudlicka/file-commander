/// content plugins

#ifndef CONTENT_H
#define CONTENT_H

#include <QSettings>
#include <QHash>

class cContent
{
	public:
		void Load();										///< loads content plugins
																/** fills qhPlugins */
		bool Loaded(QString qsName);					///< checks if plugin qsName has been succesfully loaded
																/**< \param qsName plugin name */
		void SetSettings(QSettings *qsSettings);	///< sets main settings "file"
																/**< \param qsSettings settings "file" */

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
		/// column info
		struct sField {
			QString qsName;								///< displayed column name
			QString qsUnits;								///< units for column
		};
		/// plugin info
		struct sPluginInfo {
			QList<sField> qlFields;						///< plugin's columns
			tContentGetValue tcgvContentGetValue;	///< pointer to plugin's ContentGetValue function
		};

		QHash<QString, sPluginInfo> qhPlugins;		///< table of plugins
																/**< key is plugin name, value contains plugin's info */
		QSettings *qsSettings;							///< main settings "file"
}; // cContent

#endif