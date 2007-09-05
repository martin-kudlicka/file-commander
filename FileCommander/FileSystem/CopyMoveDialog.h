/// foreground copy/move dialog

#ifndef COPY_MOVE_DIALOG_H
#define COPY_MOVE_DIALOG_H

#include "ui_CopyMoveDialog.h"

#include <QtGui/QMainWindow>

class cCopyMoveDialog : public QDialog, private Ui::qdCopyMove
{
	Q_OBJECT

	public:
		cCopyMoveDialog(QMainWindow *qmwParent);													///< constructor
																												/**< \param qmwParent parent window of this dialog
																													  \param bArchive for use with archive files */

	signals:
		void Background() const;																		///< set operation as background
		void Cancel() const;																				///< operation canceled

	private slots:
		const void on_cCopyMove_SetCurrentMaximum(const qint64 &qi64Value) const;		///< set maximum for current file
																												/**< \param qi64Value maximum for current file */
		const void on_cCopyMove_SetCurrentValue(const qint64 &qi64Value) const;			///< set progress for current file
																												/**< \param qi64Value progress for current file */
		const void on_cCopyMove_SetDestination(const QString &qsDestination) const;	///< set destination file
																												/**< \param qsDestination destination file */
		const void on_cCopyMove_SetSource(const QString &qsSource) const;					///< set source file
																												/**< \param qsSource source file */
		const void on_cCopyMove_SetTotalMaximum(const qint64 &qi64Value) const;			///< set overall maximum
																												/**< \param qi64Value overall maximum */
		const void on_cCopyMove_SetTotalValue(const qint64 &qi64Value) const;			///< set overall progress
																												/**< \param qi64Value overall progress */
		const void on_qpbBackground_clicked(bool checked = false) const;					///< Background button is clicked on
																												/**< \param checked true if button is checkable and checked */
		const void on_qpbCancel_clicked(bool checked = false) const;						///< Cancel button is clicked on
																												/**< \param checked true if button is checkable and checked */
}; // cCopyMoveDialog

#endif
