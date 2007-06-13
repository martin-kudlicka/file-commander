#include "ArchiveOperation/UnpackFilesDialog.h"

cUnpackFilesDialog::cUnpackFilesDialog(QMainWindow *qmwParent, const QString &qsDestination, cSettings *csSettings)
{
	setParent(qmwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);

	qleDestination->setText(qsDestination);
} // cUnpackFilesDialog
