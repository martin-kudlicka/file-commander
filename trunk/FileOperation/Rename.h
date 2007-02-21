/// rename

#ifndef RENAME_H
#define RENAME_H

#include <QMainWindow>
#include "FileOperation/RenameDialog.h"

class cRename : public QObject
{
	Q_OBJECT

	public:
		cRename(QMainWindow *qmwParent);					///< constructor
																	/**< \param qmwParent parent window of conflict dialog */
		~cRename();												///< destructor

	private:
		cRenameDialog *crdDialog;							///< rename dialog

	signals:
		void Finished(const QString &NewFilename);	///< dialog closed with user response
																	/**< \param NewFilename new file name */
	private slots:
		void Show(const QString &qsOldFilename);		///< show conflict dialog
																	/**< \param qsOldFilename file to rename */
}; // cRename

#endif