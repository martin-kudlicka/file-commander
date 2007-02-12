#include "FileOperation/CopyMoveConflict.h"

#include <QDateTime>

// destructor
cCopyMoveConflict::~cCopyMoveConflict()
{
	ccmcdDialog->deleteLater();
} // ~cCopyMoveConflict

// constructor
cCopyMoveConflict::cCopyMoveConflict(QMainWindow *qmwParent)
{
	ccmcdDialog = new cCopyMoveConflictDialog(qmwParent);
	// to be able to emit signal to another thread with QFileInfo argument
	qRegisterMetaType<QFileInfo>("QFileInfo");
} // cCopyMoveConflict

// show conflict dialog
void cCopyMoveConflict::Show(const QString &qsOperation, const QFileInfo &qfiSource, const QFileInfo &qfiDestination)
{
	cCopyMoveConflictDialog::eChoice ecResponse;

	// set labels
	ccmcdDialog->setWindowTitle(qsOperation);
	ccmcdDialog->qlSourceFilename->setText(qfiSource.fileName());
	ccmcdDialog->qlSourceInfo->setText(tr("%1 byte, %2").arg(qfiSource.size()).arg(qfiSource.lastModified().toString()));
	ccmcdDialog->qlDestinationFilename->setText(qfiDestination.fileName());
	ccmcdDialog->qlDestinationInfo->setText(tr("%1 byte, %2").arg(qfiDestination.size()).arg(qfiDestination.lastModified().toString()));

	ecResponse = static_cast<cCopyMoveConflictDialog::eChoice>(ccmcdDialog->exec());
	emit Finished(ecResponse);
} // Show