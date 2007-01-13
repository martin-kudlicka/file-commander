/// dir panel

#ifndef PANEL_H
#define PANEL_H

#include <QComboBox>
#include <QLabel>
#include <QStackedWidget>
#include "Settings.h"
#include <QHash>
#include "Plugins/Content.h"
#include <QTreeWidget>
#include <QFileInfo>
#include <QFileIconProvider>
#include "Panel/ShellMenu.h"
#include <QFileSystemWatcher>

class cPanel : private QObject
{
	Q_OBJECT

	public:
		// count of objects
		struct sObjects {
			uint Directories;											///< number of directories
			uint Files;													///< number of files
		};

		cPanel(QStackedWidget *qswPanel, QComboBox *qcbDrive, QLabel *qlDriveInfo, QTabBar *qtbTab, QLabel *qlPath, QLabel *qlSelected, cSettings *csSettings, cContent *ccContent);
																			///< constructor
																			/**< \param qswPanel panel for QTreeWidget
																				  \param qcbDrive drive combo box
																				  \param qlDriveInfo drive name and it's space information
																				  \param qtbTab panel's tab bar
																				  \param qlPath current path
																				  \param qlSelected information about selected directories and files
																				  \param csSettings application's settings
																				  \param ccContent application'c content plugins */
		~cPanel();														///< destructor

		void AddTab(const cSettings::sTabInfo &stiTabInfo);	///< add new tab with dir view
		static sObjects GetCount(const QFileInfoList &qfilObjects);
																			///< count objects
																			/**< \param qfilObjects objects to count
																				  \return count of objects */
		QString GetPath();											///< get path for current dir
																			/**< \return current dir view path */
		QFileInfoList GetSelectedItemsList();					///< get file infos of selected items
		void GoToRootDir();											///< go to root directory
		void GoToUpDir();												///< go up one level (directory)

	private:
		/// strings for widgets
		struct sWidgets {
			QString qsDrive;											///< selected drive
			QString qsPath;											///< path on drive
			QString qsSelected;										///< selected item info
		};
		/// tab information
		struct sTab {
			QHash<QTreeWidgetItem *, QFileInfo> *qhFiles;	///< info about files listed in dir panel
			QList<cSettings::sColumn> *qlColumns;				///< columns in specified tab
			QString qsColumnSet;										///< column set for tab
			sWidgets *swWidgets;										///< to remember displayed strings
		};

		cContent *ccContent;											///< access to content plugins
		cSettings *csSettings;										///< main settings
		cShellMenu *csmMenu;											///< right click "native" shell menu
		QComboBox *qcbDrive;											///< drive
		QFileIconProvider qfipIcon;								///< icons
		QFileSystemWatcher qfswWatcher;							///< watching directory changes
		QHash<uint, sTab> qhTabs;									///< tabs in current panel
		QLabel *qlPath;												///< path
		QLabel *qlDriveInfo;											///< drive info - label, used/free place
		QLabel *qlSelected;											///< selected items
		QStackedWidget *qswDir;										///< directory view
		QTabBar *qtbTab;												///< tabs for dir view

		void ActualizeWidgets();									///< actualize widgets with info about current directory view
		QStringList GetSelectedItemsStringList();				///< get selected files and directories from current dir view
																			/**< \return names of selected items in string list */
		void RefreshContent(const int &iIndex);					///< refresh dir content
																			/**< \param iIndex index of dir view */
		void RefreshHeader(const int &iIndex);					///< refresh column's header
																			/**< \param iIndex index of dir view */
		void SetPath(const QString &qsPath);						///< set new path for current dir view
																			/**< \param qsPath new path */

	private slots:
		void on_qfswWatcher_directoryChanged(const QString &path);
																			///< detect directory modifications
																			/**< \param path directory to watch */
		void on_qtwTree_customContextMenuRequested(const QPoint &pos);
																			///< show tree view context menu
																			/**< \param pos position of context menu */
		void on_qtwTree_itemActivated(QTreeWidgetItem *item, int column);
																			///< double click in tree view
																			/**< \param item item clicked on
																				  \param column column in item clicked in */
}; // cPanel

#endif
