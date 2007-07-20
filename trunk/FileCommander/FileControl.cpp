#include "FileControl.h"

#include <QtCore/QFileInfoList>
#include <QtCore/QDir>

// drive list actualization
const void cFileControl::ActualizeDrives() const
{
	QList<QPair<QString, sDrive> > qlDrives;

	qlDrives = GetDrives();

	if (qcbLeftDrive->count() != qlDrives.count()) {
		// assume drives have changed
		int iI;

		QString qsLeftDrive, qsRightDrive;

		qsLeftDrive = qcbLeftDrive->currentText();
		qsRightDrive = qcbRightDrive->currentText();

		qcbLeftDrive->blockSignals(true);
		qcbRightDrive->blockSignals(true);

		for (iI = 0; iI < qlDrives.count(); iI++) {
			QPair<QString, sDrive> *qpDrive;

			qpDrive = &qlDrives[iI];
			qcbLeftDrive->addItem(qpDrive->first);
			qcbRightDrive->addItem(qpDrive->first);
		} // for

		// check for selected drive change, changes are handled in Panel class
		if (qcbLeftDrive->findText(qsLeftDrive) == -1) {
			// selected drive changed
			qcbLeftDrive->blockSignals(false);
			qcbLeftDrive->setCurrentIndex(-1);
		} else {
			// selected drive not changed
			qcbLeftDrive->setCurrentIndex(qcbLeftDrive->findText(qsLeftDrive));
			qcbLeftDrive->blockSignals(false);
		} // if else
		if (qcbRightDrive->findText(qsRightDrive) == -1) {
			// selected drive changed
			qcbRightDrive->blockSignals(false);
			qcbRightDrive->setCurrentIndex(-1);
		} else {
			// selected drive not changed
			qcbRightDrive->setCurrentIndex(qcbRightDrive->findText(qsRightDrive));
			qcbRightDrive->blockSignals(false);
		} // if else
	} // if
} // ActualizeDrives

// constructor
cFileControl::cFileControl(QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings, QComboBox *qcbLeftDrive, QComboBox *qcbRightDrive)
{
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
	this->csSettings = csSettings;
	this->qcbLeftDrive = qcbLeftDrive;
	this->qcbRightDrive = qcbRightDrive;

	// automatic actualizations
	connect(&qtTimer, SIGNAL(timeout()), SLOT(on_qtTimer_timeout()));
	qtTimer.start(iTIMER_INTERVAL);
} // cFileControl

// get accessible drives
const QList<QPair<QString, cFileControl::sDrive> > cFileControl::GetDrives() const
{
	int iI;
	QFileInfoList qfilDrives;
	QList<QPair<QString, sDrive> > qlDrives;

	// local drives
	qfilDrives = QDir::drives();
	for (iI = 0; iI < qfilDrives.count(); iI++) {
		QFileInfo *qfiDrive;
		sDrive sdDrive;

		qfiDrive = &qfilDrives[iI];
		sdDrive.qsPath = qfiDrive->path();
		qlDrives.append(QPair<QString, sDrive>(qfiDrive->path().at(0), sdDrive));
	} // for

	return qlDrives;
} // GetDrives

// returns first accessible drive for application
const QPair<QString, cFileControl::sDrive> cFileControl::GetFirstDrive() const
{
	QList<QPair<QString, sDrive> > qlDrives;

	qlDrives = GetDrives();

	return qlDrives.at(0);
} // GetFirstDrive

// timer's action
const void cFileControl::on_qtTimer_timeout() const
{
	ActualizeDrives();
} // on_qtTimer_timeout
