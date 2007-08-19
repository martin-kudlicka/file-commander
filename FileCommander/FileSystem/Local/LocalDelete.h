/// handling deleting files

#ifndef LOCAL_DELETE_H
#define LOCAL_DELETE_H

#include <QtCore/QThread>
#include <QtGui/QMainWindow>
#include <QtGui/QHBoxLayout>
#include "Settings.h"
#include <QtCore/QFileInfo>
#include "FileSystem/FileOperation.h"
#include "FileSystem/DeleteDialog.h"
#include "FileSystem/DeleteNonEmptyDirectory.h"
#include "FileSystem/DeleteWidget.h"
#ifdef Q_WS_WIN
#include "FileSystem/Permission.h"
#endif
#include "FileSystem/Retry.h"
#include <QtCore/QSemaphore>

class cLocalDelete : public QThread
{
	Q_OBJECT

	public:
		cLocalDelete(QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings);
																		///< constructor
																		/**< \param qmwParent parent window for foreground dialog
																			  \param qhblOperations layout for background widget
																			  \param csSettings application's configuration (Windows only)*/
		void Delete(const QFileInfoList &qfilSource, const QString &qsFilter, const cFileOperation::eOperationPosition &eopPosition);
																			///< start of delete operation
																			/**< \param qfilSource source file list
																				  \param qsFilter filter for input files
																				  \param eStyle foreground or background operation */

	private:
		bool bCanceled;											///< true if operation is canceled
		cDeleteDialog *cddDialog;								///< delete dialog
		cDeleteNonEmptyDirectory cdnedDeleteNonEmptyDir;
																		///< delete non empty directory dialog
		cDeleteNonEmptyDirectory::eChoice ecDeleteNonEmptyDirectoryCurrent;
																		///< delete non empty directory dialog user's response
		cDeleteWidget *cdwWidget;								///< delete widget
#ifdef Q_WS_WIN
		cPermission cpPermission;								///< permission dialog
		cPermission::eChoice ecPermissionCurrent;			///< current permission dialog user's response
#endif
		cRetry crRetry;											///< retry dialog
		cRetry::eChoice ecRetryCurrent;						///< current retry dialog user's response
		cSettings *csSettings;									///< application's configuration
		QFileInfoList qfilSource;								///< source file list
		QHBoxLayout *qhblOperations;							///< layout for background operations
		qint64 qi64TotalMaximum;								///< total count of files to delete
		QMainWindow *qmwParent;									///< parent window for foreground operation window
#ifdef Q_WS_WIN
		QSemaphore qsPause;										///< to wait for answer on dialog
#endif
		QString qsFilter;											///< filter for input files

		cFileOperation::eCheckResult CheckDeleteNonEmptyDirectory(const QFileInfoList *qfilSources, cDeleteNonEmptyDirectory::eChoice *ecDeleteNonEmptyDirectory, qint64 *qi64Total);
																		///< delete non empty directory check
																		/**< \param qlSources sources for one selected item in directory view
																			  \param ecDeleteNonEmptyDirectory permanent delete non empty directory user answer
																			  \param qi64Total total deleted files
																			  \return action after conflict check */
#ifdef Q_WS_WIN
		cFileOperation::eCheckResult CheckPermission(const QFileInfo *qfiSource, cPermission::eChoice *ecPermission);
																		///< check target file permission
																		/**< \param qfiSource source file to change permission
																			  \param ecPermission permanent permission user answer
																			  \return action after permission check */
#endif
		cFileOperation::eCheckResult CheckRetry(const QFileInfo *qfiSource, cRetry::eChoice *ecRetry);
																		///< retry if delete unsuccesfull
																		/**< \param qfiSource source file to try to retry
																			  \param ecRetry permanent retry user answer
																			  \return action after retry */
		void CreateWidget();										///< create widget for background operation
		void run();													///< separate thread process

	signals:
		void SetSource(const QString &qsSource);			///< set source file
																		/**< \param qsSource source file */
		void SetTotalMaximum(const qint64 &qi64Value);	///< set overall maximum
																		/**< \param qi64Value overall maximum */
		void SetTotalValue(const qint64 &qi64Value);		///< set overall progress
																		/**< \param qi64Value overall progress */
		void ShowDeleteNonEmptyDirectoryDialog(const QString &qsDirectory);
																		///< show question dialog
																		/**< \param qsDirectory directory to delete */
#ifdef Q_WS_WIN
		void ShowPermissionDialog(const QString &qsFilename, const QString &qsInformation);
																		///< show permission dialog
																		/**< \param qsFilename concerned file
																			  \param qsInformation question about file */
#endif
		void ShowRetryDialog(const QString &qsInformation, const QString &qsFilename);
																		///< show retry dialog
																		/**< \param qsInformation question about file
																			  \param qsFilename concerned file */

	private slots:
		void on_cd_OperationCanceled();						///< delete operation was canceled
		void on_cdDeleteDialog_Background();				///< delete operation to background
		void on_cdnedDeleteNonEmptyDirectory_Finished(const cDeleteNonEmptyDirectory::eChoice &ecResponse);
																		///< delete non empty directory dialog closed with user response
																		/**< \param ecResponse dialog result */
#ifdef Q_WS_WIN
		void on_cpPermission_Finished(const cPermission::eChoice &ecResponse);
																		///< permission dialog closed with user response
																		/**< \param ecResponse dialog result */
#endif
		void on_crRetry_Finished(const cRetry::eChoice &ecResponse);
																		///< retry dialog closed with user response
																		/**< \param ecResponse dialog result */
}; // cLocalDelete

#endif
