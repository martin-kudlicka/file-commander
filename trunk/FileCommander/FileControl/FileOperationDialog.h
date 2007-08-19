/// dialog for copy and move accepting

#ifndef FILE_OPERATION_DIALOG_H
#define FILE_OPERATION_DIALOG_H

#include "ui_FileOperationDialog.h"

#include <QtGui/QMainWindow>
#include "Settings.h"
#include <QtGui/QCompleter>

class cFileOperationDialog : public QDialog, private Ui::qdFileOperation
{
	Q_OBJECT

	public:
		/// file operation type
		enum eOperation {
			CopyOperation,									///< copy
			MoveOperation,									///< move
			DeleteOperation								///< delete
		};
		/// action done by user
		enum eUserAction {
			CancelAction,									///< cancel button selected
			OkAction,										///< OK button selected
			EnqueueAction									///< enqueue button selected
		};

		cFileOperationDialog(QMainWindow *qmwParent, cSettings *csSettings);
																///< constructor
																/**< \param qmwParent parent window of this dialog
																	  \param csSettings application's settings file */

		const eUserAction ShowDialog(const eOperation &eoOperation, const QString &qsCount, QString *qsDestination, QString *qsFilter);
																///< shows copy or move dialog
																/**< \param eoOperation type of operation
																	  \param qsCount string with count of files and directories
																	  \param qsDestination destination path
																	  \param qsFilter filter for operation
																	  \param bArchive manipulate with archive
																	  \return user action */

		private:
			cSettings *csSettings;						///< application's settings file
			eUserAction euaResult;						///< dialog result
			QCompleter qcDestination;					///< completer for destination combo box

		private slots:
			const void on_qpbBrowse_clicked(bool checked = false);
																///< Browse button clicked on
																/**< \param checked true if button is checkable and checked */
			const void on_qpbCancel_clicked(bool checked = false);
																///< Cancel button clicked on
																/**< \param checked true if button is checkable and checked */
			const void on_qpbEnqueue_clicked(bool checked = false);
																///< Enqueue button clicked on
																/**< \param checked true if button is checkable and checked */
			const void on_qpbOK_clicked(bool checked = false);
																///< OK button clicked on
																/**< \param checked true if button is checkable and checked */
}; // cFileOperationDialog

#endif
