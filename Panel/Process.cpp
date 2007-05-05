#include "Panel/Process.h"

#ifdef Q_WS_WIN
#include <windows.h>
#include <QDir>
#else
#include <QProcess>
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
		ShellExecute(NULL, NULL, reinterpret_cast<LPCWSTR>(qsProcess.unicode()), NULL, reinterpret_cast<LPCWSTR>(qsPath.unicode()), SW_SHOWNORMAL);
	} // if else
#else
	QProcess::startDetached(qsProcess);
#endif
} // Execute