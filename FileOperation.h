// handling file operations

#ifndef FILE_OPERATION_H
#define FILE_OPERATION_H

#include <QFileInfoList>
#include "Panel.h"
#include <QMainWindow>
#include <QHBoxLayout>
#include <QQueue>

class cFileOperation : private QObject
{
	public:
		enum eOperation {						///< type of file operation
			CopyOperation,						///< copy
			MoveOperation,						///< move
			DeleteOperation					///< delete
		};

		cFileOperation(QMainWindow *qmwParent, QHBoxLayout *qhblOperations);
													///< constructor
													/**< \param qmwParent parent window for dialogs
														  \param qbnlOperations layout for bacground and queued windows */

		static QFileInfoList GetDirectoryContent(const QString qsPath, const QDir::Filters fFilters);
													///< lists directory for it's content
													/**< \param qsPath path to list
														  \param fFilters filter for directory listing
														  \return content of qsPath directory */
		static QFileInfoList GetSources(const QFileInfoList qfilFileAndDirList);
													///< return list of sources (within subdirectories too)
													/**< \param qfilFileAndDirList list of directories and files
														  \return source list */
		void Operate(const eOperation eoOperation, cPanel *cpSource, cPanel *cpDestination);
													///< prepare operation
													/**< \param eoOperation operation type
														  \param cpSource source file panel
														  \param cpDestination destination file panel */

	private:
		/// queued operation description
		struct sOperation {
			eOperation eoOperation;			///< operation type
			QFileInfoList qfilSource;		///< source file list
			QString qsDestination;			///< destination path
		};

		QMainWindow *qmwParent;				///< parent window for dialogs
		QHBoxLayout *qhblOperations;		///< background and queued operation windows
		QQueue<sOperation> qqQperations;	///< queued operations

		void Enque(const eOperation eoOperation, const QFileInfoList qfilSource, const QString qsDestination);
													///< place operation into queue
													/**< \param eoOperation type of operation
														  \param qfilSource source file list
														  \param qsDestination destination path */
		void ProcessQueue();					///< process first queued operation
}; // cFileOperation

#endif