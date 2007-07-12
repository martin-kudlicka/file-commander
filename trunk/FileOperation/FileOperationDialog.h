/// dialog for copy and move accepting

#ifndef FILE_OPERATION_DIALOG_H
#define FILE_OPERATION_DIALOG_H

#include "ui_FileOperationDialog.h"

#include "FileOperation/FileRoutine.h"
#include <QtGui/QMainWindow>
#include "Settings.h"
#include <QtGui/QCompleter>

class cFileOperationDialog : public QDialog, private Ui::qdFileOperation
{
	Q_OBJECT

	public:
		enum eUserAction {								///< action done by user
			CancelAction,									///< cancel button selected
			OkAction,										///< OK button selected
			EnqueueAction									///< enqueue button selected
		};

		cFileOperationDialog(QMainWindow *qmwParent, cSettings *csSettings);
																///< constructor
																/**< \param qmwParent parent window of this dialog
																	  \param csSettings application's settings file */

		eUserAction ShowDialog(const cFileRoutine::eOperation &eoOperation, const QString &qsCount, QString *qsDestination, QString *qsFilter, const bool &bArchive = false);
																///< shows copy or move dialog
																/**< \param eoOperation type of operation
																	  \param qsCount string with count of files and directories
																	  \param qsDestination destination path
																	  \param qsFilter filter for operation
																	  \param bArchive manipulate with archive
																	  \return user action */

		private:
			eUserAction euaResult;						///< dialog result
			QCompleter qcDestination;					///< completer for destination combo box

		private slots:
			void on_qpbBrowse_clicked(bool checked = false);
																///< Browse button clicked on
																/**< \param checked true if button is checkable and checked */
			void on_qpbCancel_clicked(bool checked = false);
																///< Cancel button clicked on
																/**< \param checked true if button is checkable and checked */
			void on_qpbEnqueue_clicked(bool checked = false);
																///< Enqueue button clicked on
																/**< \param checked true if button is checkable and checked */
			void on_qpbOK_clicked(bool checked = false);
																///< OK button clicked on
																/**< \param checked true if button is checkable and checked */
}; // cFileOperationDialog

#endif
