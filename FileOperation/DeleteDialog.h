/// foreground delete dialog

#ifndef DELETE_DIALOG_H
#define DELETE_DIALOG_H

#include "ui_DeleteDialog.h"

class cDeleteDialog : public QDialog, private Ui::qdDelete
{
	Q_OBJECT

	public:
		cDeleteDialog(QWidget *qwParent);								///< constructor
																					/**< \param qwParent parent widget (window) of this dialog */

	signals:
		void Background();													///< set operation as background
		void Cancel();															///< operation canceled

	private slots:
		void on_cDeleteDialog_SetSource(const QString qsSource);	///< set source file
																					/**< \param qsSource source file */
		void on_cDeleteDialog_SetTotalMaximum(const qint64 qi64Value);
																					///< set overall maximum
																					/**< \param qi64Value overall maximum */
		void on_cDeleteDialog_SetTotalValue(const qint64 qi64Value);
																					///< set overall progress
																					/**< \param qi64Value overall progress */
		void on_qpbBackground_clicked(bool checked = false);		///< Background button is clicked on
																					/**< \param checked true if button is checkable and checked */
		void on_qpbCancel_clicked(bool checked = false);			///< Cancel button is clicked on
																					/**< \param checked true if button is checkable and checked */
}; // cDeleteDialog

#endif