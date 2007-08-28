/// archive file system

#ifndef ARCHIVE_COPY_H
#define ARCHIVE_COPY_H

#include <QtCore/QThread>
#include <QtGui/QMainWindow>
#include <QtGui/QHBoxLayout>
#include "Settings.h"

class cArchiveCopy : public QThread
{
	public:
		cArchiveCopy(QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings);
												///< constructor
												/**< \param qmwParent parent window for foreground dialog
													  \param qhblOperations layout for background widget
													  \param csSettings application's configuration */

	private:
		cSettings *csSettings;			///< application's configuration
		QHBoxLayout *qhblOperations;	///< layout for background operations
		QMainWindow *qmwParent;			///< parent window for foreground operation window

		void run();							///< separate thread process
}; // cArchiveCopy

#endif