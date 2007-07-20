/// local file system

#ifndef LOCAL_H
#define LOCAL_H

#include "FileSystem.h"
#include <QtCore/QDir>
#include "Settings.h"
#include <QtGui/QFileIconProvider>
#include "Plugins/ContentPlugin.h"
#include "Plugins/ContentPluginDelayed.h"

class cLocal : public cFileSystem
{
	Q_OBJECT

	public:
		cLocal(const QString &qsDrive, const QString &qsRootPath, const QString &qsPath, cSettings *csSettings, cContentPlugin *ccpContentPlugin);
																								///< constructor
																								/**< \param qsDrive drive handled by this file system class
																									  \param qsRootPath path to root of this file system
																									  \param qsPath to initialize local file system
																				 					  \param csSettings main settings
																				 					  \param ccpContentPlugin content plugin interface */
		~cLocal();																			///< destructor

	private:
		static const uint uiVOLUME_NAME = 32;										///< volume name buffer size

		cContentPlugin *ccpContentPlugin;											///< content plugin interface
		cContentPluginDelayed *ccpdContentPluginDelayed;						///< thread to get delayed content plugins values
		cSettings *csSettings;															///< main settings
		QDir qdDir;																			///< current directory
		QFileIconProvider qfipIconProvider;											///< file icon provider
		QHash<QTreeWidgetItem *, QFileInfo> qhFiles;								///< files in current directory
		QQueue<cContentPluginDelayed::sParameters> qqContentDelayedParameters;
																								///< parameters for content delayed values
		QString qsDrive;																	///< drive handled by this file system class
		QString qsRootPath;																///< path to root of this file system

		const bool CheckPath();															///< check if current path available
																								/**< \return true if avalable (at least some upper directory) */
		const QString GetContentPluginValue(const sContentPluginRequest &sContent);
																								///< get value from content plugin
																								/**< \param sContent request description
																									  \return content plugin (nondelayed) value */
		const QList<QTreeWidgetItem *> GetDirectoryContent();					///< get tree items for current directory
																								/**< \return  tree items for current directory */
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
		const QString GetFileName(QTreeWidgetItem *qtwiFile, const bool &bBracketsAllowed = true);
																								///< get file name without extension
																								/**< \param qtwiFile file to find name for
																									  \param bBracketsAllowed brackets around file name allowed flag
																									  \return file name without extension */
		const qint64 GetFileSize(QTreeWidgetItem *qtwiFile) const;			///< get file size
																								/**< \param qtwiFile file to find size for
																									  \return file size */
		const QDateTime GetLastModified(QTreeWidgetItem *qtwiFile) const;	///< get file's last modified date/time stamp
																								/**< \param qtwiFile file to check
																									  \return file's last modified date/time stamp */
		const QString GetPath() const;												///< current path on file system
																								/**< \return path on file system */
		const QString GetTabText() const;											///< get text for tab in directory view
																								/**< \return tab text */
		const QString GetVolumeName() const;										///< find out name of the disk
																								/**< \return name of the disk */
		const bool IsDir(QTreeWidgetItem *qtwiFile) const;						///< check if file is directory
																								/**< \param qtwiFile file check
																									  \return true if directory */
		const void RetreiveContentDelayedValues();								///< start retreiving of content delayed values
		const void SetPath(const QString &qsDrive, const QString &qsRootPath, const QString &qsPath);
																								///< change path for this file system
																								/**< \param qsDrive drive handled by this file system class
																									  \param qsRootPath path to root of this file system
																									  \param qsPath to initialize local file system */

	signals:
		void ContentChanged(const cFileSystem *cfsFileSystem) const;		///< directory content changed for this filesystem
																								/**< \param cfsFileSystem filesystem identifier */
		void GotColumnValue(const cContentPluginDelayed::sOutput &soOutput) const;
																								///< got golumn value from plugin
																								/**< \param soOutput information to update dir view */
		void InterruptContentDelayed() const;										///< interrupt delayed content processing before refresh dir view content
		void Unaccessible() const;														///< file system unaccessible

	private slots:
		const void on_ccpdContentPluginDelayed_GotColumnValue(const cContentPluginDelayed::sOutput &soOutput) const;
																								///< got golumn value from plugin
																								/**< \param soOutput information to update dir view */
}; // cLocal

#endif
