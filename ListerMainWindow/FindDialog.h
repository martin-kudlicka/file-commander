/// lister find dialog

#ifndef FIND_DIALOG_H
#define FIND_DIALOG_H

#include "ui_FindDialog.h"

#include <QtGui/QMainWindow>

class cFindDialog : public QDialog, public Ui::qdFind
{
	Q_OBJECT

	public:
		cFindDialog(QMainWindow *qmwParent, const bool &bPlugin);	///< constructor
																						/**< \param qwParent parent window of this dialog
																							  \param bPlugin true if search in plugin */
}; // cFindDialog

#endif
