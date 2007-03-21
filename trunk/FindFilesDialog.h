/// find files dialog

#ifndef FIND_FILES_DIALOG_H
#define FIND_FILES_DIALOG_H

#include "ui_FindFilesDialog.h"

#include <QMainWindow>
#include "Panel.h"

class cFindFilesDialog : public QDialog, private Ui::qdFindFiles
{
	Q_OBJECT

	public:
		cFindFilesDialog(QMainWindow *qmwParent, cPanel *cpPanel);	///< constructor
																						/**< \param qmwParent parent window of this dialog
																							  \param cpPanel panel to work with */

	private:
		bool bStop;																	///< interrupt finding files
		cPanel *cpPanel;															///< panel to work with
		QFileInfoList qfilSearch;												///< search result

		void closeEvent(QCloseEvent *event);								///< catch dialog close
																						/**< \param event close event description */
		bool ConditionsSuit(const QFileInfo &qfFile);					///< check search conditions on found file
																						/**< \param qfFile file to check conditions on
																							  \return true if conditions ok */

	private slots:
		void on_qpbBrowse_clicked(bool checked = false);				///< browse button is clicked on
																						/**< \param checked true if button is checkable and checked */
		void on_qpbDrives_clicked(bool checked = false);				///< drives button is clicked on
																						/**< \param checked true if button is checkable and checked */
		void on_qpbStart_clicked(bool checked = false);					///< start button is clicked on
																						/**< \param checked true if button is checkable and checked */
		void on_qpbStop_clicked(bool checked = false);					///< stop button is clicked on
																						/**< \param checked true if button is checkable and checked */
}; // cFindFilesDialog

#endif