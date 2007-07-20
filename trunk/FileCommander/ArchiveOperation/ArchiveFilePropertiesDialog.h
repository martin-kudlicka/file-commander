/// archive file properties dialog

#ifndef ARCHIVE_FILE_PROPERTIES_DIALOG_H
#define ARCHIVE_FILE_PROPERTIES_DIALOG_H

#include "ui_ArchiveFilePropertiesDialog.h"

#include <QtGui/QMainWindow>
#include "Plugins/WCXHead.h"

class cArchiveFilePropertiesDialog : public QDialog, public Ui::qdArchiveFileProperties
{
	Q_OBJECT

	public:
		cArchiveFilePropertiesDialog(QMainWindow *qmwParent, const tHeaderData &thdFile);
			///< constructor
			/**< \param qmwParent parent widget (window) of this dialog
				  \param thdFile file header data */
}; // cArchiveFilePropertiesDialog

#endif
