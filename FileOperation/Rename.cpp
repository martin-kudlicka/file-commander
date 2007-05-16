#include "FileOperation/Rename.h"

#include <QtGui/QInputDialog>

// show conflict dialog
void cRename::Show(const QString &qsOldFilename)
{
	QString qsNewName;

	qsNewName = QInputDialog::getText(NULL, tr("Rename"), tr("New filename:"));

	if (qsNewName.isEmpty()) {
		emit Finished(NULL);
	} else {
		emit Finished(qsNewName);
	} // if else
} // Show
