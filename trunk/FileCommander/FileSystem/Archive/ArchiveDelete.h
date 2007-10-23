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

		void Delete(const QString &qsArchiveFilePath, const QList<tHeaderData> &qlSource, const QHash<QString, QHash<QTreeWidgetItem *, tHeaderData> *> &qhDirectories, const QString &qsFilter, cPackerPlugin::sPluginInfo *spiPluginInfo, const cFileOperation::eOperationPosition &eopPosition);
																			///< start of delete operation
																			/**< \param qsArchiveFilePath archive file path
																				  \param qlSource source file list
																				  \param qsFilter filter for input files
																				  \param spiPluginInfo interace for archive access
																				  \param eStyle foreground or background operation */

	private:
		bool bCanceled;												///< true if operation is canceled
		cDeleteDialog *cddDialog;									///< delete dialog
		cDeleteNonEmptyDirectory *cdnedDeleteNonEmptyDir;	///< delete non empty directory dialog
		cDeleteWidget *cdwWidget;									///< delete widget
		cPermission *cpPermission;									///< permission dialog
		cPermission::eChoice ecPermissionCurrent;				///< current permission dialog user's response
		cSettings *csSettings;										///< application's configuration
		cDeleteNonEmptyDirectory::eChoice ecDeleteNonEmptyDirectoryCurrent;
																			///< delete non empty directory dialog user's response
		QList<tHeaderData> qlSource;								///< source file list
		QHash<QString, QHash<QTreeWidgetItem *, tHeaderData> *> qhDirectories;
																			///< table of all directories in archive
		QHBoxLayout *qhblOperations;								///< layout for background operations
		qint64 qi64TotalMaximum;									///< total count of files to delete
		QMainWindow *qmwParent;										///< parent window for foreground operation window
		QSemaphore qsPause;											///< to wait for answer on dialog
		QString qsArchiveFilePath;									///< archive file path
		QString qsFilter;												///< filter for input files
		cPackerPlugin::sPluginInfo *spiPluginInfo;			///< packer plugin interface

		void AddDirToSourceList(const char cDirectory[260], QStringList *qslDirectories, QStringList *qslFiles, QStringList *qslIgnore, cPermission::eChoice *ecPermission);
																			///< add directory content to source list
																			/**< \param cDirectory directory name
																				  \param qslDirectories directories to delete
																				  \param qslFiles files to delete
																				  \param qslIgnore directories to ignore
																				  \param ecPermission permanent permission user answer */
		const cFileOperation::eCheckResult CheckReadOnlyAttribute(const tHeaderData *thdFile, cPermission::eChoice *ecPermission);
																			///< check target file read only attribute
																			/**< \param thdFile target file path
																				  \param ecPermissionCurrent current permission user answer
																				  \return action after permission check */
		void CreateWidget();											///< create widget for background operation
		void run();														///< separate thread process

	signals:
		void SetSource(const QString &qsSource);				///< set source file
																			/**< \param qsSource source file */
		void SetTotalMaximum(const qint64 &qi64Value);		///< set overall maximum
																			/**< \param qi64Value overall maximum */
		void SetTotalValue(const qint64 &qi64Value);			///< set overall progress
																			/**< \param qi64Value overall progress */

	private slots:
		void on_cdDeleteDialog_Background();					///< delete operation to background
		void on_cdnedDeleteNonEmptyDirectory_Finished(const cDeleteNonEmptyDirectory::eChoice &ecResponse);
																			///< delete non empty directory dialog closed with user response
																			/**< \param ecResponse dialog result */
		void on_cLocalDelete_OperationCanceled();				///< delete operation was canceled
		void on_cpPermission_Finished(const cPermission::eChoice &ecResponse);
																			///< permission dialog closed with user response
																			/**< \param ecResponse dialog result */
}; // cArchiveDelete

#endif
