/// dir panel

#ifndef PANEL_H
#define PANEL_H

#include <QComboBox>
#include <QLabel>
#include <QStackedWidget>
#include "Settings.h"
#include <QHash>
#include "Plugins/Content.h"
#include <QFileInfo>
#include <QFileIconProvider>
#include "Panel/ShellMenu.h"
#include <QFileSystemWatcher>
#include "FileOperation/FileRoutine.h"
#include "Panel/TreeWidget.h"
#include <QTimer>
#include "Plugins/ContentDelayed.h"

class cPanel : private QObject
{
	Q_OBJECT

	public:
		// count of objects
		struct sObjects {
			uint Directories;											///< number of directories
			uint Files;													///< number of files
		};

		cPanel(QStackedWidget *qswPanel, QComboBox *qcbDrive, QLabel *qlDriveInfo, QTabBar *qtbTab, QLabel *qlPath, QLabel *qlSelected, cSettings *csSettings, cContent *ccContent, QMap<QString, cFileRoutine::sDriveInfo> *qmDrives);
																			///< constructor
																			/**< \param qswPanel panel for cTreeWidget
																				  \param qcbDrive drive combo box
																				  \param qlDriveInfo drive name and it's space information
																				  \param qtbTab panel's tab bar
																				  \param qlPath current path
																				  \param qlSelected information about selected directories and files
																				  \param csSettings application's settings
																				  \param ccContent application'c content plugins
																				  \param qmDrives information about system drives */
		~cPanel();														///< destructor

		void AddTab(const cSettings::sTabInfo &stiTabInfo);
																			///< add new tab with dir view
		void EditFile();												///< edit selected file in editor
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
		static const qint64 qi64_1KILOBYTE = 1024;			///< 1 kilobyte in bytes
		static const qint64 qi64_1MEGABYTE = 1048576;		///< 1 megabyte in bytes

		static const int iTIMER_INTERVAL = 1000;				///< timer interval

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
		cContentDelayed *ccdContentDelayed;						///< thread to get delayed content plugins values
		cSettings *csSettings;										///< main settings
		cShellMenu *csmMenu;											///< right click "native" shell menu
		QComboBox *qcbDrive;											///< drive
		QFileIconProvider qfipIcon;								///< icons
		QFileSystemWatcher qfswWatcher;							///< watching directory changes
		QHash<uint, sTab> qhTabs;									///< tabs in current panel
		QHash<QString, QString> qhLastPaths;					///< last visited paths for drives
		QLabel *qlPath;												///< path
		QLabel *qlDriveInfo;											///< drive info - label, used/free place
		QLabel *qlSelected;											///< selected items
		QMap<QString, cFileRoutine::sDriveInfo> *qmDrives;	///< drives in system
		QStackedWidget *qswDir;										///< directory view
		QTabBar *qtbTab;												///< tabs for dir view
		QTimer qtTimer;												///< timer for requesting changeable informations
		static cSettings::sSort ssSort;							///< sort information

		void ActualizeVolumeInfo();								///< actualize volume information - disk name and space
		void ActualizeWidgets();									///< actualize widgets with info about current directory view
		int GetNativeColumnIndex(const QString &qsColumn, const int &iTabIndex);
																			///< find index of native column
																			/**< \param qsColumn native column name
																				  \param iTabIndex tab index
																				  \return index of native column */
		QStringList GetSelectedItemsStringList();				///< get selected files and directories from current dir view
																			/**< \return names of selected items in string list */
		QString GetSizeString(const qint64 &qi64Size);		///< "convert" size to string according to setting in options
																			/**< \param qi64Size size
																				  \return file size in string with suffix */
		void RefreshContent(const int &iIndex);				///< refresh dir content
																			/**< \param iIndex index of dir view */
		void RefreshHeader(const int &iIndex);					///< refresh column's header
																			/**< \param iIndex index of dir view */
		void SetPath(const QString &qsPath);					///< set new path for current dir view
																			/**< \param qsPath new path */
		void Sort(const int &iIndex);								///< sort dir content
																			/**< \param iIndex index of dir view */
		static bool TreeSortByString(const QTreeWidgetItem *qtwiItem1, const QTreeWidgetItem *qtwiItem2);
																			///< compare items by text
																			/**< \param qtwiItem1 1st item
																				  \param qtwiItem2 2nd item */

	signals:
		void InterruptContentDelayed();							///< interrupt delayed content processing before refresh dir view content

	private slots:
		void on_ccdContentDelayed_GotColumnValue(const cContentDelayed::sOutput &soOutput);
																			///< got golumn value from plugin
																			/**< \param soOutput information to update dir view */
		void on_ctwTree_customContextMenuRequested(const QPoint &pos);
																			///< show tree view context menu
																			/**< \param pos position of context menu */
		void on_ctwTree_itemActivated(QTreeWidgetItem *item, int column);
																			///< double click in tree view
																			/**< \param item item clicked on
																				  \param column column in item clicked in */
		void on_ctwTree_itemSelectionChanged(const cTreeWidget *ctwTree);
																			///< changed selected items in directory view
																			/**< \param ctwTree changed directory view */
		void on_ctwTree_SpacePressed(QTreeWidgetItem *qtwiItem);
																			///< space pressed in dir view
																			/**< \param qtwiItem item space pressed on */
		void on_qcbDrive_activated(int index);					///< drive selected
																			/**< \param index index of selected drive */
		void on_qcbDrive_currentIndexChanged(int index);	///< selected drive changes
																			/**< \param index index of selected drive in combo box */
		void on_qfswWatcher_directoryChanged(const QString &path);
																			///< detect directory modifications
																			/**< \param path directory to watch */
		void on_qhvTreeHeader_sectionClicked(int logicalIndex);
																			///< click on header in tree (dir) view
																			/**< \param logicalIndex column index clicked on */
		void on_qtTimer_timeout();									///< timer's timeout
}; // cPanel

#endif
