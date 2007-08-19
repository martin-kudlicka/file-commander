/// background delete window

#ifndef DELETE_WIDGET_H
#define DELETE_WIDGET_H

#include "ui_DeleteWidget.h"

class cDeleteWidget : public QWidget, private Ui::qwDelete
{
	Q_OBJECT

	public:
		cDeleteWidget();																					///< constructor

	signals:
		void Cancel() const;																				///< operation canceled

	private slots:
		const void on_cDeleteWidget_SetSource(const QString &qsSource) const;			///< set source file
																												/**< \param qsSource source file */
		const void on_cDeleteWidget_SetTotalMaximum(const qint64 &qi64Value) const;	///< set overall maximum
																												/**< \param qi64Value overall maximum */
		const void on_cDeleteWidget_SetTotalValue(const qint64 &qi64Value) const;		///< set overall progress
																												/**< \param qi64Value overall progress */
		const void on_qpbCancel_clicked(bool checked = false) const;						///< copy button is clicked on
																												/**< \param checked true if button is checkable and checked */
}; // cDeleteWidget

#endif
