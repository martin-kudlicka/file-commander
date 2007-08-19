/// common file operation code

#ifndef FILE_OPERATION_H
#define FILE_OPERATION_H

#include "FileSystem/Permission.h"
#include "Settings.h"

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

#ifdef Q_WS_WIN
		static const cPermission::eChoice GetDefaultReadonlyOverwritePermission(cSettings *csSettings);
																			///< default readonly overwrite permission
																			/**< \param csSettings settings file
																				  \return default readonly overwrite permission */
		static const bool SuitsFilter(const QString &qsName, const QString &qsFilter, const bool &bRegularExpression = false);
																			///< check if filename suits filter
																			/**< \param qsName filename to check
																				  \param qsFilter filter to suit
																				  \param bRegularExpression qsFilter is regular expression
																				  \return true if filename suits filter */
#endif
}; // cFileOperation

#endif