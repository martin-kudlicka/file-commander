#include "ArchiveOperation/UnpackFilesDialog.h"

#include <QtGui/QDirModel>

cUnpackFilesDialog::cUnpackFilesDialog(QMainWindow *qmwParent, const QString &qsDestination, cSettings *csSettings)
{
	setParent(qmwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);

	qcbDestination->setEditText(qsDestination);

	// completer
	qcDestination.setModel(new QDirModel(&qcDestination));
	qcbDestination->setCompleter(&qcDestination);
} // cUnpackFilesDialog
