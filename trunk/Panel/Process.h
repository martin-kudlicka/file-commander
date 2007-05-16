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
}; // cProcess

#endif
