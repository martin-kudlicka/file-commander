/// process execution handling

#ifndef PROCESS_H
#define PROCESS_H

#include <QtCore/QString>
#include <QtCore/QObject>

class cProcess : private QObject
{
	public:
		static void StartDetached(const QString &qsProcess, const QString &qsPath = NULL);	///< execute file
																														/**< \param qsProcess file to execute
																															  \param qsPath path to run file from */
#ifdef Q_WS_WIN
	private:
		static QString GetErrorString(const int &iErrorCode);											///< error string for specified error code
																														/**< \param iErrorCode error code
																															  \return error string */
#endif
}; // cProcess

#endif
