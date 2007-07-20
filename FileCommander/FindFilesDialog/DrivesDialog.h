/// drives dialog

#ifndef DRIVES_DIALOG_H
#define DRIVES_DIALOG_H

#include "ui_DrivesDialog.h"

#include "FileOperation/FileRoutine.h"

class cDrivesDialog : public QDialog, public Ui::qdDrives
{
	Q_OBJECT

	public:
		cDrivesDialog(QDialog *qdParent, const QMap<QString, cFileRoutine::sDriveInfo> &qmDrives);
																///< constructor
																/**< \param qdParent parent window of this dialog
																	  \param qmDrives detected drives in system */

	private slots:
		void on_qtwDrives_itemSelectionChanged();	///< selected drives changed
}; // cDrivesDialog

#endif
