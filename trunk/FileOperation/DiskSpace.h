/// disk space

#ifndef DISK_SPACE_H
#define DISK_SPACE_H

#include <QObject>

class cDiskSpace : public QObject
{
	Q_OBJECT

	public:
		/// user response
		enum eChoice {
			Ask,																///< no permanent answer yet
			Yes,																///< yes
			No,																///< no
			Skip,																///< skip
			YesToAll,														///< yes to all
			SkipAll															///< skip all
		};

	signals:
		void Finished(const cDiskSpace::eChoice &ecResponse);	///< dialog closed with user response
																				/**< \param ecResponse dialog result */
	private slots:
		void Show(const QString &qsFilename, const qint64 &qi64FileSize, const qint64 &qi64FreeSpace);
																				///< show disk space dialog
																				/**< \param qsFilename concerned file
																					  \param qi64FileSize file size
																					  \param qi64FreeSpace free space on target disk */
}; // cDiskSpace

#endif