/// search for files

#ifndef FIND_FILES_THREAD_H
#define FIND_FILES_THREAD_H

#include <QtCore/QThread>
#include "Settings.h"
#include "FileSystem.h"

class cFindFilesThread : public QThread
{
	Q_OBJECT

	public:
		void Start(const cSettings::sFindSettings &sfsSearch, cFileSystem *cfsFileSystem, const QString qsPath, const bool &bMarking);
														///< start of searching for files
														/**< \param sfsSearch search settings
															  \param cfsFileSystem file system to search in
															  \param qsPath path to start search on file system
															  \param bMarking true if called for marking files */
	private:
		bool bMarking;								///< true if called for marking files
		cFileSystem *cfsFileSystem;			///< file system to search in
		cSettings::sFindSettings sfsSearch;	///< current search settings

		void run();									///< thread's main code
}; // cFindFilesThread

#endif