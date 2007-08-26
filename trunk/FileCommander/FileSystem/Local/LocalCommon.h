/// common local file system methods

#ifndef LOCAL_COMMON_H
#define LOCAL_COMMON_H

#include <QtCore/QFileInfo>

class cLocalCommon
{
	public:
		static const QFileInfoList GetFiles(const QFileInfo &qfiFile, const QString &qsFilter = "*");
			///< return list of sources (within subdirectories too)
			/**< \param qfiFile file or directory
				  \param qsFilter filter for input files
				  \return file list */
		static const QFileInfoList GetFiles(const QFileInfoList &qfilFiles, const QString &qsFilter = "*");
			///< return list of sources (within subdirectories too)
			/**< \param qfilFiles files or directories
				  \param qsFilter filter for input files
				  \return file list */
		static const QFileInfoList GetFiles(const QStringList &qslFiles, const QString &qsFilter = "*");
			///< return list of sources (within subdirectories too)
			/**< \param qfilFiles files or directories
				  \param qsFilter filter for input files
				  \return file list */
}; // cLocalCommon

#endif