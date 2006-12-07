// handling file operations

#ifndef FILE_OPERATION_H
#define FILE_OPERATION_H

#include <QFileInfoList>
#include "Panel.h"
#include <QMainWindow>
#include <QHBoxLayout>

class cFileOperation : private QObject
{
	public:
		enum eOperation {					///< type of file operation
			CopyOperation,					///< copy
			MoveOperation,					///< move
			DeleteOperation				///< delete
		};

		cFileOperation(QMainWindow *qmwParent, QHBoxLayout *qhblOperations);
												///< constructor
												/**< \param qmwParent parent window for dialogs
													  \param qbnlOperations layout for bacground and queued windows */

		void Operate(const eOperation eoOperation, cPanel *cpSource, cPanel *cpDestination);
												///< prepare operation
												/**< \param eoOperation operation type
													  \param cpSource source file panel
													  \param cpDestination destination file panel */

	private:
		QMainWindow *qmwParent;			///< parent window for dialogs
		QHBoxLayout *qhblOperations;	///< background and queued operation windows
}; // cFileOperation

#endif