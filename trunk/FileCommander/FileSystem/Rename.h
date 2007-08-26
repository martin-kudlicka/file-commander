/// rename

#ifndef RENAME_H
#define RENAME_H

#include <QtCore/QObject>

class cRename : public QObject
{
	Q_OBJECT

	signals:
		void Finished(const QString &NewFilename) const;		///< dialog closed with user response
																				/**< \param NewFilename new file name */
	private slots:
		const void Show(const QString &qsOldFilename) const;	///< show conflict dialog (multithread)
																				/**< \param qsOldFilename file to rename */
}; // cRename

#endif
