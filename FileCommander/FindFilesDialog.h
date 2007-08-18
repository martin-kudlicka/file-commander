/// find files dialog

#ifndef FIND_FILES_DIALOG_H
#define FIND_FILES_DIALOG_H

#include "ui_FindFilesDialog.h"

#include "FileControl.h"
#include "Plugins/ListerPlugin.h"
#include "Panel.h"
#include <QtGui/QCompleter>
#include "FindFilesDialog/FindFilesThread.h"

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
		/// file system search description
		struct sToSearch {
			cFileSystem *cfsFileSystem;																///< file system to search in
			QString qsPath;																				///< path to start on the file system
		};

		cFileControl *cfcFileControl;																	///< file operations control
		cFindFilesThread cfftFindThread;																///< thread for searching files
		cListerPlugin *clpListerPlugin;																///< lister plugin's class
		cPanel *cpPanel;																					///< panel to work with
		cSettings *csSettings;																			///< application's settings file
		QCompleter qcDirModel;																			///< completer based on dir model
		QHash<QTreeWidgetItem *, cFileSystem *> qhFound;										///< found files table
		QQueue<sToSearch> qqToSearch;																	///< searches description
		cSettings::sFindSettings sfsCurrentSearch;												///< current search settings

		const cSettings::sFindSettings GetSettings() const;									///< store settings in sFindSettings structure
																												/**< \return find settings */
		const void RefreshSavedSettings() const;													///< refreshes list of saved settings

	private slots:
		const void on_cfftFindThread_finished();													///< search thread finished
		const void on_cfftFindThread_Found(QTreeWidgetItem *qtwiFile, cFileSystem *cfsFileSystem);
																												///< found file matches conditions
																												/**< \param qtwiFile found file
																													  \param cfsFileSystem file system found in */
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
		const void on_qpbDrives_clicked(bool checked = false);								///< drives button is clicked on
																												/**< \param checked true if button is checkable and checked */
		const void on_qpbFeedToPanel_clicked(bool checked = false) const;					///< feed to panel button is clicked on
																												/**< \param checked true if button is checkable and checked */
		const void on_qpbGoToFile_clicked(bool checked = false) const;						///< go to file button is clicked on
																												/**< \param checked true if button is checkable and checked */
		const void on_qpbLoadFind_clicked(bool checked = false) const;						///< load find is clicked on
																												/**< \param checked true if button is checkable and checked */
		const void on_qpbSaveFind_clicked(bool checked = false);								///< save find is clicked on
																												/**< \param checked true if button is checkable and checked */
		const void on_qtwSavedFinds_itemSelectionChanged() const;							///< selected item changed in saved finds view
		const void on_qpbStart_clicked(bool checked = false);									///< start button is clicked on
																												/**< \param checked true if button is checkable and checked */
		const void on_qpbStop_clicked(bool checked = false) const;							///< stop button is clicked on
																												/**< \param checked true if button is checkable and checked */
		const void on_qpbView_clicked(bool checked = false) const;							///< view button is clicked on
																												/**< \param checked true if button is checkable and checked */
		const void on_qtwSearch_itemSelectionChanged() const;									///< selected item changed in found files list
}; // cFindFilesDialog

#endif