#include "FileOperation.h"

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
void cFileOperation::Process(const eOperation eoOperation, const QFileInfoList qfilSource, const QString qsDestination)
{
	if (qfilSource.count() == 0) {
		// no items selected
		return;
	} // if

	// TODO Process
} // Process