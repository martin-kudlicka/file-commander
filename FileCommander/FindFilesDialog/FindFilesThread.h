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
		/// thread called from
		enum eCalledFrom {
			FindFilesDialog,														///< Find files dialog
			BranchView,																///< branch view
			SelectFiles																///< select/unselect files
		};

		void Start(const cSettings::sFindSettings &sfsSearch, cFileSystem *cfsFileSystem, const QString qsPath, const eCalledFrom &ecfCalledFrom = FindFilesDialog);
																						///< start of searching for files
																						/**< \param sfsSearch search settings
																							  \param cfsFileSystem file system to search in
																							  \param qsPath path to start search on file system
																							  \param ecfCalledFrom thread called from */
	private:
		static const qint64 qi64SEARCH_BUFFER = 1048576;				///< search for text in files in this buffer size

		bool bStop;																	///< stop searching
		cFileSystem *cfsFileSystem;											///< file system to search in
		cSettings::sFindSettings sfsSearch;									///< current search settings
		eCalledFrom ecfCalledFrom;												///< thread called from
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
