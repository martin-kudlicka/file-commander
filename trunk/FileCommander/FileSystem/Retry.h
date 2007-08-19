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
			Ask,																									///< no permanent choice
			Skip,																									///< skip file
			Retry,																								///< retry operation
			SkipAll,																								///< skip all files with same problem
			Abort																									///< abort whole operation
		};

	signals:
		void Finished(const cRetry::eChoice &ecResponse) const;									///< dialog closed with user response
																													/**< \param ecResponse dialog result */
	private slots:
		const void Show(const QString &qsInformation, const QString &qsFilename) const;	///< show retry dialog
																													/**< \param qsInformation question about file
																														  \param qsFilename concerned file */
}; // cRetry

#endif
