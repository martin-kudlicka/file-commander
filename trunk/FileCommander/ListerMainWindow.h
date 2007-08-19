/// lister's main window

#ifndef LISTER_MAIN_WINDOW_H
#define LISTER_MAIN_WINDOW_H

#include "ui_ListerMainWindow.h"

#include "Plugins/ListerPlugin.h"
#include <QtGui/QPrinter>

class cListerMainWindow : public QMainWindow, private Ui::qmwLister
{
	Q_OBJECT

	public:
		cListerMainWindow(cSettings *csSettings, cListerPlugin *clpListerPlugin, const QString &qsFile);
																											///< creates lister window
																											/**< \param csSettings application's settings
																												  \param clpListerPlugin lister plugins
																												  \param qsFile file to show */
		~cListerMainWindow();																		///< destructor


	private:
		bool bMenuBarVisible;																		///< menu bar visible flag
		cListerPlugin *clpListerPlugin;															///< lister plugin's class
		cSettings *csSettings;																		///< application's main settings file
		int iSearchFlags;																				///< last used flags for plugin search
		QHash<QString, cListerPlugin::sPluginInfo> qhPlugins;								///< table of plugins
		QHashIterator<QString, cListerPlugin::sPluginInfo> *qhiPlugins;				///< currently used plugin
		QPrinter qpPrinter;																			///< printer settings
		QString qsFile;																				///< file content to show
		QString qsSearchedText;																		///< last searched text
		QTextDocument::FindFlags ffFlags;														///< last used flags for native search
		QTextDocument qtdDocument;																	///< text to show in QTextEdit
		WId hwPlugin;																					///< plugin's window

		void closeEvent(QCloseEvent *event);													///< lister window close
																											/**< \param event close description */
		const void ClosePlugin();																	///< destroy of plugin's window
#ifdef Q_WS_WIN
		bool eventFilter(QObject *watched, QEvent *event);									///< event filter
																											/**< \param watched filtered object
																												  \param event event description
																												  \return true if event is handled */
#endif
		const bool FindNextPlugin(const bool &bNextPlugin, const bool &bForceShow);
																											///< find next usable plugin for file
																											/**< \param bNextPlugin move onto next plugin or not
																												  \param bForceShow force usage of plugin on file
																												  \return true if suitable plugin found */
		const int GetSendCommandParameters() const;											///< parameters for plugin
																											/**< \return parameters for plugin */
#ifdef Q_WS_WIN
		void keyPressEvent(QKeyEvent *event);													///< key pressed in lister
																											/**< \param event key press event description */
#endif
		const void PermitMenuActions() const;													///< enable/disable menu actions
#ifdef Q_WS_WIN
		void resizeEvent(QResizeEvent *event);													///< resize of lister window occurs
																											/**< \param event resize parameters */
#endif
		const void ShowContent(const bool &bNextPlugin = false, const bool &bForceShow = false);
																											///< show file content
																											/**< \param bNextPlugin move onto next plugin
																												  \param bForceShow force usage of plugin */

	signals:
		void Close(cListerMainWindow *clmwLister) const;									///< closing lister window
																											/**< \param clmwLister this window */

	private slots:
		const void on_qaANSI_triggered(bool checked = false) const;						///< ANSI char set selected
																											/**< \param checked true if menu item is checkable and checked */
		const void on_qaASCII_triggered(bool checked = false) const;					///< ASCII char set selected
																											/**< \param checked true if menu item is checkable and checked */
		const void on_qaBinary_triggered(bool checked = false);							///< binary mode selected
																											/**< \param checked true if menu item is checkable and checked */
		const void on_qaCopyAsText_triggered(bool checked = false) const;				///< copy as text selected
																											/**< \param checked true if menu item is checkable and checked */
		const void on_qaFind_triggered(bool checked = false);								///< find selected
																											/**< \param checked true if menu item is checkable and checked */
		const void on_qaFindNext_triggered(bool checked = false);						///< find next selected
																											/**< \param checked true if menu item is checkable and checked */
		const void on_qaFitImageToWindow_triggered(bool checked = false) const;		///< fit image to window selected
																											/**< \param checked true if menu item is checkable and checked */
		const void on_qaHex_triggered(bool checked = false);								///< hex mode selected
																											/**< \param checked true if menu item is checkable and checked */
		const void on_qaMultimedia_triggered(bool checked = false);						///< multimedia mode selected
																											/**< \param checked true if menu item is checkable and checked */
		const void on_qaOpen_triggered(bool checked = false);								///< open selected
																											/**< \param checked true if menu item is checkable and checked */
		const void on_qaPrint_triggered(bool checked = false);							///< print selected
																											/**< \param checked true if menu item is checkable and checked */
		const void on_qaPrintSetup_triggered(bool checked = false);						///< print setup selected
																											/**< \param checked true if menu item is checkable and checked */
		const void on_qaSaveAs_triggered(bool checked = false);							///< save as selected
																											/**< \param checked true if menu item is checkable and checked */
		const void on_qaSelectAll_triggered(bool checked = false) const;				///< select all selected
																											/**< \param checked true if menu item is checkable and checked */
		const void on_qaText_triggered(bool checked = false);								///< text mode selected
																											/**< \param checked true if menu item is checkable and checked */
		const void on_qaVariableCharWidth_triggered(bool checked = false) const;	///< variable char width selected
																											/**< \param checked true if menu item is checkable and checked */
		const void on_qaWrapText_triggered(bool checked = false) const;				///< variable char width selected
																											/**< \param checked true if menu item is checkable and checked */
}; // cListerMainWindow

#endif
