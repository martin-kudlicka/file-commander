/// common file operation code

#ifndef FILE_OPERATION_H
#define FILE_OPERATION_H

#include "FileSystem/Permission.h"
#include "Settings.h"
#include "FileSystem/CopyMoveConflict.h"

class cFileOperation
{
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
}; // cFileOperation

#endif