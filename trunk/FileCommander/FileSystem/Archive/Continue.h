/// continue

#ifndef CONTINUE_H
#define CONTINUE_H

#include <QtCore/QObject>

class cContinue : public QObject
{
	Q_OBJECT

	public:
		/// user response
		enum eChoice {
			Ask,																							///< no permanent choice
			Yes,																							///< continue on next file
			No,																							///< cancel
			YesToAll																						///< always continue on next file
		};

	signals:
		void Finished(const cContinue::eChoice &ecResponse) const;						///< dialog closed with user response
																											/**< \param ecResponse dialog result */
	private slots:
		const void Show(const QString &qsError, const QString &qsFilename) const;	///< show continue dialog
																											/**< \param qsError error string
																												  \param qsFilename concerned file */
}; // cContinue

#endif
