/// archive operation

#ifndef ARCHIVE_OPERATION_H
#define ARCHIVE_OPERATION_H

#include <QtCore/QObject>
#include "Plugins/WCXHead.h"
#include "Plugins/Packer.h"
#include <QtGui/QMainWindow>
#include "Panel.h"

class cArchiveOperation : private QObject
{
	Q_OBJECT

	public:
		/// archive operation
		enum eOperation {
			Extract																			///< extract files from archive
		};

		cArchiveOperation(QMainWindow *qmwParent, cSettings *csSettings);	///< constructor
																								/**< \param qmwParent parent window for dialogs
																									  \param csSettings application's settings file */

		void Operate(const eOperation &eoOperation, const cPanel::sArchive &saSourceArchive, const QList<tHeaderData> &qlSourceSelected, QString &qsDestination);
																								///< manipulate with archive files
																								/**< \param eoOperation type of archive operation
																									  \param saSourceArchive source archive info
																									  \param qlSourceSelected source archive files to manipulate with
																									  \param qsDestination destination path */

	private:
		bool bCanceled;																	///< operation in progress is canceled
		cSettings *csSettings;															///< application's settings file
		QMainWindow *qmwParent;															///< parent window for dialogs

		void ExtractFiles(const cPanel::sArchive &saSourceArchive, const QList<tHeaderData> &qlSourceSelected, QString &qsDestination);
																								///< extract files from archive to local directory
																								/**< \param saSourceArchive source archive info
																									  \param qlSourceSelected selected source archive files
																									  \param qsDestination destination path */
		QList<tHeaderData> GetAllArchiveFiles(const QList<tHeaderData> &qlSelected, const QList<tHeaderData> &qlAll);
																								///< gather all archive files that are selected
																								/**< \param qlSourceSelected selected files
																									  \param qlSourceAll all files */
		cFileOperation::sObjects GetCount(const QList<tHeaderData> &qlArchive);
																								///< count of objects
																								/**< \param qlArchive objects to count
																									  \return count of objects */
#ifdef Q_WS_WIN
		static int __stdcall ProcessDataProc(char *cFileName, int iSize);	///< callback progress function
																								/**< \param cFileName file processed
																									  \param iSize bytes processed since last call
																									  \return zero if operation canceled */
#endif

	signals:
		void SetCurrentMaximum(const qint64 &qi64Value);						///< set maximum for current file
																								/**< \param qi64Value maximum for current file */
		void SetCurrentValue(const qint64 &qi64Value);							///< set progress for current file
																								/**< \param qi64Value progress for current file */
		void SetDestination(const QString &qsDestination);						///< set destination file
																								/**< \param qsDestination destination file */
		void SetSource(const QString &qsSource);									///< set source file
																								/**< \param qsSource source file */
		void SetTotalMaximum(const qint64 &qi64Value);							///< set overall maximum
																								/**< \param qi64Value overall maximum */
		void SetTotalValue(const qint64 &qi64Value);								///< set overall progress
																								/**< \param qi64Value overall progress */

	private slots:
		void on_ccmdDialog_OperationCanceled();									///< operation canceled
}; // cArchiveOperation

#endif