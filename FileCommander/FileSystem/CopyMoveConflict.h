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
			Ask,																					///< no response yet
			Overwrite,																			///< overwrite once
			Cancel,																				///< cancel
			OverwriteAll,																		///< overwrite all
			OverwriteAllOlder,																///< overwrite all older
			Skip,																					///< skip once
			SkipAll,																				///< skip all
			Rename,																				///< rename
			Append																				///< append to another file
		};

		cCopyMoveConflict();																	///< constructor

	signals:
		void Finished(const cCopyMoveConflict::eChoice &ecResponse) const;	///< dialog closed with user response
																									/**< \param ecResponse user's response */
	private slots:
		const void Show(const QString &qsOperation, const QString &qsSource, const qint64 &qi64SourceSize, const QDateTime &qdtSourceLastModified, const QFileInfo &qfiDestination) const;
																									///< show conflict dialog
																									/**< \param qsOperation type of operation - copy or move
																										  \param qsSource source file
																										  \param qi64SourceSize source file size
																										  \param qdtSourceLastModified source file last modified timestamp
																										  \param qfiDestination destination file information */
}; // cCopyMoveConflict

#endif
