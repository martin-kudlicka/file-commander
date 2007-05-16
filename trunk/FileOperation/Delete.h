/// handling deleting files

#ifndef DELETE_H
#define DELETE_H

#include <QtCore/QThread>
#include <QtGui/QMainWindow>
#include <QtGui/QHBoxLayout>
#include "FileOperation/FileRoutine.h"
#include "FileOperation/DeleteDialog.h"
#include "FileOperation/DeleteWidget.h"
#ifdef Q_WS_WIN
#include "Settings.h"
#include "FileOperation/Permission.h"
#include <QtCore/QSemaphore>
#endif
#include "FileOperation/Retry.h"

class cDelete : public QThread
{
	Q_OBJECT

	public:
		cDelete(QMainWindow *qmwParent, QHBoxLayout *qhblOperations
#ifdef Q_WS_WIN
			, cSettings *csSettings);
#else
			);
#endif
																		///< constructor
																		/**< \param qmwParent parent window for foreground dialog
																			  \param qhblOperations layout for background widget
																			  \param csSettings application's configuration (Windows only)*/
		void Delete(const QFileInfoList &qfilSource, const QString &qsFilter, const cFileRoutine::eWindow &eStyle);
																			///< start of delete operation
																			/**< \param qfilSource source file list
																				  \param qsFilter filter for input files
																				  \param eStyle foreground or background operation */

	private:
		bool bCanceled;											///< true if operation is canceled
		cDeleteDialog *cddDialog;								///< delete dialog
		cDeleteWidget *cdwWidget;								///< delete widget
#ifdef Q_WS_WIN
		cPermission cpPermission;								///< permission dialog
		cPermission::eChoice ecPermissionCurrent;			///< current permission dialog user's response
#endif
		cRetry crRetry;											///< retry dialog
		cRetry::eChoice ecRetryCurrent;						///< current retry dialog user's response
#ifdef Q_WS_WIN
		cSettings *csSettings;									///< application's configuration
#endif
		QFileInfoList qfilSource;								///< source file list
		QHBoxLayout *qhblOperations;							///< layout for background operations
		qint64 qi64TotalMaximum;								///< total count of files to delete
		QMainWindow *qmwParent;									///< parent window for foreground operation window
#ifdef Q_WS_WIN
		QSemaphore qsPause;										///< to wait for answer on dialog
#endif
		QString qsFilter;											///< filter for input files

		void CreateWidget();										///< create widget for background operation
		void run();													///< separate thread process

	signals:
		void SetSource(const QString &qsSource);			///< set source file
																		/**< \param qsSource source file */
		void SetTotalMaximum(const qint64 &qi64Value);	///< set overall maximum
																		/**< \param qi64Value overall maximum */
		void SetTotalValue(const qint64 &qi64Value);		///< set overall progress
																		/**< \param qi64Value overall progress */
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
#ifdef Q_WS_WIN
		void on_cpPermission_Finished(const cPermission::eChoice &ecResponse);
																		///< permission dialog closed with user response
																		/**< \param ecResponse dialog result */
#endif
		void on_crRetry_Finished(const cRetry::eChoice &ecResponse);
																		///< retry dialog closed with user response
																		/**< \param ecResponse dialog result */
}; // cDelete

#endif
