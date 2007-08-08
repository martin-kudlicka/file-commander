#include "FindFilesDialog/FindFilesThread.h"

// thread's main code
void cFindFilesThread::run()
{
} // run

// start of searching for files
void cFindFilesThread::Start(const cSettings::sFindSettings &sfsSearch, cFileSystem *cfsFileSystem, const QString qsPath, const bool &bMarking)
{
	this->sfsSearch = sfsSearch;
	this->cfsFileSystem = cfsFileSystem;
	this->bMarking = bMarking;

	cfsFileSystem->SetPath(QString(), QString(), qsPath);

	run();
} // Start