#include "FileSystem/DiskSpace.h"

#include <QtGui/QMessageBox>
#include <QtGui/QPushButton>

// show retry dialog
const void cDiskSpace::Show(const QString &qsFilename, const qint64 &qi64FileSize, const qint64 &qi64FreeSpace) const
{
	eChoice ecResponse;
	QMessageBox qmbDialog;
	QPushButton *qpbNo, *qpbSkip, *qpbSkipAll, *qpbYes, *qpbYesToAll;

	// prepare dialog
	qmbDialog.setIcon(QMessageBox::Warning);
	qmbDialog.setWindowTitle(tr("Disk space"));
	qmbDialog.setText(tr("Not enough disk space to copy file %1 of size %2. Free space on target drive is %3. Continue?").arg(qsFilename).arg(qi64FileSize).arg(qi64FreeSpace));
	qpbYes = qmbDialog.addButton(tr("&Yes"), QMessageBox::YesRole);
	qpbNo = qmbDialog.addButton(tr("&No"), QMessageBox::NoRole);
	qpbSkip = qmbDialog.addButton(tr("&Skip"), QMessageBox::NoRole);
	qpbYesToAll = qmbDialog.addButton(tr("Yes &to all"), QMessageBox::YesRole);
	qpbSkipAll = qmbDialog.addButton(tr("Skip &all"), QMessageBox::NoRole);

	qmbDialog.exec();

	if (qmbDialog.clickedButton() == qpbYes) {
		ecResponse = Yes;
	} else {
		if (qmbDialog.clickedButton() == qpbNo) {
			ecResponse = No;
		} else {
			if (qmbDialog.clickedButton() == qpbSkip) {
				ecResponse = Skip;
			} else {
				if (qmbDialog.clickedButton() == qpbYesToAll) {
					ecResponse = YesToAll;
				} else {
					ecResponse = SkipAll;
				} // if else
			} // if else
		} // if else
	} // if else

	emit Finished(ecResponse);
} // Show
