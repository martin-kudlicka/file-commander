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

		virtual const QString GetContentPluginValue(const sContentPluginRequest &sContent) = 0;
																										///< get value from content plugin
																										/**< \param sContent request description
																											  \return content plugin (nondelayed) value */
		virtual const QList<QTreeWidgetItem *> GetDirectoryContent() = 0;			///< get tree items for current directory
																										/**< \return  tree items for current directory */
		virtual const sDiskSpace GetDiskSpace() const = 0;								///< find out disk space information
																										/**< \return disk space information */
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
		virtual const qint64 GetFileSize(QTreeWidgetItem *qtwiFile) const = 0;	///< get file size
																										/**< \param qtwiFile file to find size for
																											  \return file size */
		virtual const QDateTime GetLastModified(QTreeWidgetItem *qtwiFile) const = 0;
																										///< get file's last modified date/time stamp
																										/**< \param qtwiFile file to check
																											  \return file's last modified date/time stamp */
		virtual const QString GetPath() const = 0;										///< current path on file system
																										/**< \return path on file system */
		virtual const QString GetTabText() const = 0;									///< get text for tab in directory view
																										/**< \return tab text */
		virtual const QString GetVolumeName() const = 0;								///< find out name of the disk
																										/**< \return name of the disk */
		virtual const bool IsDir(QTreeWidgetItem *qtwiFile) const = 0;				///< check if file is directory
																										/**< \param qtwiFile file check
																											  \return true if directory */
		virtual const void RetreiveContentDelayedValues() = 0;						///< start retreiving of content delayed values
		virtual const void SetPath(const QString &qsDrive, const QString &qsRootPath, const QString &qsPath) = 0;
																										///< change path for this file system
																										/**< \param qsDrive drive handled by this file system class
																											  \param qsRootPath path to root of this file system
																											  \param qsPath to initialize local file system */
}; // cFileSystem

#endif
