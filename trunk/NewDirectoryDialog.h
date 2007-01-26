/// new directory dialog

#ifndef NEW_DIRECTORY_DIALOG_H
#define NEW_DIRECTORY_DIALOG_H

#include "ui_NewDirectoryDialog.h"

#include <QMainWindow>

class cNewDirectoryDialog : public QDialog, public Ui::qdNewDirectory
{
	Q_OBJECT

	public:
		cNewDirectoryDialog(QMainWindow *qmwParent);	///< constructor
																	/**< \param qmwParent parent window of this dialog */
}; // cNewDirectoryDialog

#endif