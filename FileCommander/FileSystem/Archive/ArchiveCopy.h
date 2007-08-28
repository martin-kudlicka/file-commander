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

class cArchiveCopy : public QThread
{
	public:
		cArchiveCopy(QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings);
																	///< constructor
																	/**< \param qmwParent parent window for foreground dialog
																	  \param qhblOperations layout for background widget
																	  \param csSettings application's configuration */

		const void Copy(const QList<tHeaderData> &qlOperation, const QFileInfo &qfiArchive, const QString &qsDestination, const QString &qsFilter, cPackerPlugin *cppPackerPlugin, cPackerPlugin::sPluginInfo *spiPluginInfo, const cFileOperation::eOperationPosition &eopPosition);
																	///< start of copy or move operation
																	/**< \param qlOperation source file list
																		  \param qfiArchive archive file path
																		  \param qsDestination destination path
																		  \param qsFilter filter for input files
																		  \param cppPackerPlugin packer plugin interface
																		  \param spiPluginInfo interace for archive access
																		  \param eStyle foreground or background operation */

	private:
		cSettings *csSettings;								///< application's configuration
		QFileInfo qfiArchive;								///< archive represented by this file system
		QHBoxLayout *qhblOperations;						///< layout for background operations
		QList<tHeaderData> qlOperation;					///< operation file list
		QMainWindow *qmwParent;								///< parent window for foreground operation window
		QString qsDestination;								///< destination path on local file system
		QString qsFilter;										///< filter for source files
		cPackerPlugin::sPluginInfo	spiPluginInfo;		///< plugin description for current archive

		void run();												///< separate thread process
}; // cArchiveCopy

#endif