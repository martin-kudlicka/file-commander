/// search for files

#ifndef FIND_FILES_THREAD_H
#define FIND_FILES_THREAD_H

#include <QtCore/QThread>
#include "Settings.h"
#include "FileSystem.h"
#include <QtCore/QQueue>

const QString qsDAYS = "day(s)";
const QString qsHOURS = "hour(s)";
const QString qsKILOBYTES2 = "kilobyte(s)";
const QString qsMEGABYTES2 = "megabyte(s)";
const QString qsMONTHS = "month(s)";
const QString qsWEEKS = "week(s)";

class cFindFilesThread : public QThread
{
	Q_OBJECT

	public:
		void Start(const cSettings::sFindSettings &sfsSearch, cFileSystem *cfsFileSystem, const QString qsPath, const bool &bRunAsThread = true, const bool &bBranchView = false);
																						///< start of searching for files
																						/**< \param sfsSearch search settings
																							  \param cfsFileSystem file system to search in
																							  \param qsPath path to start search on file system
																							  \param bRunAsThread true run in separate thread
																							  \param bBranchView true if started for branch view */
	private:
		static const qint64 qi64SEARCH_BUFFER = 1048576;				///< search for text in files in this buffer size

		bool bBranchView;															///< true if started for branch view
		bool bStop;																	///< stop searching
		cFileSystem *cfsFileSystem;											///< file system to search in
		cSettings::sFindSettings sfsSearch;									///< current search settings
		QQueue<QString> qqDirectories;										///< directories to go through
		QString qsPath;															///< original path

		const bool ConditionSuit(QTreeWidgetItem *qtwiFile) const;	///< check search conditions on found file
																						/**< \param qtwiFile file to check conditions on
																							  \return true if conditions ok */
		void run();																	///< thread's main code

	signals:
		void Found(QTreeWidgetItem *qtwiFile, cFileSystem *cfsFileSystem) const;
																						///< found file matches conditions
																						/**< \param qtwiFile found file
																							  \param cfsFileSystem file system found in */

	private slots:
		const void on_StopSearching();										///< interrupt thread searching
}; // cFindFilesThread

#endif