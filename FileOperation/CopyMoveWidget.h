/// background copy/move window

#ifndef COPY_MOVE_WIDGET_H
#define COPY_MOVE_WIDGET_H

#include "ui_CopyMoveWidget.h"

class cCopyMoveWidget : public QWidget, private Ui::qwCopyMove
{
	Q_OBJECT

	public:
		cCopyMoveWidget();														///< constructor

	signals:
		void Cancel();																///< operation canceled

	private slots:
		void on_cCopyMove_SetCurrentMaximum(const qint64 qi64Value);
																						///< set maximum for current file
																						/**< \param qi64Value maximum for current file */
		void on_cCopyMove_SetCurrentValue(const qint64 qi64Value);	///< set progress for current file
																						/**< \param qi64Value progress for current file */
		void on_cCopyMove_SetDestination(const QString qsDestination);
																						///< set destination file
																						/**< \param qsDestination destination file */
		void on_cCopyMove_SetSource(const QString qsSource);			///< set source file
																						/**< \param qsSource source file */
		void on_cCopyMove_SetTotalMaximum(const qint64 qi64Value);	///< set overall maximum
																						/**< \param qi64Value overall maximum */
		void on_cCopyMove_SetTotalValue(const qint64 qi64Value);		///< set overall progress
																						/**< \param qi64Value overall progress */
		void on_qpbCancel_clicked(bool checked = false);				///< copy button is clicked on
																						/**< \param checked true if button is checkable and checked */
}; // cCopyMoveWidget

#endif