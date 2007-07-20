/// file control

#ifndef FILE_CONTROL_H
#define FILE_CONTROL_H

#include "Settings.h"
#include <QtGui/QHBoxLayout>
#include <QtGui/QMainWindow>
#include <QtGui/QComboBox>
#include <QtCore/QTimer>

class cFileControl : public QObject
{
	Q_OBJECT

	public:
		/// drive information
		struct sDrive {
			QString qsPath;								///< path to drive's root
			// TODO sDrive add drive type and show next to drive letters
		};

		cFileControl(QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings, QComboBox *qcbLeftDrive, QComboBox *qcbRightDrive);
																///< constructor
																/**< \param qmwParent parent window for dialogs
																	  \param qbnlOperations layout for background and queued operations
																	  \param csSettings application's configuration
																	  \param qcbLeftDrive left drive\s combo box
																	  \param qcbRightDrive right drive's combo box */

		const QPair<QString, sDrive> GetFirstDrive() const;
																///< returns first accessible drive for application
																/**< \param first accessible drive for application */
	private:		
		static const int iTIMER_INTERVAL = 1000;	///< timer interval (ms)

		cSettings *csSettings;							///< application's configuration
		QComboBox *qcbLeftDrive;						///< left drive's combo box
		QComboBox *qcbRightDrive;						///< right drive's combo box
		QHBoxLayout *qhblOperations;					///< background and queued operation windows
		QMainWindow *qmwParent;							///< parent window for dialogs
		QTimer qtTimer;									///< timer for periodic actualizations

		const void ActualizeDrives() const;			///< drive list actualization
		const QList<QPair<QString, sDrive> > GetDrives() const;
																///< get accessible drives

	private slots:
		const void on_qtTimer_timeout() const;		///< timer's action
}; // cFileControl

#endif
