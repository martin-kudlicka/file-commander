/// pack files dialog

#ifndef PACK_FILES_DIALOG_H
#define PACK_FILES_DIALOG_H

#include "ui_PackFilesDialog.h"

#include "Settings.h"
#include <QtGui/QMainWindow>
#include <QtGui/QCompleter>

class cPackFilesDialog : public QDialog, public Ui::qdPackFiles
{
	Q_OBJECT

	public:
		cPackFilesDialog(QMainWindow *qmwParent, cSettings *csSettings);
																			///< constructor
																			/**< \param qmwParent parent widget (window) of this dialog
																				  \param csSettings application's settings */

	private:
		cSettings *csSettings;										///< application's settings file
		QCompleter qcDestination;									///< completer for destination combo box

	private slots:
		const void on_qpbCancel_clicked(bool checked = false);
																			///< Cancel button clicked on
																			/**< \param checked true if button is checkable and checked */
		const void on_qpbEnqueue_clicked(bool checked = false);
																			///< Enqueue button clicked on
																			/**< \param checked true if button is checkable and checked */
		const void on_qpbOK_clicked(bool checked = false);	///< OK button is clicked on
																			/**< \param checked true if button is checkable and checked */
}; // cPackFilesDialog

#endif
