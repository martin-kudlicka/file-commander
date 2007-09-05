/// archive file system

#ifndef ARCHIVE_COPY_H
#define ARCHIVE_COPY_H

#include <QtCore/QThread>
#include <QtGui/QMainWindow>
#include <QtGui/QHBoxLayout>
#include "Settings.h"
#include "Plugins/WCXHead.h"
#include "Plugins/PackerPlugin.h"
#include <QtCore/QFileInfo>
#include "FileSystem/FileOperation.h"
#include "FileSystem/CopyMoveDialog.h"
#include "FileSystem/CopyMoveWidget.h"
#include <QtGui/QTreeWidgetItem>

class cArchiveCopy : public QThread
{
	Q_OBJECT

	public:
		cArchiveCopy(QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings);
																										///< constructor
																										/**< \param qmwParent parent window for foreground dialog
																										  \param qhblOperations layout for background widget
																										  \param csSettings application's configuration */

		const void Copy(const QList<tHeaderData> &qlOperation, const QFileInfo &qfiArchive, const QString &qsFilter, const QHash<QString, QHash<QTreeWidgetItem *, tHeaderData> *> &qhDirectories, const QString &qsDestination, cPackerPlugin *cppPackerPlugin, cPackerPlugin::sPluginInfo *spiPluginInfo, const cFileOperation::eOperationPosition &eopPosition);
																										///< start of copy or move operation
																										/**< \param qlOperation file list to extract
																											  \param qfiArchive archive file path
																											  \param qsFilter destination path
																											  \param qhDirectories list of all source archive files
																											  \param qsDestination filter for input files
																											  \param cppPackerPlugin packer plugin interface
																											  \param spiPluginInfo interace for archive access
																											  \param eStyle foreground or background operation */

	private:
		bool bCanceled;																			///< true if operation is canceled
		cCopyMoveDialog *ccmdDialog;															///< copy/move dialog
		cCopyMoveWidget *ccmwWidget;															///< copy/move widget
		cSettings *csSettings;																	///< application's configuration
		QFileInfo qfiArchive;																	///< archive represented by this file system
		QHash<QString, QHash<QTreeWidgetItem *, tHeaderData> *> qhDirectories;	///< list of all source archive files
		QHBoxLayout *qhblOperations;															///< layout for background operations
		qint64 qi64CurrentMaximum;																///< size of currently copied file
		qint64 qi64TotalMaximum;																///< total size of all files
		QList<tHeaderData> qlOperation;														///< file list to extract
		QMainWindow *qmwParent;																	///< parent window for foreground operation window
		QString qsDestination;																	///< destination path on local file system
		QString qsFilter;																			///< filter for source files
		QString qsSource;																			///< currently copied source file
		QString qsTarget;																			///< target of currently copied file
		cPackerPlugin::sPluginInfo	spiPluginInfo;											///< plugin description for current archive

		const void CreateWidget();																///< create widget for background operation
		const QStringList GetFilesToExtractAndCountTotalSize();						///< get file list to extract and count size of all those files
																										/**< \return file list to extract */
		const QStringList GetFilesToExtractAndCountTotalSizeInDirectory(const char *cDirectory);
																										///< get file list to extract from specified source directory
																										/**< \param cDirectory source directory
																											  \return file list to extract from specified source directory */
		void run();																					///< separate thread process

	signals:
		void SetCurrentMaximum(const qint64 &qi64Value) const;						///< set maximum for current file
																										/**< \param qi64Value maximum for current file */
		void SetCurrentValue(const qint64 &qi64Value) const;							///< set progress for current file
																										/**< \param qi64Value progress for current file */
		void SetDestination(const QString &qsDestination) const;						///< set destination file
																										/**< \param qsDestination destination file */
		void SetSource(const QString &qsSource) const;									///< set source file
																										/**< \param qsSource source file */
		void SetTotalMaximum(const qint64 &qi64Value) const;							///< set overall maximum
																										/**< \param qi64Value overall maximum */
		void SetTotalValue(const qint64 &qi64Value) const;								///< set overall progress

	private slots:
		const void on_ccmdCopyMoveDialog_Background();									///< move operation to background
		const void on_cLocalCopyMove_OperationCanceled();								///< copy or move operation was canceled
}; // cArchiveCopy

#endif