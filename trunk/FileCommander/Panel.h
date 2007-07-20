/// dir panel

#ifndef PANEL_H
#define PANEL_H

#include <QtGui/QStackedWidget>
#include "Settings.h"
#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include "Plugins/ContentPlugin.h"
#include "FileSystem.h"
#include <QtCore/QTimer>
#include "FileControl.h"
#include "Plugins/ContentPluginDelayed.h"

class cPanel : public QObject
{
	Q_OBJECT

	public:
		cPanel(QStackedWidget *qswDirs, QComboBox *qcbDrive, QLabel *qlDriveInfo, QTabBar *qtbTab, QLabel *qlPath, QLabel *qlSelected, cSettings *csSettings, cContentPlugin *ccpContentPlugin, QLabel *qlGlobalPath, QComboBox *qcbCommand, cFileControl *cfcFileControl, QLineEdit *qleQuickSearch);
																					///< constructor
																					/**< \param qswDirs panel for cTreeWidget
																						  \param qcbDrive drive combo box
																						  \param qlDriveInfo drive name and it's space information
																						  \param qtbTab panel's tab bar
																						  \param qlPath current path
																						  \param qlSelected information about selected directories and files
																						  \param csSettings application's settings
																						  \param ccpContentPlugin application's content plugins
																						  \param qlGlobalPath path visible in the bottom of main window
																						  \param qcbCommand command combo box
																						  \param cfcFileControl file operations control
																						  \param qleQuickSearch quick search window */

		const int AddTab(const cSettings::sTabInfo &stiTabInfo, const bool &bStartUp = false);
																					///< add new tab with dir view
																					/**< \param stiTabInfo new tab description
																						  \param bStartUp true if tab is added on application startup
																						  \return new tab index */
		const void RefreshAllHeaders();									///< refresh all dir view headers

	private:
		static const int iTIMER_INTERVAL = 1000;						///< timer interval (ms)

		static const qint64 qi64_GIGABYTE = 1073741824;				///< 1 gigabyte in bytes
		static const qint64 qi64_KILOBYTE = 1024;						///< 1 kilobyte in bytes
		static const qint64 qi64_MEGABYTE = 1048576;					///< 1 megabyte in bytes

		/// strings for widgets
		struct sWidgets {
			QString qsDrive;													///< selected drive
			QString qsPath;													///< path on drive
			QString qsSelected;												///< selected item info
		};
		/// tab information
		struct sTab {
			bool bValid;														///< tab content is validated flag
			QList<cSettings::sColumn> qlColumns;						///< columns in specified tab
			QString qsColumnSet;												///< column set for tab
			sWidgets swWidgets;												///< to remember displayed strings
			cFileSystem *cfsFileSystem;									///< attached file system handle
			//sHistory shHistory;											///< directory history information
		};

		cContentPlugin *ccpContentPlugin;								///< access to content plugins
		cFileControl *cfcFileControl;										///< file operations control
		cSettings *csSettings;												///< main settings
		QComboBox *qcbCommand;												///< command combo box
		QComboBox *qcbDrive;													///< drive combo box
		QLabel *qlDriveInfo;													///< drive info - label, used/free place
		QLabel *qlGlobalPath;												///< path visible in the bottom of main window
		QLabel *qlPath;														///< path
		QLabel *qlSelected;													///< selected items
		QLineEdit *qleQuickSearch;											///< quick search window
		QList<sTab> qlTabs;													///< tabs for dir view
		QStackedWidget *qswDirs;											///< directory views
		static QStackedWidget *qswLastActive;							///< last active panel
		QTabBar *qtbTab;														///< tabs for dir views
		QTimer qtTimer;														///< timer for periodic actualizations
		static cSettings::sSort ssSort;									///< sort information

		const void ActualizeDrives() const;								///< drive list actualization
		const QString GetDateTimeString(const QDateTime &qdtDateTime) const;
																					///< convert QDateTime to user defined format
																					/**< \param qdtDateTime date/time to convert
																						  \return user defined date/time format */
		const int GetNativeColumnIndex(const QString &qsColumn, const int &iTabIndex);
																					///< find index of native column
																					/**< \param qsColumn native column name
																						  \param iTabIndex tab index
																						  \return index of native column */
		const QString GetSizeString(const qint64 &qi64Size) const;
																					///< "convert" size to string according to setting in options
																					/**< \param qi64Size size
																						  \return file size in string with suffix */
		const void HideOrShowTabBar() const;							///< hide or show tab bar as set in options
		const void RefreshContent(const int &iIndex);				///< refresh dir content
																					/**< \param iIndex index of dir view */
		const void RefreshHeader(const int &iIndex, const bool &bContent = false);
																					///< refresh column's header
																					/**< \param iIndex index of dir view
																						  \param bContent refresh content too flag */
		const void ShowHideHeader(const int &iTabIndex) const;	///< show or hide header for specified tab
																					/**< \param iTabIndex tab index to show/hide header */
		const void Sort(const int &iIndex, const QList<QTreeWidgetItem *> &qlToSort);
																					///< sort dir content
																					/**< \param iIndex index of dir view
																						  \param qlToSort files to sort */
		static const bool TreeSortByQDateTime(const QTreeWidgetItem *qtwiItem1, const QTreeWidgetItem *qtwiItem2);
																					///< compare items by QDateTime
																					/**< \param qtwiItem1 1st item
																						  \param qtwiItem2 2nd item */
		static const bool TreeSortByQInt64(const QTreeWidgetItem *qtwiItem1, const QTreeWidgetItem *qtwiItem2);
																					///< compare items by qint64
																					/**< \param qtwiItem1 1st item
																						  \param qtwiItem2 2nd item */
		static const bool TreeSortByString(const QTreeWidgetItem *qtwiItem1, const QTreeWidgetItem *qtwiItem2);
																					///< compare items by text
																					/**< \param qtwiItem1 1st item
																						  \param qtwiItem2 2nd item */

	signals:
		void GotFocus();														///< panel got focus

	private slots:
		const void on_cfsFileSystem_GotColumnValue(const cContentPluginDelayed::sOutput &soOutput) const;
																					///< got column value from content plugin
																					/**< \param soOutput information to update dir view */
		const void on_ctwTree_GotFocus();								///< dir view got focus
		const void on_qtTimer_timeout() const;							///< timer's action
}; // cPanel

#endif