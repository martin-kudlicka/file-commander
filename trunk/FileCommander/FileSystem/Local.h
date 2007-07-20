/// local file system

#ifndef LOCAL_H
#define LOCAL_H

#include "FileSystem.h"
#include <QtCore/QDir>
#include "Settings.h"
#include <QtGui/QFileIconProvider>

class cLocal : public cFileSystem
{
	public:
		cLocal(const QString &qsPath, cSettings *csSettings);					///< constructor
																								/**< \param qsPath to initialize local file system
																				 					  \param csSettings main settings */

	private:
		cSettings *csSettings;															///< main settings
		QDir qdDir;																			///< current directory
		QFileIconProvider qfipIconProvider;											///< file icon provider
		QHash<QTreeWidgetItem *, QFileInfo> qhFiles;								///< files in current directory
		
		const QList<QTreeWidgetItem *> GetDirectoryContent();					///< get tree items for current directory
																								/**< \return  tree items for current directory */
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
		const QString GetFileName(QTreeWidgetItem *qtwiFile);					///< get file name without extension
																								/**< \param qtwiFile file to find name for
																									  \return file name without extension */
		const qint64 GetFileSize(QTreeWidgetItem *qtwiFile) const;			///< get file size
																								/**< \param qtwiFile file to find size for
																									  \return file size */
		const QDateTime GetLastModified(QTreeWidgetItem *qtwiFile) const;	///< get file's last modified date/time stamp
																								/**< \param qtwiFile file to check
																									  \return file's last modified date/time stamp */
		const bool IsDir(QTreeWidgetItem *qtwiFile) const;						///< check if file is directory
																								/**< \param qtwiFile file check
																									  \return true if directory */
}; // cLocal

#endif
