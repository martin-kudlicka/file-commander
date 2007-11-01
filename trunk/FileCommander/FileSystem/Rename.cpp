#include "FileSystem/Rename.h"

#include <QtGui/QInputDialog>

// constructor
cRename::cRename(QMainWindow *qmwParent)
{
	this->qmwParent = qmwParent;
} // cRename

// show conflict dialog
const void cRename::Show(QString *qsNewFilename) const
{
	*qsNewFilename = QInputDialog::getText(qmwParent, tr("Rename"), tr("New filename:"), QLineEdit::Normal, *qsNewFilename);

	emit Finished();
} // Show
