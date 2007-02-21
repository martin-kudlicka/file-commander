/// rename

#ifndef PERMISSION_H
#define PERMISSION_H

#include <QMainWindow>
#include "FileOperation/PermissionDialog.h"

class cPermission : public QObject
{
	Q_OBJECT

	public:
		cPermission(QMainWindow *qmwParent);											///< constructor
																									/**< \param qmwParent parent window of conflict dialog */
		~cPermission();																		///< destructor

	private:
		cPermissionDialog *cpdDialog;														///< permission dialog

	signals:
		void Finished(const cPermissionDialog::eChoice &ecResponse);			///< dialog closed with user response
																									/**< \param ecResponse dialog result */
	private slots:
		void Show(const QString &qsFilename, const QString &qsInformation);	///< show permission dialog
																									/**< \param qsFilename concerned file
																										  \param qsInformation question about file */
}; // cRename

#endif