#include "FileSystem/FileOperation.h"

#ifdef Q_WS_WIN
// default readonly overwrite permission
const cPermission::eChoice cFileOperation::GetDefaultReadonlyOverwritePermission(cSettings *csSettings)
{
	cPermission::eChoice ecPermission;
	QString qsOverwrite;

	qsOverwrite = csSettings->GetReadonlyFileOverwrite();

	if (qsOverwrite == qsASK) {
		ecPermission = cPermission::Ask;
	} else {
		if (qsOverwrite == qsYES_TO_ALL) {
			ecPermission = cPermission::YesToAll;
		} else {
			ecPermission = cPermission::NoToAll;
		} // if else
	} // if else

	return ecPermission;
} // GetDefaultReadonlyOverwritePermission
#endif