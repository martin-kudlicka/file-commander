/// handling copying and moving files

#ifndef COPY_MOVE_H
#define COPY_MOVE_H

#include "FileSystem/CopyMoveConflict.h"
#include "FileSystem/CopyMoveDialog.h"
#include "FileSystem/CopyMoveWidget.h"
#include "FileSystem/DiskSpace.h"
#include "FileSystem/Rename.h"
#include <QtCore/QThread>
#include <QtGui/QMainWindow>
#include <QtGui/QHBoxLayout>
#include "Settings.h"
#include "FileControl/FileOperationDialog.h"
#include "FileSystem/FileOperation.h"
#include "FileSystem/Retry.h"
#include <QtCore/QSemaphore>

class cLocalCopyMove : public QThread
{
	Q_OBJECT

	public:
		cLocalCopyMove(QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings);
																					///< constructor
																					/**< \param qmwParent parent window for foreground dialog
																						  \param qhblOperations layout for background widget
																						  \param csSettings application's configuration */

		const void CopyMove(const cFileOperationDialog::eOperation &eoOperation, const QStringList &qslSource, const QString &qsFilter, const QString &qsDestination, const cFileOperation::eOperationPosition &eopPosition);
																					///< start of copy or move operation
																					/**< \param eoOperation copy or move operation
																						  \param qslSource source file list
																						  \param qsFilter destination path
																						  \param qsDestination filter for input files
																						  \param eStyle foreground or background operation */

	private:
		bool bCanceled;														///< true if operation is canceled
		cCopyMoveConflict ccmcConflict;									///< conflict dialog
		cCopyMoveConflict::eChoice ecConflictCurrent;				///< current conflict user's response
		cCopyMoveDialog *ccmdDialog;										///< copy/move dialog
		cCopyMoveWidget *ccmwWidget;										///< copy/move widget
		cDiskSpace cdsDiskSpace;											///< disk space dialog
		cDiskSpace::eChoice ecDiskSpaceCurrent;						///< current disk space user's response
		cFileOperationDialog::eOperation eoOperation;				///< copy or move operation
#ifdef Q_WS_WIN
		cPermission *cpPermission;											///< permission dialog
		cPermission::eChoice ecPermissionCurrent;						///< current permission dialog user's response
#endif
		cRename *crRename;													///< rename dialog
		cRetry crRetry;														///< retry dialog
		cRetry::eChoice ecRetryCurrent;									///< current retry dialog user's response
		cSettings *csSettings;												///< application's configuration
		int iBufferSize;														///< buffer for copy/move file
		QHBoxLayout *qhblOperations;										///< layout for background operations
		qint64 qi64CurrentMaximum;											///< size of currently copied/moved file
		qint64 qi64TotalMaximum;											///< total size of all files
		QMainWindow *qmwParent;												///< parent window for foreground operation window
		QSemaphore qsPause;													///< to wait for answer on dialog
		QString qsDestination;												///< destination path
		QString qsFilter;														///< filter for input files
		QString qsSource;														///< currently copied/moved source file
		QString qsTarget;														///< target of currently copied/moved file
		QStringList qslSources;												///< source file list

		const bool Copy(const QString &qsSource, const QString &qsDestination, qint64 *qi64TotalValue);
																					///< copy file
																					/**< \param qsSource source file
																						  \param qsDestination destination file
																						  \param qi64TotalValue total progress
																						  \return true if operation succesfull */
		const void CreateWidget();											///< create widget for background operation
		void run();																///< separate thread process

	signals:
		void SetCurrentMaximum(const qint64 &qi64Value) const;	///< set maximum for current file
																					/**< \param qi64Value maximum for current file */
		void SetCurrentValue(const qint64 &qi64Value) const;		///< set progress for current file
																					/**< \param qi64Value progress for current file */
		void SetDestination(const QString &qsDestination) const;	///< set destination file
																					/**< \param qsDestination destination file */
		void SetSource(const QString &qsSource) const;				///< set source file
																					/**< \param qsSource source file */
		void SetTotalMaximum(const qint64 &qi64Value) const;		///< set overall maximum
																					/**< \param qi64Value overall maximum */
		void SetTotalValue(const qint64 &qi64Value) const;			///< set overall progress
																					/**< \param qi64Value overall progress */

	private slots:
		const void on_ccmcConflict_Finished(const cCopyMoveConflict::eChoice &ecResponse);
																					///< conflict dialog closed with user response
																					/**< \param ecResponse user's response */
		const void on_ccmdCopyMoveDialog_Background();				///< move operation to background
		const void on_cdsDiskSpace_Finished(const cDiskSpace::eChoice &ecResponse);
																					///< disk space dialog closed with user response
																					/**< \param ecResponse dialog result */
		const void on_cLocalCopyMove_OperationCanceled();			///< copy or move operation was canceled
#ifdef Q_WS_WIN
		const void on_cpPermission_Finished(const cPermission::eChoice &ecResponse);
																					///< permission dialog closed with user response
																					/**< \param ecResponse dialog result */
#endif
		const void on_crRename_Finished();								///< rename dialog closed with user's reponse
		const void on_crRetry_Finished(const cRetry::eChoice &ecResponse);
																					///< retry dialog closed with user response
																					/**< \param ecResponse dialog result */
}; // cLocalCopyMove

#endif
