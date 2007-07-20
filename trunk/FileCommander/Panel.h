/// dir panel

#ifndef PANEL_H
#define PANEL_H

#include <QtGui/QStackedWidget>
#include "Settings.h"
#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include "Plugins/ContentPlugin.h"

class cPanel : public QObject
{
	Q_OBJECT

	public:
		cPanel(QStackedWidget *qswPanels, QLabel *qlDriveInfo, QTabBar *qtbTab, QLabel *qlPath, QLabel *qlSelected, cSettings *csSettings, cContentPlugin *ccpContentPlugin, QLabel *qlGlobalPath, QComboBox *qcbCommand, QLineEdit *qleQuickSearch);
																					///< constructor
																					/**< \param qswPanels panel for cTreeWidget
																						  \param qlDriveInfo drive name and it's space information
																						  \param qtbTab panel's tab bar
																						  \param qlPath current path
																						  \param qlSelected information about selected directories and files
																						  \param csSettings application's settings
																						  \param ccpContentPlugin application's content plugins
																						  \param qlGlobalPath path visible in the bottom of main window
																						  \param qcbCommand command combo box
																						  \param qleQuickSearch quick search window */

		const int AddTab(cSettings::sTabInfo &stiTabInfo, const bool &bStartUp = false);
																					///< add new tab with dir view
																					/**< \param stiTabInfo new tab description
																						  \param bStartUp true if tab is added on application startup
																						  \return new tab index */
		const void RefreshAllHeaders();									///< refresh all dir view headers

	private:
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
			//sHistory shHistory;											///< directory history information
		};

		cContentPlugin *ccpContentPlugin;								///< access to content plugins
		cSettings *csSettings;												///< main settings
		QComboBox *qcbCommand;												///< command combo box
		QLabel *qlDriveInfo;													///< drive info - label, used/free place
		QLabel *qlGlobalPath;												///< path visible in the bottom of main window
		QLabel *qlPath;														///< path
		QLabel *qlSelected;													///< selected items
		QLineEdit *qleQuickSearch;											///< quick search window
		QList<sTab> qlTabs;													///< tabs for dir view
		QStackedWidget *qswDirs;											///< directory views
		static QStackedWidget *qswLastActive;							///< last active panel
		QTabBar *qtbTab;														///< tabs for dir views

		const void HideOrShowTabBar() const;							///< hide or show tab bar as set in options
		const void RefreshHeader(const int &iIndex, const bool &bContent = false);
																					///< refresh column's header
																					/**< \param iIndex index of dir view
																						  \param bContent refresh content too flag */
		const void ShowHideHeader(const int &iTabIndex) const;	///< show or hide header for specified tab
																					/**< \param iTabIndex tab index to show/hide header */

	signals:
		void GotFocus();														///< panel got focus

	private slots:
		const void on_ctwTree_GotFocus();								///< dir view got focus
}; // cPanel

#endif
