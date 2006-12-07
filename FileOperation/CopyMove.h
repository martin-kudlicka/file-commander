/// handling copying and moving files

#ifndef COPY_MOVE_H
#define COPY_MOVE_H

#include <QHBoxLayout>
#include "FileOperation.h"

class cCopyMove {
	public:
		/// window style
		enum eWindow {
			ForegroundWindow,								///< modal foreground window
			BackgroundWindow								///< window in qhblOperations layout
		};

		cCopyMove(QMainWindow *qmwParent, QHBoxLayout *qhblOperations);
																///< constructor
																/**< \param qmwParent parent window for foreground dialog
																	  \param qhblOperations layout for background widget */

		void CopyMove(const cFileOperation::eOperation eoOperation, const QFileInfoList qfilSource, const QString qsDestination, const eWindow eStyle);
																///< start of copy or move operation
																/**< \param eoOperation copy or move operation
																	  \param qfilSource source file list
																	  \param qsDestination destination path
																	  \eStyle foreground or background operation */

	private:
		QHBoxLayout *qhblOperations;
		QMainWindow *qmwParent;
}; // cCopyMove

#endif