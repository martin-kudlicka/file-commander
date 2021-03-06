/// select files dialog

#ifndef SELECT_FILES_DIALOG_H
#define SELECT_FILES_DIALOG_H

#include "ui_SelectFilesDialog.h"

#include <QtGui/QMainWindow>
#include "Settings.h"
#include "Plugins/ListerPlugin.h"

class cSelectFilesDialog : public QDialog, public Ui::qdSelectFiles
{
	Q_OBJECT

	public:
		/// selection type
		enum eSelectType {
			Select,																///< select files
			Unselect																///< unselect files
		};

		cSelectFilesDialog(QMainWindow *qmwParent, const eSelectType &estType, cSettings *csSettings, cListerPlugin *clpListerPlugin);
																					///< constructor
																					/**< \param qmwParent parent window of this dialog
																						  \param estType type of dialog
																						  \param csSettings application's settings
																						  \param clpListerPlugin lister plugins */

	private:
		cListerPlugin *clpListerPlugin;									///< lister plugins
		cSettings *csSettings;												///< application's settings

		bool eventFilter(QObject *watched, QEvent *event);			///< event filter
																					/**< \param watcher object to filter
																						  \param event event description
																						  \return true if event is handled */
		const void RefreshFindSets() const;								///< refresh finds sets from settings file

	private slots:
		const void on_qpbDefine_clicked(bool checked = false);	///< define button is clicked on
																					/**< \param checked true if button is checkable and checked */
		const void on_qpbOK_clicked(bool checked = false);			///< OK button is clicked on
																					/**< \param checked true if button is checkable and checked */
}; // cSelectFilesDialog

#endif
