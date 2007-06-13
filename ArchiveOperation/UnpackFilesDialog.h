/// unpack files dialog

#ifndef UNPACK_FILES_DIALOG_H
#define UNPACK_FILES_DIALOG_H

#include "ui_UnpackFilesDialog.h"

#include "Settings.h"
#include <QtGui/QMainWindow>

class cUnpackFilesDialog : public QDialog, public Ui::qdUnpackFiles
{
	Q_OBJECT

	public:
		cUnpackFilesDialog(QMainWindow *qmwParent, const QString &qsDestination, cSettings *csSettings);
			///< constructor
			/**< \param qmwParent parent widget (window) of this dialog
				  \param qsDestination destination path for extraction
				  \param csSettings application's settings */
}; // cUnpackFilesDialog

#endif
