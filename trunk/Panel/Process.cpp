#include "Panel/Process.h"

#ifdef Q_WS_WIN
#include <windows.h>
#include <QtCore/QDir>
#include <QtGui/QMessageBox>
#else
#include <QtCore/QProcess>
#endif

// execute file
void cProcess::Execute(const QString &qsProcess, const QString &qsPath /* NULL */)
{
#ifdef Q_WS_WIN
	QDir qdDir;

	qdDir.setPath(qsPath);
	if (qdDir.cd(qsProcess)) {
		// open directory
		ShellExecute(NULL, NULL, reinterpret_cast<LPCWSTR>(qdDir.path().unicode()), NULL, NULL, SW_SHOWNORMAL);
	} else {
		// open file
		HINSTANCE hiErrorCode;
		QString qsName, qsParameters;

		if (qsProcess.at(0) != '"') {
			qsName = qsProcess.left(qsProcess.indexOf(' '));
			if (qsProcess.indexOf(' ') != -1) {
				qsParameters = qsProcess.mid(qsProcess.indexOf(' ') + 1);
			} // if
		} else {
			qsName = qsProcess.mid(1, qsProcess.indexOf('"', 2) - 1);
			qsParameters = qsProcess.mid(qsProcess.indexOf('"', 2) + 2);
		} // if else

		hiErrorCode = ShellExecute(NULL, NULL, reinterpret_cast<LPCWSTR>(qsName.unicode()), reinterpret_cast<LPCWSTR>(qsParameters.unicode()), reinterpret_cast<LPCWSTR>(qsPath.unicode()), SW_SHOWNORMAL);

		if (reinterpret_cast<int>(hiErrorCode) <= 32) {
			QString qsError;
			
			qsError = GetErrorString(reinterpret_cast<int>(hiErrorCode));
			QMessageBox::critical(NULL, tr("Execute"), tr("Can't execute %1:\n%2.").arg(qsName).arg(qsError));
		} // if
	} // if else
#else
	QProcess::startDetached(qsProcess);
#endif
} // Execute

#ifdef Q_WS_WIN
// error string for specified error code
QString cProcess::GetErrorString(const int &iErrorCode)
{
	switch (iErrorCode) {
		case 0:
			return tr("the operating system is out of memory or resources");
		case ERROR_FILE_NOT_FOUND:
			return tr("the specified file was not found");
		case ERROR_PATH_NOT_FOUND:
			return tr("the specified path was not found");
		case ERROR_BAD_FORMAT:
			return tr("the .exe file is invalid (non-Microsoft Win32 .exe or error in .exe image)");
		case SE_ERR_ACCESSDENIED:
			return tr("the operating system denied access to the specified file");
		case SE_ERR_ASSOCINCOMPLETE:
			return tr("the file name association is incomplete or invalid");
		case SE_ERR_DDEBUSY:
			return tr("the Dynamic Data Exchange (DDE) transaction could not be completed because other DDE transactions were being processed");
		case SE_ERR_DDEFAIL:
			return tr("the DDE transaction failed");
		case SE_ERR_DDETIMEOUT:
			return tr("the DDE transaction could not be completed because the request timed out");
		case SE_ERR_DLLNOTFOUND:
			return tr("the specified DLL was not found");
		/*case SE_ERR_FNF:
			return tr("the specified file was not found");*/
		case SE_ERR_NOASSOC:
			return tr("there is no application associated with the given file name extension. This error will also be returned if you attempt to print a file that is not printable");
		case SE_ERR_OOM:
			return tr("there was not enough memory to complete the operation");
		/*case SE_ERR_PNF:
			return tr("the specified path was not found");*/
		case SE_ERR_SHARE:
			return tr("a sharing violation occurred");
	} // switch
	
	return QString();
} // GetErrorString
#endif
