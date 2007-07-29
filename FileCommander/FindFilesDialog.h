/// find files dialog

#ifndef FIND_FILES_DIALOG_H
#define FIND_FILES_DIALOG_H

#include "ui_FindFilesDialog.h"

#include "FileControl.h"
#include "Plugins/ListerPlugin.h"
#include "Panel.h"
#include <QtGui/QCompleter>

class cFindFilesDialog : public QDialog, private Ui::qdFindFiles
{
	Q_OBJECT

	public:
		cFindFilesDialog(QMainWindow *qmwParent, cPanel *cpPanel, cFileControl *cfcFileControl, cSettings *csSettings, cListerPlugin *clpListerPlugin);
																												///< constructor
																												/**< \param qmwParent parent window of this dialog
																													  \param cpPanel panel to work with
																													  \param cfcFileControl file operations control
																													  \param csSettings application's settings file
																													  \param clpListerPlugin lister plugins */

	private:
		cFileControl *cfcFileControl;																	///< file operations control
		cListerPlugin *clpListerPlugin;																///< lister plugin's class
		cPanel *cpPanel;																					///< panel to work with
		cSettings *csSettings;																			///< application's settings file
		QCompleter qcDirModel;																			///< completer based on dir model

		const cSettings::sFindSettings GetSettings() const;									///< store settings in sFindSettings structure
																												/**< \return find settings */
		const void RefreshSavedSettings() const;													///< refreshes list of saved settings

	private slots:
		const void on_qcbDateTimeBetween_stateChanged(int state) const;					///< search files in specified date/time range
																												/**< \param state search between dates/time flag */
		const void on_qcbDateTimeNotOlderThan_stateChanged(int state) const;				///< search files not older than specified
																												/**< \param state search between dates/time flag */
		const void on_qcbFileSize_stateChanged(int state) const;								///< search files by their size
																												/**< \param state search by size flag */
		const void on_qcbSearchInSelectedDirectories_stateChanged(int state) const;	///< search only in selected directories
																												/**< \param state type of search */
		const void on_qgbFullText_toggled(bool on) const;										///< search for text in files
																												/**< \param on search for text flag */
		const void on_qpbBrowse_clicked(bool checked = false);								///< browse button is clicked on
																												/**< \param checked true if button is checkable and checked */
		const void on_qpbDeleteFind_clicked(bool checked = false) const;					///< delete find is clicked on
																												/**< \param checked true if button is checkable and checked */
		const void on_qpbLoadFind_clicked(bool checked = false) const;						///< load find is clicked on
																												/**< \param checked true if button is checkable and checked */
		const void on_qpbSaveFind_clicked(bool checked = false);								///< save find is clicked on
																												/**< \param checked true if button is checkable and checked */
		const void on_qtwSavedFinds_itemSelectionChanged() const;							///< selected item changed in saved finds view
}; // cFindFilesDialog

#endif