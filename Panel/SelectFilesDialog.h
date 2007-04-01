/// select files dialog

#ifndef SELECT_FILES_DIALOG_H
#define SELECT_FILES_DIALOG_H

#include "ui_SelectFilesDialog.h"

#include <QMainWindow>
#include "Settings.h"
#include "Plugins/Lister.h"

class cSelectFilesDialog : public QDialog, private Ui::qdSelectFiles
{
	Q_OBJECT

	public:
		/// selection type
		enum eSelectType {
			Select,
			Unselect
		};

		cSelectFilesDialog(QMainWindow *qmwParent, const eSelectType &estType, cSettings *csSettings, cLister *clLister);
																			///< constructor
																			/**< \param qmwParent parent window of this dialog
																				  \param estType type of dialog
																				  \param csSettings application's settings
																				  \param clLister lister plugins */

	private:
		cLister *clLister;											///< lister plugins
		cSettings *csSettings;										///< application's settings

		bool eventFilter(QObject *watched, QEvent *event);	///< event filter
																			/**< \param watcher object to filter
																				  \param event event description
																				  \return true if event is handled */
		void RefreshFindSets();										///< refresh finds sets from settings file

	private slots:
		void on_qpbDefine_clicked(bool checked = false);	///< define button is clicked on
																			/**< \param checked true if button is checkable and checked */
}; // cSelectFilesDialog

#endif