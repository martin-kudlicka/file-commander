/// configuration dialog

#ifndef OPTIONS_DIALOG_H
#define OPTIONS_DIALOG_H

#include "ui_OptionsDialog.h"

#include "Settings.h"
#include <QMenu>
#include "Plugins/Content.h"

class cOptionsDialog : public QDialog, private Ui::qdOptions
{
	Q_OBJECT

	public:
		cOptionsDialog(QWidget *qmwParent, cSettings *csSettings, cContent *ccContent);
																						///< constructor
																						/**< \param qmwParent parent widget (window) of this dialog
																							  \param csSettings application's settings
																							  \param ccContent content plugins */
		~cOptionsDialog();														///< destructor

	private:
		static const int iCOLUMNS = 5;										///< columns count in column view

		static const int iPANELS_PAGE = 0;									///< main panel's page
		static const int iDISPLAY_PAGE = 1;									///< panel's display page
		static const int iCOLUMN_SETS_PAGE = 2;							///< column's sets page
		static const int iPLUGINS_PAGE = 3;									///< main plugin's page
		static const int iCONTENT_PLUGINS_PAGE = 4;						///< content plugin's page
		static const int iOTHERS_PAGE = 5;									///< others page
		static const int iCONFIRMATION_PAGE = 6;							///< confirmation page

		static const int iPLUGIN_NAME_COLUMN = 0;							///< name of plugin
		static const int iPLUGIN_ENABLED_COLUMN = 1;						///< plugin enabled / disabled

		static const int iTYPE_COLUMN = 0;									///< type of column (native / plugin name)
		static const int iNAME_COLUMN = 1;									///< internal name of column
		static const int iUNIT_COLUMN = 2;									///< column's unit
		static const int iSHOW_COLUMN = 3;									///< user's name of column
		static const int iWIDTH_COLUMN = 4;

		/// specific options to save
		enum eOption {
			Columns																	///< columns from column set
		};

		cContent *ccContent;														///< content plugins
		cSettings *csSettings;													///< application's settings
		QMap <QString, QString> qlOldOptions;								///< original application's settings
		QMenu qmColumns;															///< columns menu
		QMenu *qmNative;															///< native part of columns menu
		QMenu *qmPlugins;															///< plugins part of columns menu

		QTreeWidgetItem *AddColumnToColumns(const cSettings::sColumn &scColumn, const int &iPos = INT_MAX);
																						///< add new column to current column set
																						/**< \param scColumn new column to add
																							  \param iPos position to insert new column
																							  \return added row */
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
		cSettings::sColumn GetColumnInfo(QTreeWidgetItem *qtwiItem);
																						///< get information about column from column set
																						/**< \param qtwiItem column set column
																							  \return column information */
		void PrepareColumnsMenu();												///< prepare context columns menu
		void SaveOption(const eOption &eoType);							///< save specific changes into settings file
																						/**< \param eoType type of changes */
		void SaveOptions();														///< save changes into application's settings file

	private slots:
		void on_qcbColumnSet_currentIndexChanged(const QString &text);
																						///< column set changed
																						/**< \param text new selected column set */
		void on_qdbbRespond_accepted();										///< changes accepted
		void on_qdbbRespond_rejected();										///< changes rejected
		void on_qmColumns_triggered(QAction *action);					///< column selected into column set
																						/**< \param action selected column (or column's unit) */
		void on_qpbAddContentPlugin_clicked(bool checked = false);	///< add button is clicked on in content plugins
																						/**< \param checked true if button is checkable and checked */
		void on_qpbColumnDown_clicked(bool checked = false);			///< column down button is clicked on in columns view
																						/**< \param checked true if button is checkable and checked */
		void on_qpbColumnRemove_clicked(bool checked = false);		///< column remove button is clicked on in columns view
																						/**< \param checked true if button is checkable and checked */
		void on_qpbColumnSetAdd_clicked(bool checked = false);		///< column set add button is clicked on in columns view
																						/**< \param checked true if button is checkable and checked */
		void on_qpbColumnSetRemove_clicked(bool checked = false);	///< column set remove button is clicked on in columns view
																						/**< \param checked true if button is checkable and checked */
		void on_qpbColumnUp_clicked(bool checked = false);				///< column up button is clicked on in columns view
																						/**< \param checked true if button is checkable and checked */
		void on_qpbRemoveContentPlugin_clicked(bool checked = false);
																						///< remove content plugin button is clicked on
																						/**< \param checked true if button is checkable and checked */
		void on_qsbWidth_valueChanged(int val);							///< changed width of column
																						/**< \param val new column width */
		void on_qtwChoices_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
																						///< choice change
																						/**< \param current new choice
																							  \param previous last choice */
		void on_qtwColumns_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
																						///< selected cell changed
																						/**< \param current actually selected item
																							  \param previously selected item */
		void on_qtwColumns_itemSelectionChanged();						///< selected column changed
		void on_qtwContentPlugins_itemSelectionChanged();				///< selected content plugin changed
}; // cOptionsDialog

#endif