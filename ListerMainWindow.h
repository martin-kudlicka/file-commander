/// lister's main window

#ifndef LISTER_MAIN_WINDOW_H
#define LISTER_MAIN_WINDOW_H

#include "ui_ListerMainWindow.h"

#include "Plugins/Lister.h"
#include <QTextEdit>

class cListerMainWindow : public QMainWindow, private Ui::qmwLister
{
	Q_OBJECT

	public:
		cListerMainWindow(cSettings *csSettings, cLister *clLister, const QString &qsFile);
																										///< creates lister window
																										/**< \param csSettings application's settings
																											  \param clLister lister plugins
																											  \param qsFile file content to show */
		~cListerMainWindow();																	///< destructor


	private:
		cLister *clLister;																		///< lister plugin's class
		cSettings *csSettings;																	///< application's main settings file
		QHash<QString, cLister::sPluginInfo> qhPlugins;									///< table of plugins
		QHashIterator<QString, cLister::sPluginInfo> *qhiPlugins;					///< currently used plugin
		QString qsFile;																			///< file content to show
		QTextEdit *qteContent;																	///< file content in standard widget
		WId hwPlugin;																				///< plugin's window

		void closeEvent(QCloseEvent *event);												///< lister window close
																										/**< \param event close description */
		void ClosePlugin();																		///< destroy of plugin's window
		bool FindNextPlugin(const bool &bNextPlugin, const bool &bForceShow);
																										///< find next usable plugin for file
																										/**< \param bNextPlugin move onto next plugin or not
																											  \param bForceShow force usage of plugin on file
																											  \return true if suitable plugin found */
		void resizeEvent(QResizeEvent *event);												///< resize of lister window occurs
																										/**< \param event resize parameters */
		void ShowContent(const bool &bNextPlugin = false, const bool &bForceShow = false);
																										///< show file content
																										/**< \param bNextPlugin move onto next plugin
																											  \param bForceShow force usage of plugin */

	private slots:
		void on_qaANSI_triggered(bool checked = false);									///< ANSI char set selected
																										/**< \param checked true if menu item is checkable and checked */
		void on_qaASCII_triggered(bool checked = false);								///< ASCII char set selected
																										/**< \param checked true if menu item is checkable and checked */
		void on_qaBinary_triggered(bool checked = false);								///< binary mode selected
																										/**< \param checked true if menu item is checkable and checked */
		void on_qaHex_triggered(bool checked = false);									///< hex mode selected
																										/**< \param checked true if menu item is checkable and checked */
		void on_qaMultimedia_triggered(bool checked = false);							///< multimedia mode selected
																										/**< \param checked true if menu item is checkable and checked */
		void on_qaText_triggered(bool checked = false);									///< text mode selected
																										/**< \param checked true if menu item is checkable and checked */
		void on_qaVariableCharWidth_triggered(bool checked = false);				///< variable char width selected
																										/**< \param checked true if menu item is checkable and checked */
}; // cListerMainWindow

#endif