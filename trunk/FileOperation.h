// handling file operations

#ifndef FILE_OPERATION_H
#define FILE_OPERATION_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QQueue>
#include "FileOperation/CopyMove.h"
#include "FileOperation/QueueWidget.h"
#include "FileOperation/Delete.h"

class cFileOperation : private QObject
{
	Q_OBJECT

	public:
		cFileOperation(QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings);
																			///< constructor
																			/**< \param qmwParent parent window for dialogs
																				  \param qbnlOperations layout for bacground and queued windows
																				  \param csSettings application's configuration */

		void Operate(const cFileRoutine::eOperation &eoOperation, const QFileInfoList &qfilSource, QString &qsDestination = QString());
																			///< prepare operation
																			/**< \param eoOperation operation type
																				  \param qfilSource source file list
																				  \param qsDestinationPath destination path */

	private:
		// count of objects
		struct sObjects {
			uint Directories;													///< number of directories
			uint Files;															///< number of files
		};

		/// queued operation description
		struct sOperation {
			cFileRoutine::eOperation eoOperation;				///< operation type
			QFileInfoList qfilSource;								///< source file list
			QString qsDestination;									///< destination path
			QListWidgetItem *qlwiItem;								///< item listed in queue widget
		};

		static const int iQUEUE_WIDGET_POS = 0;				///< position of queue widget on top layout

		cCopyMove *ccmInQueue;										///< processed copy / move operation in queue
		cSettings *csSettings;										///< application's configuration
		cDelete *cdInQueue;											///< processed delete operation in queue
		cQueueWidget cqwQueue;										///< list of queued operations
		QMainWindow *qmwParent;										///< parent window for dialogs
		QHBoxLayout *qhblOperations;								///< background and queued operation windows
		QList<cCopyMove *> qlCopyMove;							///< list of copy / move threads
		QList<cDelete *> qlDelete;									///< list of delete threads
		QQueue<sOperation> qqQperations;							///< queued operations

		void Enque(const cFileRoutine::eOperation &eoOperation, const QFileInfoList &qfilSource, const QString &qsDestination);
																			///< place operation into queue
																			/**< \param eoOperation type of operation
																				  \param qfilSource source file list
																				  \param qsDestination destination path */
		sObjects GetCount(const QFileInfoList &qfilObjects);
																			///< count of objects
																			/**< \param qfilObjects objects to count
																				  \return count of objects */
		void ProcessQueue();											///< process first queued operation

	signals:
		void AddIntoQueueList(QListWidgetItem *qlwiItem);	///< add new item into queue list
																			/**< \param qlwiItem new item (operation) */
	private slots:
		void on_cCopyMove_finished();								///< copy / move thread finished
		void on_cDelete_finished();								///< delete thread finished
		void on_cqwQueue_RemoveQueuedItems(QList<QListWidgetItem *> qlItems);
																			///< remove queued items (operations)
																			/**< \param qlItems operations to remove */
}; // cFileOperation

#endif