/// handling copying and moving files

#ifndef COPY_MOVE_H
#define COPY_MOVE_H

#include <QHBoxLayout>
#include "FileOperation.h"

class cCopyMove : private QObject
{
	Q_OBJECT

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
																	  \param eStyle foreground or background operation */

	private:
		bool bCanceled;									///< true if operation is canceled
		QHBoxLayout *qhblOperations;					///< layout for background operations
		QMainWindow *qmwParent;							///< parent window for foreground operation window

	private slots:
		void on_ccm_OperationCanceled();				///< copy or move operation was canceled
}; // cCopyMove

#endif