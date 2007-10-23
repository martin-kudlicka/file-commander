/// permission

#ifndef PERMISSION_H
#define PERMISSION_H

#include <QtCore/QObject>
#include <QtGui/QMainWindow>

class cPermission : public QObject
{
	Q_OBJECT

	public:
		/// user response
		enum eChoice {
			Ask,																									///< no permanent answer yet
			Yes,																									///< yes
			No,																									///< no
			YesToAll,																							///< yes to all
			NoToAll,																								///< no to all
			Cancel																								///< cancel
		};

		cPermission(QMainWindow *qmwParent);															///< constructor
																													/**< \param qmwParent parent window of this dialog */

	private:
		QMainWindow *qmwParent;																				///< parent window of this dialog

	signals:
		void Finished(const cPermission::eChoice &ecResponse) const;							///< dialog closed with user response
																													/**< \param ecResponse dialog result */
	private slots:
		const void Show(const QString &qsFilename, const QString &qsInformation) const;	///< show permission dialog
																													/**< \param qsFilename concerned file
																														  \param qsInformation question about file */
}; // cPermission

#endif
