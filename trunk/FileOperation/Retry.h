/// retry

#ifndef RETRY_H
#define RETRY_H

#include <QtCore/QObject>

class cRetry : public QObject
{
	Q_OBJECT

	public:
		/// user response
		enum eChoice {
			Ask,																								///< no permanent choice
			Skip,																								///< skip file
			Retry,																							///< retry operation
			SkipAll,																							///< skip all files with same problem
			Abort																								///< abort whole operation
		};

		eChoice Exec(const QString &qsInformation, const QString &qsFilename);			///< show retry dialog (single thread)
																												/**< \param qsInformation question about file
																													  \param qsFilename concerned file */
	private:
		eChoice ShowDialog(const QString &qsInformation, const QString &qsFilename);	///< show retry dialog
																												/**< \param qsInformation question about file
																													  \param qsFilename concerned file */
	signals:
		void Finished(const cRetry::eChoice &ecResponse);										///< dialog closed with user response
																												/**< \param ecResponse dialog result */
	private slots:
		void Show(const QString &qsInformation, const QString &qsFilename);				///< show retry dialog (multithread)
																												/**< \param qsInformation question about file
																													  \param qsFilename concerned file */
}; // cRetry

#endif
