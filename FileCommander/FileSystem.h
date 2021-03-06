/// file system (abstract)

#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <QtGui/QTreeWidgetItem>
#include "FileSystem/FileOperation.h"
#include "FileControl/FileOperationDialog.h"
#include <QtGui/QFileIconProvider>

class cFileSystem : public QObject
{
	Q_OBJECT

	public:
		/// context menu  actions
		enum eContextAction {
			ContextNothing,																		///< no action
			ContextCopyAction																		///< copy action
		};
		/// drive types
		enum eType {
			Local,																					///< local drive
			Archive																					///< archive
		};

		/// parameters for content plugin value
		struct sContentPluginRequest {
			QTreeWidgetItem *qtwiFile;															///< file to get value from
			QString qsPlugin;																		///< plugin to check with
			QString qsColumn;																		///< plugin's column name
			QString qsUnit;																		///< column's unit
			int iColumn;																			///< column index to update in directory view (qtwiFile)
		};
		/// disk space
		struct sDiskSpace {
			qint64 qi64Free;																		///< free disk space
			qint64 qi64Total;																		///< total disk space
		};
		/// destination parameters
		struct sDestinationParameters {
			cFileSystem *cfsDestination;										///< destination file system
			QString qsDestination;												///< destination for copy/move operation
			bool bFullPath;														///< preserve full file path
			bool bMoveTo;															///< move to destination from source
			bool bOneArchivePerSource;											///< one output archive file per source file or directory
		};
		/// source parameters
		struct sSourceParameters {
			cFileSystem *cfsSource;												///< source file system
			QString qsFilter;														///< filter for source files
			bool bSubdirectories;												///< including subdirectories
		};
		/// operation description
		struct sOperation {
			cFileOperationDialog::eOperation eoType;						///< type of operation
			sSourceParameters sspSource;										///< source parameters
			sDestinationParameters sdpDestination;							///< destination parameters
			cFileOperation::eOperationPosition eopPosition;				///< operation's position
		};

		virtual ~cFileSystem();																	///< destructor

		virtual const void ActivateCurrent(QTreeWidgetItem *qtwiFile) = 0;		///< activate current file
																										/**< \param qtwiFile file to activate */
		virtual QTreeWidgetItem *AddToCustomList(QTreeWidgetItem *qtwiFile) = 0;
																										///< add file to custom file list
																										/**< \param qtwiFile file to add to custom list
																											  \return new item in custom list */
		virtual const void BeginSearch() = 0;												///< begin of searching files
		//virtual const bool CanCopy() const = 0;											///< file system can copy files to local file system
		virtual const bool CanCreateDir() const = 0;										///< file system can create directory in current location
		virtual const bool CanDelete() const = 0;											///< file system can delete files
		virtual const void CreateDir(const QString &qsName) = 0;						///< create new directory
																										/**< \param qsName new directory name */
		virtual const void Delete(const sOperation &soOperation) = 0;				///< delete files in operation file list
																										/**< \param soOperation operation description */
		virtual const bool DirExists(const QString &qsDirectory) const = 0;		///< check if specified directory exists
																										/**< \param qsDirectory directory to check
																											  \return true if directory exists */
		virtual const void EndSearch(const bool &bClearCustomOnly = false) = 0;	///< searching of files finished
																										/**< \param bClearCustomOnly just clear custom file list if true */
		virtual const void FreeOperationList(void *vFileList) const = 0;			///< free file operation list from memory
																										/**< \param vFileList pointer to file operation list in memory */
		virtual const void ForceRefresh() = 0;												///< force reload of file system content
		virtual const QFileInfo &GetArchiveFilePath() const = 0;						///< archive file path
																										/**< \return archive file path if archive file system */
		virtual const QString GetContentPluginValue(const sContentPluginRequest &sContent) = 0;
																										///< get value from content plugin
																										/**< \param sContent request description
																											  \return content plugin (nondelayed) value */
		virtual const QList<QTreeWidgetItem *> GetCustomFileList() const = 0;	///< custom file list
																										/**< \return custom file list */
		virtual const QString GetCustomFileNameWithExtension(QTreeWidgetItem *qtwiFile) const = 0;
																										///< get file name with extension from custom list
																										/**< \param qtwiFile file to find file name with extension for
																											  \return file name with extension */
		virtual const QString GetCustomFilePath(QTreeWidgetItem *qtwiFile) = 0;	///< get file name from custom list with full path
																										/**< \param qtwiFile file to find file path for
																											  \return file name with full path */
		virtual const bool GetDirectoryContent(QList<QTreeWidgetItem *> *qlFiles, const bool &bRefresh = true) = 0;
																										///< get tree items for current directory
																										/**< \param qlFiles tree items for current directory
																											  \param bRefresh reload directory content if true
																											  \return true if file system is accessible */
		virtual const qint64 GetDirectorySize() const = 0;								///< get currently selected directory size
																										/**< \return selected directory size */
		virtual const QString GetDirName() const = 0;									///< get current directory name
																										/**< \return current directory name */
		virtual const sDiskSpace GetDiskSpace() const = 0;								///< find out disk space information
																										/**< \return disk space information */
		virtual const QString &GetDrive() const = 0;										///< drive represented by file system
																										/**< \return drive represented by file system */
#ifdef Q_WS_WIN
		virtual const QString GetFileAttr(QTreeWidgetItem *qtwiFile) const = 0;	///< get file attributes
																										/**< \param qtwiFile file to find attributes for
																											  \return file attributes */
#endif
		virtual const QString GetFileExtension(QTreeWidgetItem *qtwiFile) const = 0;
																										///< get file extension
																										/**< \param qtwiFile file to find extension for
																											  \return file extension */
		virtual const QIcon GetFileIcon(QTreeWidgetItem *qtwiFile) const = 0;	///< get icon for specified file
																										/**< \param qtwiFile file to find icon for
																											  \return file icon */
		virtual void *GetFileList(const QList<QTreeWidgetItem *> &qlSelected) const = 0;
																										///< file list of specified file system's type
																										/**< \param qlSelected selected files to get file list for
																											  \return file list of specified file system's type */
		virtual const QString GetFileName(QTreeWidgetItem *qtwiFile, const bool &bBracketsAllowed = true) = 0;
																										///< get file name without extension
																										/**< \param qtwiFile file to find name for
																											  \param bBracketsAllowed brackets around file name allowed flag
																											  \return file name without extension */
		virtual const QString GetFileNameWithExtension(QTreeWidgetItem *qtwiFile, const bool &bBracketsAllowed = true) = 0;
																										///< get file name with extension
																										/**< \param \param qtwiFile file to find name with extension for
																											  \param bBracketsAllowed brackets around file name allowed flag
																											  \return file name with extension */
		virtual const QString GetFilePath(QTreeWidgetItem *qtwiFile) const = 0;	///< get file name with full path
																										/**< \param qtwiFile file to find file path for
																											  \return file name with full path */
		virtual const qint64 GetFileSize(QTreeWidgetItem *qtwiFile) const = 0;	///< get file size
																										/**< \param qtwiFile file to find size for
																											  \return file size */
		virtual const QDateTime GetLastModified(QTreeWidgetItem *qtwiFile) const = 0;
																										///< get file's last modified date/time stamp
																										/**< \param qtwiFile file to check
																											  \return file's last modified date/time stamp */
		virtual const QStringList GetOperationStringList() const = 0;				///< file paths from operation file list
																										/**< \return file paths from operation file list */
		virtual const QString GetPath(const bool &bLocal = false) const = 0;		///< current path on file system
																										/**< \param bLocal only local path if true
																											  \return path on file system */
		virtual const QString GetPath(QTreeWidgetItem *qtwiFile) const = 0;		///< retreive path of a file
																										/**< \param qtwiFile file to get path for
																											  \return path for specified file */
		virtual const QStringList GetSelectedDirectoryStringList() const = 0;	///< selected directory list for current directory
																										/**< \return selected directory list */
		virtual const QList<QTreeWidgetItem *> GetSelectedFiles() const = 0;		///< selected items from current directory list
																										/**< \return selected items from current directory list */
		virtual const QString GetTabText() const = 0;									///< get text for tab in directory view
																										/**< \return tab text */
		virtual const bool GetVolumeName(QString *qsName) const = 0;				///< find out name of the disk
																										/**< \param qsName name of the disk
																											  \return true if successful */
		virtual const void GoToRootDir() = 0;												///< set path to root directory
		virtual const void GoToUpDir() = 0;													///< go one directory up if possible
		virtual const bool IsDir(QTreeWidgetItem *qtwiFile) const = 0;				///< check if file is directory
																										/**< \param qtwiFile file to check
																											  \return true if directory */
		virtual const bool IsFile(QTreeWidgetItem *qtwiFile) const = 0;			///< check if file is really file
																										/**< \param qtwiFile file to check
																											  \return true if file */
		const bool IsInPanel() const;															///< check if file system is connected to a panel
																										/**< \return true if file system is connected to a panel */
		virtual const void Read(const sOperation &soOperation) = 0;					///< read local files to this file system
																										/**< \param soOperation operation description */
		virtual const void RetreiveContentDelayedValues() = 0;						///< start retreiving of content delayed values
		virtual const void SetOperationFileList(void *vFileList) = 0;				///< set file list for file operation
																										/**< \param vFileList file list to store */
		virtual const void SetPath(const QString &qsDrive, const QString &qsRootPath, const QString &qsPath, const bool &bStartup = false) = 0;
																										///< change path for this file system
																										/**< \param qsDrive drive handled by this file system class
																											  \param qsRootPath path to root of this file system
																											  \param qsPath to initialize local file system
																											  \param bStartup true if initializing file system class */
		virtual const bool SetPath(const QString qsPath, const bool &bStartup = false) = 0;
																										///< change path for this file system without drive change
																										/**< \param qsPath to initialize local file system
																											  \param bStartup true if initializing file system class
																											  \return true if succesfull */
		virtual const eContextAction ShowContextMenu(const QPoint &qcPosition
#ifdef Q_WS_WIN
			, const HWND hwParent
#endif
		) = 0;																						///< custom context menu on right click
																										/**< \param qcPosition cursor position on the screen
																											  \param hwParent parent window to show menu in
																											  \return type of user action */
		virtual const bool TryPath(const QString &qsPath) const = 0;				///< try if path exists on file system
																										/**< \param qsPath path to check
																											  \return true if possible to set path to qsPath */
		virtual const eType Type() const = 0;												///< file system type
		virtual const void Write(const sOperation &soOperation) = 0;				///< write local files to this file system
																										/**< \param soOperation operation description */

	protected:
		cSettings *csSettings;																	///< main settings
		QFileIconProvider qfipIconProvider;													///< file icon provider
		QHBoxLayout *qhblOperations;															///< layout for background widget
		QMainWindow *qmwParent;																	///< qmwParent parent window for dialogs
		QString qsDrive;																			///< drive handled by this file system class
		QString qsRootPath;																		///< path to root of this file system

	signals:
		void ContentChanged(const cFileSystem *cfsFileSystem) const;				///< directory content changed for this filesystem
																										/**< \param cfsFileSystem filesystem identifier */
		void OperationFinished(cFileSystem *cfsFileSystem) const;					///< file operation finished
																										/**< \param cfsFileSystem filesystem identifier */
}; // cFileSystem

#endif
