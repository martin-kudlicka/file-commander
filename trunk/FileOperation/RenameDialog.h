/// rename dialog

#ifndef RENAME_DIALOG_H
#define RENAME_DIALOG_H

#include "ui_RenameDialog.h"
#include <QMainWindow>

class cRenameDialog : public QDialog, public Ui::qdRename
{
	Q_OBJECT

	public:
		cRenameDialog(QMainWindow *qmwParent);	///< constructor
															/**< \param qmwParent parent window of this dialog */
}; // cRenameDialog

#endif