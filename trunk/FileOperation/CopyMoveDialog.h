/// foreground copy/move dialog

#ifndef COPY_MOVE_DIALOG_H
#define COPY_MOVE_DIALOG_H

#include "ui_CopyMoveDialog.h"

class cCopyMoveDialog : public QDialog, public Ui::qdCopyMove
{
	public:
		cCopyMoveDialog(QWidget *qwParent);	///< constructor
														/**< \param qwParent parent widget (window) of this dialog */
}; // cCopyMoveDialog

#endif