#include "FileSystem/Rename.h"

#include <QtGui/QInputDialog>

// show conflict dialog (multithread)
const void cRename::Show(const QString &qsOldFilename) const
{
	QString qsNewName;

	qsNewName = QInputDialog::getText(NULL, tr("Rename"), tr("New filename:"));

	if (qsNewName.isEmpty()) {
		emit Finished(NULL);
	} else {
		emit Finished(qsNewName);
	} // if else
} // Show