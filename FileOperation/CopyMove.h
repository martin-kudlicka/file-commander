/// handling copying and moving files

#ifndef COPY_MOVE_H
#define COPY_MOVE_H

#include <QtGui/QHBoxLayout>
#include "FileOperation/CopyMoveDialog.h"
#include "FileOperation/CopyMoveWidget.h"
#include <QtCore/QThread>
#include "FileOperation/FileRoutine.h"
#include <QtGui/QMainWindow>
#include "FileOperation/CopyMoveConflict.h"
#include <QtCore/QSemaphore>
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
		static cCopyMoveConflict::eChoice GetDefaultOverwriteMode(cSettings *csSettings);
																				///< default overwrite mode from settings file
																				/**< \param csSettings settings file
																					  \return default overwrite mode from settings file */
#ifdef Q_WS_WIN
		static cPermission::eChoice GetDefaultReadonlyOverwritePermission(cSettings *csSettings);
																				///< default readonly overwrite permission
																				/**< \param csSettings settings file
																					  \return default readonly overwrite permission */
#endif

	private:
		/// copy/move checks
		enum eCheckResult {
			Nothing,															///< do nothing
			Cancel,															///< cancel processing
			NextFile															///< move onto next file
		};

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

		eCheckResult CheckConflict(const QFileInfo &qfiSource, cCopyMoveConflict::eChoice *ecConflict, qint64 *qi64TotalValue);
																				///< check existing destination file conflict
																				/**< \param qfiSource source file to check conflict
																					  \param ecConflict permanent conflict user answer
																					  \param qi64TotalValue total copied file size
																					  \return action after conflict check */
		eCheckResult CheckDiskSpace(const QFileInfo &qfiSource, cDiskSpace::eChoice *ecDiskSpace, qint64 *qi64TotalValue);
																				///< check disk space
																				/**< \param qfiSource source file to check disk space for
																					  \param ecDiskSpace permanent disk space user answer
																					  \param qi64TotalValue total copied file size
																					  \return action after disk space check */
		eCheckResult CheckPermission(const QFileInfo &qfiSource, cPermission::eChoice *ecPermission, qint64 *qi64TotalValue);
																				///< check target file permission
																				/**< \param qfiSource source file to increase copied file size by if skipped
																					  \param ecPermission permanent permission user answer
																					  \param qi64TotalValue total copied file size
																					  \return action after permission check */
		eCheckResult CheckRetry(const QFileInfo &qfiSource, cRetry::eChoice *ecRetry, qint64 *qi64TotalValue);
																				///< retry if copy/move unsuccesfull
																				/**< \param qfiSource source file to try to retry
																					  \param ecRetry permanent retry user answer
																					  \param qi64TotalValue total copied file size
																					  \return action after retry check */
		bool Copy(const QString &qsSource, const QString &qsDestination, qint64 *qi64TotalValue);
																				///< copy file
																				/**< \param qsSource source file
																					  \param qsDestination destination file
																					  \param qi64TotalValue total progress
																					  \return true if operation succesfull */
		void CreateWidget();												///< create widget for background operation
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
