/// archive file system

#ifndef ARCHIVE_COPY_H
#define ARCHIVE_COPY_H

#include <QtCore/QThread>
#include <QtGui/QMainWindow>
#include <QtGui/QHBoxLayout>
#include "Settings.h"
#include "Plugins/PackerPlugin.h"
#include <QtCore/QFileInfo>
#include "FileSystem/FileOperation.h"
#include "FileSystem/CopyMoveDialog.h"
#include "FileSystem/CopyMoveWidget.h"
#include <QtGui/QTreeWidgetItem>
#include "FileSystem/Archive/Continue.h"
#include "FileSystem/DiskSpace.h"

class cArchiveCopy : public QThread
{
	Q_OBJECT

	public:
		cArchiveCopy(QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings);
																											///< constructor
																											/**< \param qmwParent parent window for foreground dialog
																											  \param qhblOperations layout for background widget
																											  \param csSettings application's configuration */

		const void Copy(const QList<tHeaderData> &qlOperation, const QString &qsArchiveFilePath, const QString &qsArchivePath, const QString &qsFilter, const QHash<QString, QHash<QTreeWidgetItem *, tHeaderData> *> &qhDirectories, const QString &qsDestination, cPackerPlugin *cppPackerPlugin, cPackerPlugin::sPluginInfo *spiPluginInfo, const cFileOperation::eOperationPosition &eopPosition, const bool &bFullPath);
																											///< start of copy or move operation
																											/**< \param qlOperation file list to extract
																												  \param qsArchiveFilePath archive file path
																												  \param qsArchivePath startup path in archive
																												  \param qsFilter destination path
																												  \param qhDirectories list of all source archive files
																												  \param qsDestination filter for input files
																												  \param cppPackerPlugin packer plugin interface
																												  \param spiPluginInfo interace for archive access
																												  \param eStyle foreground or background operation
																												  \param bFullPath extract files with full path if true */

	private:
		bool bCanceled;																				///< true if operation is canceled
		bool bFullPath;																				///< extract files with full path
		cContinue ccContinue;																		///< continue dialog
		cCopyMoveConflict ccmcConflict;															///< conflict dialog
		cCopyMoveDialog *ccmdDialog;																///< copy/move dialog
		cCopyMoveWidget *ccmwWidget;																///< copy/move widget
		cDiskSpace cdsDiskSpace;																	///< disk space dialog
		cRename crRename;																				///< rename dialog
		cSettings *csSettings;																		///< application's configuration
		cCopyMoveConflict::eChoice ecConflictCurrent;										///< current conflict user's response
		cContinue::eChoice ecContinueCurrent;													///< current continue user's response
		cDiskSpace::eChoice ecDiskSpaceCurrent;												///< current disk space user's response
#ifdef Q_WS_WIN
		cPermission *cpPermission;																	///< permission dialog
		cPermission::eChoice ecPermissionCurrent;												///< current permission dialog user's response
#endif
		static QHash<QString, cArchiveCopy *> qhCallback;									///< callback function table for several background operations
		QHash<QString, QHash<QTreeWidgetItem *, tHeaderData> *> qhDirectories;		///< list of all source archive files
		QHBoxLayout *qhblOperations;																///< layout for background operations
		qint64 qi64CurrentMaximum;																	///< size of currently copied file
		qint64 qi64CurrentValue;																	///< current file progress
		qint64 qi64TotalMaximum;																	///< total size of all files
		qint64 qi64TotalValue;																		///< total progress
		QList<tHeaderData> qlOperation;															///< file list to extract
		QMainWindow *qmwParent;																		///< parent window for foreground operation window
		QSemaphore qsPause;																			///< to wait for answer on dialog
		QString qsArchiveFilePath;																	///< archive represented by this file system
		QString qsArchivePath;																		///< startup path in archive
		QString qsDestination;																		///< destination path on local file system
		QString qsFilter;																				///< filter for source files
		QString qsSource;																				///< currently copied source file
		QString qsTarget;																				///< target of currently copied file
		cPackerPlugin::sPluginInfo	spiPluginInfo;												///< plugin description for current archive

		const void CheckContinue(const int &iErrorCode, cContinue::eChoice *ecContinue);
																											///< continue after unsuccessfull file extraction
																											/**< \param qsSource unsuccessfully extracted file
																												  \param iErrorCode error code to get error string from
																												  \param ecContinue continue permanent user answer */
		const void CreateWidget();																	///< create widget for background operation
		const QStringList GetFilesToExtractAndCountTotalSize();							///< get file list to extract and count size of all those files
																											/**< \return file list to extract */
		const QStringList GetFilesToExtractAndCountTotalSizeInDirectory(const char *cDirectory);
																											///< get file list to extract from specified source directory
																											/**< \param cDirectory source directory
																												  \return file list to extract from specified source directory */
#ifdef Q_WS_WIN
		static int __stdcall ProcessDataProc(char *cFileName, int iSize);				///< callback progress function
																											/**< \param cFileName file processed
																												  \param iSize bytes processed since last call
																												  \return zero if operation canceled */
		const int ProcessDataProc2(char *cFileName, int iSize);							///< nonstatic callback progress function
																											/**< \param cFileName file processed
																												  \param iSize bytes processed since last call
																												  \return zero if operation canceled */
#endif
		void run();																						///< separate thread process

	signals:
		void SetCurrentMaximum(const qint64 &qi64Value) const;							///< set maximum for current file
																											/**< \param qi64Value maximum for current file */
		void SetCurrentValue(const qint64 &qi64Value) const;								///< set progress for current file
																											/**< \param qi64Value progress for current file */
		void SetDestination(const QString &qsDestination) const;							///< set destination file
																											/**< \param qsDestination destination file */
		void SetSource(const QString &qsSource) const;										///< set source file
																											/**< \param qsSource source file */
		void SetTotalMaximum(const qint64 &qi64Value) const;								///< set overall maximum
																											/**< \param qi64Value overall maximum */
		void SetTotalValue(const qint64 &qi64Value) const;									///< set overall progress
		void ShowContinueDialog(const QString &qsError, const QString &qsFilename) const;
																											///< show continue dialog
																											/**< \param qsError error string
																												  \param qsFilename concerned file */

	private slots:
		const void on_ccContinue_Finished(const cContinue::eChoice &ecResponse);	///< continue dialog closed with user response
																											/**< \param ecResponse user's response */
		const void on_ccmcConflict_Finished(const cCopyMoveConflict::eChoice &ecResponse);
																											///< conflict dialog closed with user response
																											/**< \param ecResponse user's response */
		const void on_ccmdCopyMoveDialog_Background();										///< move operation to background
		const void on_cdsDiskSpace_Finished(const cDiskSpace::eChoice &ecResponse);
																											///< disk space dialog closed with user response
																											/**< \param ecResponse dialog result */
		const void on_cLocalCopyMove_OperationCanceled();									///< copy or move operation was canceled
#ifdef Q_WS_WIN
		const void on_cpPermission_Finished(const cPermission::eChoice &ecResponse);
																											///< permission dialog closed with user response
																											/**< \param ecResponse dialog result */
#endif
		const void on_crRename_Finished();														///< rename dialog closed with user's reponse
}; // cArchiveCopy

#endif
