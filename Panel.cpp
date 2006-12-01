#include "Panel.h"

#include <QDir>
#include <QDateTime>

// actualize widgets with info about current directory view
void cPanel::ActualizeWidgets()
{
	QDir qdDir;

	qdDir.setPath(qhTabs.value(qswDir->currentIndex()).swWidgets->qsPath);

	// tab bar
	if (qdDir.dirName() == "") {
		qtbTab->setTabText(qswDir->currentIndex(), qdDir.path().at(0));
	} else {
		qtbTab->setTabText(qswDir->currentIndex(), qdDir.dirName());
	} // if else
	// path
	qlPath->setText(qhTabs.value(qswDir->currentIndex()).swWidgets->qsPath);
} // ActualizeWidgets

// add new tab with dir view
void cPanel::AddTab(const cSettings::sTabInfo stiTabInfo)
{
	int iIndex;
	QTreeWidget *qtwTree;
	sTab stTab;

	// create tab
	qtbTab->addTab("");
	qtwTree = new QTreeWidget();
	qtwTree->setRootIsDecorated(false);
	qtwTree->setContextMenuPolicy(Qt::CustomContextMenu);
	qtwTree->setSelectionMode(QAbstractItemView::ExtendedSelection);
	iIndex = qswDir->addWidget(qtwTree);

	// connect signals to slots
	connect(qtwTree, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(on_qtwTree_customContextMenuRequested(const QPoint &)));
	connect(qtwTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), SLOT(on_qtwTree_itemDoubleClicked(QTreeWidgetItem *, int)));

	// set tab properties
	stTab.qhFiles = new QHash<QTreeWidgetItem *, QFileInfo>;
	stTab.qlColumns = new QList<cSettings::sColumn>;
	stTab.swWidgets = new sWidgets();
	stTab.swWidgets->qsPath = stiTabInfo.qsPath;
	stTab.qsColumnSet = stiTabInfo.qsColumnSet;
	qhTabs.insert(iIndex, stTab);

	// set header
	RefreshHeader(iIndex);
} // AddTab

// constructor
cPanel::cPanel(QStackedWidget *qswPanel)
{
	qswDir = qswPanel;
	csmMenu.hwParent = qswPanel->winId();
} // cPanel

// show tree view context menu
void cPanel::on_qtwTree_customContextMenuRequested(const QPoint &pos)
{
	csmMenu.qslObjects = GetSelectedItems();
	csmMenu.Show(static_cast<QTreeWidget *>(qswDir->currentWidget())->viewport()->mapToGlobal(pos));
} // on_qtwTree_customContextMenuRequested

// double click in tree view
void cPanel::on_qtwTree_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
	QFileInfo qfiFile;

	qfiFile = qhTabs.value(qswDir->currentIndex()).qhFiles->value(item);
	if (qfiFile.isDir()) {
		// double click on directory
		SetPath(qfiFile.filePath());
	} // if
} // on_qtwTree_itemDoubleClicked

// refresh dir content
void cPanel::RefreshContent(const int iIndex)
{
	int iI;
	QDir qdDir;
	QFileInfoList qfilFiles;

	// clear previous file contents
	static_cast<QTreeWidget *>(qswDir->widget(iIndex))->clear();
	qhTabs.value(iIndex).qhFiles->clear();

	// get file list
	qdDir.setPath(qhTabs.value(iIndex).swWidgets->qsPath);
	qfilFiles = qdDir.entryInfoList();

	// go through files and fill dir panel and add them into file list (qhFiles)
	for (iI = 0; iI < qfilFiles.count(); iI++) {
		int iJ;
		QTreeWidgetItem *qtwiFile;

		// omit some entries
		if (qfilFiles.at(iI).fileName() == ".") {
			QString a = qfilFiles.at(iI).fileName();
			continue;
		} // if

		// add to internal file list
		qtwiFile = new QTreeWidgetItem(static_cast<QTreeWidget *>(qswDir->widget(iIndex)));
		qhTabs.value(iIndex).qhFiles->insert(qtwiFile, qfilFiles.at(iI));

		// fill columns
		for (iJ = 0; iJ < qhTabs.value(iIndex).qlColumns->count(); iJ++) {
			if (qhTabs.value(iIndex).qlColumns->at(iJ).qsPlugin == qsNO) {
				// native
				if (qhTabs.value(iIndex).qlColumns->at(iJ).qsIdentifier == qsICON) {
					qtwiFile->setIcon(iJ, qfipIcon.icon(qfilFiles.at(iI)));
				} else {
					if (qhTabs.value(iIndex).qlColumns->at(iJ).qsIdentifier == qsNAME) {
						qtwiFile->setText(iJ, qfilFiles.at(iI).fileName());
					} else {
						if (qhTabs.value(iIndex).qlColumns->at(iJ).qsIdentifier == qsEXTENSION) {
							qtwiFile->setText(iJ, qfilFiles.at(iI).suffix());
						} else {
							if (qhTabs.value(iIndex).qlColumns->at(iJ).qsIdentifier == qsDATE) {
								qtwiFile->setText(iJ, qfilFiles.at(iI).lastModified().toString());
							} // if
						} // if else
					} // if else
				} // if else
			} else {
				// plugin
				qtwiFile->setText(iJ, ccContent->GetPluginValue(qfilFiles.at(iI).filePath(), qhTabs.value(iIndex).qlColumns->at(iJ).qsPlugin, qhTabs.value(iIndex).qlColumns->at(iJ).qsIdentifier, qhTabs.value(iIndex).qlColumns->at(iJ).qsUnit));
			} // if else
		} // for
	} // for

	ActualizeWidgets();
} // RefreshContent

// get selected files and directories from current dir view
QStringList cPanel::GetSelectedItems()
{
	int iI;
	QList<QTreeWidgetItem *> qlSelected;
	QStringList qslSelected;

	qlSelected = static_cast<QTreeWidget *>(qswDir->currentWidget())->selectedItems();
	for (iI = 0; iI < qlSelected.count(); iI++) {
		qslSelected.append(QDir::toNativeSeparators(qhTabs.value(qswDir->currentIndex()).qhFiles->value(qlSelected.at(iI)).filePath()));
	} // for

	return qslSelected;
} // GetSelectedItems

// refresh column's header
void cPanel::RefreshHeader(const int iIndex)
{
	int iI;
	QStringList qslColumns;
	QTreeWidgetItem *qtwiHeader;

	// clear previous header contents and fill new information
	qhTabs.value(iIndex).qlColumns->clear();
	qslColumns = csSettings->GetColumnsInSet(qhTabs.value(iIndex).qsColumnSet);
	if (qslColumns.count() == 0 && qhTabs.value(iIndex).qsColumnSet == qsFULL) {
		// create default column set (Full) if not created yet
		csSettings->CreateDefaultColumnSet();
		qslColumns = csSettings->GetColumnsInSet(qhTabs.value(iIndex).qsColumnSet);
	} // if
	for (iI = 0; iI < qslColumns.count(); iI++) {
		cSettings::sColumn scColumn;

		scColumn = csSettings->GetColumnInfo(qhTabs.value(iIndex).qsColumnSet, qslColumns.at(iI));
		// test if is native or plugin and loaded
		if (scColumn.qsPlugin == qsNO || ccContent->Loaded(scColumn.qsPlugin)) {
			// ok -> add to column list
			qhTabs.value(iIndex).qlColumns->append(scColumn);
		} // if
	} // for

	// set header according to qlColumns list
	qtwiHeader = new QTreeWidgetItem();
	for (iI = 0; iI < qhTabs.value(iIndex).qlColumns->count(); iI++) {
		qtwiHeader->setText(iI, qhTabs.value(iIndex).qlColumns->at(iI).qsName);
	} // for
	static_cast<QTreeWidget *>(qswDir->widget(iIndex))->setHeaderItem(qtwiHeader);

	// refresh dir content according to new header
	RefreshContent(iIndex);
} // RefreshHeader

// set new path for current dir view
void cPanel::SetPath(const QString qsPath)
{
	qhTabs.value(qswDir->currentIndex()).swWidgets->qsPath = QDir::cleanPath(qsPath);
	RefreshContent(qswDir->currentIndex());
} // SetPath