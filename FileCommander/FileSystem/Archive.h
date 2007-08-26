/// archive file system

#ifndef ARCHIVE_H
#define ARCHIVE_H

#include "FileSystem.h"
#include "Plugins/PackerPlugin.h"
//#include <QtGui/QTreeWidgetItem>
//#include "FileSystem/FileOperation.h"
//#include "FileControl/FileOperationDialog.h"

class cArchive : public cFileSystem
{
	public:
		cArchive(const QString &qsDrive, const QString &qsRootPath, const QString &qsArchive, const QString &qsPath, QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings, cPackerPlugin *cppPackerPlugin);
																								///< constructor
																								/**< \param qsDrive drive handled by this file system class
																									  \param qsRootPath path to root of this file system
																									  \param qsArchive file path to archive
																									  \param qsPath to initialize local file system
																									  \param qmwParent parent window for dialogs
																									  \param qhblOperations layout for background widget
																				 					  \param csSettings main settings
																				 					  \param cppPackerPlugin packer plugin interface */

		const void SetPath(const QString &qsDrive, const QString &qsRootPath, const QString &qsPath, const bool &bStartup = false);
																								///< change path for this file system
																								/**< \param qsDrive drive handled by this file system class
																									  \param qsRootPath path to root of this file system
																									  \param qsPath to initialize archive file system
																									  \param bStartup true if initializing file system class */
		const bool SetPath(const QString &qsPath, const bool &bStartup = false);
																								///< change path for this file system without drive change
																								/**< \param qsPath to initialize archive file system
																									  \param bStartup true if initializing file system class
																									  \return true if succesfull */

	private:
		cPackerPlugin *cppPackerPlugin;												///< packer plugin interface
		QString qsArchive;																///< archive file path represented by this file system

		const void ActivateCurrent(QTreeWidgetItem *qtwiFile);				///< activate current file
																								/**< \param qtwiFile file to activate */
		QTreeWidgetItem *AddToCustomList(QTreeWidgetItem *qtwiFile);		///< add file to custom file list
																								/**< \param qtwiFile file to add to custom list
																									  \return new item in custom list */
		const void BeginSearch();														///< begin of searching files
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
																								/**< \return disk space information */
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
		const void RetreiveContentDelayedValues();								///< start retreiving of content delayed values
		const void SetOperationFileList(void *vFileList);						///< set file list for file operation
																								/**< \param vFileList file list to store */
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
}; // cArchive

#endif
