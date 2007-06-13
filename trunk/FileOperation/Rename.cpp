#include "FileOperation/Rename.h"

#include <QtGui/QInputDialog>

// show conflict dialog (single thread)
QString cRename::Exec(const QString &qsOldFilename)
{
	return ShowDialog(qsOldFilename);
} // Exec

// show conflict dialog (multithread)
void cRename::Show(const QString &qsOldFilename)
{
	QString qsNewName;

	qsNewName = ShowDialog(qsOldFilename);

	if (qsNewName.isEmpty()) {
		emit Finished(NULL);
	} else {
		emit Finished(qsNewName);
	} // if else
} // Show

// show conflict dialog
QString cRename::ShowDialog(const QString &qsOldFilename)
{
	QString qsNewName;

	qsNewName = QInputDialog::getText(NULL, tr("Rename"), tr("New filename:"));

	return qsNewName;
} // ShowDialog
