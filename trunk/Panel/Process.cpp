#include "Panel/Process.h"

#ifdef Q_WS_WIN
#include <windows.h>
#else
#include <QProcess>
#endif

// execute file
void cProcess::Execute(const QString &qsProcess, const QString &qsPath /* NULL */)
{
#ifdef Q_WS_WIN
	ShellExecute(NULL, NULL, reinterpret_cast<LPCWSTR>(qsProcess.unicode()), NULL, reinterpret_cast<LPCWSTR>(qsPath.unicode()), SW_SHOWNORMAL);
#else
	QProcess::startDetached(qsProcess);
#endif
} // Execute