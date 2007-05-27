/// delete non empty directory

#ifndef DELETE_NON_EMPTY_DIRECTORY_H
#define DELETE_NON_EMPTY_DIRECTORY_H

#include <QtCore/QObject>

class cDeleteNonEmptyDirectory : public QObject
{
	Q_OBJECT

	public:
		/// user response
		enum eChoice {
			Ask,																					///< no permanent answer yet
			Yes,																					///< yes
			No,																					///< no
			YesToAll,																			///< yes to all
			NoToAll,																				///< no to all
			Cancel																				///< cancel
		};

	signals:
		void Finished(const cDeleteNonEmptyDirectory::eChoice &ecResponse);	///< dialog closed with user response
																									/**< \param ecResponse dialog result */
	private slots:
		void Show(const QString &qsDirectory);											///< show question dialog
																									/**< \param qsDirectory directory to delete */
}; // cDeleteNonEmptyDirectory

#endif
