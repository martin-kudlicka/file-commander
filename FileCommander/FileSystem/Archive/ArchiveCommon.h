/// common archive file system methods

#ifndef ARCHIVE_COMMON_H
#define ARCHIVE_COMMON_H

#include <QtCore/QDateTime>

class cArchiveCommon
{
	public:
		static const QDateTime ToQDateTime(const int &iDateTime);	///< converts packer plugin's date time format to QDateTime
																						/**< \param iDateTime packed file date time
																							  \return date time in Qt format */
}; // cArchiveCommon

#endif