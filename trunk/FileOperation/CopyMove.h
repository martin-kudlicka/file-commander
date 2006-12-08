/// handling copying and moving files

#ifndef COPY_MOVE_H
#define COPY_MOVE_H

#include <QHBoxLayout>
#include "FileOperation/CopyMoveDialog.h"
#include "FileOperation/CopyMoveWidget.h"
#include <QThread>
#include "FileOperation/FileRoutine.h"
#include <QMainWindow>

class cCopyMove : public QThread
{
	Q_OBJECT

	public:
		/// window style
		enum eWindow {
			ForegroundWindow,									///< modal foreground window
			BackgroundWindow									///< window in qhblOperations layout
		};

		cCopyMove(QMainWindow *qmwParent, QHBoxLayout *qhblOperations);
																	///< constructor
																	/**< \param qmwParent parent window for foreground dialog
																		  \param qhblOperations layout for background widget */

		void CopyMove(const cFileRoutine::eOperation eoOperation, const QFileInfoList qfilSource, const QString qsDestination, const eWindow eStyle);
																	///< start of copy or move operation
																	/**< \param eoOperation copy or move operation
																		  \param qfilSource source file list
																		  \param qsDestination destination path
																		  \param eStyle foreground or background operation */

	private:
		static const qint64 qi64BUFFER_SIZE = 16384;	///< buffer size for copy

		bool bCanceled;										///< true if operation is canceled
		cCopyMoveDialog *ccmdDialog;						///< copy dialog
		cCopyMoveWidget *ccmwWidget;						///< copy widget
		cFileRoutine::eOperation eoOperation;			///< copy or move operation
		QHBoxLayout *qhblOperations;						///< layout for background operations
		QFileInfoList qfilSource;							///< source file list
		QMainWindow *qmwParent;								///< parent window for foreground operation window
		QString qsDestination;								///< destination path

		void Copy(const QString qsSource, const QString qsDestination, qint64 *qi64Total);
																	///< copy file
																	/**< \param qsSource source file
																		  \param qsDestination destination file
																		  \param qi64Total total progress */
		void run();												///< thread process

	signals:
		void SetCurrentMaximum(const qint64 qi64Value);
		void SetCurrentValue(const qint64 qi64Value);
		void SetDestination(const QString qsDestination);
		void SetSource(const QString qsSource);
		void SetTotalMaximum(const qint64 qi64Value);
		void SetTotalValue(const qint64 qi64Value);

	private slots:
		void on_ccm_OperationCanceled();					///< copy or move operation was canceled
}; // cCopyMove

#endif