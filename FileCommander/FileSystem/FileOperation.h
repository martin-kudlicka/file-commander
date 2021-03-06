/// common file operation code

#ifndef FILE_OPERATION_H
#define FILE_OPERATION_H

#include "FileSystem/Permission.h"
#include "Settings.h"
#include "FileSystem/CopyMoveConflict.h"
#include "FileSystem/DiskSpace.h"
#include <QtCore/QSemaphore>
#include "FileSystem/Rename.h"
#include "FileControl/FileOperationDialog.h"
#include "FileSystem/Retry.h"
#include "FileSystem/DeleteNonEmptyDirectory.h"

class cFileOperation : public QObject
{
	Q_OBJECT

	public:
		/// operation checks
		enum eCheckResult {
			Nothing,														///< do nothing
			Cancel,														///< cancel processing
			NextFile														///< move onto next file
		};
		/// operation position type
		enum eOperationPosition {
			ForegroundOperation,										///< foreground operation
			BackgroundOperation										///< background (queued) operation
		};

		static const int iQUEUED_OPERATION_POSITION = 1;	///< position of queued background operation in layout

		static const eCheckResult CheckConflict(const cFileOperationDialog::eOperation &eoOperation, const cCopyMoveConflict *ccmcConflict, const cRename *crRename, const QString &qsSource, const qint64 &qi64SourceSize, const QDateTime &qdtSourceLastModified, QString &qsTarget, cCopyMoveConflict::eChoice *ecConflict, cCopyMoveConflict::eChoice *ecConflictCurrent, QSemaphore *qsPause);
																			///< check existing destination file conflict
																			/**< \param eoOperation copy or move operation
																				  \param ccmcConflict copy/move conflict dialog
																				  \param crRename rename dialog
																				  \param qsSource source file to check conflict
																				  \param qi64SourceSize source file size
																				  \param qdtSourceLastModified source file last modified timestamp
																				  \param qsTarget target file path
																				  \param ecConflict permanent conflict user answer
																				  \param ecConflictCurrent current conflict user answer
																				  \param qsPause thread pause
																				  \return action after conflict check */
		static const eCheckResult CheckDeleteNonEmptyDirectory(const cDeleteNonEmptyDirectory *cdnedDeleteNonEmptyDir, const QString &qsPath, cDeleteNonEmptyDirectory::eChoice *ecDeleteNonEmptyDirectory, cDeleteNonEmptyDirectory::eChoice *ecDeleteNonEmptyDirectoryCurrent, QSemaphore *qsPause);
																			///< delete non empty directory check
																			/**< \param cdnedDeleteNonEmptyDir non empty directory dialog
																				  \param qsPath path of non empty directory
																				  \param ecDeleteNonEmptyDirectory permanent delete non empty directory user answer
																				  \param ecDeleteNonEmptyDirectoryCurrent current delete non empty directory user answer
																				  \param qsPause thread pause
																				  \return action after conflict check */
		static const eCheckResult CheckDiskSpace(const cDiskSpace *cdsDiskSpace, const QString &qsSource, const QString &qsTarget, const qint64 &qi64SourceSize, cDiskSpace::eChoice *ecDiskSpace, cDiskSpace::eChoice *ecDiskSpaceCurrent, QSemaphore *qsPause);
																			///< check disk space
																			/**< \param cdsDiskSpace disk space dialog
																				  \param qsSource source file to read
																				  \param qsTarget target file to write
																				  \param qi64SourceSize size of source file
																				  \param ecDiskSpace disk space permanent user answer
																				  \param ecDiskSpaceCurrent disk space current user answer
																				  \param qsPause thread pause
																				  \return action after disk space check */
		static const eCheckResult CheckReadOnlyAttribute(const cPermission *cpPermission, const QString &qsTarget, cPermission::eChoice *ecPermission, cPermission::eChoice *ecPermissionCurrent, QSemaphore *qsPause, const bool &bForce = false);
																			///< check target file read only attribute
																			/**< \param cpPermission permission dialog
																				  \param qsTarget target file path
																				  \param ecPermission permanent permission user answer
																				  \param ecPermissionCurrent current permission user answer
																				  \param qsPause thread pause
																				  \param bForce force dialog to show
																				  \return action after permission check */
		static const eCheckResult CheckRetry(const cRetry *crRetry, const cFileOperationDialog::eOperation &eoOperation, const QFileInfo &qfiSource, cRetry::eChoice *ecRetry, cRetry::eChoice *ecRetryCurrent, QSemaphore *qsPause);
																			///< retry if file operation unsuccesfull
																			/**< \param crRetry retry dialog
																				  \param eoOperation file operation type
																				  \param qfiSource source file to try to retry
																				  \param ecRetry permanent retry user answer
																				  \param ecRetryCurrent current retry user answer
																				  \param qsPause thread pause
																				  \return action after retry check */
		static const cDeleteNonEmptyDirectory::eChoice GetDefaultDeleteNonEmptyDirectory(cSettings *csSettings);
																			///< default delete non empty directory mode from settings file
																			/**< \param csSettings settings file
																				  \return default delete non empty directory mode from settings file */
		static const cCopyMoveConflict::eChoice GetDefaultOverwriteMode(cSettings *csSettings);
																			///< default overwrite mode from settings file
																			/**< \param csSettings settings file
																				  \return default overwrite mode from settings file */
#ifdef Q_WS_WIN
		static const cPermission::eChoice GetDefaultReadonlyOverwritePermission(cSettings *csSettings);
																			///< default readonly overwrite permission
																			/**< \param csSettings settings file
																				  \return default readonly overwrite permission */
#endif
		static const QString GetWildcardedName(const QFileInfo &qfiFile, const QString &qsSourcePath, const QString &qsDestination);
																			///< return file name modified by wildcard
																			/**< \param qfiFile source file to copy or move
																				  \param qsSourcePath path for all sources
																				  \param qsDestination destination string
																				  \return modified file name */
		static const bool SuitsFilter(const QString &qsName, const QString &qsFilter, const bool &bCaseSensitive = false, const bool &bRegularExpression = false);
																			///< check if filename suits filter
																			/**< \param qsName filename to check
																				  \param qsFilter filter to suit
																				  \param bCaseSensitive case sensitive file name compare
																				  \param bRegularExpression qsFilter is regular expression
																				  \return true if filename suits filter */

	signals:
		void ShowConflictDialog(const QString &qsOperation, const QString &qsSource, const qint64 &qi64SourceSize, const QDateTime &qdtSourceLastModified, const QFileInfo &qfiDestination) const;
																			///< show conflict dialog
																			/**< \param qsOperation type of operation - copy or move
																				  \param qsSource source file
																				  \param qi64SourceSize source file size
																				  \param qdtSourceLastModified source file last modified timestamp
																				  \param qfiDestination destination file information */
		void ShowDeleteNonEmptyDirectoryDialog(const QString &qsDirectory);
																			///< show question dialog
																			/**< \param qsDirectory directory to delete */
		void ShowDiskSpaceDialog(const QString &qsFilename, const qint64 &qi64FileSize, const qint64 &qi64FreeSpace) const;
																			///< show disk space dialog
																			/**< \param qsFilename concerned file
																				  \param qi64FileSize file size
																				  \param qi64FreeSpace free space on target disk */
#ifdef Q_WS_WIN
		void ShowPermissionDialog(const QString &qsFilename, const QString &qsInformation) const;
																			///< show permission dialog
																			/**< \param qsFilename concerned file
																				  \param qsInformation question about file */
#endif
		void ShowRenameDialog(QString *qsOldFilename) const;
																			///< show rename dialog
																			/**< \param qsOldFilename file to rename */
		void ShowRetryDialog(const QString &qsInformation, const QString &qsFilename) const;
																			///< show retry dialog
																			/**< \param qsInformation question about file
																				  \param qsFilename concerned file */
}; // cFileOperation

#endif
