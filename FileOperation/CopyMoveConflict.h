/// copy/move conflict

#ifndef COPY_MOVE_CONFLICT_H
#define COPY_MOVE_CONFLICT_H

#include <QtCore/QFileInfo>

class cCopyMoveConflict : public QObject
{
	Q_OBJECT

	public:
		/// user response
		enum eChoice {
			Ask,																			///< no response yet
			Overwrite,																	///< overwrite once
			Cancel,																		///< cancel
			OverwriteAll,																///< overwrite all
			OverwriteAllOlder,														///< overwrite all older
			Skip,																			///< skip once
			SkipAll,																		///< skip all
			Rename,																		///< rename
			Append																		///< append to another file
		};

		cCopyMoveConflict();															///< constructor

	signals:
		void Finished(const cCopyMoveConflict::eChoice &ecResponse);	///< dialog closed with user response
																							/**< \param ecResponse user's response */
	private slots:
		void Show(const QString &qsOperation, const QFileInfo &qfiSource, const QFileInfo &qfiDestination);
																							///< show conflict dialog
																							/**< \param qsOperation type of operation - copy or move
																								  \param qfiSource source file information
																								  \param qfiDestination destination file information */
}; // cCopyMoveConflict

#endif
