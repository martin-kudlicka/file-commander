/// process handling

#ifndef PROCESS_H
#define PROCESS_H

#include <QString>

class cProcess
{
	public:
		static void Execute(const QString &qsProcess, const QString &qsPath = NULL);	///< execute file
																												/**< \param qsProcess file to execute */
}; // cProcess

#endif