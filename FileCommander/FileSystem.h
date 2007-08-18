/// file system (abstract)

#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <QtGui/QTreeWidgetItem>

class cFileSystem : public QObject
{
	public:
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

		virtual ~cFileSystem();																	///< destructor

		virtual const void ActivateCurrent(QTreeWidgetItem *qtwiFile) = 0;		///< activate current file
																										/**< \param qtwiFile file to activate */
		virtual QTreeWidgetItem *AddToCustomList(QTreeWidgetItem *qtwiFile) = 0;
																										///< add file to custom file list
																										/**< \param qtwiFile file to add to custom list
																											  \return new item in custom list */
		virtual const void BeginSearch() = 0;												///< begin of searching files
		virtual const void EndSearch() = 0;													///< searching of files finished
		virtual const QString GetContentPluginValue(const sContentPluginRequest &sContent) = 0;
																										///< get value from content plugin
																										/**< \param sContent request description
																											  \return content plugin (nondelayed) value */
		virtual const QString GetCustomFilePath(QTreeWidgetItem *qtwiFile) = 0;	///< get file name from custom list with full path
																										/**< \param qtwiFile file to find file path for
																											  \return file name with full path */
		virtual QList<QTreeWidgetItem *> GetDirectoryContent(const bool &bRefresh = true) = 0;
																										///< get tree items for current directory
																										/**< \param bRefresh reload directory content if true
																											  \return  tree items for current directory */
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
		virtual const QString GetPath() const = 0;										///< current path on file system
																										/**< \return path on file system */
		virtual const QString GetPath(QTreeWidgetItem *qtwiFile) const = 0;		///< retreive path of a file
																										/**< \param qtwiFile file to get path for
																											  \return path for specified file */
		virtual const QStringList GetSelectedDirectoryStringList() const = 0;	///< selected directory list for current directory
																										/**< \return selected directory list */
		virtual const QString GetTabText() const = 0;									///< get text for tab in directory view
																										/**< \return tab text */
		virtual const QString GetVolumeName() const = 0;								///< find out name of the disk
																										/**< \return name of the disk */
		virtual const void GoToUpDir() = 0;													///< go one directory up if possible
		virtual const bool IsDir(QTreeWidgetItem *qtwiFile) const = 0;				///< check if file is directory
																										/**< \param qtwiFile file to check
																											  \return true if directory */
		virtual const bool IsFile(QTreeWidgetItem *qtwiFile) const = 0;			///< check if file is really file
																										/**< \param qtwiFile file to check
																											  \return true if file */
		virtual const void RetreiveContentDelayedValues() = 0;						///< start retreiving of content delayed values
		virtual const void SetPath(const QString &qsDrive, const QString &qsRootPath, const QString &qsPath) = 0;
																										///< change path for this file system
																										/**< \param qsDrive drive handled by this file system class
																											  \param qsRootPath path to root of this file system
																											  \param qsPath to initialize local file system */
		virtual const void SetPath(const QString &qsPath) = 0;						///< change path for this file system without drive change
																										/**< \param qsPath to initialize local file system */
		virtual const void ShowContextMenu(const QPoint &qcPosition
#ifdef Q_WS_WIN
			, const HWND hwParent
#endif
		) const = 0;																				///< custom context menu on right click
																										/**< \param qcPosition cursor position on the screen
																											  \param hwParent parent window to show menu in */
		const bool SuitsFilter(const QString &qsName, const QString &qsFilter, const bool &bRegularExpression = false) const;
																										///< check if filename suits filter
																										/**< \param qsName filename to check
																											  \param qsFilter filter to suit
																											  \param bRegularExpression qsFilter is regular expression
																											  \return true if filename suits filter */
}; // cFileSystem

#endif
