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

		void AddPluginIntoTree(const cSettings::sPlugin &spPlugin, QTreeWidget *qtwTree);
																						///< add another plugin into tree
																						/**< \param spPlugin plugin to add
																							  \param qtwTree tree to put plugin into */
		void CreateChoices();													///< build choices tree
		void FillPluginsTree(const QList<cSettings::sPlugin> &qlPlugins, QTreeWidget *qtwTree);
																						///< fills plugin information into tree
																						/**< \param qlPlugins plugin list
																							  \param qtwTree tree to fill */
		void FillOptions();														///< fill options with set settings

	private slots:
		void on_qpbAddContentPlugin_clicked(bool checked = false);
																						///< add button is clicked on in content plugins
																						/**< \param checked true if button is checkable and checked */
		void on_qpbRemoveContentPlugin_clicked(bool checked = false);
																						///< remove content plugin button is clicked on
																						/**< \param checked true if button is checkable and checked */
		void on_qtwChoices_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
																						///< choice change
																						/**< \param current new choice
																							  \param previous last choice */
		void on_qtwContentPlugins_itemSelectionChanged();				///< selected content plugin changed
}; // cOptionsDialog

#endif