#include "FileSystem/Archive/InformationDialog.h"

#include <QtCore/QMetaType>

// constructor
cInformationDialog::cInformationDialog(QMainWindow *qmwParent)
{
	this->qmwParent = qmwParent;
	qRegisterMetaType<enum QMessageBox::Icon>("enum QMessageBox::Icon");
} // cInformationDialog

// show permission dialog
const void cInformationDialog::Show(const enum QMessageBox::Icon &iIcon, const QString &qsInformation) const
{
	QMessageBox *qmbDialog;
	QString qsTitle;

	switch (iIcon) {
		case QMessageBox::Critical:
			qsTitle = tr("Critical");
			break;
		case QMessageBox::Information:
			qsTitle = tr("Information");
			break;
		case QMessageBox::Question:
			qsTitle = tr("Question");
			break;
		case QMessageBox::Warning:
			qsTitle = tr("Warning");
		default:
			;
	} // switch
	qmbDialog = new QMessageBox(iIcon, qsTitle, qsInformation, QMessageBox::Ok, qmwParent);

	qmbDialog->exec();

	qmbDialog->deleteLater();
	emit Finished();
} // Show
