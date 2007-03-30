/// save find settings dialog

#ifndef SAVE_FIND_SETTINGS_DIALOG_H
#define SAVE_FIND_SETTINGS_DIALOG_H

#include "ui_SaveFindSettingsDialog.h"

class cSaveFindSettingsDialog : public QDialog, public Ui::qdSaveFindSettings
{
	Q_OBJECT

	public:
		cSaveFindSettingsDialog(QDialog *qdParent);	///< constructor
																	/**< \param qdParent parent window of this dialog */
}; // cSaveFindSettingsDialog

#endif