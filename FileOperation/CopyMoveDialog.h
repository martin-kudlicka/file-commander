/// foreground copy/move dialog

#ifndef COPY_MOVE_DIALOG_H
#define COPY_MOVE_DIALOG_H

#include "ui_CopyMoveDialog.h"

class cCopyMoveDialog : public QDialog, public Ui::qdCopyMove
{
	Q_OBJECT

	public:
		cCopyMoveDialog(QWidget *qwParent);						///< constructor
																			/**< \param qwParent parent widget (window) of this dialog */

	signals:
		void Cancel();													///< operation canceled

	private slots:
		void on_qpbCancel_clicked(bool checked = false);	///< copy button is clicked on
																			/**< \param checked true if button is checkable and checked */
}; // cCopyMoveDialog

#endif