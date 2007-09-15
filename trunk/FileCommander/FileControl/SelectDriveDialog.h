/// select drive dialog

#ifndef SELECT_DRIVE_DIALOG_H
#define SELECT_DRIVE_DIALOG_H

#include "ui_SelectDriveDialog.h"

#include <QtGui/QMainWindow>
#include "FileOperation/FileRoutine.h"

class cSelectDriveDialog : public QDialog, public Ui::qdSelectDrive
{
	Q_OBJECT

	public:
		cSelectDriveDialog(QMainWindow *qmwParent, const QMap<QString, cFileRoutine::sDriveInfo> &qmDrives, const QString &qsDrive);
																			///< constructor
																			/**< \param qmwParent parent window of this dialog
																				  \param qmDrives detected drives in system
																				  \param qsDrive currently selected drive */
}; // cSelectDriveDialog

#endif
