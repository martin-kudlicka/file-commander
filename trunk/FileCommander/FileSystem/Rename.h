/// rename

#ifndef RENAME_H
#define RENAME_H

#include <QtCore/QObject>

class cRename : public QObject
{
	Q_OBJECT

	signals:
		void Finished() const;									///< dialog closed with user response

	private slots:
		const void Show(QString *qsNewFilename) const;	///< show conflict dialog
																		/**< \param qsOldFilename new file name */
}; // cRename

#endif
