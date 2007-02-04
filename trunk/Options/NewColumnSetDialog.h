/// new column set dialog

#ifndef NEW_COLUMN_SET_DIALOG_H
#define NEW_COLUMN_SET_DIALOG_H

#include "ui_NewColumnSetDialog.h"


class cNewColumnSetDialog : public QDialog, public Ui::qdNewColumnSet
{
	Q_OBJECT

	public:
		cNewColumnSetDialog(QDialog *qdParent);	///< constructor
																/**< \param qdParent parent window of this dialog */
}; // cNewColumnSetDialog

#endif