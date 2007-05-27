/// some common file operations

#ifndef FILE_ROUTINE_H
#define FILE_ROUTINE_H

#include <QtCore/QFileInfoList>
#include <QtCore/QDir>
#include <QtCore/QMap>

class cFileRoutine
{
	public:
		static const int iQUEUED_OPERATION_POSITION = 1;			///< position of queued background operation in layout

		/// file operation type
		enum eOperation {														///< type of file operation
			CopyOperation,														///< copy
			MoveOperation,														///< move
			DeleteOperation													///< delete
		};
		/// window style
		enum eWindow {
			ForegroundWindow,													///< modal foreground window
			BackgroundWindow													///< window in qhblOperations layout
		};

		/// disk space
		struct sDiskSpace {
			qint64 qi64Free;													///< free disk space
			qint64 qi64Total;													///< total disk space
		};
		/// information about drive
		struct sDriveInfo {
			QString qsPath;													///< path for drive (C:/, D:/, /, ...)
			QString qsType;													///< drive type (HDD, USB, ...)
		};

		static QFileInfoList GetDirectoryContent(const QString &qsPath, const QDir::Filters &fFilters);
																					///< lists directory for it's content
																					/**< \param qsPath path to list
																						  \param fFilters filter for directory listing
																						  \return content of qsPath directory */
		static sDiskSpace GetDiskSpace(const QString &qsPath);	///< find out disk space information
																					/**< \param qsPath path to detect space information
																						  \return disk space information */
		static QMap<QString, sDriveInfo> GetDrives();				///< detect drives in system
																					/**< \return information about drive in system */
		static QFileInfoList GetSources(const QFileInfo &qfiFile, const QString &qsFilter = "*");
																					///< return list of sources (within subdirectories too)
																					/**< \param qfiFile file or directory
																						  \param qsFilter filter for input files
																						  \return source list */
		static QFileInfoList GetSources(const QFileInfoList &qfilFileAndDirList, const QString &qsFilter = "*");
																					///< return list of sources (with or within subdirectories too)
																					/**< \param qfilFileAndDirList list of directories and files
																						  \param qsFilter filter for input files
																						  \return source list */
#ifdef Q_WS_WIN
		static QString GetVolumeName(const QString &qsRootPath);	///< find out name of the disk
																					/**< \param qsRootPath root path on disk to detect name
																						  \return volume name */
#endif
		static bool SuitsFilter(const QString &qsName, const QString &qsFilter, const bool &bRegularExpression = false);
																					///< check if filename suits filter
																					/**< \param qsName filename to check
																						  \param qsFilter filter to suit
																						  \param bRegularExpression qsFilter is regular expression
																						  \return true if filename suits filter */

#ifdef Q_WS_WIN
	private:
		static const uint uiVOLUME_NAME = 32;							///< volume name buffer size
#endif
}; // cFileRoutine

#endif
