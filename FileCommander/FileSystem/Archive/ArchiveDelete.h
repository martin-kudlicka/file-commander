/// handling deleting archive files

#ifndef ARCHIVE_DELETE_H
#define ARCHIVE_DELETE_H

#include <QtCore/QThread>
#include <QtGui/QMainWindow>
#include <QtGui/QHBoxLayout>
#include "Settings.h"
#include "Plugins/PackerPlugin.h"
#include "FileSystem/FileOperation.h"
#include "FileSystem/DeleteDialog.h"
#include "FileSystem/DeleteWidget.h"
#include <QtGui/QTreeWidgetItem>

class cArchiveDelete : public QThread
{
	Q_OBJECT

	public:
		cArchiveDelete(QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings);
																		///< constructor
																		/**< \param qmwParent parent window for foreground dialog
																			  \param qhblOperations layout for background widget
																			  \param csSettings application's configuration (Windows only)*/

		void Delete(const QList<tHeaderData> &qlSource, const QHash<QString, QHash<QTreeWidgetItem *, tHeaderData> *> &qhDirectories, const QString &qsFilter, const cFileOperation::eOperationPosition &eopPosition);
																		///< start of delete operation
																		/**< \param qlSource source file list
																			  \param qsFilter filter for input files
																			  \param eStyle foreground or background operation */

	private:
		bool bCanceled;											///< true if operation is canceled
		cDeleteDialog *cddDialog;								///< delete dialog
		cDeleteWidget *cdwWidget;								///< delete widget
		cSettings *csSettings;									///< application's configuration
		QList<tHeaderData> qlSource;							///< source file list
		QHash<QString, QHash<QTreeWidgetItem *, tHeaderData> *> qhDirectories;
																		///< table of all directories in archive
		QHBoxLayout *qhblOperations;							///< layout for background operations
		qint64 qi64TotalMaximum;								///< total count of files to delete
		QMainWindow *qmwParent;									///< parent window for foreground operation window
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

	private slots:
		void on_cLocalDelete_OperationCanceled();			///< delete operation was canceled
		void on_cdDeleteDialog_Background();				///< delete operation to background
}; // cArchiveDelete

#endif
