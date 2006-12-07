// handling file operations

#ifndef FILE_OPERATION_H
#define FILE_OPERATION_H

#include <QFileInfoList>
#include "Panel.h"
#include <QMainWindow>

class cFileOperation : private QObject
{
	public:
		enum eOperation {									///< type of file operation
			CopyOperation,									///< copy
			MoveOperation,									///< move
			DeleteOperation								///< delete
		};

		cFileOperation(QMainWindow *qmwParent);	///< constructor, set parent window for dialogs
																/**< \param qmwParent parent window for dialogs */

		void Operate(const eOperation eoOperation, cPanel *cpSource, cPanel *cpDestination);
																///< prepare operation
																/**< \param eoOperation operation type
																	  \param cpSource source file panel
																	  \param cpDestination destination file panel */

	private:
		QMainWindow *qmwParent;							///< parent window for dialogs
}; // cFileOperation

#endif