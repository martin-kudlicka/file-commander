#include "FileOperation/CopyMoveConflict.h"

#include <QtCore/QDateTime>
#include <QtGui/QMessageBox>
#include <QtCore/QMetaType>
#include <QtGui/QPushButton>

// constructor
cCopyMoveConflict::cCopyMoveConflict()
{
	// to be able to emit signal to another thread with QFileInfo argument
	qRegisterMetaType<QFileInfo>("QFileInfo");
} // cCopyMoveConflict

// show conflict dialog (single thread)
cCopyMoveConflict::eChoice cCopyMoveConflict::Exec(const QString &qsOperation, const char *cSourceFileName, const int &iSourceSize, const QDateTime &qdtSourceDateTime, const QFileInfo &qfiDestination)
{
	return ShowDialog(qsOperation, tr("Overwrite %1\n\t%2 byte, %3\nWith %4\n\t%5 byte, %6").arg(qfiDestination.fileName()).arg(qfiDestination.size()).arg(qfiDestination.lastModified().toString()).arg(cSourceFileName).arg(iSourceSize).arg(qdtSourceDateTime.toString()));
} // Exec

// show conflict dialog (multithread)
void cCopyMoveConflict::Show(const QString &qsOperation, const QFileInfo &qfiSource, const QFileInfo &qfiDestination)
{
	eChoice ecResponse;

	ecResponse = ShowDialog(qsOperation, tr("Overwrite %1\n\t%2 byte, %3\nWith %4\n\t%5 byte, %6").arg(qfiDestination.fileName()).arg(qfiDestination.size()).arg(qfiDestination.lastModified().toString()).arg(qfiSource.fileName()).arg(qfiSource.size()).arg(qfiSource.lastModified().toString()));

	emit Finished(ecResponse);
} // Show

// show conflict dialog
cCopyMoveConflict::eChoice cCopyMoveConflict::ShowDialog(const QString &qsOperation, const QString &qsText)
{
	eChoice ecResponse;
	QMessageBox qmbDialog;
	QPushButton *qpbAppend, *qpbCancel, *qpbOverwrite, *qpbOverwriteAll, *qpbOverwriteAllOlder, *qpbRename, *qpbSkip, *qpbSkipAll;

	// prepare dialog
	qmbDialog.setIcon(QMessageBox::Warning);
	qmbDialog.setWindowTitle(qsOperation);
	qmbDialog.setText(qsText);
	qpbOverwrite = qmbDialog.addButton(tr("&Overwrite"), QMessageBox::YesRole);
	qpbCancel = qmbDialog.addButton(QMessageBox::Cancel);
	qpbOverwriteAll = qmbDialog.addButton(tr("Overwrite &all"), QMessageBox::YesRole);
	qpbOverwriteAllOlder = qmbDialog.addButton(tr("Overwrite all o&lder"), QMessageBox::YesRole);
	qpbSkip = qmbDialog.addButton(tr("&Skip"), QMessageBox::NoRole);
	qpbSkipAll = qmbDialog.addButton(tr("S&kip all"), QMessageBox::NoRole);
	qpbRename = qmbDialog.addButton(tr("&Rename"), QMessageBox::YesRole);
	qpbAppend = qmbDialog.addButton(tr("&Append"), QMessageBox::YesRole);

	qmbDialog.exec();

	if (qmbDialog.clickedButton() == qpbOverwrite) {
		ecResponse = Overwrite;
	} else {
		if (qmbDialog.clickedButton() == qpbCancel) {
			ecResponse = Cancel;
		} else {
			if (qmbDialog.clickedButton() == qpbOverwriteAll) {
				ecResponse = OverwriteAll;
			} else {
				if (qmbDialog.clickedButton() == qpbOverwriteAllOlder) {
					ecResponse = OverwriteAllOlder;
				} else {
					if (qmbDialog.clickedButton() == qpbSkip) {
						ecResponse = Skip;
					} else {
						if (qmbDialog.clickedButton() == qpbSkipAll) {
							ecResponse = SkipAll;
						} else {
							if (qmbDialog.clickedButton() == qpbRename) {
								ecResponse = Rename;
							} else {
								ecResponse = Append;
							} // if else
						} // if else
					} // if else
				} // if else
			} // if else
		} // if else
	} // if else

	return ecResponse;
} // ShowDialog