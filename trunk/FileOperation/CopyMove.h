/// handling copying and moving files

#ifndef COPY_MOVE_H
#define COPY_MOVE_H

#include <QHBoxLayout>
#include "FileOperation/CopyMoveDialog.h"
#include "FileOperation/CopyMoveWidget.h"
#include <QThread>
#include "FileOperation/FileRoutine.h"
#include <QMainWindow>
#include "FileOperation/CopyMoveConflict.h"
#include <QSemaphore>
#include "FileOperation/Rename.h"
#include "Settings.h"
#ifdef Q_WS_WIN
#include "FileOperation/Permission.h"
#endif
#include "FileOperation/Retry.h"
#include "FileOperation/DiskSpace.h"

class cCopyMove : public QThread
{
	Q_OBJECT

	public:
		cCopyMove(QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings);
																				///< constructor
																				/**< \param qmwParent parent window for foreground dialog
																					  \param qhblOperations layout for background widget
																					  \param csSettings application's configuration */

		void CopyMove(const cFileRoutine::eOperation &eoOperation, const QFileInfoList &qfilSource, const QString &qsDestination, const QString &qsFilter, const cFileRoutine::eWindow &eStyle);
																				///< start of copy or move operation
																				/**< \param eoOperation copy or move operation
																					  \param qfilSource source file list
																					  \param qsDestination destination path
																					  \param qsFilter filter for input files
																					  \param eStyle foreground or background operation */

	private:
		bool bCanceled;													///< true if operation is canceled
		cCopyMoveConflict ccmcConflict;								///< conflict dialog
		cCopyMoveConflict::eChoice ecConflictCurrent;			///< current conflict user's response
		cCopyMoveDialog *ccmdDialog;									///< copy/move dialog
		cCopyMoveWidget *ccmwWidget;									///< copy/move widget
		cDiskSpace cdsDiskSpace;										///< disk space dialog
		cDiskSpace::eChoice ecDiskSpaceCurrent;					///< current disk space user's response
		cFileRoutine::eOperation eoOperation;						///< copy or move operation
#ifdef Q_WS_WIN
		cPermission cpPermission;										///< permission dialog
		cPermission::eChoice ecPermissionCurrent;					///< current permission dialog user's response
#endif
		cRename crRename;													///< rename dialog
		cRetry crRetry;													///< retry dialog
		cRetry::eChoice ecRetryCurrent;								///< current retry dialog user's response
		cSettings *csSettings;											///< application's configuration
		int iBufferSize;													///< buffer for copy/move file
		QHBoxLayout *qhblOperations;									///< layout for background operations
		QFileInfoList qfilSource;										///< source file list
		qint64 qi64CurrentMaximum;										///< size of currently copied/moved file
		qint64 qi64TotalMaximum;										///< total size of all files
		QMainWindow *qmwParent;											///< parent window for foreground operation window
		QSemaphore qsPause;												///< to wait for answer on dialog
		QString qsDestination;											///< destination path
		QString qsFilter;													///< filter for input files
		QString qsNewFilename;											///< new filename after rename
		QString qsSource;													///< currently copied/moved source file
		QString qsTarget;													///< target of currently copied/moved file

		bool Copy(const QString &qsSource, const QString &qsDestination, qint64 *qi64TotalValue);
																				///< copy file
																				/**< \param qsSource source file
																					  \param qsDestination destination file
																					  \param qi64TotalValue total progress
																					  \return true if operation succesfull */
		void CreateWidget();												///< create widget for background operation
		QString GetWildcardedName(const QFileInfo &qfiFile, const QString &qsSourcePath, const QString &qsDestination);
																				///< return file name modified by wildcard
																				/**< \param qfiFile source file to copy or move
																					  \param qsSourcePath path for all sources
																					  \param qsDestination destination string
																					  \return modified file name */
		void run();															///< separate thread process

	signals:
		void SetCurrentMaximum(const qint64 &qi64Value);		///< set maximum for current file
																				/**< \param qi64Value maximum for current file */
		void SetCurrentValue(const qint64 &qi64Value);			///< set progress for current file
																				/**< \param qi64Value progress for current file */
		void SetDestination(const QString &qsDestination);		///< set destination file
																				/**< \param qsDestination destination file */
		void SetSource(const QString &qsSource);					///< set source file
																				/**< \param qsSource source file */
		void SetTotalMaximum(const qint64 &qi64Value);			///< set overall maximum
																				/**< \param qi64Value overall maximum */
		void SetTotalValue(const qint64 &qi64Value);				///< set overall progress
																				/**< \param qi64Value overall progress */
		void ShowConflictDialog(const QString &qsOperation, const QFileInfo &qfiSource, const QFileInfo &qfiDestination);
																				///< show conflict dialog
																				/**< \param qsOperation type of operation - copy or move
																					  \param qfiSource source file information
																					  \param qfiDestination destination file information */
		void ShowDiskSpaceDialog(const QString &qsFilename, const qint64 &qi64FileSize, const qint64 &qi64FreeSpace);
																				///< show disk space dialog
																				/**< \param qsFilename concerned file
																					  \param qi64FileSize file size
																					  \param qi64FreeSpace free space on target disk */
#ifdef Q_WS_WIN
		void ShowPermissionDialog(const QString &qsFilename, const QString &qsInformation);
																				///< show permission dialog
																				/**< \param qsFilename concerned file
																					  \param qsInformation question about file */
#endif
		void ShowRenameDialog(const QString &qsOldFilename);
																				///< show rename dialog
																				/**< \param qsOldFilename file to rename */
		void ShowRetryDialog(const QString &qsInformation, const QString &qsFilename);
																				///< show retry dialog
																				/**< \param qsInformation question about file
																					  \param qsFilename concerned file */

	private slots:
		void on_ccm_OperationCanceled();								///< copy or move operation was canceled
		void on_ccmcConflict_Finished(const cCopyMoveConflict::eChoice &ecResponse);
																				///< conflict dialog closed with user response
																				/**< \param ecResponse user's response */
		void on_ccmdCopyMoveDialog_Background();					///< move operation to background
		void on_cdsDiskSpace_Finished(const cDiskSpace::eChoice &ecResponse);
																				///< disk space dialog closed with user response
																				/**< \param ecResponse dialog result */
#ifdef Q_WS_WIN
		void on_cpPermission_Finished(const cPermission::eChoice &ecResponse);
																				///< permission dialog closed with user response
																				/**< \param ecResponse dialog result */
#endif
		void on_crRename_Finished(const QString &qsNewFilename);
																				///< rename dialog closed with user's reponse
																				/**< \param NewFilename new file name */
		void on_crRetry_Finished(const cRetry::eChoice &ecResponse);
																				///< retry dialog closed with user response
																				/**< \param ecResponse dialog result */
}; // cCopyMove

#endif