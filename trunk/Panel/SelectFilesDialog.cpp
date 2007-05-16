#include "Panel/SelectFilesDialog.h"

#include <QtGui/QHeaderView>
#include "FindFilesDialog.h"

// constructor
cSelectFilesDialog::cSelectFilesDialog(QMainWindow *qmwParent, const eSelectType &estType, cSettings *csSettings, cLister *clLister)
{
	setParent(qmwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);

	this->csSettings = csSettings;
	this->clLister = clLister;

	qcbFilter->installEventFilter(this);
	qtwFindSets->header()->setHidden(true);

	if (estType == Select) {
		setWindowTitle(tr("Select files"));
	} else {
		setWindowTitle(tr("Unselect files"));
	} // if else

	RefreshFindSets();
} // cSelectFilesDialog

// event filter
bool cSelectFilesDialog::eventFilter(QObject *watched, QEvent *event)
{
	if (watched == qcbFilter) {
		// clear selection of predefined filter if combo box gets focus
		if (event->type() == QEvent::FocusIn) {
			qtwFindSets->clearSelection();
		} // if
		return false;
	} else {
		return QDialog::eventFilter(watched, event);
	} // if else
} // eventFilter

// define button is clicked on
void cSelectFilesDialog::on_qpbDefine_clicked(bool checked /* false */)
{
	cFindFilesDialog cffdFind(this, NULL, csSettings, clLister);

	cffdFind.exec();
	RefreshFindSets();
} // on_qpbDefine_clicked

// refresh finds sets from settings file
void cSelectFilesDialog::RefreshFindSets()
{
	int iI;
	QStringList qslFindSets;

	qslFindSets = csSettings->GetSavedFinds();

	qtwFindSets->clear();
	for (iI = 0; iI < qslFindSets.count(); iI++) {
		QTreeWidgetItem *qtwiFind;

		qtwiFind = new QTreeWidgetItem(qtwFindSets);
		qtwiFind->setText(0, qslFindSets.at(iI));
	} // for
} // RefreshFindSets
