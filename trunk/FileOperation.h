// handling file operations

#ifndef FILE_OPERATION_H
#define FILE_OPERATION_H

#include "Panel.h"
#include <QMainWindow>
#include <QHBoxLayout>
#include <QQueue>
#include "FileOperation/CopyMove.h"

class cFileOperation : private QObject
{
	Q_OBJECT

	public:
		cFileOperation(QMainWindow *qmwParent, QHBoxLayout *qhblOperations);
																///< constructor
																/**< \param qmwParent parent window for dialogs
																	  \param qbnlOperations layout for bacground and queued windows */

		void Operate(const cFileRoutine::eOperation eoOperation, cPanel *cpSource, cPanel *cpDestination);
																///< prepare operation
																/**< \param eoOperation operation type
																	  \param cpSource source file panel
																	  \param cpDestination destination file panel */

	private:
		/// queued operation description
		struct sOperation {
			cFileRoutine::eOperation eoOperation;	///< operation type
			QFileInfoList qfilSource;					///< source file list
			QString qsDestination;						///< destination path
		};

		QMainWindow *qmwParent;							///< parent window for dialogs
		QHBoxLayout *qhblOperations;					///< background and queued operation windows
		QList<cCopyMove *> qlCopyMove;				///< list of copy / move threads
		QQueue<sOperation> qqQperations;				///< queued operations

		void Enque(const cFileRoutine::eOperation eoOperation, const QFileInfoList qfilSource, const QString qsDestination);
																///< place operation into queue
																/**< \param eoOperation type of operation
																	  \param qfilSource source file list
																	  \param qsDestination destination path */
		void ProcessQueue();								///< process first queued operation

	private slots:
		void on_cCopyMove_finished();					///< copy / move thread finished
}; // cFileOperation

#endif