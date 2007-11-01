/// rename

#ifndef RENAME_H
#define RENAME_H

#include <QtCore/QObject>
#include <QtGui/QMainWindow>

class cRename : public QObject
{
	Q_OBJECT

	public:
		cRename(QMainWindow *qmwParent);						///< constructor
																		/**< \param qmwParent parent window of this dialog */

	private:
		QMainWindow *qmwParent;									///< parent window for dialog

	signals:
		void Finished() const;									///< dialog closed with user response

	private slots:
		const void Show(QString *qsNewFilename) const;	///< show conflict dialog
																		/**< \param qsOldFilename new file name */
}; // cRename

#endif
