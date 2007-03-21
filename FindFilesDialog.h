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
		cPanel *cpPanel;															///< panel to work with

	private slots:
		void on_qpbDrives_clicked(bool checked = false);				///< drives button is clicked on
																						/**< \param checked true if button is checkable and checked */
}; // cFindFilesDialog

#endif