/// foreground copy/move dialog

#ifndef COPY_MOVE_DIALOG_H
#define COPY_MOVE_DIALOG_H

#include "ui_CopyMoveDialog.h"

#include <QtGui/QMainWindow>

class cCopyMoveDialog : public QDialog, public Ui::qdCopyMove
{
	Q_OBJECT

	public:
		cCopyMoveDialog(QMainWindow *qmwParent, const bool &bArchive = false);
																						///< constructor
																						/**< \param qmwParent parent window of this dialog
																							  \param bArchive for use with archive files */

	signals:
		void Background();														///< set operation as background
		void Cancel();																///< operation canceled

	private slots:
		void on_cCopyMove_SetCurrentMaximum(const qint64 &qi64Value);
																						///< set maximum for current file
																						/**< \param qi64Value maximum for current file */
		void on_cCopyMove_SetCurrentValue(const qint64 &qi64Value);	///< set progress for current file
																						/**< \param qi64Value progress for current file */
		void on_cCopyMove_SetDestination(const QString &qsDestination);
																						///< set destination file
																						/**< \param qsDestination destination file */
		void on_cCopyMove_SetSource(const QString &qsSource);			///< set source file
																						/**< \param qsSource source file */
		void on_cCopyMove_SetTotalMaximum(const qint64 &qi64Value);	///< set overall maximum
																						/**< \param qi64Value overall maximum */
		void on_cCopyMove_SetTotalValue(const qint64 &qi64Value);	///< set overall progress
																						/**< \param qi64Value overall progress */
		void on_qpbBackground_clicked(bool checked = false);			///< Background button is clicked on
																						/**< \param checked true if button is checkable and checked */
		void on_qpbCancel_clicked(bool checked = false);				///< Cancel button is clicked on
																						/**< \param checked true if button is checkable and checked */
}; // cCopyMoveDialog

#endif
