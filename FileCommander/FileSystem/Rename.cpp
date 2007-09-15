#include "FileSystem/Rename.h"

#include <QtGui/QInputDialog>

// show conflict dialog
const void cRename::Show(QString *qsNewFilename) const
{
	*qsNewFilename = QInputDialog::getText(NULL, tr("Rename"), tr("New filename:"));

	emit Finished();
} // Show
