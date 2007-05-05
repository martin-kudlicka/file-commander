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
		QString qsName, qsParameters;

		if (qsProcess.at(0) != '"') {
			qsName = qsProcess.left(qsProcess.indexOf(' '));
			qsParameters = qsProcess.mid(qsProcess.indexOf(' ') + 1);
		} else {
			qsName = qsProcess.mid(1, qsProcess.indexOf('"', 2) - 1);
			qsParameters = qsProcess.mid(qsProcess.indexOf('"', 2) + 2);
		} // if else

		ShellExecute(NULL, NULL, reinterpret_cast<LPCWSTR>(qsName.unicode()), reinterpret_cast<LPCWSTR>(qsParameters.unicode()), reinterpret_cast<LPCWSTR>(qsPath.unicode()), SW_SHOWNORMAL);
	} // if else
#else
	QProcess::startDetached(qsProcess);
#endif
} // Execute