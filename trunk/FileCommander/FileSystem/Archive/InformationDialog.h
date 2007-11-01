/// information

#ifndef INFORMATION_DIALOG_H
#define INFORMATION_DIALOG_H

#include <QtCore/QObject>
#include <QtGui/QMainWindow>
#include <QtGui/QMessageBox>

class cInformationDialog : public QObject
{
	Q_OBJECT

	public:
		cInformationDialog(QMainWindow *qmwParent);																///< constructor
																																/**< \param qmwParent parent window of this dialog */

	private:
		QMainWindow *qmwParent;																							///< parent window of this dialog

	signals:
		void Finished() const;																							///< dialog closed

	private slots:
		const void Show(const enum QMessageBox::Icon &iIcon, const QString &qsInformation) const;	///< show information dialog
																																/**< \param iIcon dialog's icon
																																	  \param qsInformation information to show */
}; // cInformationDialog

#endif
