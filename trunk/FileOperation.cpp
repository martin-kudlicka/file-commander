#include "FileOperation.h"

#include "CopyMoveDialog.h"

// copy files
void cFileOperation::Copy(cPanel *cpSource, cPanel *cpDestination)
{
	QFileInfoList qfilSource;
	QString qsDestination;

	qfilSource = cpSource->GetSelectedItemsList();
	qsDestination = cpDestination->GetPath();

	Process(CopyOperation, qfilSource, qsDestination);
} // Copy

// process file operation
void cFileOperation::Process(const eOperation eoOperation, const QFileInfoList qfilSource, QString qsDestination)
{
	cCopyMoveDialog ccmdDialog;
	cPanel::sObjects soObjects;
	cCopyMoveDialog::eUserAction euaAction;
	QString qsFilter;

	if (qfilSource.count() == 0) {
		// no items selected
		return;
	} // if

	soObjects = cPanel::GetCount(qfilSource);
	euaAction = ccmdDialog.ShowDialog(tr("Copy"),
												 QString("%1 files and %2 directories to:").arg(soObjects.Files).arg(soObjects.Directories),
												 &qsDestination,
												 &qsFilter);

	if (euaAction == cCopyMoveDialog::CancelAction) {
		return;
	} // if

	// TODO Process
} // Process