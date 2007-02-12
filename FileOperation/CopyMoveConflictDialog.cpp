#include "FileOperation/CopyMoveConflictDialog.h"

// constructor
cCopyMoveConflictDialog::cCopyMoveConflictDialog(QMainWindow *qmwParent)
{
	setParent(qmwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);
} // cCopyMoveDialog

// append button is clicked on
void cCopyMoveConflictDialog::on_qpbAppend_clicked(bool checked /* false */)
{
	done(Append);
} // on_qpbAppend_clicked

// cancel button is clicked on
void cCopyMoveConflictDialog::on_qpbCancel_clicked(bool checked /* false */)
{
	done(Cancel);
} // on_qpbCancel_clicked

// overwrite button is clicked on
void cCopyMoveConflictDialog::on_qpbOverwrite_clicked(bool checked /* false */)
{
	done(Overwrite);
} // on_qpbOverwrite_clicked

// overwrite all button is clicked on
void cCopyMoveConflictDialog::on_qpbOverwriteAll_clicked(bool checked /* false */)
{
	done(OverwriteAll);
} // on_qpbOverwriteAll_clicked

// overwrite all older button is clicked on
void cCopyMoveConflictDialog::on_qpbOverwriteAllOlder_clicked(bool checked /* false */)
{
	done(OverwriteAllOlder);
} // on_qpbOverwriteAllOlder_clicked

// rename button is clicked on
void cCopyMoveConflictDialog::on_qpbRename_clicked(bool checked /* false */)
{
	done(Rename);
} // on_qpbRename_clicked

// skip button is clicked on
void cCopyMoveConflictDialog::on_qpbSkip_clicked(bool checked /* false */)
{
	done(Skip);
} // on_qpbSkip_clicked

// skip all button is clicked on
void cCopyMoveConflictDialog::on_qpbSkipAll_clicked(bool checked /* false */)
{
	done(SkipAll);
} // on_qpbSkipAll_clicked