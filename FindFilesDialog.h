/// find files dialog

#ifndef FIND_FILES_DIALOG_H
#define FIND_FILES_DIALOG_H

#include "ui_FindFilesDialog.h"

#include <QMainWindow>
#include "Panel.h"

const QString qsBYTES2 = "byte(s)";
const QString qsDAYS = "day(s)";
const QString qsGIGABYTES2 = "gigabyte(s)";
const QString qsHOURS = "hour(s)";
const QString qsKILOBYTES2 = "kilobyte(s)";
const QString qsMEGABYTES2 = "megabyte(s)";
const QString qsMINUTES = "minute(s)";
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
		cFindFilesDialog(QMainWindow *qmwParent, cPanel *cpPanel, QFileInfoList &qfilSelectedDirectories);
																								///< constructor
																								/**< \param qmwParent parent window of this dialog
																									  \param cpPanel panel to work with
																									  \param qfilSelectedDirectories selected directories in dir view */

	private:
		bool bStop;																			///< interrupt finding files
		cPanel *cpPanel;																	///< panel to work with
		QFileInfoList qfilSearch;														///< search result
		QFileInfoList qfilSelectedDirectories;										///< selected directories in dir view

		void closeEvent(QCloseEvent *event);										///< catch dialog close
																								/**< \param event close event description */
		bool ConditionsSuit(const QFileInfo &qfFile);							///< check search conditions on found file
																								/**< \param qfFile file to check conditions on
																									  \return true if conditions ok */

	private slots:
		void on_qcbDateTimeBetween_stateChanged(int state);					///< search files in specified date/time range
																								/**< \param state search between dates/time flag */
		void on_qcbDateTimeNotOlderThan_stateChanged(int state);				///< search files not older than specified
																								/**< \param state search between dates/time flag */
		void on_qcbFileSize_stateChanged(int state);								///< search files by their size
																								/**< \param state search by size flag */
		void on_qcbSearchForText_stateChanged(int state);						///< search for text in files
																								/**< \param state search for text flag */
		void on_qcbSearchInSelectedDirectories_stateChanged(int state);	///< search only in selected directories
																								/**< \param state type of search */
		void on_qpbBrowse_clicked(bool checked = false);						///< browse button is clicked on
																								/**< \param checked true if button is checkable and checked */
		void on_qpbDrives_clicked(bool checked = false);						///< drives button is clicked on
																								/**< \param checked true if button is checkable and checked */
		void on_qpbStart_clicked(bool checked = false);							///< start button is clicked on
																								/**< \param checked true if button is checkable and checked */
		void on_qpbStop_clicked(bool checked = false);							///< stop button is clicked on
																								/**< \param checked true if button is checkable and checked */
}; // cFindFilesDialog

#endif