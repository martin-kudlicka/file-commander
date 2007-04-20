/// retry dialog

#ifndef RETRY_DIALOG_H
#define RETRY_DIALOG_H

#include "ui_RetryDialog.h"

#include <QMainWindow>

class cRetryDialog : public QDialog, public Ui::qdRetry
{
	Q_OBJECT

	public:
		/// user response
		enum eChoice {
			Ask,															///< no permanent choice
			Skip,															///< skip file
			Retry,														///< retry operation
			SkipAll,														///< skip all files with same problem
			Abort															///< abort whole operation
		};

		cRetryDialog(QMainWindow *qmwParent);					///< constructor
																			/**< \param qmwParent parent window of this dialog */

	private slots:
		void on_qpbAbort_clicked(bool checked = false);		///< abort button is clicked on
																			/**< \param checked true if button is checkable and checked */
		void on_qpRetry_clicked(bool checked = false);		///< retry button is clicked on
																			/**< \param checked true if button is checkable and checked */
		void on_qpbSkip_clicked(bool checked = false);		///< skip button is clicked on
																			/**< \param checked true if button is checkable and checked */
		void on_qpbSkipAll_clicked(bool checked = false);	///< skip all button is clicked on
																			/**< \param checked true if button is checkable and checked */
}; // cRetryDialog

#endif