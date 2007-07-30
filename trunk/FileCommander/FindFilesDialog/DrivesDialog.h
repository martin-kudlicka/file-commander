/// drives dialog

#ifndef DRIVES_DIALOG_H
#define DRIVES_DIALOG_H

#include "ui_DrivesDialog.h"

class cDrivesDialog : public QDialog, public Ui::qdDrives
{
	Q_OBJECT

	public:
		cDrivesDialog(QDialog *qdParent, const QStringList &qslDrives);
																				///< constructor
																				/**< \param qdParent parent window of this dialog
																					  \param qslDrives detected drives in system */

	private slots:
		const void on_qtwDrives_itemSelectionChanged() const;	///< selected drives changed
}; // cDrivesDialog

#endif
