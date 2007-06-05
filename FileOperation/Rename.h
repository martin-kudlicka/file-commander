/// rename

#ifndef RENAME_H
#define RENAME_H

#include <QtCore/QObject>

class cRename : public QObject
{
	Q_OBJECT

	public:
		QString Exec(const QString &qsOldFilename);			///< show conflict dialog (single thread)
																			/**< \param qsOldFilename file to rename */
	private:
		QString ShowDialog(const QString &qsOldFilename);	///< show conflict dialog
																			/**< \param qsOldFilename file to rename */
	signals:
		void Finished(const QString &NewFilename);			///< dialog closed with user response
																			/**< \param NewFilename new file name */
	private slots:
		void Show(const QString &qsOldFilename);				///< show conflict dialog (multithread)
																			/**< \param qsOldFilename file to rename */
}; // cRename

#endif
