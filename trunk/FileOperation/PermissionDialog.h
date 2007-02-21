/// permission dialog

#ifndef PERMISSION_DIALOG_H
#define PERMISSION_DIALOG_H

#include "ui_PermissionDialog.h"
#include <QMainWindow>

class cPermissionDialog : public QDialog, public Ui::qdPermission
{
	Q_OBJECT

	public:
		/// user response
		enum eChoice {
			Ask,															///< no permanent answer yet
			Yes,															///< yes
			No,															///< no
			YesToAll,													///< yes to all
			NoToAll,														///< no to all
			Cancel														///< cancel
		};

		cPermissionDialog(QMainWindow *qmwParent);			///< constructor
																			/**< \param qmwParent parent window of this dialog */

	private slots:
		void on_qpbCancel_clicked(bool checked = false);	///< cancel button is clicked on
																			/**< \param checked true if button is checkable and checked */
		void on_qpbNo_clicked(bool checked = false);			///< no button is clicked on
																			/**< \param checked true if button is checkable and checked */
		void on_qpbNoToAll_clicked(bool checked = false);	///< no to all button is clicked on
																			/**< \param checked true if button is checkable and checked */
		void on_qpbYes_clicked(bool checked = false);		///< yes button is clicked on
																			/**< \param checked true if button is checkable and checked */
		void on_qpbYesToAll_clicked(bool checked = false);	///< yes to all button is clicked on
																			/**< \param checked true if button is checkable and checked */
}; // cPermissionDialog

#endif