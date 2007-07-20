/// file control

#ifndef FILE_CONTROL_H
#define FILE_CONTROL_H

#include "Settings.h"
#include <QtGui/QHBoxLayout>
#include <QtGui/QMainWindow>
#include "FileSystem.h"

class cFileControl : public QObject
{
	Q_OBJECT

	public:
		/// drive types
		enum eDriveType {
			Local												///< local drive
		};

		/// drive information
		struct sDrive {
			QString qsPath;								///< path to drive's root
			eDriveType edtType;							///< drive type
			// TODO sDrive add drive type and show next to drive letters
		};

		cFileControl(QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings);
																///< constructor
																/**< \param qmwParent parent window for dialogs
																	  \param qbnlOperations layout for background and queued operations
																	  \param csSettings application's configuration */

		const QList<QPair<QString, sDrive> > GetDrives() const;
																///< get accessible drives
		cFileSystem *GetFileSystem(const QString &qsDrive) const;
																///< get file system for specified drive
																/**< \param qsDrive drive to get file system for
																	  \return file system handling class */
		const QPair<QString, sDrive> GetFirstDrive() const;
																///< returns first accessible drive for application
																/**< \param first accessible drive for application */
	private:
		cSettings *csSettings;							///< application's configuration
		QHBoxLayout *qhblOperations;					///< background and queued operation windows
		QMainWindow *qmwParent;							///< parent window for dialogs
}; // cFileControl

#endif
