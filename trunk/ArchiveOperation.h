/// archive operation

#ifndef ARCHIVE_OPERATION_H
#define ARCHIVE_OPERATION_H

#include <QtCore/QObject>
#include "Plugins/WCXHead.h"
#include "Plugins/PackerPlugin.h"
#include <QtGui/QMainWindow>
#include "FileOperation/CopyMoveDialog.h"
#include <QtGui/QTreeWidgetItem>
#include <QtCore/QFileInfo>
#include "FileOperation/CopyMove.h"
#include "FileOperation.h"

class cArchiveOperation : private QObject
{
	Q_OBJECT

	public:
		/// archive operation
		enum eOperation {
			Extract																			///< extract files from archive
		};

		/// archive information
		struct sArchive {
			cPackerPlugin::sPluginInfo spiPlugin;									///< access to packer methods for this archive
			QList<tHeaderData> qlFiles;												///< files in archive
			QString qsPath;																///< path in archive
			QHash<QTreeWidgetItem *, tHeaderData> qhFiles;						///< info about archive files listed in dir panel
			QString qsArchive;															///< archive filepath
		};

		cArchiveOperation(QMainWindow *qmwParent, cSettings *csSettings);	///< constructor
																								/**< \param qmwParent parent window for dialogs
																									  \param csSettings application's settings file */
		~cArchiveOperation();															///< destructor

		static bool OpenArchive(const QFileInfo &qfiFile, sArchive *saArchive, cSettings *csSettings, cPackerPlugin *cppPackerPlugin);
																								///< try to open archive file
																								/**< \param qfiFile archive file
																									  \param saArchive information about opened archive
																									  \param csSettings application's settings file
																									  \param cppPackerPlugin packer plugins interface
																									  \return true if success */
		void Operate(const eOperation &eoOperation, const sArchive &saSourceArchive, const QList<tHeaderData> &qlSourceSelected, QString &qsDestination);
																								///< manipulate with archive files
																								/**< \param eoOperation type of archive operation
																									  \param saSourceArchive source archive info
																									  \param qlSourceSelected source archive files to manipulate with
																									  \param qsDestination destination path */
		void UnpackSelectedFiles(const QFileInfoList &qfilArchives, const QString &qsDestination, cPackerPlugin *cppPackerPlugin);
																								///< unpack selected archives
																								/**< \param qfilArchives list of archive files
																									  \param qsDestination destination path for extraction
																									  \param cppPackerPlugin packer plugin's interface */

	private:
		/// continue after extract error
		enum eContinue {
			Ask,																				///< no permanent decision yet
			Yes,																				///< yes
			No,																				///< no
			YesToAll																			///< always yes
		};

		static bool bCanceled;															///< operation in progress is canceled
		static cCopyMoveDialog *ccmdDialog;											///< copy/move (progress) dialog
		cSettings *csSettings;															///< application's settings file
		static qint64 qi64CurrentValue;												///< current file progress
		static qint64 qi64TotalValue;													///< total progress
		QMainWindow *qmwParent;															///< parent window for dialogs

		cCopyMove::eCheckResult CheckConflict(const QFileInfo &qfiSource, QString *qsTarget, cCopyMoveConflict::eChoice *ecConflictCurrent, cCopyMoveConflict::eChoice *ecConflict);
																								///< check existing destination file conflict
																								/**< \param qfiSource source file to check conflict for
																									  \param qsTarget target file in conflict
																									  \param ecConflictCurrent conflict current user answer
																									  \param ecConflict conflict permanent user answer
																									  \return action after conflict check */
		void CheckContinue(const QString &qsSource, eContinue *ecContinueCurrent, eContinue *ecContinue);
																								///< continue after unsuccessfull file extraction
																								/**< \param qsSource unsuccessfully extracted file
																									  \param ecContinueCurrent continue current user answer
																									  \param ecContinue continue permanent user answer */
		cCopyMove::eCheckResult CheckDiskSpace(const QString &qsSource, const QString &qsTargetPath, const int &iUnpackedSize, cDiskSpace::eChoice *ecDiskSpaceCurrent, cDiskSpace::eChoice *ecDiskSpace);
																								///< check disk space
																								/**< \param qfiSource source file to check disk space for
																									  \param qsTargetPath target path to check disk space
																									  \param iUnpackedSize size of unpacked source file
																									  \param ecDiskSpaceCurrent disk space current user answer
																									  \param ecDiskSpace disk space permanent user answer
																									  \return action after disk space check */
#ifdef Q_WS_WIN
		cCopyMove::eCheckResult CheckPermission(const QString &qsTarget, cPermission::eChoice *ecPermissionCurrent, cPermission::eChoice *ecPermission);
																								///< check target file permission
																								/**< \param qsTarget file to check for permission
																									  \param ecPermissionCurrent permission current user answer
																									  \param ecPermission permission permanent user answer
																									  \return action after permission check */
#endif
		void ExtractFiles(const sArchive &saSourceArchive, const QList<tHeaderData> &qlSourceSelected, QString &qsDestination, const QString &qsFilter = QString(), const bool &bFullPath = true);
																								///< extract files from archive to local directory
																								/**< \param saSourceArchive source archive info
																									  \param qlSourceSelected selected source archive files
																									  \param qsDestination destination path
																									  \param qsFilter filter for files to unpack
																									  \param bFullPath extract files with full path */
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
		static QList<tHeaderData> ReadArchiveFiles(const HANDLE &hArchive, const cPackerPlugin::sPluginInfo &spiPlugin);
																								// read archive contents
																								/**< \param hArchive archive handle
																									  \param spiPlugin access to plugin's methods
																									  \return list of files in archive */
		QList<tHeaderData> SelectAllRoot(const QList<tHeaderData> &qlArchive);
																								///< select all root archive files and directories
																								/**< \param qlArchive all archive files
																									  \return all root archive files and directories */
		static int ToPackerDateTime(const QDateTime &qdtDateTime);			///< converts Qt's date time format to packer's
																								/**< \param qdtDateTime Qt date time format
																									  \return packer plugin's date time format */

	private slots:
		void on_ccmdDialog_OperationCanceled();									///< operation canceled
}; // cArchiveOperation

#endif
