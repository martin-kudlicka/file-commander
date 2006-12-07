// handling file operations

#ifndef FILE_OPERATION_H
#define FILE_OPERATION_H

#include <QFileInfoList>
#include "Panel.h"

class cFileOperation : private QObject
{
	public:
		enum eOperation {					///< type of file operation
			CopyOperation,					///< copy
			MoveOperation,					///< move
			DeleteOperation				///< delete
		};

	void Copy(cPanel *cpSource, cPanel *cpDestination);
												///< copy files
												/**< \param cpSource source file panel
													  \param cpDestination destination file panel */

	private:
		void Process(const eOperation eoOperation, const QFileInfoList qfilSource, QString qsDestination);
												///< process file operation
												/**< \param eoOperation type of operation
													  \param qlSource source files
													  \param qsDestination destination place */
}; // cFileOperation

#endif