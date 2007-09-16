/// unpack files dialog

#ifndef UNPACK_FILES_DIALOG_H
#define UNPACK_FILES_DIALOG_H

#include "ui_UnpackFilesDialog.h"

#include "Settings.h"
#include <QtGui/QMainWindow>
#include <QtGui/QCompleter>

class cUnpackFilesDialog : public QDialog, public Ui::qdUnpackFiles
{
	Q_OBJECT

	public:
		cUnpackFilesDialog(QMainWindow *qmwParent, const QString &qsDestination, cSettings *csSettings);
																			///< constructor
																			/**< \param qmwParent parent widget (window) of this dialog
																				  \param qsDestination destination path for extraction
																				  \param csSettings application's settings */

	private:
		cSettings *csSettings;										///< application's settings file
		QCompleter qcDestination;									///< completer for destination combo box

	private slots:
		const void on_qpbOK_clicked(bool checked = false);	///< OK button is clicked on
																			/**< \param checked true if button is checkable and checked */
}; // cUnpackFilesDialog

#endif
