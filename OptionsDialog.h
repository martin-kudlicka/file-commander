/// configuration dialog

#ifndef OPTIONS_DIALOG_H
#define OPTIONS_DIALOG_H

#include "ui_OptionsDialog.h"

#include "Settings.h"

class cOptionsDialog : public QDialog, private Ui::qdOptions
{
	Q_OBJECT

	public:
		cOptionsDialog(QWidget *qmwParent, cSettings *csSettings);	///< constructor
																						/**< \param qmwParent parent widget (window) of this dialog
																							  \param csSettings application's settings */

	private:
		static const int iPLUGINS_PAGE = 0;									///< main plugin's page
		static const int iCONTENT_PLUGINS_PAGE = 1;						///< content plugin's page

		cSettings *csSettings;													///< application's settings

		void BuildTree();															///< build settings tree with choices
		void FillPluginsTable(const QList<cSettings::sPlugin> &qlPlugins, QTableWidget *qtwTable);
																						///< fills plugin information into table
																						/**< \param qlPlugins plugin list
																							  \param qtwTable table to fill */
		void FillSettings();														///< fill options with set settings

	private slots:
		void on_qtwChoices_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
																						///< choice change
																						/**< \param current new choice
																							  \param previous last choice */
}; // cOptionsDialog

#endif