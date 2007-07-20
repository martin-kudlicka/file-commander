/// new favourite directory dialog

#ifndef NEW_FAVOURITE_DIRECTORY_DIALOG_H
#define NEW_FAVOURITE_DIRECTORY_DIALOG_H

#include "ui_NewFavouriteDirectoryDialog.h"

class cNewFavouriteDirectoryDialog : public QDialog, public Ui::qdNewFavouriteDirectory
{
	Q_OBJECT

	public:
		/// position of new favourite directory
		enum ePosition {
			Disabled,															///< disable positioning
			Before,																///< place before currently selected favourite
			After																	///< place after currently selected favourite
		};
		/// favourite directory type
		enum eType {
			Directory,															///< favourite is directory
			Submenu																///< favourite is submenu
		};

		cNewFavouriteDirectoryDialog(QDialog *qdParent, const eType &etType, QString *qsDirectory, ePosition *epPosition, bool *bInSubmenu);
																					///< constructor
																					/**< \param qdParent parent window of this dialog
																						  \param etType favourite type to add
																						  \param qsDirectory user directory name
																						  \param epPosition place before or after current item
																						  \param bInSubmenu place into submenu */

	private:
		bool *bInSubmenu;														///< place into submenu
		ePosition *epPosition;												///< position of new favourite directory
		QString *qsDirectory;												///< user directory name

	private slots:
		const void on_qpbCancel_clicked(bool checked = false);	///< dialog rejected
																							/**< \param checked true if button is checkable and checked */
		const void on_qpbOK_clicked(bool checked = false);			///< dialog accepted
																							/**< \param checked true if button is checkable and checked */
}; // cNewFavouriteDirectoryDialog

#endif
