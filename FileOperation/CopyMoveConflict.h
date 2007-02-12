/// copy/move conflict

#ifndef COPY_MOVE_CONFLICT_H
#define COPY_MOVE_CONFLICT_H

#include <QMainWindow>
#include "FileOperation/CopyMoveConflictDialog.h"
#include <QFileInfo>

class cCopyMoveConflict : public QObject
{
	Q_OBJECT

	public:
		cCopyMoveConflict(QMainWindow *qmwParent);	///< constructor
																	/**< \param qmwParent parent window of conflict dialog */
		~cCopyMoveConflict();								///< destructor

	private:
		cCopyMoveConflictDialog *ccmcdDialog;			///< conflict dialog

	signals:
		void Finished(const cCopyMoveConflictDialog::eChoice &ecResponse);
																	///< dialog closed with user response
																	/**< \param ecResponse user's response */
	private slots:
		void Show(const QString &qsOperation, const QFileInfo &qfiSource, const QFileInfo &qfiDestination);
																	///< show conflict dialog
																	/**< \param qsOperation type of operation - copy or move
																		  \param qfiSource source file information
																		  \param qfiDestination destination file information */
}; // cCopyMoveConflict

#endif