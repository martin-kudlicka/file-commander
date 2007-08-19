/// foreground delete dialog

#ifndef DELETE_DIALOG_H
#define DELETE_DIALOG_H

#include "ui_DeleteDialog.h"

#include <QtGui/QMainWindow>

class cDeleteDialog : public QDialog, private Ui::qdDelete
{
	Q_OBJECT

	public:
		cDeleteDialog(QMainWindow *qmwParent);														///< constructor
																												/**< \param qmwParent parent window of this dialog */

	signals:
		void Background() const;																		///< set operation as background
		void Cancel() const;																				///< operation canceled

	private slots:
		const void on_cDeleteDialog_SetSource(const QString &qsSource) const;			///< set source file
																												/**< \param qsSource source file */
		const void on_cDeleteDialog_SetTotalMaximum(const qint64 &qi64Value) const;	///< set overall maximum
																												/**< \param qi64Value overall maximum */
		const void on_cDeleteDialog_SetTotalValue(const qint64 &qi64Value) const;		///< set overall progress
																												/**< \param qi64Value overall progress */
		const void on_qpbBackground_clicked(bool checked = false) const;					///< Background button is clicked on
																												/**< \param checked true if button is checkable and checked */
		const void on_qpbCancel_clicked(bool checked = false) const;						///< Cancel button is clicked on
																												/**< \param checked true if button is checkable and checked */
}; // cDeleteDialog

#endif
