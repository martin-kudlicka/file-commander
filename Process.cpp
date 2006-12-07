#include "Process.h"

#ifdef Q_WS_WIN
#include <windows.h>
#else
#include <QProcess>
#endif

void cProcess::Execute(QString qsProcess)
{
#ifdef Q_WS_WIN
	ShellExecute(NULL, NULL, reinterpret_cast<LPCWSTR>(qsProcess.unicode()), NULL, NULL, SW_SHOWNORMAL);
#else
	QProcess::startDetached(qsProcess);
#endif
} // Execute