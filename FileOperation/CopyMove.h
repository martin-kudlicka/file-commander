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
#include "FileOperation/Permission.h"

class cCopyMove : public QThread
{
	Q_OBJECT

	public:
		cCopyMove(QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings);
																				///< constructor
																				/**< \param qmwParent parent window for foreground dialog
																					  \param qhblOperations layout for background widget
																					  \param csSettings application's configuration */

		void CopyMove(const cFileRoutine::eOperation &eoOperation, const QFileInfoList &qfilSource, const QString &qsDestination, const cFileRoutine::eWindow &eStyle);
																				///< start of copy or move operation
																				/**< \param eoOperation copy or move operation
																					  \param qfilSource source file list
																					  \param qsDestination destination path
																					  \param eStyle foreground or background operation */

	private:
		static const qint64 qi64BUFFER_SIZE = 16384;				///< buffer size for copy

		bool bCanceled;													///< true if operation is canceled
		cCopyMoveConflict *ccmcConflict;								///< conflict dialog
		cCopyMoveConflictDialog::eChoice ecConflict;				///< global conflict user's response
		cCopyMoveConflictDialog::eChoice ecConflictCurrent;	///< current conflict user's response
		cCopyMoveDialog *ccmdDialog;									///< copy/move dialog
		cCopyMoveWidget *ccmwWidget;									///< copy/move widget
		cFileRoutine::eOperation eoOperation;						///< copy or move operation
		cPermission *cpPermission;										///< permission dialog
		cPermissionDialog::eChoice ecPermission;					///< global permission dialog user's response
		cPermissionDialog::eChoice ecPermissionCurrent;			///< current permission dialog user's response
		cRename *crRename;												///< rename dialog
		cSettings *csSettings;											///< application's configuration
		QHBoxLayout *qhblOperations;									///< layout for background operations
		QFileInfoList qfilSource;										///< source file list
		qint64 qi64CurrentMaximum;										///< size of currently copied/moved file
		qint64 qi64TotalMaximum;										///< total size of all files
		QMainWindow *qmwParent;											///< parent window for foreground operation window
		QSemaphore qsPause;												///< to wait for answer on dialog
		QString qsDestination;											///< destination path
		QString qsNewFilename;											///< new filename after rename
		QString qsSource;													///< currently copied/moved source file
		QString qsTarget;													///< target of currently copied/moved file

		void Copy(const QString &qsSource, const QString &qsDestination, qint64 *qi64TotalValue);
																				///< copy file
																				/**< \param qsSource source file
																					  \param qsDestination destination file
																					  \param qi64TotalValue total progress */
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
		void ShowPermissionDialog(const QString &qsFilename, const QString &qsInformation);
																				///< show permission dialog
																				/**< \param qsFilename concerned file
																					  \param qsInformation question about file */
		void ShowRenameDialog(const QString &qsOldFilename);
																				///< show rename dialog
																				/**< \param qsOldFilename file to rename */

	private slots:
		void on_ccm_OperationCanceled();								///< copy or move operation was canceled
		void on_ccmcConflict_Finished(const cCopyMoveConflictDialog::eChoice &ecResponse);
																				///< dialog closed with user response
																				/**< \param ecResponse user's response */
		void on_ccmdCopyMoveDialog_Background();					///< move operation to background
		void on_cpPermission_Finished(const cPermissionDialog::eChoice &ecResponse);
																				///< permission dialog closed with user response
																				/**< \param ecResponse dialog result */
		void on_crRename_Finished(const QString &qsNewFilename);
																				///< rename dialog closed with user's reponse
																				/**< \param NewFilename new file name */
}; // cCopyMove

#endif