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
}; // cLocalCommon

#endif