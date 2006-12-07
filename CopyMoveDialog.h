/// dialog for copy and move accepting

#ifndef COPY_MOVE_DIALOG_H
#define COPY_MOVE_DIALOG_H

#include "ui_CopyMoveDialog.h"

class cCopyMoveDialog : public QDialog, private Ui::qdCopyMove
{
	Q_OBJECT

	public:
		enum eUserAction {			///< action done by user
			CancelAction,				///< cancel button selected
			OkAction,					///< OK button selected
			EnqueueAction				///< enqueue button selected
		};

		cCopyMoveDialog();			///< constructor

		eUserAction ShowDialog(const QString qsTitle, const QString qsCount, QString *qsDestination, QString *qsFilter);
											///< shows copy or move dialog
											/**< \param qsTitle title of dialog
												  \param qsCount string with count of files and directories
												  \param qsDestination destination path
												  \param qsFilter filter for operation
												  \return user action */

		private:
			eUserAction euaResult;	///< dialog result

		private slots:
			void on_qpbCancel_clicked(bool checked = false);
											///< Cancel button clicked on
											/**< \param checked true if button is checkable and checked */
			void on_qpbEnqueue_clicked(bool checked = false);
											///< Enqueue button clicked on
											/**< \param checked true if button is checkable and checked */
			void on_qpbOK_clicked(bool checked = false);
											///< OK button clicked on
											/**< \param checked true if button is checkable and checked */
}; // cCopyMoveDialog

#endif