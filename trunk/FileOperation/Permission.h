/// permission

#ifndef PERMISSION_H
#define PERMISSION_H

#include <QtCore/QObject>

class cPermission : public QObject
{
	Q_OBJECT

	public:
		/// user response
		enum eChoice {
			Ask,																								///< no permanent answer yet
			Yes,																								///< yes
			No,																								///< no
			YesToAll,																						///< yes to all
			NoToAll,																							///< no to all
			Cancel																							///< cancel
		};

		eChoice Exec(const QString &qsFilename, const QString &qsInformation);			///< show permission dialog (single thread)
																												/**< \param qsFilename concerned file
																													  \param qsInformation question about file */
	private:
		eChoice ShowDialog(const QString &qsFilename, const QString &qsInformation);	///< show permission dialog
																												/**< \param qsFilename concerned file
																													  \param qsInformation question about file */
	signals:
		void Finished(const cPermission::eChoice &ecResponse);								///< dialog closed with user response
																												/**< \param ecResponse dialog result */
	private slots:
		void Show(const QString &qsFilename, const QString &qsInformation);				///< show permission dialog (multithread)
																												/**< \param qsFilename concerned file
																													  \param qsInformation question about file */
}; // cPermission

#endif
