#include "FileSystem/Archive/ArchiveDelete.h"

// constructor
cArchiveDelete::cArchiveDelete(QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings)
{
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
	this->csSettings = csSettings;
} // cLocalDelete

// start of delete operation
void cArchiveDelete::Delete(const QList<tHeaderData> &qlSource, const QString &qsFilter, const cFileOperation::eOperationPosition &eopPosition) const
{
} // Delete

// separate thread process
void cArchiveDelete::run()
{
} // run
