/// common file operation code

#ifndef FILE_OPERATION_H
#define FILE_OPERATION_H

#include "FileSystem/Permission.h"
#include "Settings.h"
#include "FileSystem/CopyMoveConflict.h"
#include "FileSystem/DiskSpace.h"
#include <QtCore/QSemaphore>

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
		static const bool SuitsFilter(const QString &qsName, const QString &qsFilter, const bool &bRegularExpression = false);
																			///< check if filename suits filter
																			/**< \param qsName filename to check
																				  \param qsFilter filter to suit
																				  \param bRegularExpression qsFilter is regular expression
																				  \return true if filename suits filter */

	signals:
		void ShowDiskSpaceDialog(const QString &qsFilename, const qint64 &qi64FileSize, const qint64 &qi64FreeSpace) const;
																			///< show disk space dialog
																			/**< \param qsFilename concerned file
																				  \param qi64FileSize file size
																				  \param qi64FreeSpace free space on target disk */
}; // cFileOperation

#endif