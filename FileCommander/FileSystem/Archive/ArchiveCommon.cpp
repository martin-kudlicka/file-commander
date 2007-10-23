#include "FileSystem/Archive/ArchiveCommon.h"

#include "Plugins/WCXHead.h"

// get error string from error code
const QString cArchiveCommon::GetErrorString(const int &iError)
{
	switch (iError) {
		case E_END_ARCHIVE:
			return QT_TR_NOOP("no more files in archive");
		case E_NO_MEMORY:
			return QT_TR_NOOP("not enough memory");
		case E_BAD_DATA:
			return QT_TR_NOOP("data is bad");
		case E_BAD_ARCHIVE:
			return QT_TR_NOOP("CRC error in archive data");
		case E_UNKNOWN_FORMAT:
			return QT_TR_NOOP("archive format unknown");
		case E_EOPEN:
			return QT_TR_NOOP("Cannot open existing file");
		case E_ECREATE:
			return QT_TR_NOOP("cannot create file");
		case E_ECLOSE:
			return QT_TR_NOOP("error closing file");
		case E_EREAD:
			return QT_TR_NOOP("error reading from file");
		case E_EWRITE:
			return QT_TR_NOOP("error writing to file");
		case E_SMALL_BUF:
			return QT_TR_NOOP("buffer too small");
		case E_EABORTED:
			return QT_TR_NOOP("function aborted by user");
		case E_NO_FILES:
			return QT_TR_NOOP("no files found");
		case E_TOO_MANY_FILES:
			return QT_TR_NOOP("too many files to pack");
		case E_NOT_SUPPORTED:
			return QT_TR_NOOP("function not supported");
	} // switch
	
	return QString();
} // ///< get error string from error code

// show information dialog about processed action
const void cArchiveCommon::ShowInformationDialog(const cInformationDialog *cidInformationDialog, const enum QMessageBox::Icon &iIcon, const QString &qsInformation, QSemaphore *qsPause)
{
	cArchiveCommon cacDialog;

	// show information dialog
	connect(&cacDialog, SIGNAL(ShowInformationDialog2(const enum QMessageBox::Icon &, const QString &)), cidInformationDialog, SLOT(Show(const enum QMessageBox::Icon &, const QString &)));
	emit cacDialog.ShowInformationDialog2(iIcon, qsInformation);
	// wait for answer
	qsPause->acquire();
} // ShowInformationDialog

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
