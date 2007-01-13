/// background delete window

#ifndef DELETE_WIDGET_H
#define DELETE_WIDGET_H

#include "ui_DeleteWidget.h"

class cDeleteWidget : public QWidget, private Ui::qwDelete
{
	Q_OBJECT

	public:
		cDeleteWidget();															///< constructor

	signals:
		void Cancel();																///< operation canceled

	private slots:
		void on_cDeleteWidget_SetSource(const QString qsSource);		///< set source file
																						/**< \param qsSource source file */
		void on_cDeleteWidget_SetTotalMaximum(const qint64 qi64Value);
																						///< set overall maximum
																						/**< \param qi64Value overall maximum */
		void on_cDeleteWidget_SetTotalValue(const qint64 qi64Value);
																						///< set overall progress
																						/**< \param qi64Value overall progress */
		void on_qpbCancel_clicked(bool checked = false);				///< copy button is clicked on
																						/**< \param checked true if button is checkable and checked */
}; // cDeleteWidget

#endif