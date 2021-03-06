/// delete non empty directory

#ifndef DELETE_NON_EMPTY_DIRECTORY_H
#define DELETE_NON_EMPTY_DIRECTORY_H

#include <QtCore/QObject>
#include <QtGui/QMainWindow>

class cDeleteNonEmptyDirectory : public QObject
{
	Q_OBJECT

	public:
		/// user response
		enum eChoice {
			Ask,																							///< no permanent answer yet
			Yes,																							///< yes
			No,																							///< no
			YesToAll,																					///< yes to all
			NoToAll,																						///< no to all
			Cancel																						///< cancel
		};

		cDeleteNonEmptyDirectory(QMainWindow *qmwParent);									///< constructor
																											/**< \param qmwParent parent window of this dialog */

	private:
		QMainWindow *qmwParent;																		///< parent window of this dialog

	signals:
		void Finished(const cDeleteNonEmptyDirectory::eChoice &ecResponse) const;	///< dialog closed with user response
																											/**< \param ecResponse dialog result */
	private slots:
		const void Show(const QString &qsDirectory) const;									///< show question dialog
																											/**< \param qsDirectory directory to delete */
}; // cDeleteNonEmptyDirectory

#endif
