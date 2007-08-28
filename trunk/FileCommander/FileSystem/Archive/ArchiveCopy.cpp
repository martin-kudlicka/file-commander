#include "FileSystem/Archive/ArchiveCopy.h"

// constructor
cArchiveCopy::cArchiveCopy(QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings)
{
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
	this->csSettings = csSettings;

	//bCanceled = false;
	//qi64TotalMaximum = 0;
} // cLocalCopyMove

// separate thread process
void cArchiveCopy::run()
{
} // run