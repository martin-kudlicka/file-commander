/// configuration dialog

#ifndef OPTIONS_DIALOG_H
#define OPTIONS_DIALOG_H

#include "ui_OptionsDialog.h"

#include "Settings.h"
#include <QtGui/QMenu>
#include "Plugins/ContentPlugin.h"
#include <QtGui/QToolBar>
#include "Options/NewFavouriteDirectoryDialog.h"

class cOptionsDialog : public QDialog, private Ui::qdOptions
{
	Q_OBJECT

	public:
		/// actions after confirmation changes
		enum eToDo {
			Nothing = 0,															///< no action
			ReassignShortcuts = 1,												///< shortcut change
			ReloadPlugins = 2,													///< plugins change
			RefreshContent = 4,													///< dir view content change
			RefreshHeader = 8,													///< dir view header change
			RefreshTabs = 16,														///< tab change
			RefreshFavouriteDirectories = 32,								///< favourite directories change
			RefreshColumnSets = 64												///< column set change
		};
		Q_DECLARE_FLAGS(ToDo, eToDo);
		/// favourite directory
		struct sFavouriteDirectory {
			QString qsSource;														///< path for source panel
			bool bTarget;															///< set target path too
			QString qsTarget;														///< path for destination panel
		};

		cOptionsDialog(QWidget *qmwParent, cSettings *csSettings, cContentPlugin *ccpContentPlugin);
																						///< constructor
																						/**< \param qmwParent parent widget (window) of this dialog
																							  \param csSettings application's settings
																							  \param ccpContentPlugin content plugins */
		~cOptionsDialog();														///< destructor

	private:
		static const int iPANELS_TAB = 0;									///< tab number of panels options
		static const int iOPERATIONS_TAB = 1;								///< tab number of operations options
		static const int iPLUGINS_TAB = 2;									///< tab number of plugins options
		static const int iOTHERS_TAB = 3;									///< tab number of others options

		static const int iPLUGIN_NAME_COLUMN = 0;							///< name of plugin
		static const int iPLUGIN_EXTENSIONS_COLUMN = 1;					///< extensions for packer plugin
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

		cContentPlugin *ccpContentPlugin;									///< content plugins
		cSettings *csSettings;													///< application's settings
		QAction *qaOperations;													///< operations options
		QAction *qaOthers;														///< others options
		QAction *qaPanels;														///< panels options
		QAction *qaPlugins;														///< plugins options
		QActionGroup *qagToolBarActions;										///< group of all options in panel
		QFlags<eToDo> qfToDo;													///< actions after confirmation changes
		QFont qfListerFont;														///< lister font
		QHash<QTreeWidgetItem *, sFavouriteDirectory> qhFavouriteDirectories;
																						///< favourite directories
		QMap <QString, QString> qlOldOptions;								///< original application's settings
		QMenu qmColumns;															///< columns menu
		QMenu *qmNative;															///< native part of columns menu
		QMenu *qmPlugins;															///< plugins part of columns menu
		QToolBar *qtbToolBar;													///< left toolbar for navigation
		QTreeWidgetItem *qtwiShortcutCommands;								///< shortcut commands category
		QTreeWidgetItem *qtwiShortcutConfiguration;						///< shortcut configuration category
		QTreeWidgetItem *qtwiShortcutDirectoryView;						///< shortcut directory view category
		QTreeWidgetItem *qtwiShortcutFile;									///< shortcut file category
		QTreeWidgetItem *qtwiShortcutListerEdit;							///< shortcut lister edit category
		QTreeWidgetItem *qtwiShortcutListerFile;							///< shortcut lister file category
		QTreeWidgetItem *qtwiShortcutListerOptions;						///< shortcut lister options category
		QTreeWidgetItem *qtwiShortcutMainButtons;							///< shortcut main buttons category
		QTreeWidgetItem *qtwiShortcutMark;									///< shortcut mark category
		QTreeWidgetItem *qtwiShortcutOperationsDialog;					///< shortcut operations dialog category
		QTreeWidgetItem *qtwiShortcutPanel;									///< shortcut panel category
		QTreeWidgetItem *qtwiShortcutTabs;									///< shortcut tabs category

		QTreeWidgetItem *AddColumnToColumns(const cSettings::sColumn &scColumn, const int &iPos = INT_MAX);
																						///< add new column to current column set
																						/**< \param scColumn new column to add
																							  \param iPos position to insert new column
																							  \return added row */
		void AddPluginIntoTree(const cSettings::sPlugin &spPlugin, QTreeWidget *qtwTree);
																						///< add another plugin into tree
																						/**< \param spPlugin plugin to add
																							  \param qtwTree tree to put plugin into */
		void CreateToolBar();													///< create left toolbar for navigation
		bool eventFilter(QObject *watched, QEvent *event);				///< event filter
																						/**< \param watched filtered object
																							  \param event event description
																							  \return true if event is handled */
		void FavouriteAdd(const cNewFavouriteDirectoryDialog::eType &cnfdType);
																						///< add new favourite directory/submenu
																						/**< \param cnfdType type of favourite */
		void FillPluginsTree(const QList<cSettings::sPlugin> &qlPlugins, QTreeWidget *qtwTree);
																						///< fills plugin information into tree
																						/**< \param qlPlugins plugin list
																							  \param qtwTree tree to fill */
		void FillOptions();														///< fill options with set settings
		void FillShortcutItems(const cSettings::eShortcutCategory &escCategory, const QStringList &qslItems);
																						///< fill shortcuts for selected category
																						/**< \param escCategory shortcut category
																							  \param qslItems shortcut list */
		QList<cSettings::sPlugin> GetPluginList(const QTreeWidget *qtwPlugins);
																						///< get info about specified plugins
																						/**< \param qtwPlugins plugins to get info
																							  \return plugin info list */
		cSettings::sColumn GetColumnInfo(QTreeWidgetItem *qtwiItem);
																						///< get information about column from column set
																						/**< \param qtwiItem column set column
																							  \return column information */
		QList<QPair<QString, cSettings::sFavouriteDirectory> > GetFavouriteDirectories(QTreeWidgetItem *qtwiParent);
																						///< collect favourite directories
																						/**< \param qtwiParent favourite to start with
																							  \return list of favourites description */
		void PrepareColumnsMenu();												///< prepare context columns menu
		void SaveOption(const eOption &eoType);							///< save specific changes into settings file
																						/**< \param eoType type of changes */
		void SaveOptions();														///< save changes into application's settings file
		void SetFavouriteDirectories(QTreeWidgetItem *qtwiParent, const QList<QPair<QString, cSettings::sFavouriteDirectory> > &qlFavouriteDirectories);
																						///< fill favourite directories into tree widget
																						/**< \param qtwiParent favourite to start with
																							  \param qlFavouriteDirectories list of favourites description */

	private slots:
		void on_qagToolBarActions_triggered(QAction *qaAction);		///< clicked on action in tool bar panel
																						/**< \param qaAction tool bar's action */
		void on_qcbColumnSet_currentIndexChanged(const QString &text);
																						///< column set changed
																						/**< \param text new selected column set */
		void on_qcbFavouriteTargetDirectory_stateChanged(int state);
																						///< set target favourite directory too
																						/**< \param state set target favourite directory too */
		void on_qcbShowDriveLetter_stateChanged(int state);			///< change of show drive letter in tab bar
																						/**< \param state show drive letter in tab bar flag */
		void on_qcbShowHiddenFiles_stateChanged(int state);			///< change of show hidden files
																						/**< \param state show hidden files flag */
		void on_qcbShowSystemFiles_stateChanged(int state);			///< change of show system files
																						/**< \param state show system files flag */
		void on_qcbShowTabBarWithOnlyOneTab_stateChanged(int state);
																						///< change of show tab bar with only one tab
																						/**< \param state show tab bar with only one tab flag */
		void on_qcbSquareBracketsAroundDirectoryName_stateChanged(int state);
																						///< change of show square brackets around directory name
																						/**< \param state show square brackets around directory name flag */
		void on_qdbbResponse_accepted();										///< changes accepted
		void on_qdbbResponse_rejected();										///< changes rejected
		void on_qleDateTimeDisplay_textEdited(const QString &text);	///< date/time format changed
																						/**< \param text new date/time format */
		void on_qleFavouriteSource_textChanged(const QString &text);
																						///< source favourite directory path changed
																						/**< \param text source favourite directory path */
		void on_qleFavouriteTarget_textChanged(const QString &text);
																						///< target favourite directory path changed
																						/**< \param text target favourite directory path */
		void on_qlePluginDateTimeDisplay_textEdited(const QString &text);
																						///< date/time format for plugin changed
																						/**< \param text new date/time format */
		void on_qlePluginTimeDisplay_textEdited(const QString &text);
																						///< time format for plugin changed
																						/**< \param text new time format */
		void on_qleShortcut_textChanged(const QString &text);			///< shortcut changed
																						/**< \param text new shortcut key sequence */
		void on_qleShow_textEdited(const QString &text);				///< changed column name visible in dir view
																						/**< \param text new column name */
		void on_qmColumns_triggered(QAction *action);					///< column selected into column set
																						/**< \param action selected column (or column's unit) */
		void on_qpbAddContentPlugin_clicked(bool checked = false);	///< add button is clicked on in content plugins
																						/**< \param checked true if button is checkable and checked */
		void on_qpbAddListerPlugin_clicked(bool checked = false);	///< add button is clicked on in lister plugins
																						/**< \param checked true if button is checkable and checked */
		void on_qpbAddPackerPlugin_clicked(bool checked = false);	///< add button is clicked on in packer plugins
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
		void on_qpbExternalEditorBrowse_clicked(bool checked = false);
																						///< external editor browse button is clicked on
																						/**< \param checked true if button is checkable and checked */
		void on_qpbExternalViewerBrowse_clicked(bool checked = false);
																						///< external viewer browse button is clicked on
																						/**< \param checked true if button is checkable and checked */
		void on_qpbFavouriteAddDirectory_clicked(bool checked = false);
																						///< add favourite directory button is clicked on
																						/**< \param checked true if button is checkable and checked */
		void on_qpbFavouriteAddSubmenu_clicked(bool checked = false);
																						///< add submenu button is clicked on
																						/**< \param checked true if button is checkable and checked */
		void on_qpbFavouriteRemove_clicked(bool checked = false);	///< remove favourite button is clicked on
																						/**< \param checked true if button is checkable and checked */
		void on_qpbFavouriteSourceBrowse_clicked(bool checked = false);
																						///< favourite source browse button is clicked on
																						/**< \param checked true if button is checkable and checked */
		void on_qpbFavouriteTargetBrowse_clicked(bool checked = false);
																						///< favourite target browse button is clicked on
																						/**< \param checked true if button is checkable and checked */
		void on_qpbListerChangeFont_clicked(bool checked = false);	///< change font in lister button is clicked on
																						/**< \param checked true if button is checkable and checked */
		void on_qpbRemoveContentPlugin_clicked(bool checked = false);
																						///< remove content plugin button is clicked on
																						/**< \param checked true if button is checkable and checked */
		void on_qpbRemoveListerPlugin_clicked(bool checked = false);
																						///< remove lister plugin button is clicked on
																						/**< \param checked true if button is checkable and checked */
		void on_qpbRemovePackerPlugin_clicked(bool checked = false);
																						///< remove packer plugin button is clicked on
																						/**< \param checked true if button is checkable and checked */
		void on_qpbShortcutClear_clicked(bool checked = false);		///< shortcut clear button is clicked on
																						/**< \param checked true if button is checkable and checked */
		void on_qrbSizeBytes_toggled(bool checked);						///< size in bytes status change
																						/**< \param checked size in bytes flag */
		void on_qrbSizeDynamic_toggled(bool checked);					///< dynamic size status change
																						/**< \param checked dynamic size flag */
		void on_qrbSizeGigabytes_toggled(bool checked);					///< size in gigabytes status change
																						/**< \param checked size in gigabytes flag */
		void on_qrbSizeKilobytes_toggled(bool checked);					///< size in kilobytes status change
																						/**< \param checked size in kilobytes flag */
		void on_qrbSizeMegabytes_toggled(bool checked);					///< size in megabytes status change
																						/**< \param checked size in megabytes flag */
		void on_qsbWidth_valueChanged(int val);							///< changed width of column
																						/**< \param val new column width */
		void on_qtwColumns_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
																						///< selected cell changed
																						/**< \param current actually selected item
																							  \param previously selected item */
		void on_qtwColumns_itemSelectionChanged();						///< selected column changed
		void on_qtwContentPlugins_itemChanged(QTreeWidgetItem *item, int column);
																						///< selected content plugin changed
																						/**< \param item selected content plugin
																							  \param column changed column of plugin list */
		void on_qtwContentPlugins_itemSelectionChanged();				///< selected content plugin changed
		void on_qtwFavouriteDirectories_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
																						///< another favourite directory is selected
																						/**< \param current new favourite directory
																							  \param previous old favourite directory */
		void on_qtwListerPlugins_itemChanged(QTreeWidgetItem *item, int column);
																						///< selected lister plugin changed
																						/**< \param item selected lister plugin
																							  \param column changed column of plugin list */
		void on_qtwListerPlugins_itemSelectionChanged();				///< selected lister plugin changed
		void on_qtwPackerPlugins_itemChanged(QTreeWidgetItem *item, int column);
																						///< selected packer plugin changed
																						/**< \param item selected packer plugin
																							  \param column changed column of plugin list */
		void on_qtwPackerPlugins_itemSelectionChanged();				///< selected packer plugin changed
		void on_qtwShortcutCategory_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
																						///< selected shortcut category changed
																						/**< \param current current shortcut category
																							  \param previous old shortcut category */
		void on_qtwShortcutItem_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
																						///< selected shortcut changed
																						/**< \param current current shortcut
																							  \param previous old shortcut */
}; // cOptionsDialog

Q_DECLARE_OPERATORS_FOR_FLAGS(cOptionsDialog::ToDo)

#endif
