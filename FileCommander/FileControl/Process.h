/// process handling

#ifndef PROCESS_H
#define PROCESS_H

#include <QtCore/QString>
#include <QtCore/QObject>

class cProcess : public QObject
{
	public:
		static void Execute(const QString &qsProcess, const QString &qsPath = NULL);	///< execute file
																												/**< \param qsProcess file to execute */
#ifdef Q_WS_WIN
		static QString GetErrorString(const int &iErrorCode);											///< error string for specified error code
																												/**< \param iErrorCode error code
																													  \return error string */
#endif
}; // cProcess

#endif
