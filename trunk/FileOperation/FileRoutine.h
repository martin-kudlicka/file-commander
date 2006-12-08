/// some common file operations

#ifndef FILE_ROUTINE_H
#define FILE_ROUTINE_H

#include <QFileInfoList>
#include <QDir>

class cFileRoutine
{
	public:
		/// file operation type
		enum eOperation {						///< type of file operation
			CopyOperation,						///< copy
			MoveOperation,						///< move
			DeleteOperation					///< delete
		};

		static QFileInfoList GetDirectoryContent(const QString qsPath, const QDir::Filters fFilters);
													///< lists directory for it's content
													/**< \param qsPath path to list
														  \param fFilters filter for directory listing
														  \return content of qsPath directory */
		static QFileInfoList GetSources(const QFileInfoList qfilFileAndDirList);
													///< return list of sources (within subdirectories too)
													/**< \param qfilFileAndDirList list of directories and files
														  \return source list */

}; // cFileRoutine

#endif