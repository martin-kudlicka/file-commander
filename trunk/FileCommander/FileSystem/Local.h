/// local file system

#ifndef LOCAL_H
#define LOCAL_H

#include "FileSystem.h"
#include <QtCore/QDir>
#include "Settings.h"
#include <QtGui/QFileIconProvider>
#include "Plugins/ContentPlugin.h"
#include "Plugins/ContentPluginDelayed.h"
#include <QtCore/QFileSystemWatcher>
#include <QtGui/QMainWindow>
#include <QtGui/QHBoxLayout>
#include "FileSystem/Local/LocalDelete.h"
#include "FileSystem/Local/LocalCopyMove.h"

class cLocal : public cFileSystem
{
	Q_OBJECT

	public:
		cLocal(const QString &qsDrive, const QString &qsRootPath, const QString &qsPath, QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings, cContentPlugin *ccpContentPlugin);
																								///< constructor
																								/**< \param qsDrive drive handled by this file system class
																									  \param qsRootPath path to root of this file system
																									  \param qsPath to initialize local file system
																									  \param qmwParent parent window for dialogs
																									  \param qhblOperations layout for background widget
																				 					  \param csSettings main settings
																				 					  \param ccpContentPlugin content plugin interface */
		~cLocal();																			///< destructor

	private:
		static const uint uiVOLUME_NAME = 32;										///< volume name buffer size

		cContentPlugin *ccpContentPlugin;											///< content plugin interface
		cContentPluginDelayed *ccpdContentPluginDelayed;						///< thread to get delayed content plugins values
		cLocalCopyMove *clcmCopyMove;													///< copy/move local files thread
		cLocalDelete *cldDelete;														///< delete local files thread
		cSettings *csSettings;															///< main settings
		QDir qdDir;																			///< current directory
		QFileIconProvider qfipIconProvider;											///< file icon provider
		QFileInfoList qfilOperation;													///< operation file list
		QFileSystemWatcher qfswWatcher;												///< file system watcher for changes in current directory
		QHash<QTreeWidgetItem *, QFileInfo> qhCustom;							///< custom file list
		QHash<QTreeWidgetItem *, QFileInfo> qhFiles;								///< files in current directory
		QHBoxLayout *qhblOperations;													///< layout for background widget
		QMainWindow *qmwParent;															///< qmwParent parent window for dialogs
		QQueue<cContentPluginDelayed::sParameters> qqContentDelayedParameters;
																								///< parameters for content delayed values
		QString qsDrive;																	///< drive handled by this file system class
		QString qsRootPath;																///< path to root of this file system

		const void ActivateCurrent(QTreeWidgetItem *qtwiFile);				///< activate current file
																								/**< \param qtwiFile file to activate */
		QTreeWidgetItem *AddToCustomList(QTreeWidgetItem *qtwiFile);		///< add file to custom file list
																								/**< \param qtwiFile file to add to custom list
																									  \return new item in custom list */
		const void BeginSearch();														///< begin of searching files
		const bool CheckPath();															///< check if current path available
																								/**< \return true if avalable (at least some upper directory) */
		const void ClearFileTable(QHash<QTreeWidgetItem *, QFileInfo> &qhTable) const;
																								///< clear file table before next fill of it
																								/**< \param qhTable file table to clear */
		const void CreateDir(const QString &qsName);								///< create new directory
																								/**< \param qsName new directory name */
		const void Delete(const QString &qsFilter, const cFileOperation::eOperationPosition &eopPosition);
																								///< delete files in operation file list
																								/**< \param qsFilter filter to chose files by
																									  \param eopPosition operation position type */
		const bool DirExists(const QString &qsDirectory) const;				///< check if specified directory exists
																								/**< \param qsDirectory directory to check
																									  \return true if directory exists */
		const void EndSearch(const bool &bClearCustomOnly = false);			///< searching of files finished
																								/**< \param bClearCustomOnly just clear custom file list if true */
		const QString GetContentPluginValue(const sContentPluginRequest &sContent);
																								///< get value from content plugin
																								/**< \param sContent request description
																									  \return content plugin (nondelayed) value */
		const QList<QTreeWidgetItem *> GetCustomFileList() const;			///< custom file list
																								/**< \return custom file list */
		const QString GetCustomFileNameWithExtension(QTreeWidgetItem *qtwiFile) const;
																								///< get file name with extension from custom list
																								/**< \param qtwiFile file to find file name with extension for
																									  \return file name with extension */
		const QString GetCustomFilePath(QTreeWidgetItem *qtwiFile);			///< get file name from custom list with full path
																								/**< \param qtwiFile file to find file path for
																									  \return file name with full path */
		QList<QTreeWidgetItem *> GetDirectoryContent(const bool &bRefresh = true);
																								///< get tree items for current directory
																								/**< \param bRefresh reload directory content if true
																									  \return  tree items for current directory */
		const qint64 GetDirectorySize() const;										///< get currently selected directory size
																								/**< \return selected directory size */
		const QString GetDirName() const;											///< get current directory name
																								/**< \return current directory name */
		const sDiskSpace GetDiskSpace() const;										///< find out disk space information
																								/**< \param qsPath path to detect space information
																									  \return disk space information */
		const QString &GetDrive() const;												///< drive represented by file system
																								/**< \return drive represented by file system */
#ifdef Q_WS_WIN
		const QString GetFileAttr(QTreeWidgetItem *qtwiFile) const;			///< get file attributes
																								/**< \param qtwiFile file to find attributes for
																									  \return file attributes */
#endif
		const QString GetFileExtension(QTreeWidgetItem *qtwiFile) const;	///< get file extension
																								/**< \param qtwiFile file to find extension for
																									  \return file extension */
		const QIcon GetFileIcon(QTreeWidgetItem *qtwiFile) const;			///< get icon for specified file
																								/**< \param qtwiFile file to find icon for
																									  \return file icon */
		void *GetFileList(const QList<QTreeWidgetItem *> &qlSelected) const;
																								///< file list of specified file system's type
																								/**< \param qlSelected selected files to get file list for
																									  \return file list of specified file system's type */
		const QString GetFileName(QTreeWidgetItem *qtwiFile, const bool &bBracketsAllowed = true);
																								///< get file name without extension
																								/**< \param qtwiFile file to find name for
																									  \param bBracketsAllowed brackets around file name allowed flag
																									  \return file name without extension */
		const QString GetFileNameWithExtension(QTreeWidgetItem *qtwiFile, const bool &bBracketsAllowed = true);
																										///< get file name with extension
																										/**< \param \param qtwiFile file to find name with extension for
																											  \param bBracketsAllowed brackets around file name allowed flag
																											  \return file name with extension */
		const QString GetFilePath(QTreeWidgetItem *qtwiFile) const;			///< get file name with full path
																								/**< \param qtwiFile file to find file path for
																									  \return file name with full path */
		const qint64 GetFileSize(QTreeWidgetItem *qtwiFile) const;			///< get file size
																								/**< \param qtwiFile file to find size for
																									  \return file size */
		const QDateTime GetLastModified(QTreeWidgetItem *qtwiFile) const;	///< get file's last modified date/time stamp
																								/**< \param qtwiFile file to check
																									  \return file's last modified date/time stamp */
		const QStringList GetOperationStringList() const;						///< file paths from operation file list
																								/**< \return file paths from operation file list */
		const QString GetPath() const;												///< current path on file system
																								/**< \return path on file system */
		const QString GetPath(QTreeWidgetItem *qtwiFile) const;				///< retreive path of a file
																								/**< \param qtwiFile file to get path for
																									  \return path for specified file */
		const QStringList GetSelectedDirectoryStringList() const;			///< selected directory list for current directory
																								/**< \return selected directory list */
		const QFileInfoList GetSelectedFiles() const;							///< selected files in tree view
																								/**< \return selected files in tree view */
		const QString GetTabText() const;											///< get text for tab in directory view
																								/**< \return tab text */
		const QString GetVolumeName() const;										///< find out name of the disk
																								/**< \return name of the disk */
		const void GoToRootDir();														///< set path to root directory
		const void GoToUpDir();															///< go one directory up if possible
		const bool IsDir(QTreeWidgetItem *qtwiFile) const;						///< check if file is directory
																								/**< \param qtwiFile file to check
																									  \return true if directory */
		const bool IsFile(QTreeWidgetItem *qtwiFile) const;					///< check if file is really file
																								/**< \param qtwiFile file to check
																									  \return true if file */
#ifdef Q_WS_WIN
		const bool PathExists(const QString &qsPath) const;					///< check if path is valid
																								/**< \param qsPath path to test
																									  \return true if path exists and is accessible */
#endif
		const void RetreiveContentDelayedValues();								///< start retreiving of content delayed values
		const void SetOperationFileList(void *vFileList);						///< set file list for file operation
																								/**< \param vFileList file list to store */
		const void SetPath(const QString &qsDrive, const QString &qsRootPath, const QString &qsPath, const bool &bStartup = false);
																								///< change path for this file system
																								/**< \param qsDrive drive handled by this file system class
																									  \param qsRootPath path to root of this file system
																									  \param qsPath to initialize local file system
																									  \param bStartup true if initializing file system class */
		const bool SetPath(const QString &qsPath, const bool &bStartup = false);
																								///< change path for this file system without drive change
																								/**< \param qsPath to initialize local file system
																									  \param bStartup true if initializing file system class
																									  \return true if succesfull */
		const void ShowContextMenu(const QPoint &qcPosition
#ifdef Q_WS_WIN
			, const HWND hwParent
#endif
		) const;																				///< custom context menu on right click
																								/**< \param qcPosition cursor position on the screen
																									  \param hwParent parent window to show menu in */
		const void Write(const cFileOperationDialog::eOperation &eoOperation, const QStringList &qslSources, const QString &qsFilter, const QString &qsDestination, const cFileOperation::eOperationPosition &eopPosition);
																								///< write local files to this file system
																								/**< \param eoOperation operation type
																									  \param qslSources file path list of local source files to write
																									  \param qsFilter filter to chose files by
																									  \param qsDestination destination path on this file system
																									  \param eopPosition operation position type */

	signals:
		void ContentChanged(const cFileSystem *cfsFileSystem) const;		///< directory content changed for this filesystem
																								/**< \param cfsFileSystem filesystem identifier */
		void GotColumnValue(const cContentPluginDelayed::sOutput &soOutput) const;
																								///< got golumn value from plugin
																								/**< \param soOutput information to update dir view */
		void InterruptContentDelayed() const;										///< interrupt delayed content processing before refresh dir view content
		void OperationFinished(cFileSystem *cfsFileSystem) const;			///< file operation finished
																								/**< \param cfsFileSystem filesystem identifier */
		void Unaccessible() const;														///< file system unaccessible

	private slots:
		const void on_ccpdContentPluginDelayed_GotColumnValue(const cContentPluginDelayed::sOutput &soOutput) const;
																								///< got golumn value from plugin
																								/**< \param soOutput information to update dir view */
		const void on_cLocalCopyMove_OperationFinished();						///< copy/move operation finished
		const void on_cLocalDelete_OperationFinished();							///< delete file operation finished
		const void on_qfswWatcher_directoryChanged(const QString &path) const;
																								///< current directory content changed
																								/**< \param path path to changed directory */
}; // cLocal

#endif
