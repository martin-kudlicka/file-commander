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

class cPanel
{
	public:
		cContent *ccContent;											///< access to content plugins
		cSettings *csSettings;										///< main settings
		QComboBox *qcbDrive;											///< drive
		QLabel *qlPath;												///< path
		QLabel *qlDriveInfo;											///< drive info - label, used/free place
		QLabel *qlSelected;											///< selected items
		QStackedWidget *qswDir;										///< directory view
		QTabBar *qtbTab;												///< tabs for dir view

		void AddTab(const cSettings::sTabInfo stiTabInfo);	///< add new tab with dir view

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
			sWidgets swWidgets;										///< to remember displayed strings
		};

		QFileIconProvider qfipIcon;								///< icons

		QHash<uint, sTab> qhTabs;									///< tabs in current panel

		void RefreshContent(const int iIndex);					///< refresh dir content
																			/**< /param iIndex index of dir view */
		void RefreshHeader(const int iIndex);					///< refresh column's header
																			/**< /param iIndex index of dir view */
}; // cPanel

#endif
