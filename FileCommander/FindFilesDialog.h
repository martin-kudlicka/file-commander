/// find files dialog

#ifndef FIND_FILES_DIALOG_H
#define FIND_FILES_DIALOG_H

#include "ui_FindFilesDialog.h"

#include "Panel.h"
#include "Plugins/ListerPlugin.h"
#include <QtGui/QCompleter>

const QString qsDAYS = "day(s)";
const QString qsGIGABYTES2 = "gigabyte(s)";
const QString qsHOURS = "hour(s)";
const QString qsKILOBYTES2 = "kilobyte(s)";
const QString qsMEGABYTES2 = "megabyte(s)";
const QString qsMONTHS = "month(s)";
const QString qsWEEKS = "week(s)";
const QString qsYEARS = "year(s)";
const QStringList qsFILE_SIZE_COMPARATOR = (QStringList() << "=" << "<" << ">");
const QStringList qsFILE_SIZE_TYPE = (QStringList() << qsBYTES2 << qsKILOBYTES2 << qsMEGABYTES2 << qsGIGABYTES2);
const QStringList qslOLDER_THAN = (QStringList() << qsMINUTES << qsHOURS << qsDAYS << qsWEEKS << qsMONTHS << qsYEARS);

class cFindFilesDialog : public QDialog, private Ui::qdFindFiles
{
	Q_OBJECT

	public:
		cFindFilesDialog(QWidget *qwParent, cPanel *cpPanel, cSettings *csSettings, cListerPlugin *clpListerPlugin);
																								///< constructor
																								/**< \param qwParent parent window of this dialog
																									  \param cpPanel panel to work with
																									  \param csSettings application's settings file
																									  \param clpListerPlugin lister plugins */

		QFileInfoList SearchResult();													///< founded files
																								/**< \return list of founded files */
		void Start(const cSettings::sFindSettings &sfsCurrentSearch, const bool &bMarking);
																								///< start of the new search
																								/**< \param sfsCurrentSearch search parameters
																									  \param bMarking true if finding files to mark */

	private:
		static const qint64 qi64SEARCH_BUFFER = 1048576;						///< search for text in files in this buffer size

		bool bStop;																			///< interrupt finding files
		QHash<QTreeWidgetItem *, QFileInfo> qhFiles;								///< founded files table
		cListerPlugin *clpListerPlugin;												///< lister plugin's class
		cPanel *cpPanel;																	///< panel to work with
		cSettings *csSettings;															///< application's settings file
		QCompleter qcSearchIn;															///< completer for search in combo box
		cSettings::sFindSettings sfsCurrentSearch;								///< current search settings
		QFileInfoList qfilSearch;														///< search result
		QFileInfoList qfilSelectedDirectories;										///< selected directories in dir view

		void closeEvent(QCloseEvent *event);										///< catch dialog close
																								/**< \param event close event description */
		bool ConditionsSuit(const QFileInfo &qfiFile);							///< check search conditions on found file
																								/**< \param qfFile file to check conditions on
																									  \return true if conditions ok */
		cSettings::sFindSettings GetSettings();									///< store settings in sFindSettings structure
																								/**< \return find settings */
		void RefreshSavedSettings();													///< refreshes list of saved settings

	private slots:
		void on_qcbDateTimeBetween_stateChanged(int state);					///< search files in specified date/time range
																								/**< \param state search between dates/time flag */
		void on_qcbDateTimeNotOlderThan_stateChanged(int state);				///< search files not older than specified
																								/**< \param state search between dates/time flag */
		void on_qcbFileSize_stateChanged(int state);								///< search files by their size
																								/**< \param state search by size flag */
		void on_qcbSearchInSelectedDirectories_stateChanged(int state);	///< search only in selected directories
																								/**< \param state type of search */
		void on_qgbFullText_toggled(bool on);										///< search for text in files
																								/**< \param on search for text flag */
		void on_qpbBrowse_clicked(bool checked = false);						///< browse button is clicked on
																								/**< \param checked true if button is checkable and checked */
		void on_qpbDeleteFind_clicked(bool checked = false);					///< delete find is clicked on
																								/**< \param checked true if button is checkable and checked */
		void on_qpbDrives_clicked(bool checked = false);						///< drives button is clicked on
																								/**< \param checked true if button is checkable and checked */
		void on_qpbFeedToPanel_clicked(bool checked = false);					///< feed to panel button is clicked on
																								/**< \param checked true if button is checkable and checked */
		void on_qpbGoToFile_clicked(bool checked = false);						///< go to file button is clicked on
																								/**< \param checked true if button is checkable and checked */
		void on_qpbLoadFind_clicked(bool checked = false);						///< load find is clicked on
																								/**< \param checked true if button is checkable and checked */
		void on_qpbSaveFind_clicked(bool checked = false);						///< save find is clicked on
																								/**< \param checked true if button is checkable and checked */
		void on_qpbStart_clicked(bool checked = false);							///< start button is clicked on
																								/**< \param checked true if button is checkable and checked */
		void on_qpbStop_clicked(bool checked = false);							///< stop button is clicked on
																								/**< \param checked true if button is checkable and checked */
		void on_qpbView_clicked(bool checked = false);							///< view button is clicked on
																								/**< \param checked true if button is checkable and checked */
		void on_qtwSavedFinds_itemSelectionChanged();							///< selected item changed in saved finds view
		void on_qtwSearch_itemSelectionChanged();									///< selected item changed in found files list
}; // cFindFilesDialog

#endif