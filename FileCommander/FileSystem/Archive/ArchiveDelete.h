/// handling deleting archive files

#ifndef ARCHIVE_DELETE_H
#define ARCHIVE_DELETE_H

#include <QtCore/QThread>
#include <QtGui/QMainWindow>
#include <QtGui/QHBoxLayout>
#include "Settings.h"
#include "Plugins/PackerPlugin.h"
#include "FileSystem/FileOperation.h"

class cArchiveDelete : public QThread
{
	Q_OBJECT

	public:
		cArchiveDelete(QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings);
																		///< constructor
																		/**< \param qmwParent parent window for foreground dialog
																			  \param qhblOperations layout for background widget
																			  \param csSettings application's configuration (Windows only)*/

		void Delete(const QList<tHeaderData> &qlSource, const QString &qsFilter, const cFileOperation::eOperationPosition &eopPosition) const;
																			///< start of delete operation
																			/**< \param qlSource source file list
																				  \param qsFilter filter for input files
																				  \param eStyle foreground or background operation */

	private:
		cSettings *csSettings;									///< application's configuration
		QList<tHeaderData> qlSource;							///< source file list
		QHBoxLayout *qhblOperations;							///< layout for background operations
		QMainWindow *qmwParent;									///< parent window for foreground operation window
		void run();													///< separate thread process
}; // cArchiveDelete

#endif
