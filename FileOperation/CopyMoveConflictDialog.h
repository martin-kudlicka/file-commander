/// copy/move conflict dialog

#ifndef COPY_MOVE_CONFLICT_DIALOG_H
#define COPY_MOVE_CONFLICT_DIALOG_H

#include "ui_CopyMoveConflictDialog.h"
#include <QMainWindow>

class cCopyMoveConflictDialog : public QDialog, public Ui::qdCopyMoveConflict
{
	Q_OBJECT

	public:
		/// user response
		enum eChoice {
			Ask,																		///< no response yet
			Overwrite,																///< overwrite once
			Cancel,																	///< cancel
			OverwriteAll,															///< overwrite all
			OverwriteAllOlder,													///< overwrite all older
			Skip,																		///< skip once
			SkipAll,																	///< skip all
			Rename,																	///< rename
			Append																	///< append to another file
		};

		cCopyMoveConflictDialog(QMainWindow *qmwParent);				///< constructor
																						/**< \param qmwParent parent window of this dialog */

	private slots:
		void on_qpbAppend_clicked(bool checked = false);				///< append button is clicked on
																						/**< \param checked true if button is checkable and checked */
		void on_qpbCancel_clicked(bool checked = false);				///< cancel button is clicked on
																						/**< \param checked true if button is checkable and checked */
		void on_qpbOverwrite_clicked(bool checked = false);			///< overwrite button is clicked on
																						/**< \param checked true if button is checkable and checked */
		void on_qpbOverwriteAll_clicked(bool checked = false);		///< overwrite all button is clicked on
																						/**< \param checked true if button is checkable and checked */
		void on_qpbOverwriteAllOlder_clicked(bool checked = false);	///< overwrite all older button is clicked on
																						/**< \param checked true if button is checkable and checked */
		void on_qpbRename_clicked(bool checked = false);				///< rename button is clicked on
																						/**< \param checked true if button is checkable and checked */
		void on_qpbSkip_clicked(bool checked = false);					///< skip button is clicked on
																						/**< \param checked true if button is checkable and checked */
		void on_qpbSkipAll_clicked(bool checked = false);				///< skip all button is clicked on
																						/**< \param checked true if button is checkable and checked */
}; // cCopyMoveConflictDialog

#endif