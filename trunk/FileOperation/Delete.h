/// handling deleting files

#ifndef DELETE_H
#define DELETE_H

#include <QThread>
#include <QMainWindow>
#include <QHBoxLayout>
#include "FileOperation/FileRoutine.h"
#include "FileOperation/DeleteDialog.h"
#include "FileOperation/DeleteWidget.h"

class cDelete : public QThread
{
	Q_OBJECT

	public:
		cDelete(QMainWindow *qmwParent, QHBoxLayout *qhblOperations);
																		///< constructor
																		/**< \param qmwParent parent window for foreground dialog
																			  \param qhblOperations layout for background widget */
		void Delete(const QFileInfoList &qfilSource, const cFileRoutine::eWindow &eStyle);
																			///< start of delete operation
																			/**< \param qfilSource source file list
																				  \param eStyle foreground or background operation */

	private:
		bool bCanceled;											///< true if operation is canceled
		cDeleteDialog *cddDialog;								///< delete dialog
		cDeleteWidget *cdwWidget;								///< delete widget
		QFileInfoList qfilSource;								///< source file list
		QHBoxLayout *qhblOperations;							///< layout for background operations
		qint64 qi64TotalMaximum;								///< total count of files to delete
		QMainWindow *qmwParent;									///< parent window for foreground operation window

		void CreateWidget();										///< create widget for background operation
		void run();													///< separate thread process

	signals:
		void SetSource(const QString &qsSource);			///< set source file
																		/**< \param qsSource source file */
		void SetTotalMaximum(const qint64 &qi64Value);	///< set overall maximum
																		/**< \param qi64Value overall maximum */
		void SetTotalValue(const qint64 &qi64Value);		///< set overall progress
																		/**< \param qi64Value overall progress */

	private slots:
		void on_cd_OperationCanceled();						///< delete operation was canceled
		void on_cdDeleteDialog_Background();				///< delete operation to background
}; // cDelete

#endif