/// retry

#ifndef RETRY_H
#define RETRY_H

#include <QObject>
#include "FileOperation/RetryDialog.h"

class cRetry : public QObject
{
	Q_OBJECT

	public:
		~cRetry();																///< destructor
		cRetry(QMainWindow *qmwParent);									///< constructor
																					/**< \param qmwParent parent window of Retry dialog */

	private:
		cRetryDialog *crdDialog;											///< retry dialog

	signals:
		void Finished(const cRetryDialog::eChoice &ecResponse);	///< dialog closed with user response
																					/**< \param ecResponse dialog result */
	private slots:
		void Show(const QString &qsInformation, const QString &qsFilename);
																					///< show retry dialog
																					/**< \param qsInformation question about file
																						  \param qsFilename concerned file */
}; // cRename

#endif