/// rename

#ifndef RENAME_H
#define RENAME_H

#include <QObject>

class cRename : public QObject
{
	Q_OBJECT

	signals:
		void Finished(const QString &NewFilename);	///< dialog closed with user response
																	/**< \param NewFilename new file name */
	private slots:
		void Show(const QString &qsOldFilename);		///< show conflict dialog
																	/**< \param qsOldFilename file to rename */
}; // cRename

#endif