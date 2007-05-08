#include "Panel.h"

#include <QDir>
#include <QDateTime>
#include "Panel/Process.h"
#include <QHeaderView>
#include <QProcess>
#include "Panel/SelectDriveDialog.h"
#include <QKeyEvent>
#include "FindFilesDialog.h"
#include <QMenu>
#include <QUrl>
#include <QMessageBox>
#include <QLineEdit>

cSettings::sSort cPanel::ssSort;			///< sort information (static class variable)
QStackedWidget *cPanel::qswLastActive;	///< last active panel (static class variable)

// destructor
cPanel::~cPanel()
{
	delete csmMenu;
	ccdContentDelayed->deleteLater();
} // ~cPanel

// actualize volume information - disk name and space
void cPanel::ActualizeVolumeInfo()
{
	cFileRoutine::sDiskSpace sdsInfo;
	QString qsName;

#ifdef Q_WS_WIN
	if (qhTabs.count() == 0) {
		// do not show information before at least one tab is created to prevent main window resizing
		return;
	} // if
	qsName = cFileRoutine::GetVolumeName(qmDrives->value(qcbDrive->currentText()).qsPath);
#else
	qsName = qcbDrive->currentText();
#endif
	sdsInfo = cFileRoutine::GetDiskSpace(qmDrives->value(qcbDrive->currentText()).qsPath);
	qlDriveInfo->setText(tr("[%1] %2 of %3 free").arg(qsName).arg(GetSizeString(sdsInfo.qi64Free)).arg(GetSizeString(sdsInfo.qi64Total)));
} // ActualizeVolumeInfo

// actualize widgets with info about current directory view
void cPanel::ActualizeWidgets()
{
	SetTabText(qswDir->currentIndex());

	qcbDrive->blockSignals(true);
	qcbDrive->setCurrentIndex(qcbDrive->findText(qhTabs.value(qswDir->currentIndex()).swWidgets->qsDrive));
	qcbDrive->blockSignals(false);
	ActualizeVolumeInfo();
	qlPath->setText(qhTabs.value(qswDir->currentIndex()).swWidgets->qsPath);
	qlSelected->setText(qhTabs.value(qswDir->currentIndex()).swWidgets->qsSelected);
} // ActualizeWidgets

// add new tab with dir view
int cPanel::AddTab(const cSettings::sTabInfo &stiTabInfo, const bool &bStartUp /* false */)
{
	cTreeWidget *ctwTree;
	int iIndex;
	sTab stTab;

	// create tab
	ctwTree = new cTreeWidget();
	ctwTree->setRootIsDecorated(false);
	ctwTree->setContextMenuPolicy(Qt::CustomContextMenu);
	ctwTree->setSelectionMode(QAbstractItemView::ExtendedSelection);
	if (bStartUp || !csSettings->GetOpenNewTabNextToCurrentTab()) {
		iIndex = qswDir->addWidget(ctwTree);
	} else {
		iIndex = qswDir->insertWidget(qswDir->currentIndex() + 1, ctwTree);
	} // if else

	// connect signals to slots
	connect(ctwTree, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(on_ctwTree_customContextMenuRequested(const QPoint &)));
	connect(ctwTree, SIGNAL(itemActivated(QTreeWidgetItem *, int)), SLOT(on_ctwTree_itemActivated(QTreeWidgetItem *, int)));
	connect(ctwTree, SIGNAL(itemSelectionChanged(const cTreeWidget *)), SLOT(on_ctwTree_itemSelectionChanged(const cTreeWidget *)));
	connect(ctwTree, SIGNAL(KeyPressed(QKeyEvent *, QTreeWidgetItem *)), SLOT(on_ctwTree_KeyPressed(QKeyEvent *, QTreeWidgetItem *)));
	connect(ctwTree, SIGNAL(GotFocus()), SLOT(on_ctwTree_GotFocus()));
	connect(ctwTree, SIGNAL(DropEvent(const cTreeWidget::eDropAction &, const QList<QUrl> &)), SLOT(on_ctwTree_DropEvent(const cTreeWidget::eDropAction &, const QList<QUrl> &)));
	connect(ctwTree, SIGNAL(DragEvent(cTreeWidget *)), SLOT(on_ctwTree_DragEvent(cTreeWidget *)));

	// set tab properties
	stTab.qhFiles = new QHash<QTreeWidgetItem *, QFileInfo>;
	stTab.qlColumns = new QList<cSettings::sColumn>;
	stTab.swWidgets = new sWidgets();
	stTab.swWidgets->qsDrive = stiTabInfo.qsDrive;
	stTab.swWidgets->qsPath = stiTabInfo.qsPath;
	stTab.qsColumnSet = stiTabInfo.qsColumnSet;
	stTab.bValid = new bool();
	if (qhTabs.contains(iIndex)) {
		// move tabs by one place
		int iI;

		for (iI = qhTabs.count() - 1; iI >= iIndex; iI--) {
			qhTabs.insert(iI + 1, qhTabs.value(iI));
		} // for
	} // if
	qhTabs.insert(iIndex, stTab);

	// add new tab into GUI
	qtbTab->blockSignals(true);
	qtbTab->insertTab(iIndex, "");
	qtbTab->blockSignals(false);
	SetTabText(iIndex);

	// add path to watcher
	qfswWatcher.addPath(stiTabInfo.qsPath);

	// set header, refresh content only in first added tab
	RefreshHeader(iIndex, qhTabs.count() == 1);

	// set sorting
	ctwTree->header()->setSortIndicator(stiTabInfo.ssSort.iSortedColumn, stiTabInfo.ssSort.soSortOrder);

	// connect signals to slots
	connect(ctwTree->header(), SIGNAL(sectionClicked(int)), SLOT(on_qhvTreeHeader_sectionClicked(int)));

	HideOrShowTabBar();

	return iIndex;
} // AddTab

// close all other tabs than selected
void cPanel::CloseAllOtherTabs(const int &iTabIndex)
{
	if (qhTabs.count() > 1) {
		bool bCloseAllOtherTabs;

		if (csSettings->GetConfirmCloseOfAllTabs()) {
			if (QMessageBox::question(qmwParent, tr("Close all other tabs"), tr("Continue?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
				bCloseAllOtherTabs = true;
			} else {
				bCloseAllOtherTabs = false;
			} // if else
		} else {
			bCloseAllOtherTabs = true;
		} // if else

		if (bCloseAllOtherTabs) {
			int iI;

			qhTabs.insert(0, qhTabs.value(iTabIndex));
			qtbTab->setTabText(0, qtbTab->tabText(iTabIndex));
			qswDir->insertWidget(0, qswDir->widget(iTabIndex));
			for (iI = qhTabs.count() - 1; iI > 0; iI--) {
				qhTabs.remove(iI);
				qtbTab->removeTab(iI);
				qswDir->removeWidget(qswDir->widget(iTabIndex));
			} // for
		} // if
	} // if
} // CloseAllOtherTabs

// close tab
void cPanel::CloseTab(const int &iTabIndex)
{
	if (qhTabs.count() > 1) {
		int iI;

		// reposition of tabs following remove tab
		for (iI = iTabIndex + 1; iI < qhTabs.count(); iI++) {
			qhTabs.insert(iI - 1, qhTabs.value(iI));
		} // for
		qhTabs.remove(qhTabs.count() - 1);
		qtbTab->removeTab(iTabIndex);
		qswDir->removeWidget(qswDir->widget(iTabIndex));

		HideOrShowTabBar();
		ActualizeWidgets();

		static_cast<cTreeWidget *>(qswDir->currentWidget())->setFocus(Qt::OtherFocusReason);
	} // if
} // CloseTab

// close tab
void cPanel::CloseTab(const QMouseEvent *qmeEvent)
{
	if (csSettings->GetCloseTabOnDoubleClick() && qhTabs.count() > 1) {
		int iTabIndex;

		iTabIndex = GetTabIndex(qmeEvent->pos());
		if (iTabIndex != -1) {
			CloseTab(iTabIndex);
		} // if
	} // if
} // CloseTab

// constructor
cPanel::cPanel(QMainWindow *qmwParent, QStackedWidget *qswPanel, QComboBox *qcbDrive, QLabel *qlDriveInfo, QTabBar *qtbTab, QLabel *qlPath, QLabel *qlSelected, cSettings *csSettings, cContent *ccContent, QMap<QString, cFileRoutine::sDriveInfo> *qmDrives, QLabel *qlGlobalPath, QComboBox *qcbCommand, cFileOperation *cfoFileOperation, QLineEdit *qleQuickSearch)
{
	qswDir = qswPanel;
	this->qcbDrive = qcbDrive;
	this->qlDriveInfo = qlDriveInfo;
	this->qtbTab = qtbTab;
	this->qlPath = qlPath;
	this->qlSelected = qlSelected;
	this->csSettings = csSettings;
	this->ccContent = ccContent;
	this->qmDrives = qmDrives;
	this->qmwParent = qmwParent;
	this->qlGlobalPath = qlGlobalPath;
	this->qcbCommand = qcbCommand;
	this->cfoFileOperation = cfoFileOperation;
	this->qleQuickSearch = qleQuickSearch;

	csmMenu = new cShellMenu(
#ifdef Q_WS_WIN
	qswDir->winId()
#endif
	);

	// signals for controls
	connect(qcbDrive, SIGNAL(activated(int)), SLOT(on_qcbDrive_activated(int)));
	connect(qcbDrive, SIGNAL(currentIndexChanged(int)), SLOT(on_qcbDrive_currentIndexChanged(int)));
	connect(&qfswWatcher, SIGNAL(directoryChanged(const QString &)), SLOT(on_qfswWatcher_directoryChanged(const QString &)));
	connect(qtbTab, SIGNAL(currentChanged(int)), SLOT(on_qtbTab_currentChanged(int)));

	// timer
	connect(&qtTimer, SIGNAL(timeout()), SLOT(on_qtTimer_timeout()));
	qtTimer.start(iTIMER_INTERVAL);

	// delayed column update
	ccdContentDelayed = new cContentDelayed(ccContent);
	connect(ccdContentDelayed, SIGNAL(GotColumnValue(const cContentDelayed::sOutput &)), SLOT(on_ccdContentDelayed_GotColumnValue(const cContentDelayed::sOutput &)));
	connect(this, SIGNAL(InterruptContentDelayed()), ccdContentDelayed, SLOT(on_InterruptContentDelayed()));

	// event filters
	qcbCommand->installEventFilter(this);
	qtbTab->installEventFilter(this);
	qleQuickSearch->installEventFilter(this);
} // cPanel

// create new tab by duplicate one
int cPanel::DuplicateTab(const int &iTabIndex)
{
	cSettings::sTabInfo stiTabInfo;

	stiTabInfo.qsColumnSet = qhTabs.value(iTabIndex).qsColumnSet;
	stiTabInfo.qsDrive = qhTabs.value(iTabIndex).swWidgets->qsDrive;
	stiTabInfo.qsPath = qhTabs.value(iTabIndex).swWidgets->qsPath;
	stiTabInfo.ssSort.iSortedColumn = static_cast<cTreeWidget *>(qswDir->widget(iTabIndex))->sortColumn();
	stiTabInfo.ssSort.soSortOrder = static_cast<cTreeWidget *>(qswDir->widget(iTabIndex))->header()->sortIndicatorOrder();

	return AddTab(stiTabInfo);
} // DuplicateTab

// edit selected file in editor
void cPanel::EditFile()
{
	QFileInfo qfiFile;

	qfiFile = qhTabs.value(qswDir->currentIndex()).qhFiles->value(static_cast<cTreeWidget *>(qswDir->currentWidget())->currentItem());

	if (qfiFile.isFile()) {
		QString qsCommand;

		qsCommand = csSettings->GetExternalEditor();
		qsCommand = qsCommand.replace("%1", qfiFile.filePath());

		cProcess::Execute(qsCommand);
	} // if
} // EditFile

// event filter
bool cPanel::eventFilter(QObject *watched, QEvent *event)
{
	if (watched == qcbCommand) {
		// command combo box
		if (event->type() == QEvent::KeyPress) {
			if (static_cast<QKeyEvent *>(event)->key() == Qt::Key_Down || static_cast<QKeyEvent *>(event)->key() == Qt::Key_Up) {
				if (qswLastActive == qswDir) {
					qswLastActive->currentWidget()->setFocus(Qt::OtherFocusReason);
					QApplication::sendEvent(qswLastActive->currentWidget(), event);
					return true;
				} else {
					return false;
				} // if else
			} // if
		} // if
		return false;
	} else {
		if (watched == qtbTab) {
			// tab bar
			if (event->type() == QEvent::MouseButtonDblClick) {
				CloseTab(static_cast<QMouseEvent *>(event));
				return true;
			} else {
				return false;
			} // if else
		} else {
			if (watched == qleQuickSearch) {
				// quick search
				switch (event->type()) {
					case QEvent::FocusIn:	qleQuickSearch->clear();
													if (csSettings->GetQuickSearchShowSearchWindow()) {
														qleQuickSearch->show();
													} // if
													return false;
					case QEvent::FocusOut:	qleQuickSearch->hide();
													return false;
					case QEvent::KeyPress:	switch (static_cast<QKeyEvent *>(event)->key()) {
														case Qt::Key_Backspace: return false;
														case Qt::Key_Down:		return !QuickSearch(NULL, SearchDown);
														case Qt::Key_Enter:
														case Qt::Key_Return:		if (qswLastActive == qswDir) {
																							qswLastActive->currentWidget()->setFocus(Qt::OtherFocusReason);
																							QApplication::sendEvent(qswLastActive->currentWidget(), event);
																							return true;
																						} else {
																							return false;
																						} // if else
														case Qt::Key_Escape:		if (qswLastActive == qswDir) {
																							qswLastActive->currentWidget()->setFocus(Qt::OtherFocusReason);
																							return true;
																						} else {
																							return false;
																						} // if else
														case Qt::Key_Up:			return !QuickSearch(NULL, SearchUp);
														default:						if (static_cast<QKeyEvent *>(event)->text().isEmpty()) {
																							// white char obtained
																							return false;
																						} else {
																							bool bSearch;

																							bSearch = QuickSearch(static_cast<QKeyEvent *>(event)->text(), SearchDown);

																							if (qleQuickSearch->isVisible()) {
																								// search in window
																								return !bSearch;
																							} else {
																								// search without window -> set focus back to dir view
																								if (qswLastActive == qswDir) {
																									qswLastActive->currentWidget()->setFocus(Qt::OtherFocusReason);
																									return true;
																								} else {
																									return false;
																								} // if else
																							} // if else
																						} // if else
													} // switch
					default:						return false;
				} // switch
			} else {
				// the rest
				return QObject::eventFilter(watched, event);
			} // if else
		} // if else
	} // if else
} // eventFilter

// show custom list of files in current dir view
void cPanel::FeedToPanel(QFileInfoList &qfilFiles)
{
	RefreshContent(qswDir->currentIndex(), qfilFiles);
} // FeedToPanel

// columns for current dir view
QList<cSettings::sColumn> *cPanel::GetColumns()
{
	return qhTabs.value(qswDir->currentIndex()).qlColumns;
} // GetColumns

// get content of the directory view
QHash<QTreeWidgetItem *, QFileInfo> *cPanel::GetDirContent()
{
	return qhTabs.value(qswDir->currentIndex()).qhFiles;
} // GetDirContent

// find index of native column
int cPanel::GetNativeColumnIndex(const QString &qsColumn, const int &iTabIndex)
{
	int iI;

	for (iI = 0; iI < qhTabs.value(iTabIndex).qlColumns->count(); iI++) {
		if (qhTabs.value(iTabIndex).qlColumns->at(iI).qsIdentifier == qsColumn) {
			return iI;
		}
	} // for

	return -1;
} // GetNativeColumnIndex

// get path for current dir
QString cPanel::GetPath()
{
	return qhTabs.value(qswDir->currentIndex()).swWidgets->qsPath;
} // GetPath

// get dir infos of selected items
QFileInfoList cPanel::GetSelectedDirItemsList()
{
	int iI;
	QFileInfoList qfilDirectories;
	QList<QTreeWidgetItem *> qlSelected;

	qlSelected = static_cast<cTreeWidget *>(qswDir->currentWidget())->selectedItems();
	for (iI = 0; iI < qlSelected.count(); iI++) {
		if (qhTabs.value(qswDir->currentIndex()).qhFiles->value(qlSelected.at(iI)).fileName() != ".." && qhTabs.value(qswDir->currentIndex()).qhFiles->value(qlSelected.at(iI)).isDir()) {
			// ignore ".." directories
			qfilDirectories.append(qhTabs.value(qswDir->currentIndex()).qhFiles->value(qlSelected.at(iI)));
		} // if
	} // for

	return qfilDirectories;
} // GetSelectedDirItemsList

// get file infos of selected items
QFileInfoList cPanel::GetSelectedItemsList()
{
	int iI;
	QFileInfoList qfilFiles;
	QList<QTreeWidgetItem *> qlSelected;

	qlSelected = static_cast<cTreeWidget *>(qswDir->currentWidget())->selectedItems();
	for (iI = 0; iI < qlSelected.count(); iI++) {
		if (qhTabs.value(qswDir->currentIndex()).qhFiles->value(qlSelected.at(iI)).fileName() != "..") {
			// ignore ".." directories
			qfilFiles.append(qhTabs.value(qswDir->currentIndex()).qhFiles->value(qlSelected.at(iI)));
		} // if
	} // for

	return qfilFiles;
} // GetSelectedItemsList

// get selected files and directories from current dir view
QStringList cPanel::GetSelectedItemsStringList()
{
	int iI;
	QList<QTreeWidgetItem *> qlSelected;
	QStringList qslSelected;

	qlSelected = static_cast<cTreeWidget *>(qswDir->currentWidget())->selectedItems();
	for (iI = 0; iI < qlSelected.count(); iI++) {
		qslSelected.append(QDir::toNativeSeparators(qhTabs.value(qswDir->currentIndex()).qhFiles->value(qlSelected.at(iI)).filePath()));
	} // for

	return qslSelected;
} // GetSelectedItemsStringList

// "convert" size to string according to setting in options
QString cPanel::GetSizeString(const qint64 &qi64Size)
{
	QString qsFileSizeIn;

	qsFileSizeIn = csSettings->GetFileSizeIn();

	if (qsFileSizeIn == qsDYNAMIC) {
		if (qi64Size < qi64_KILOBYTE) {
			qsFileSizeIn = qsBYTES;
		} else {
			if (qi64Size < qi64_MEGABYTE) {
				qsFileSizeIn = qsKILOBYTES;
			} else {
				if (qi64Size < qi64_GIGABYTE) {
					qsFileSizeIn = qsMEGABYTES;
				} else {
					qsFileSizeIn = qsGIGABYTES;
				} // if else
			} // if else
		} // if else
	} // if

	if (qsFileSizeIn == qsBYTES) {
		return QVariant(qi64Size).toString() + tr(" B");
	} else {
		if (qsFileSizeIn == qsKILOBYTES) {
			return QString("%1 k").arg(static_cast<double>(qi64Size) / qi64_KILOBYTE, 0, 'f', 1);
		} else {
			if (qsFileSizeIn == qsMEGABYTES) {
				return QString("%1 M").arg(static_cast<double>(qi64Size) / qi64_MEGABYTE, 0, 'f', 1);
			} else {
				return QString("%1 G").arg(static_cast<double>(qi64Size) / qi64_GIGABYTE, 0, 'f', 1);
			} // if else
		} // if else
	} // if else
} // GetSizeString

// filters for file query
QDir::Filters cPanel::GetStandardFilters()
{
	QDir::Filters fFilters;

	fFilters = QDir::Dirs | QDir::Files;

	if (csSettings->GetShowSystemFiles()) {
		fFilters |= QDir::System;
	} // if
	if (csSettings->GetShowHiddenFiles()) {
		fFilters |= QDir::Hidden;
	} // if

	return fFilters;
} // GetStandardFilters

// find out tab index in tab bar
int cPanel::GetTabIndex(const QPoint &qpPos)
{
	int iI;

	for (iI = 0; iI < qtbTab->count(); iI++) {
		if (qtbTab->tabRect(iI).contains(qpPos)) {
			return iI;
		} // if
	} // for

	return -1;
} // GetTabIndex

// show dir view with specified file
void cPanel::GoToFile(const QString &qsFile)
{
	// refresh content to new path
	SetPath(QFileInfo(qsFile).path());

	// find the right file
	QHashIterator<QTreeWidgetItem *, QFileInfo> qhiFiles(*qhTabs.value(qswDir->currentIndex()).qhFiles);
	while (qhiFiles.hasNext()) {
		qhiFiles.next();
		if (qhiFiles.value().fileName() == QFileInfo(qsFile).fileName()) {
			static_cast<QTreeWidget *>(qswDir->currentWidget())->setCurrentItem(qhiFiles.key());
			break;
		} // if
	} // while
} // GoToFile

// go to root directory
void cPanel::GoToRootDir()
{
	QDir qdDir;

	qdDir.setPath(qhTabs.value(qswDir->currentIndex()).swWidgets->qsPath);
	if(!qdDir.isRoot()) {
		SetPath(qdDir.rootPath());
	} // if
} // GoToRootDir

// go up one level (directory)
void cPanel::GoToUpDir()
{
	QDir qdDir;

	qdDir.setPath(qhTabs.value(qswDir->currentIndex()).swWidgets->qsPath);
	if(!qdDir.isRoot()) {
		QString qsFrom;

		// remember directory going from
		qsFrom = QFileInfo(GetPath()).fileName();

		SetPath(qhTabs.value(qswDir->currentIndex()).swWidgets->qsPath + "/..");

		// find directory went from and set it as current
		QHashIterator<QTreeWidgetItem *, QFileInfo> qhiFile(*qhTabs.value(qswDir->currentIndex()).qhFiles);
		while (qhiFile.hasNext()) {
			qhiFile.next();
			if (qhiFile.value().fileName() == qsFrom) {
				static_cast<QTreeWidget *>(qswDir->currentWidget())->setCurrentItem(qhiFile.key());
				break;
			} // if
		} // while
	} // if
} // GoToUpDir

// hide or show tab bar as set in options
void cPanel::HideOrShowTabBar()
{
	if (csSettings->GetShowTabBarWithOnlyOneTab()) {
		qtbTab->show();
	} else {
		if (qhTabs.count() == 1) {
			qtbTab->hide();
		} else {
			qtbTab->show();
		} // if else
	} // if else
} // HideOrShowTabBar

// invert selection of files
void cPanel::InvertSelection()
{
	QString qsMode;

	qsMode = csSettings->GetSelectionMode();
	QHashIterator<QTreeWidgetItem *, QFileInfo> qhiFile(*qhTabs.value(qswDir->currentIndex()).qhFiles);
	while (qhiFile.hasNext()) {
		qhiFile.next();
		if (qhiFile.value().isDir()) {
			if (qsMode == qsFILES_AND_DIRECTORIES) {
				qhiFile.key()->setSelected(!qhiFile.key()->isSelected());
			} // if
		} else {
			qhiFile.key()->setSelected(!qhiFile.key()->isSelected());
		} // if else
	} // while
} // InvertSelection

// find out if qsDirectory points to the root
bool cPanel::IsRootDirectory(const QString &qsDirectory)
{
	if (QDir(qsDirectory).cdUp()) {
		return false;
	} else {
		return true;
	} // if else
} // IsRootDirectory

// active panel flag
bool cPanel::IsActive()
{
	return qswDir == qswLastActive;
} // IsActive

// got golumn value from plugin
void cPanel::on_ccdContentDelayed_GotColumnValue(const cContentDelayed::sOutput &soOutput)
{
	soOutput.qtwiItem->setText(soOutput.iColumn, soOutput.qsValue);
} // on_ccdContentDelayed_GotColumnValue

// show tree view context menu
void cPanel::on_ctwTree_customContextMenuRequested(const QPoint &pos)
{
	csmMenu->Show(GetSelectedItemsStringList(), static_cast<cTreeWidget *>(qswDir->currentWidget())->viewport()->mapToGlobal(pos));
} // on_ctwTree_customContextMenuRequested

// start dragging of selected objects
void cPanel::on_ctwTree_DragEvent(cTreeWidget *ctwSource)
{
	QStringList qfilFiles;

	qfilFiles = GetSelectedItemsStringList();

	if (qfilFiles.count() > 0) {
		int iI;
		QDrag *qdDrag;
		QList<QUrl> qlUrls;
		QMimeData *qmdMimeData;

		for (iI = 0; iI < qfilFiles.count(); iI++) {
			qlUrls.append(QUrl::fromLocalFile(qfilFiles.at(iI)));
		} // for

		qmdMimeData = new QMimeData();
		qmdMimeData->setUrls(qlUrls);
		qdDrag = new QDrag(ctwSource);
		qdDrag->setMimeData(qmdMimeData);
		qdDrag->start(Qt::CopyAction | Qt::MoveAction);
	} // if
} // on_ctwTree_DragEvent

// drop event occured
void cPanel::on_ctwTree_DropEvent(const cTreeWidget::eDropAction &edaAction, const QList<QUrl> &clUrls)
{
	cFileRoutine::eOperation eoOperation;
	int iI;
	QFileInfoList qfilFiles;

	if (edaAction == cTreeWidget::CopyDropAction) {
		eoOperation = cFileRoutine::CopyOperation;
	} else {
		QAction *qaCancel, *qaChoice, *qaCopy, *qaMove;
		QMenu qmMenu;

		qaCopy = qmMenu.addAction(tr("C&opy"));
		qaMove = qmMenu.addAction(tr("&Move"));
		qmMenu.addSeparator();
		qaCancel = qmMenu.addAction(tr("&Cancel"));

		qaChoice = qmMenu.exec(QCursor::pos());

		if (qaChoice == qaCopy) {
			eoOperation = cFileRoutine::CopyOperation;
		} else {
			if (qaChoice == qaMove) {
				eoOperation = cFileRoutine::MoveOperation;
			} else {
				return;
			} // if else
		} // if else
	} // if else

	// get list of source files
	for (iI = 0; iI < clUrls.count(); iI++) {
		qfilFiles.append(QFileInfo(clUrls.at(iI).toLocalFile()));
	} // for

	cfoFileOperation->Operate(eoOperation, qfilFiles, GetPath());
} // on_ctwTree_DropEvent

// dir view got focus
void cPanel::on_ctwTree_GotFocus()
{
	if (qswLastActive != qswDir) {
		qswLastActive = qswDir;
		qlGlobalPath->setText(qhTabs.value(qswDir->currentIndex()).swWidgets->qsPath);
		emit GotFocus();
	} // if
} // on_ctwTree_GotFocus

// double click in tree view
void cPanel::on_ctwTree_itemActivated(QTreeWidgetItem *item, int column)
{
	QFileInfo qfiFile;

	qfiFile = qhTabs.value(qswDir->currentIndex()).qhFiles->value(item);
	if (qfiFile.isDir()) {
		// double click on directory -> go into directory
		if (qfiFile.fileName() == "..") {
			GoToUpDir();
		} else {
			SetPath(qfiFile.filePath());
		} // if else
	} else {
		// double click on file -> execute it
		cProcess::Execute(QString("\"%1\"").arg(qfiFile.filePath()));
	} // if else
} // on_ctwTree_itemActivated

// changed selected items in directory view
void cPanel::on_ctwTree_itemSelectionChanged(const cTreeWidget *ctwTree)
{
	int iDirectories, iDirectoriesTotal, iFiles, iFilesTotal, iI;
	qint64 qi64Size, qi64TotalSize;

	// find tab for tree widget
	QHashIterator<uint, sTab> qhiTab(qhTabs);
	while (qhiTab.hasNext()) {
		qhiTab.next();
		if (static_cast<cTreeWidget *>(qswDir->widget(qhiTab.key())) == ctwTree) {
			break;
		} // if
	} // while

	iDirectories = 0;
	iDirectoriesTotal = 0;
	iFiles = 0;
	iFilesTotal = 0;
	qi64Size = 0;
	qi64TotalSize = 0;
	for (iI = 0; iI < ctwTree->topLevelItemCount(); iI++) {
		if (qhiTab.value().qhFiles->value(ctwTree->topLevelItem(iI)).isDir()) {
			iDirectoriesTotal++;
			if (ctwTree->topLevelItem(iI)->isSelected()) {
				iDirectories++;
			} // if
		} else {
			iFilesTotal++;
			qi64TotalSize += qhiTab.value().qhFiles->value(ctwTree->topLevelItem(iI)).size();
			if (ctwTree->topLevelItem(iI)->isSelected()) {
				iFiles++;
				qi64Size += qhiTab.value().qhFiles->value(ctwTree->topLevelItem(iI)).size();
			} // if
		} // if else
	} // for

	qhiTab.value().swWidgets->qsSelected = tr("%1 / %2 in %3 / %4 files and %5 / %6 directories").arg(GetSizeString(qi64Size)).arg(GetSizeString(qi64TotalSize)).arg(iFiles).arg(iFilesTotal).arg(iDirectories).arg(iDirectoriesTotal);
	if (static_cast<cTreeWidget *>(qswDir->currentWidget()) == ctwTree) {
		ActualizeWidgets();
	} // if
} // on_ctwTree_itemSelectionChanged

// space pressed in dir view
void cPanel::on_ctwTree_KeyPressed(QKeyEvent *qkeEvent, QTreeWidgetItem *qtwiItem)
{
	int iColumnExtension, iI;
	QFileInfo qfiFile;
	QFileInfoList qfilFiles;
	qint64 qi64Size;
	QString qsName;

	switch (qkeEvent->key()) {
		case Qt::Key_Backspace:	GoToUpDir();
										break;
		case Qt::Key_Space:		qfiFile = qhTabs.value(qswDir->currentIndex()).qhFiles->value(qtwiItem);

										// refresh content plugin values
										for (iI = 0; iI < qhTabs.value(qswDir->currentIndex()).qlColumns->count(); iI++) {
											if (qhTabs.value(qswDir->currentIndex()).qlColumns->at(iI).qsPlugin != qsNO) {
												qtwiItem->setText(iI, ccContent->GetPluginValue(qfiFile.filePath(), qhTabs.value(qswDir->currentIndex()).qlColumns->at(iI).qsPlugin, qhTabs.value(qswDir->currentIndex()).qlColumns->at(iI).qsIdentifier, qhTabs.value(qswDir->currentIndex()).qlColumns->at(iI).qsUnit));
											} // if
										} // for

										if (qfiFile.isFile()) {
											// selected item is file
											return;
										} // if

										iColumnExtension = GetNativeColumnIndex(qsEXTENSION, qswDir->currentIndex());
										if (iColumnExtension == -1) {
											// no place to show occupied space
											return;
										} // if

										qfilFiles = cFileRoutine::GetSources(QFileInfoList() << qfiFile);
										qi64Size = 0;
										for (iI = 0; iI < qfilFiles.count(); iI++) {
											qi64Size += qfilFiles.at(iI).size();
										} // for

										qtwiItem->setText(iColumnExtension, GetSizeString(qi64Size));
										break;
		case Qt::Key_Enter:
		case Qt::Key_Return:		if (static_cast<bool>(qkeEvent->modifiers() & Qt::ControlModifier) && static_cast<bool>(qkeEvent->modifiers() & Qt::ShiftModifier)) {
											// ctrl+shift+enter -> copy filePath to command line
											qsName = qhTabs.value(qswDir->currentIndex()).qhFiles->value(qtwiItem).filePath();
										} else {
											qsName = qhTabs.value(qswDir->currentIndex()).qhFiles->value(qtwiItem).fileName();
											// ctrl+enter -> copy fileName to command line
										} // if else
										if (qsName.contains(' ')) {
											qsName = '"' + qsName + '"';
										} // if
										qcbCommand->setEditText(qcbCommand->currentText() + qsName);
										break;
		default:						if (csSettings->GetQuickSearchEnabled() &&
											 static_cast<bool>(qkeEvent->modifiers() & Qt::ControlModifier) == csSettings->GetQuickSearchCtrl() &&
											 static_cast<bool>(qkeEvent->modifiers() & Qt::AltModifier) == csSettings->GetQuickSearchAlt() &&
											 static_cast<bool>(qkeEvent->modifiers() & Qt::ShiftModifier) == csSettings->GetQuickSearchShift()) {
											// quick search activated
											qleQuickSearch->setFocus(Qt::OtherFocusReason);
											QApplication::sendEvent(qleQuickSearch, qkeEvent);
										} else {
											// pass key to command line
											QApplication::sendEvent(qcbCommand, qkeEvent);
											qcbCommand->setFocus(Qt::OtherFocusReason);
										} // if else
	} // switch
} // on_ctwTree_KeyPressed

// drive selected
void cPanel::on_qcbDrive_activated(int index)
{
	static_cast<cTreeWidget *>(qswDir->currentWidget())->setFocus(Qt::OtherFocusReason);
} // on_qcbDrive_activated

// selected drive changes
void cPanel::on_qcbDrive_currentIndexChanged(int index)
{
	if (qswDir->currentIndex() == -1) {
		// tabs are not yet initialized
		return;
	} // if

	QMapIterator<QString, cFileRoutine::sDriveInfo> qmiDrives(*qmDrives);
	while (qmiDrives.hasNext()) {
		qmiDrives.next();
		if (qmiDrives.key() == qcbDrive->currentText()) {
			QDir qdDir;

			if (qdDir.exists(qhLastPaths.value(qcbDrive->currentText()))) {
				SetPath(qhLastPaths.value(qcbDrive->currentText()));
			} else {
				SetPath(qmiDrives.value().qsPath);
			} // if else
			break;
		} // if
	} // while
} // on_qcbDrive_currentIndexChanged

// detect directory modifications
void cPanel::on_qfswWatcher_directoryChanged(const QString &path)
{
	int iI;

	// refresh content in tabs with set directory to path
	for (iI = 0; iI < qhTabs.count(); iI++) {
		if (qhTabs.value(iI).swWidgets->qsPath == path) {
			RefreshContent(iI);
		} // if
	} // for
} // on_qfswWatcher_directoryChanged

// click on header in tree (dir) view
void cPanel::on_qhvTreeHeader_sectionClicked(int logicalIndex)
{
	Sort(qswDir->currentIndex());
} // on_qhvTreeHeader_sectionClicked

// tab bar's index changed
void cPanel::on_qtbTab_currentChanged(int index)
{
	qswDir->setCurrentIndex(index);
	if (!*qhTabs.value(index).bValid) {
		RefreshContent();
	} // if
	ActualizeWidgets();
} // on_qtbTab_currentChanged

// timer's timeout
void cPanel::on_qtTimer_timeout()
{
	ActualizeVolumeInfo();
} // on_qtTimer_timeout

#ifdef Q_WS_WIN
// check if path is valid
bool cPanel::PathExists(const QString &qsPath)
{
	bool bResult;
	HANDLE hHandle;
	WIN32_FIND_DATA fdFindData;

	hHandle = FindFirstFile(reinterpret_cast<LPCWSTR>(QString(qsPath + "/*").unicode()), &fdFindData);
	if (hHandle == INVALID_HANDLE_VALUE) {
		bResult = false;
	} else {
		bResult = true;
	} // if else
	FindClose(hHandle);

	return bResult;
} // PathExists
#endif

// search if quick searched file exists in current dir view
bool cPanel::QuickSearch(const QString &qsNextChar, const eQuickSearchDirection &eqsdDirection)
{
	bool bOnStart;
	int iPos;
	QString qsFilename;

	if (!qleQuickSearch->isVisible()) {
		// search for only one first character
		qleQuickSearch->clear();
	} // if
	qsFilename = qleQuickSearch->text() + qsNextChar;

	iPos = static_cast<QTreeWidget *>(qswDir->currentWidget())->indexOfTopLevelItem(static_cast<QTreeWidget *>(qswDir->currentWidget())->currentItem());
	if (qsNextChar.isEmpty() || !qleQuickSearch->isVisible()) {
		if (eqsdDirection == SearchDown) {
			iPos++;
		} else {
			iPos--;
		} // if else
	} // if

	bOnStart = false;
	while (true) {
		// correct position
		if (iPos > static_cast<QTreeWidget *>(qswDir->currentWidget())->topLevelItemCount()) {
			iPos = 0;
		} else {
			if (iPos < 0) {
				iPos = static_cast<QTreeWidget *>(qswDir->currentWidget())->topLevelItemCount() - 1;
			} // if
		} // if else

		if (qhTabs.value(qswDir->currentIndex()).qhFiles->value(static_cast<QTreeWidget *>(qswDir->currentWidget())->topLevelItem(iPos)).fileName().startsWith(qsFilename)) {
			// found
			static_cast<QTreeWidget *>(qswDir->currentWidget())->setCurrentItem(static_cast<QTreeWidget *>(qswDir->currentWidget())->topLevelItem(iPos));
			return true;
		} // if

		if (static_cast<QTreeWidget *>(qswDir->currentWidget())->topLevelItem(iPos) == static_cast<QTreeWidget *>(qswDir->currentWidget())->currentItem()) {
			// on item started from
			if (bOnStart) {
				return false;
			} else {
				bOnStart = true;
			} // if else
		} // if

		// move onto next item
		if (eqsdDirection == SearchDown) {
			iPos++;
		} else {
			iPos--;
		} // if else
	} // while
} // QuickSearch

// refresh all dir views
void cPanel::RefreshAllContents()
{
	int iI;

	for (iI = 0; iI < qhTabs.count(); iI++) {
		RefreshContent(iI);
	} // for
} // RefreshAllContents

// refresh all dir view headers
void cPanel::RefreshAllHeaders()
{
	int iI;

	for (iI = 0; iI < qhTabs.count(); iI++) {
		if (qswDir->currentIndex() == iI) {
			RefreshHeader(iI, true);
		} else {
			RefreshHeader(iI);
		} // if else
	} // for
} // RefreshAllHeaders

// refresh current dir view
void cPanel::RefreshContent()
{
	QString qsFrom;

	// remember file going from
	qsFrom = qhTabs.value(qswDir->currentIndex()).qhFiles->value(static_cast<QTreeWidget *>(qswDir->currentWidget())->currentItem()).fileName();

	RefreshContent(qswDir->currentIndex());

	// find file went from and set it as current
	QHashIterator<QTreeWidgetItem *, QFileInfo> qhiFile(*qhTabs.value(qswDir->currentIndex()).qhFiles);
	while (qhiFile.hasNext()) {
		qhiFile.next();
		if (qhiFile.value().fileName() == qsFrom) {
			static_cast<QTreeWidget *>(qswDir->currentWidget())->clearSelection();
			static_cast<QTreeWidget *>(qswDir->currentWidget())->setCurrentItem(qhiFile.key());
			break;
		} // if
	} // while
} // RefreshContent

// refresh dir content
void cPanel::RefreshContent(const int &iIndex, QFileInfoList &qfilFiles)
{
	int iI;
	QList<cContentDelayed::sParameters> qlParameters;

	// interrupt delayed content processing
	emit InterruptContentDelayed();

	// clear previous file contents
	qhTabs.value(iIndex).qhFiles->clear();

	if (qfilFiles.count() == 0) {
		// check path
		if (!PathExists(qhTabs.value(iIndex).swWidgets->qsPath)) {
			SetPath(qhTabs.value(iIndex).swWidgets->qsPath);
			return;
		} // if
		// get files
		qfilFiles = cFileRoutine::GetDirectoryContent(qhTabs.value(iIndex).swWidgets->qsPath, GetStandardFilters());
	} // if

	// go through files and add them into file list
	for (iI = 0; iI < qfilFiles.count(); iI++) {
		int iJ;
		QTreeWidgetItem *qtwiFile;

		// omit some entries
		if (qfilFiles.at(iI).fileName() == ".") {
			continue;
		} // if

		// add to internal file list
		qtwiFile = new QTreeWidgetItem();
		qhTabs.value(iIndex).qhFiles->insert(qtwiFile, qfilFiles.at(iI));

		// fill columns
		for (iJ = 0; iJ < qhTabs.value(iIndex).qlColumns->count(); iJ++) {
			if (qhTabs.value(iIndex).qlColumns->at(iJ).qsPlugin == qsNO) {
				// native
				if (qhTabs.value(iIndex).qlColumns->at(iJ).qsIdentifier == qsICON) {
					// icon
					qtwiFile->setIcon(iJ, qfipIcon.icon(qfilFiles.at(iI)));
				} else {
					if (qhTabs.value(iIndex).qlColumns->at(iJ).qsIdentifier == qsNAME) {
						// name
						QString qsName;

						if (qfilFiles.at(iI).isDir() && qfilFiles.at(iI).fileName() == "..") {
							// special handle for ".." directory to show both points
							qsName = qfilFiles.at(iI).fileName();
						} else {
							qsName = qfilFiles.at(iI).completeBaseName();
						} // if
						if (qfilFiles.at(iI).isDir() && csSettings->GetShowBracketsAroundDirectoryName()) {
							qsName = '[' + qsName + ']';
						} // if
						qtwiFile->setText(iJ, qsName);
					} else {
						if (qhTabs.value(iIndex).qlColumns->at(iJ).qsIdentifier == qsEXTENSION) {
							// extension
							qtwiFile->setText(iJ, qfilFiles.at(iI).suffix());
						} else {
							if (qhTabs.value(iIndex).qlColumns->at(iJ).qsIdentifier == qsSIZE) {
								// size
								if (qfilFiles.at(iI).isDir()) {
									qtwiFile->setText(iJ, tr("<DIR>"));
								} else {
									qtwiFile->setText(iJ, GetSizeString(qfilFiles.at(iI).size()));
								} // if else
							} else {
								// date
								qtwiFile->setText(iJ, qfilFiles.at(iI).lastModified().toString());
							} // if else
						} // if else
					} // if else
				} // if else
			} else {
				// plugin
				int iFlag;

				qtwiFile->setText(iJ, ccContent->GetPluginValue(qfilFiles.at(iI).filePath(), qhTabs.value(iIndex).qlColumns->at(iJ).qsPlugin, qhTabs.value(iIndex).qlColumns->at(iJ).qsIdentifier, qhTabs.value(iIndex).qlColumns->at(iJ).qsUnit, &iFlag));
				if (iFlag == ft_delayed) {
					cContentDelayed::sParameters spParameters;

					// thread input
					spParameters.siInput.qsFilename = qfilFiles.at(iI).filePath();
					spParameters.siInput.qsPlugin = qhTabs.value(iIndex).qlColumns->at(iJ).qsPlugin;
					spParameters.siInput.qsColumn = qhTabs.value(iIndex).qlColumns->at(iJ).qsIdentifier;
					spParameters.siInput.qsUnit = qhTabs.value(iIndex).qlColumns->at(iJ).qsUnit;

					// thread output
					spParameters.soOutput.qtwiItem = qtwiFile;
					spParameters.soOutput.iColumn = iJ;

					qlParameters.append(spParameters);
				} // if
			} // if else
		} // for
	} // for

	// sort and show files
	Sort(iIndex);
	*const_cast<sTab *>(&qhTabs.value(iIndex))->bValid = true;

	if (static_cast<cTreeWidget *>(qswDir->widget(iIndex))->topLevelItemCount() > 0) {
		// focus to the first item
		static_cast<cTreeWidget *>(qswDir->widget(iIndex))->setCurrentItem(static_cast<cTreeWidget *>(qswDir->widget(iIndex))->topLevelItem(0));
	} // if

	if (qlParameters.count() > 0) {
		// start thread to query content plugins values
		ccdContentDelayed->Start(qlParameters);
	} // if
} // RefreshContent

// refresh current dir view with custom files
void cPanel::RefreshContent(QFileInfoList &qfilFiles)
{
	RefreshContent(qswDir->currentIndex(), qfilFiles);
} // RefreshContent

// refresh column's header
void cPanel::RefreshHeader(const int &iIndex, const bool &bContent /* false */)
{
	bool bAutoStretch;
	int iI;
	QStringList qslColumns;
	QTreeWidgetItem *qtwiHeader;

	// invalidate content of tab
	*const_cast<sTab *>(&qhTabs.value(iIndex))->bValid = false;

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
	static_cast<cTreeWidget *>(qswDir->widget(iIndex))->setHeaderItem(qtwiHeader);
	bAutoStretch = false;
	// set columns width
	for (iI = 0; iI < qhTabs.value(iIndex).qlColumns->count(); iI++) {
		if (qhTabs.value(iIndex).qlColumns->at(iI).iWidth == 0) {
			static_cast<cTreeWidget *>(qswDir->widget(iIndex))->header()->setResizeMode(iI, QHeaderView::Stretch);
			bAutoStretch = true;
		} else {
			static_cast<cTreeWidget *>(qswDir->widget(iIndex))->header()->resizeSection(iI, qhTabs.value(iIndex).qlColumns->at(iI).iWidth);
		} // if else
	} // for
	if (bAutoStretch) {
		static_cast<cTreeWidget *>(qswDir->widget(iIndex))->header()->setStretchLastSection(false);
	} // if
	// show sort order
	static_cast<cTreeWidget *>(qswDir->widget(iIndex))->header()->setSortIndicatorShown(true);
	// clickable flag
	static_cast<cTreeWidget *>(qswDir->widget(iIndex))->header()->setClickable(true);

	if (bContent) {
		// refresh dir content according to new header
		RefreshContent(iIndex);
	} // if
} // RefreshHeader

// refresh tabs
void cPanel::RefreshTabs()
{
	int iI;

	for (iI = 0; iI < qhTabs.count(); iI++) {
		SetTabText(iI);
	} // for

	HideOrShowTabBar();
} // RefreshTabs

// reverse sort order
void cPanel::ReverseOrder()
{
	SortBy(static_cast<cTreeWidget *>(qswDir->currentWidget())->header()->sortIndicatorSection());
} // ReverseOrder

// save panel settings
void cPanel::SaveSettings(const cSettings::ePosition &epPosition)
{
	QList<cSettings::sTabInfo> qlTabs;

	QHashIterator<uint, sTab> qhiTab(qhTabs);
	while (qhiTab.hasNext()) {
		cSettings::sTabInfo stiTab;

		qhiTab.next();
		stiTab.qsColumnSet = qhiTab.value().qsColumnSet;
		stiTab.qsDrive = qhiTab.value().swWidgets->qsDrive;
		stiTab.qsPath = qhiTab.value().swWidgets->qsPath;
		stiTab.ssSort.iSortedColumn = static_cast<cTreeWidget *>(qswDir->widget(qhiTab.key()))->sortColumn();
		stiTab.ssSort.soSortOrder = static_cast<cTreeWidget *>(qswDir->widget(qhiTab.key()))->header()->sortIndicatorOrder();

		qlTabs.append(stiTab);
	} // while

	csSettings->SetTabs(epPosition, qlTabs);
} // SaveSettings

// select or unselect some files
void cPanel::Select(const cSelectFilesDialog::eSelectType &estType, cLister *clLister)
{
	cSelectFilesDialog csfdSelect(qmwParent, estType, csSettings, clLister);

	if (csfdSelect.exec() == QDialog::Accepted) {
		cFindFilesDialog cffdFindFiles(NULL, NULL, csSettings, clLister);
		cSettings::sFindSettings sfsFind;
		QFileInfoList qfilFiles;

		if (csfdSelect.qtwFindSets->selectedItems().count() > 0) {
			sfsFind = csSettings->GetFindSettings(csfdSelect.qtwFindSets->selectedItems().at(0)->text(0));
		} else {
			sfsFind.qsSearchFor = csfdSelect.qcbFilter->currentText();
			sfsFind.bSearchForRegularExpression = false;
			sfsFind.bSearchForText = false;
			sfsFind.bDateTimeBetween = false;
			sfsFind.bNotOlderThan = false;
			sfsFind.bFileSize = false;
		} // if else
		sfsFind.qsSearchIn = GetPath();
		sfsFind.iSubdirectoryDepth = 0;

		cffdFindFiles.Start(sfsFind, true);
		qfilFiles = cffdFindFiles.SearchResult();

		QHashIterator<QTreeWidgetItem *, QFileInfo> qhiFile(*qhTabs.value(qswDir->currentIndex()).qhFiles);
		while (qhiFile.hasNext()) {
			int iI;

			qhiFile.next();
			for (iI = 0; iI < qfilFiles.count(); iI++) {
				if (qhiFile.value().fileName() == qfilFiles.at(iI).fileName()) {
					if (estType == cSelectFilesDialog::Select) {
						qhiFile.key()->setSelected(true);
					} else {
						qhiFile.key()->setSelected(false);
					} // if else
				} // if
			} // for
		} // while
	} // if
} // Select

// select all files
void cPanel::SelectAll()
{
	QString qsMode;

	qsMode = csSettings->GetSelectionMode();
	if (qsMode == qsONLY_FILES) {
		QHashIterator<QTreeWidgetItem *, QFileInfo> qhiFiles(*qhTabs.value(qswDir->currentIndex()).qhFiles);
		while (qhiFiles.hasNext()) {
			qhiFiles.next();
			if (qhiFiles.value().isFile()) {
				qhiFiles.key()->setSelected(true);
			} // if
		} // while
	} else {
		static_cast<cTreeWidget *>(qswDir->currentWidget())->selectAll();
	} // if else
} // SelectAll

// set new path for current dir view
void cPanel::SetPath(const QString &qsPath)
{
	// remove old path from watcher
	qfswWatcher.removePath(qhTabs.value(qswDir->currentIndex()).swWidgets->qsPath);

#ifdef Q_WS_WIN
	// check new path
	if (!PathExists(qsPath)) {
		qlDriveInfo->hide();

		if (IsRootDirectory(qsPath)) {
			// invalid drive
			QMap<QString, cFileRoutine::sDriveInfo> qmDrives;

			qmDrives = cFileRoutine::GetDrives();
			cSelectDriveDialog csddDrive(qmwParent, qmDrives, qcbDrive->currentText());

			if (csddDrive.exec() == QDialog::Accepted) {
				// change to newly selected drive
				qcbDrive->blockSignals(true);
				qcbDrive->setCurrentIndex(-1);
				qcbDrive->blockSignals(false);
				qcbDrive->setCurrentIndex(qcbDrive->findText(csddDrive.qcbDrive->currentText()));
			} else {
				// try to change to previous path
				qcbDrive->blockSignals(true);
				qcbDrive->setCurrentIndex(qcbDrive->findText(qhTabs.value(qswDir->currentIndex()).swWidgets->qsDrive));
				qcbDrive->blockSignals(false);
				SetPath(qhTabs.value(qswDir->currentIndex()).swWidgets->qsPath);
			} // if else
		} else {
			// maybe valid drive but invalid path
			QDir qdDir;

			qdDir.setPath(qsPath);
			qdDir.cdUp();
			if (qdDir.path() == qhTabs.value(qswDir->currentIndex()).swWidgets->qsPath) {
				// unsuccessful try to change to subdirectory
				qlDriveInfo->show();
			} else {
				// bad directory, try to go one dir up
				GoToUpDir();
			} // if else
		} // if else
	} else {
		// path ok
		qlDriveInfo->show();
#endif

		qhTabs.value(qswDir->currentIndex()).swWidgets->qsDrive = qcbDrive->currentText();
		qhTabs.value(qswDir->currentIndex()).swWidgets->qsPath = QDir::cleanPath(qsPath);
		// add new path to watcher
		qfswWatcher.addPath(qhTabs.value(qswDir->currentIndex()).swWidgets->qsPath);
		qhLastPaths.insert(qcbDrive->currentText(), qhTabs.value(qswDir->currentIndex()).swWidgets->qsPath = QDir::cleanPath(qsPath));
		qlGlobalPath->setText(qhTabs.value(qswDir->currentIndex()).swWidgets->qsPath);
		RefreshContent(qswDir->currentIndex());
#ifdef Q_WS_WIN
	} // if else
#endif
} // SetPath

// set text in tab bar
void cPanel::SetTabText(const int &iTabIndex)
{
	QDir qdDir;
	QString qsTabText;

	if (csSettings->GetShowDriveLetter()) {
		qsTabText = qhTabs.value(iTabIndex).swWidgets->qsDrive + ": ";
	} // if

	qdDir.setPath(qhTabs.value(iTabIndex).swWidgets->qsPath);

	// tab bar
	if (qdDir.dirName().isEmpty()) {
		qsTabText += qdDir.path().at(0);
	} else {
		qsTabText += qdDir.dirName();
	} // if else

	qtbTab->setTabText(iTabIndex, qsTabText);
} // SetTabText

// sort dir content and show
void cPanel::Sort(const int &iIndex)
{
	int iColumnName, iI;
	QList<QTreeWidgetItem *> qlDirectories, qlFiles, qlMarked;

	// clear tree (can't use QTreeWidget::clear because it deletes objects too) and remember marked items
	while (static_cast<cTreeWidget *>(qswDir->widget(iIndex))->topLevelItemCount() > 0) {
		if (static_cast<cTreeWidget *>(qswDir->widget(iIndex))->topLevelItem(0)->isSelected()) {
			qlMarked.append(static_cast<cTreeWidget *>(qswDir->widget(iIndex))->takeTopLevelItem(0));
		} else {
			static_cast<cTreeWidget *>(qswDir->widget(iIndex))->takeTopLevelItem(0);
		} // if else
	} // while

	// split directories and files
	QHashIterator<QTreeWidgetItem *, QFileInfo> qhiFiles(*qhTabs.value(iIndex).qhFiles);
	while (qhiFiles.hasNext()) {
		qhiFiles.next();
		if (qhiFiles.value().isDir()) {
			qlDirectories.append(qhiFiles.key());
		} else {
			qlFiles.append(qhiFiles.key());
		} // if else
	} // while

	// sort at first by name if possible (to have sorted the rest by filename as second condition)
	iColumnName = GetNativeColumnIndex(qsNAME, iIndex);
	if (iColumnName != -1) {
		ssSort.iSortedColumn = iColumnName;
		ssSort.soSortOrder = Qt::AscendingOrder;
		qStableSort(qlDirectories.begin(), qlDirectories.end(), &cPanel::TreeSortByString);
		for (iI = 0; iI < qlDirectories.count(); iI++) {
			if (qhTabs.value(iIndex).qhFiles->value(qlDirectories.at(iI)).fileName() == "..") {
				qlDirectories.move(iI, 0);
				break;
			} // if
		} // for
		qStableSort(qlFiles.begin(), qlFiles.end(), &cPanel::TreeSortByString);
	} // if

	// set sort informations for sorting functions
	ssSort.iSortedColumn = static_cast<cTreeWidget *>(qswDir->widget(iIndex))->sortColumn();
	ssSort.soSortOrder = static_cast<cTreeWidget *>(qswDir->widget(iIndex))->header()->sortIndicatorOrder();
	if ((qhTabs.value(iIndex).qlColumns->at(ssSort.iSortedColumn).qsIdentifier == qsNAME && ssSort.soSortOrder == Qt::DescendingOrder) || qhTabs.value(iIndex).qlColumns->at(ssSort.iSortedColumn).qsIdentifier == qsEXTENSION || qhTabs.value(iIndex).qlColumns->at(ssSort.iSortedColumn).qsPlugin != qsNO) {
		// let ".." be the first if possible
		if (qhTabs.value(iIndex).qhFiles->value(qlDirectories.front()).fileName() == "..") {
			qStableSort(++qlDirectories.begin(), qlDirectories.end(), &cPanel::TreeSortByString);
		} else {
			qStableSort(qlDirectories.begin(), qlDirectories.end(), &cPanel::TreeSortByString);
		} // if else
		qStableSort(qlFiles.begin(), qlFiles.end(), &cPanel::TreeSortByString);
	} // if

	// show the result
	static_cast<cTreeWidget *>(qswDir->widget(iIndex))->addTopLevelItems(qlDirectories);
	static_cast<cTreeWidget *>(qswDir->widget(iIndex))->addTopLevelItems(qlFiles);

	// mark items again
	for (iI = 0; iI < qlMarked.count(); iI++) {
		qlMarked.at(iI)->setSelected(true);
	} // for
} // Sort

// sort by specified column
void cPanel::SortBy(const int &iColumn)
{
	Qt::SortOrder soSortOrder;

	// change sort order
	if (static_cast<cTreeWidget *>(qswDir->currentWidget())->header()->sortIndicatorSection() == iColumn) {
		soSortOrder = static_cast<cTreeWidget *>(qswDir->currentWidget())->header()->sortIndicatorOrder();
		if (soSortOrder == Qt::AscendingOrder) {
			soSortOrder = Qt::DescendingOrder;
		} else {
			soSortOrder = Qt::AscendingOrder;
		} // if else
	} else {
		soSortOrder = Qt::AscendingOrder;
	} // if else
	static_cast<cTreeWidget *>(qswDir->currentWidget())->header()->setSortIndicator(iColumn, soSortOrder);

	// sort again
	Sort(qswDir->currentIndex());
} // SortBy

// compare items by text
bool cPanel::TreeSortByString(const QTreeWidgetItem *qtwiItem1, const QTreeWidgetItem *qtwiItem2)
{
	if ((qtwiItem1->text(ssSort.iSortedColumn) < qtwiItem2->text(ssSort.iSortedColumn) && ssSort.soSortOrder == Qt::AscendingOrder) || (qtwiItem1->text(ssSort.iSortedColumn) <= qtwiItem2->text(ssSort.iSortedColumn) && ssSort.soSortOrder == Qt::DescendingOrder)) {
		if (ssSort.soSortOrder == Qt::AscendingOrder) {
			return true;
		} else {
			return false;
		} // if else
	} else {
		if (ssSort.soSortOrder == Qt::AscendingOrder) {
			return false;
		} else {
			return true;
		} // if else
	} // if else
} // TreeSortByString

// unselect all files
void cPanel::UnselectAll()
{
	static_cast<cTreeWidget *>(qswDir->currentWidget())->clearSelection();
} // UnselectAll