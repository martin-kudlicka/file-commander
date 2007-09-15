#include "FileSystem/Archive/ArchiveCommon.h"

// converts packer plugin's date time format to QDateTime
const QDateTime cArchiveCommon::ToQDateTime(const int &iDateTime)
{
	QDate qdDate;
	QDateTime qdtDateTime;
	QTime qtTime;

	qdDate.setDate((iDateTime >> 25) + 1980, (iDateTime >> 21) & 0xF, (iDateTime >> 16) & 0x1F);
	qtTime.setHMS((iDateTime >> 11) & 0x1F, (iDateTime >> 5) & 0x3F, (iDateTime & 0x1F) * 2);

	qdtDateTime.setDate(qdDate);
	qdtDateTime.setTime(qtTime);

	return qdtDateTime;
} // ToQDateTime
