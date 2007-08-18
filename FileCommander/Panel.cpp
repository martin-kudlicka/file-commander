#include "Panel.h"

#include "Panel/TreeWidget.h"
#include <QtGui/QHeaderView>
#include <QtCore/QDateTime>
#include <QtGui/QMessageBox>
#include <QtGui/QLineEdit>
#include <QtCore/QEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QApplication>

QStackedWidget *cPanel::qswLastActive;	///< last active panel (static class variable)
cSettings::sSort cPanel::ssSort;			///< sort information (static class variable)

// destructor
cPanel::~cPanel()
{
	int iI;

	for (iI = 0; iI < qlTabs.count(); iI++) {
		cfcFileControl->CloseFileSystem(qlTabs.at(iI).cfsFileSystem);
	} // for
} // ~cPanel

// drive list actualization
const void cPanel::ActualizeDrives() const
{
	QList<QPair<QString, cFileControl::sDrive> > qlDrives;

	qlDrives = cfcFileControl->GetDrives();

	if (qcbDrive->count() != qlDrives.count()) {
		// assume drives have changed
		int iI;
		QString qsDrive;

		qsDrive = qcbDrive->currentText();
		qcbDrive->blockSignals(true);

		for (iI = 0; iI < qlDrives.count(); iI++) {
			qcbDrive->addItem(qlDrives.at(iI).first);
		} // for

		// check for selected drive change, changes are handled in Panel class
		if (qcbDrive->findText(qsDrive) == -1) {
			// selected drive changed
			qcbDrive->blockSignals(false);
			qcbDrive->setCurrentIndex(-1);
		} else {
			// selected drive not changed
			qcbDrive->setCurrentIndex(qcbDrive->findText(qsDrive));
			qcbDrive->blockSignals(false);
		} // if else
	} // if
} // ActualizeDrives

// actualize volume information - disk name and space
const void cPanel::ActualizeVolumeInfo()
{
	if (!qlTabs.isEmpty() && qlTabs.value(qswDirs->currentIndex()).bValid) {
		cFileSystem::sDiskSpace sdsInfo;
		QString qsName;
		sTab *stTab;

		stTab = &qlTabs[qswDirs->currentIndex()];

		qsName = stTab->cfsFileSystem->GetVolumeName();
		sdsInfo = stTab->cfsFileSystem->GetDiskSpace();
		qlDriveInfo->setText(tr("[%1] %2 of %3 free").arg(qsName).arg(GetSizeString(sdsInfo.qi64Free)).arg(GetSizeString(sdsInfo.qi64Total)));
	} // if
} // ActualizeVolumeInfo

// actualize widgets with info about current directory view
const void cPanel::ActualizeWidgets()
{
	sTab *stTab;

	stTab = &qlTabs[qswDirs->currentIndex()];

	SetTabText(qswDirs->currentIndex());

	ActualizeVolumeInfo();
	qlPath->setText(stTab->swWidgets.qsPath);
	qlSelected->setText(stTab->swWidgets.qsSelected);
} // ActualizeWidgets

// add current path to last paths history
const void cPanel::AddHistory(const int &iIndex)
{
	bool bAdd;
	sTab *stTab;

	stTab = &qlTabs[iIndex];

	if (stTab->shHistory.qlLastPaths.isEmpty()) {
		// list is clear -> new tab
		bAdd = true;
	} else {
		if (stTab->cfsFileSystem->GetPath() == stTab->shHistory.qlLastPaths[stTab->shHistory.iPosition].qsPath) {
			// same path as the last
			bAdd = false;
		} else {
			// different path than previous
			bAdd = true;
		} // if else
	} // if else

	if (bAdd) {
		// add to history
		int iI;
		cSettings::sLastPath slpLastPath;

		slpLastPath.qsShow = stTab->cfsFileSystem->GetPath();
		slpLastPath.qsPath = slpLastPath.qsShow;

		if (stTab->shHistory.qlLastPaths.isEmpty()) {
			// add as the first record
			stTab->shHistory.qlLastPaths.append(slpLastPath);
		} else {
			if (stTab->shHistory.iPosition == stTab->shHistory.qlLastPaths.count() - 1) {
				// add to the last (next) position
				stTab->shHistory.qlLastPaths.append(slpLastPath);
			} else {
				// move current record to the last position and add new one
				stTab->shHistory.qlLastPaths.append(stTab->shHistory.qlLastPaths.takeAt(stTab->shHistory.iPosition));
				stTab->shHistory.qlLastPaths.append(slpLastPath);
			} // if else
		} // if else
		
		// find previous record of currently added path
		for (iI = 0; iI < stTab->shHistory.qlLastPaths.count() - 2; iI++) {
			if (stTab->shHistory.qlLastPaths.at(iI).qsShow == slpLastPath.qsShow) {
				// remove previous identical record from history list
				stTab->shHistory.qlLastPaths.removeAt(iI);
				break;
			} // if 
		} // for

		// remove oldest records to have less than set in options
		while (stTab->shHistory.qlLastPaths.count() > csSettings->GetMaximumHistoryDirectoryListSize()) {
			stTab->shHistory.qlLastPaths.removeAt(0);
		} // while

		// set position to the last record
		stTab->shHistory.iPosition = stTab->shHistory.qlLastPaths.count() - 1;
	} // if
} // AddHistory

// add new tab with dir view
const int cPanel::AddTab(const cSettings::sTabInfo &stiTabInfo, const bool &bStartUp /* false */, cFileSystem *cfsFileSystem /* NULL */)
{
	// TODO AddTab
	cTreeWidget *ctwTree;
	int iIndex;
	sTab stTab;

	// create tab
	ctwTree = new cTreeWidget();
	ctwTree->setRootIsDecorated(false);
	ctwTree->setContextMenuPolicy(Qt::CustomContextMenu);
	ctwTree->setSelectionMode(QAbstractItemView::ExtendedSelection);
	ctwTree->header()->setSortIndicatorShown(true);
	ctwTree->header()->setClickable(true);
	if (bStartUp || !csSettings->GetOpenNewTabNextToCurrentTab()) {
		iIndex = qswDirs->addWidget(ctwTree);
	} else {
		iIndex = qswDirs->insertWidget(qswDirs->currentIndex() + 1, ctwTree);
	} // if else

	// connect signals to slots
	connect(ctwTree, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(on_ctwTree_customContextMenuRequested(const QPoint &)));
	connect(ctwTree, SIGNAL(itemActivated(QTreeWidgetItem *, int)), SLOT(on_ctwTree_itemActivated(QTreeWidgetItem *, int)));
	connect(ctwTree, SIGNAL(itemSelectionChanged()), SLOT(on_ctwTree_itemSelectionChanged()));
	connect(ctwTree, SIGNAL(KeyPressed(QKeyEvent *)), SLOT(on_ctwTree_KeyPressed(QKeyEvent *)));
	connect(ctwTree, SIGNAL(GotFocus()), SLOT(on_ctwTree_GotFocus()));
	/*connect(ctwTree, SIGNAL(DropEvent(const cTreeWidget::eDropAction &, const QList<QUrl> &, const QString &, const QString &, QTreeWidgetItem *)), SLOT(on_ctwTree_DropEvent(const cTreeWidget::eDropAction &, const QList<QUrl> &, const QString &, const QString &, QTreeWidgetItem *)));
	connect(ctwTree, SIGNAL(DragEvent()), SLOT(on_ctwTree_DragEvent()));
	connect(ctwTree, SIGNAL(MoveEvent(QTreeWidgetItem *)), SLOT(on_ctwTree_MoveEvent(QTreeWidgetItem *)));*/

	// drive letter in combo
	qcbDrive->blockSignals(true);
	qcbDrive->setCurrentIndex(qcbDrive->findText(stiTabInfo.qsDrive));
	qcbDrive->blockSignals(false);

	// set tab properties
	stTab.swWidgets.qsPath = stiTabInfo.qsPath;
	stTab.qsColumnSet = stiTabInfo.qsColumnSet;
	if (cfsFileSystem) {
		stTab.cfsFileSystem = cfsFileSystem;
	} else {
		stTab.cfsFileSystem = cfcFileControl->GetFileSystem(stiTabInfo.qsDrive, stiTabInfo.qsPath);
	} // if else

	// history
	stTab.shHistory = stiTabInfo.shHistory;

	qlTabs.insert(iIndex, stTab);

	// add new tab into GUI
	qtbTab->blockSignals(true);
	qtbTab->insertTab(iIndex, QString());
	qtbTab->blockSignals(false);
	SetTabText(iIndex);

	// set sorting
	ctwTree->header()->setSortIndicator(stiTabInfo.ssSort.iSortedColumn, stiTabInfo.ssSort.soSortOrder);

	// set header, refresh content only in first added tab
	RefreshHeader(iIndex);
	if (cfsFileSystem) {
		// feed panel with found files
		RefreshContent(iIndex, false);
	} // if

	// connect other signals to slots
	connect(ctwTree->header(), SIGNAL(sectionClicked(int)), SLOT(on_qhvTreeHeader_sectionClicked(int)));
	ConnectFileSystem(stTab.cfsFileSystem);

	// connection for file system created later -> need to call for the first time explicitly
	if (!cfsFileSystem) {
		// do not refresh when filled by found files
		on_cfsFileSystem_ContentChanged(stTab.cfsFileSystem);
	} // if

	HideOrShowTabBar();

	return iIndex;
} // AddTab

// branch view for current directory
const void cPanel::BranchView() const
{
	// TODO BranchView
} // BranchView

// close all other tabs than selected
const void cPanel::CloseAllOtherTabs(const int &iTabIndex)
{
	if (qlTabs.count() > 1) {
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

			for (iI = qlTabs.count() - 1; iI >= 0; iI--) {
				if (iI != iTabIndex) {
					CloseTab(iI);
				} // if
			} // for
		} // if
	} // if
} // CloseAllOtherTabs

// close tab
const void cPanel::CloseTab(const int &iTabIndex)
{
	if (qlTabs.count() > 1) {
		cfcFileControl->CloseFileSystem(qlTabs.at(iTabIndex).cfsFileSystem);
		qlTabs.removeAt(iTabIndex);
		qswDirs->removeWidget(qswDirs->widget(iTabIndex));
		qtbTab->removeTab(iTabIndex);

		HideOrShowTabBar();
		ActualizeWidgets();

		static_cast<cTreeWidget *>(qswDirs->currentWidget())->setFocus(Qt::OtherFocusReason);
	} // if
} // CloseTab

// close tab
const void cPanel::CloseTab(const QMouseEvent *qmeEvent)
{
	if (csSettings->GetCloseTabOnDoubleClick() && qlTabs.count() > 1) {
		int iTabIndex;

		iTabIndex = GetTabIndex(qmeEvent->pos());
		if (iTabIndex != -1) {
			CloseTab(iTabIndex);
		} // if
	} // if
} // CloseTab

// connect new file system's signals/slots
const void cPanel::ConnectFileSystem(const cFileSystem *cfsFileSystem) const
{
	connect(cfsFileSystem, SIGNAL(ContentChanged(const cFileSystem *)), SLOT(on_cfsFileSystem_ContentChanged(const cFileSystem *)));
	connect(cfsFileSystem, SIGNAL(GotColumnValue(const cContentPluginDelayed::sOutput &)), SLOT(on_cfsFileSystem_GotColumnValue(const cContentPluginDelayed::sOutput &)));
	connect(cfsFileSystem, SIGNAL(Unaccessible()), SLOT(on_cfsFileSystem_Unaccessible()));
} // ConnectFileSystem

// constructor
cPanel::cPanel(QMainWindow *qmwParent, QStackedWidget *qswDirs, QComboBox *qcbDrive, QLabel *qlDriveInfo, QTabBar *qtbTab, QLabel *qlPath, QLabel *qlSelected, cSettings *csSettings, cContentPlugin *ccpContentPlugin, QLabel *qlGlobalPath, QComboBox *qcbCommand, cFileControl *cfcFileControl, QLineEdit *qleQuickSearch)
{
	this->qmwParent = qmwParent;
	this->qswDirs = qswDirs;
	this->qcbDrive = qcbDrive;
	this->qlDriveInfo = qlDriveInfo;
	this->qtbTab = qtbTab;
	this->qlPath = qlPath;
	this->qlSelected = qlSelected;
	this->csSettings = csSettings;
	this->ccpContentPlugin = ccpContentPlugin;
	this->qlGlobalPath = qlGlobalPath;
	this->qcbCommand = qcbCommand;
	this->cfcFileControl = cfcFileControl;
	this->qleQuickSearch = qleQuickSearch;

	// signals for controls
	connect(qtbTab, SIGNAL(currentChanged(int)), SLOT(on_qtbTab_currentChanged(int)));

	// event filters
	qcbCommand->installEventFilter(this);
	qtbTab->installEventFilter(this);
	qleQuickSearch->installEventFilter(this);
#ifdef Q_WS_WIN
	qcbDrive->view()->installEventFilter(this);
#endif

	ActualizeDrives();

	// automatic actualizations
	startTimer(iTIMER_INTERVAL);
} // cPanel

// create new tab by duplicate one
const int cPanel::DuplicateTab(const int &iTabIndex)
{
	cTreeWidget *ctwDir;
	sTab *stTab;
	cSettings::sTabInfo stiTabInfo;

	ctwDir = static_cast<cTreeWidget *>(qswDirs->widget(iTabIndex));
	stTab = &qlTabs[iTabIndex];

	stiTabInfo.qsColumnSet = stTab->qsColumnSet;
	stiTabInfo.qsDrive = qcbDrive->currentText();
	stiTabInfo.qsPath = stTab->cfsFileSystem->GetPath();
	stiTabInfo.ssSort.iSortedColumn = ctwDir->sortColumn();
	stiTabInfo.ssSort.soSortOrder = ctwDir->header()->sortIndicatorOrder();

	return AddTab(stiTabInfo);
} // DuplicateTab

// event filter
bool cPanel::eventFilter(QObject *watched, QEvent *event)
{
	if (watched == qcbCommand) {
		// command combo box
		if (event->type() == QEvent::KeyPress) {
			if (static_cast<QKeyEvent *>(event)->key() == Qt::Key_Down || static_cast<QKeyEvent *>(event)->key() == Qt::Key_Up) {
				if (qswLastActive == qswDirs) {
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
					case QEvent::FocusOut:
						qleQuickSearch->hide();
						return true;
					case QEvent::KeyPress:
						switch (static_cast<QKeyEvent *>(event)->key()) {
							case Qt::Key_Backspace:
								return false;
							case Qt::Key_Down:
								return !QuickSearch(NULL, SearchDown);
							case Qt::Key_Enter:
							case Qt::Key_Return:
								if (qswLastActive == qswDirs) {
									qswLastActive->currentWidget()->setFocus(Qt::OtherFocusReason);
									QApplication::sendEvent(qswLastActive->currentWidget(), event);
									return true;
								} else {
									return false;
								} // if else
							case Qt::Key_Escape:
								if (qswLastActive == qswDirs) {
									qswLastActive->currentWidget()->setFocus(Qt::OtherFocusReason);
									return true;
								} else {
									return false;
								} // if else
							case Qt::Key_Up:
								return !QuickSearch(NULL, SearchUp);
							default:
								if (static_cast<QKeyEvent *>(event)->text().isEmpty()) {
									// white char obtained
									return true;
								} else {
									bool bSearch;

									bSearch = QuickSearch(static_cast<QKeyEvent *>(event)->text(), SearchDown);

									if (qleQuickSearch->isVisible()) {
										// search in window
										return !bSearch;
									} else {
										// search without window -> set focus back to dir view
										if (qswLastActive == qswDirs) {
											qswLastActive->currentWidget()->setFocus(Qt::OtherFocusReason);
											return true;
										} else {
											return false;
										} // if else
									} // if else
								} // if else
						} // switch
					case QEvent::Show:
						qleQuickSearch->clear();
						if (csSettings->GetQuickSearchShowSearchWindow()) {
							qleQuickSearch->setMaximumWidth(INT_MAX);
						} else {
							qleQuickSearch->setMaximumWidth(0);
						} // if else
						qleQuickSearch->setFocus(Qt::OtherFocusReason);
						return false;
					default:
						return false;
				} // switch
			} else {
#ifdef Q_WS_WIN
				if (watched == qcbDrive->view()) {
					// drives list
					if (event->type() == QEvent::KeyPress) {
						// if exists same drive as key pressed activate it
						int iIndex;

						iIndex = qcbDrive->findText(static_cast<QKeyEvent *>(event)->text().toUpper());
						if (iIndex != -1) {
							qcbDrive->setCurrentIndex(iIndex);
							qcbDrive->hidePopup();
							static_cast<cTreeWidget *>(qswDirs->currentWidget())->setFocus(Qt::OtherFocusReason);
							return true;
						} // if
					} // if
					return false;
				} else {
#endif
					// the rest
					return QObject::eventFilter(watched, event);
				} // if else
			} // if else
		} // if else
	} // if else
} // eventFilter

// columns for current dir view
const QList<cSettings::sColumn> cPanel::GetColumns() const
{
	return qlTabs.value(qswDirs->currentIndex()).qlColumns;
} // GetColumns

//< column set for current directory view
const QString cPanel::GetColumnSet() const
{
	return qlTabs.value(qswDirs->currentIndex()).qsColumnSet;
} // GetColumnSet

// convert QDateTime to user defined format
const QString cPanel::GetDateTimeString(const QDateTime &qdtDateTime) const
{
	QString qsDateTime;
	
	qsDateTime = csSettings->GetDateTimeDisplay();
	
	qsDateTime.replace("%d", QString("%1").arg(qdtDateTime.date().day(), 2, 10, QChar('0')));
	qsDateTime.replace("%o", QString("%1").arg(qdtDateTime.date().month(), 2, 10, QChar('0')));
	qsDateTime.replace("%y", QString("%1").arg(qdtDateTime.date().year()));
	qsDateTime.replace("%h", QString("%1").arg(qdtDateTime.time().hour(), 2, 10, QChar('0')));
	qsDateTime.replace("%m", QString("%1").arg(qdtDateTime.time().minute(), 2, 10, QChar('0')));
	qsDateTime.replace("%s", QString("%1").arg(qdtDateTime.time().second(), 2, 10, QChar('0')));
	
	return qsDateTime;
} // GetDateTimeString

// file system for active directory view
cFileSystem *cPanel::GetFileSystem()
{
	return qlTabs.value(qswDirs->currentIndex()).cfsFileSystem;
} // GetFileSystem

// retreive history directory list
const cPanel::sHistoryDirectoryList cPanel::GetHistoryDirectoryList()
{
	int iI;
	cSettings::sHistory *shHistory;
	sHistoryDirectoryList shdlList;

	shHistory = &qlTabs[qswDirs->currentIndex()].shHistory;

	shdlList.iPosition = shHistory->iPosition;
	for (iI = 0; iI < shHistory->qlLastPaths.count(); iI++) {
		shdlList.qslDirectories.append(shHistory->qlLastPaths.at(iI).qsShow);
	} // for

	return shdlList;
} // GetHistoryDirectoryList

// find index of native column
const int cPanel::GetNativeColumnIndex(const QString &qsColumn, const int &iTabIndex)
{
	int iI;
	sTab *stTab;

	stTab = &qlTabs[iTabIndex];

	for (iI = 0; iI < stTab->qlColumns.count(); iI++) {
		if (stTab->qlColumns.at(iI).qsIdentifier == qsColumn) {
			return iI;
		}
	} // for

	return -1;
} // GetNativeColumnIndex

// path in current directory view
const QString cPanel::GetPath() const
{
	return qlTabs.at(qswDirs->currentIndex()).cfsFileSystem->GetPath();
} // GetPath

// "convert" size to string according to setting in options
const QString cPanel::GetSizeString(const qint64 &qi64Size) const
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

// find out tab index in tab bar
const int cPanel::GetTabIndex(const QPoint &qpPos) const
{
	int iI;

	for (iI = 0; iI < qtbTab->count(); iI++) {
		if (qtbTab->tabRect(iI).contains(qpPos)) {
			return iI;
		} // if
	} // for

	return -1;
} // GetTabIndex

// tab settings for specified tab
const cSettings::sTabInfo cPanel::GetTabSettings(int iTabIndex /* -1 */)
{
	cTreeWidget *ctwDir;
	sTab *stTab;
	cSettings::sTabInfo stiTab;

	if (iTabIndex == -1) {
		iTabIndex = qswDirs->currentIndex();
	} // if

	ctwDir = static_cast<cTreeWidget *>(qswDirs->widget(iTabIndex));
	stTab = &qlTabs[iTabIndex];

	// visible parameters
	stiTab.qsColumnSet = stTab->qsColumnSet;
	stiTab.qsDrive = stTab->cfsFileSystem->GetDrive();
	stiTab.qsPath = stTab->cfsFileSystem->GetPath();
	stiTab.ssSort.iSortedColumn = ctwDir->sortColumn();
	stiTab.ssSort.soSortOrder = ctwDir->header()->sortIndicatorOrder();

	// history
	stiTab.shHistory = stTab->shHistory;

	return stiTab;
} // GetTabSettings

// set cursor onto specified file
const bool cPanel::GoToFile(const QString qsGoTo) const
{
	cTreeWidget *ctwDir;
	int iI;

	ctwDir = static_cast<cTreeWidget *>(qswDirs->currentWidget());

	for (iI = 0; iI < ctwDir->topLevelItemCount(); iI++) {
		if (qlTabs.at(qswDirs->currentIndex()).cfsFileSystem->GetFileNameWithExtension(ctwDir->topLevelItem(iI), false) == qsGoTo) {
			ctwDir->setCurrentItem(ctwDir->topLevelItem(iI));
			return true;
		} // if
	} // for

	return false;
} // GoToFile

// hide or show tab bar as set in options
const void cPanel::HideOrShowTabBar() const
{
	if (csSettings->GetShowTabBarWithOnlyOneTab()) {
		qtbTab->show();
	} else {
		if (qlTabs.count() == 1) {
			qtbTab->hide();
		} else {
			qtbTab->show();
		} // if else
	} // if else
} // HideOrShowTabBar

// go back in history directory list
const void cPanel::HistoryGoBack()
{
	cSettings::sHistory *shHistory;

	shHistory = &qlTabs[qswDirs->currentIndex()].shHistory;

	if (shHistory->iPosition > 0) {
		SetHistoryDirectory(shHistory->iPosition - 1);
	} // if
} // HistoryGoBack

// go front in history directory list
const void cPanel::HistoryGoFront()
{
	cSettings::sHistory *shHistory;

	shHistory = &qlTabs[qswDirs->currentIndex()].shHistory;

	if (shHistory->iPosition < shHistory->qlLastPaths.count() - 1) {
		SetHistoryDirectory(shHistory->iPosition + 1);
	} // if
} // HistoryGoFront

// invert selection of files
const void cPanel::InvertSelection() const
{
	cTreeWidget *ctwDir;
	int iI;
	QString qsMode;

	ctwDir = static_cast<cTreeWidget *>(qswDirs->currentWidget());
	qsMode = csSettings->GetSelectionMode();

	for (iI = 0; iI < ctwDir->topLevelItemCount(); iI++) {
		if (qlTabs.value(qswDirs->currentIndex()).cfsFileSystem->IsDir(ctwDir->topLevelItem(iI))) {
			if (qsMode == qsFILES_AND_DIRECTORIES) {
				ctwDir->topLevelItem(iI)->setSelected(!ctwDir->topLevelItem(iI)->isSelected());
			} // if
		} else {
			ctwDir->topLevelItem(iI)->setSelected(!ctwDir->topLevelItem(iI)->isSelected());
		} // if else
	} // for
} // InvertSelection

// directory content changed for filesystem
const void cPanel::on_cfsFileSystem_ContentChanged(const cFileSystem *cfsFileSystem)
{
	int iI;
	sTab *stTab;

	for (iI = 0; iI < qlTabs.count(); iI++) {
		stTab = &qlTabs[iI];

		if (stTab->cfsFileSystem == cfsFileSystem) {
			break;
		} // if
	} // for

	// refresh content
	if (iI == qswDirs->currentIndex()) {
		RefreshContent(iI);
	} else {
		stTab->bValid = false;
	} // if

	// actualize widgets
	stTab->swWidgets.qsPath = cfsFileSystem->GetPath();
	if (stTab->bValid) {
		ActualizeWidgets();
	} // if
} // on_cfsFileSystem_ContentChanged

// got column value from content plugin
const void cPanel::on_cfsFileSystem_GotColumnValue(const cContentPluginDelayed::sOutput &soOutput) const
{
	soOutput.qtwiItem->setText(soOutput.iColumn, soOutput.qsValue);
} // on_cfsFileSystem_GotColumnValue

// file system unacessible
const void cPanel::on_cfsFileSystem_Unaccessible() const
{
	// TODO on_cfsFileSystem_Unaccessible - change drive dialog
} // on_cfsFileSystem_Unaccessible

// double click in tree view
const void cPanel::on_ctwTree_itemActivated(QTreeWidgetItem *item, int column) const
{
	qlTabs.at(qswDirs->currentIndex()).cfsFileSystem->ActivateCurrent(item);
} // on_ctwTree_itemActivated

// show tree view context menu
const void cPanel::on_ctwTree_customContextMenuRequested(const QPoint &pos) const
{
	qlTabs.at(qswDirs->currentIndex()).cfsFileSystem->ShowContextMenu(pos
#ifdef Q_WS_WIN
		, qswDirs->winId()
#endif
	);
} // on_ctwTree_customContextMenuRequested

// dir view got focus
const void cPanel::on_ctwTree_GotFocus()
{
	if (qswLastActive != qswDirs) {
		qswLastActive = qswDirs;
		qlGlobalPath->setText(qlTabs.at(qswDirs->currentIndex()).swWidgets.qsPath);
		emit GotFocus();
	} // if
} // on_ctwTree_GotFocus

// changed selected items in current directory view
const void cPanel::on_ctwTree_itemSelectionChanged()
{
	cTreeWidget *ctwDir;
	int iDirectories, iDirectoriesTotal, iFiles, iFilesTotal, iI;
	qint64 qi64Size, qi64TotalSize;
	sTab *stTab;

	ctwDir = static_cast<cTreeWidget *>(qswDirs->currentWidget());
	iDirectories = 0;
	iDirectoriesTotal = 0;
	iFiles = 0;
	iFilesTotal = 0;
	qi64Size = 0;
	qi64TotalSize = 0;
	stTab = &qlTabs[qswDirs->currentIndex()];

	for (iI = 0; iI < ctwDir->topLevelItemCount(); iI++) {
		bool bDirectory;
		int iColumnSize;
		qint64 qi64DetectedSize;
		QTreeWidgetItem *qtwiFile;

		qtwiFile = ctwDir->topLevelItem(iI);
		bDirectory = stTab->cfsFileSystem->IsDir(qtwiFile);

		iColumnSize = GetNativeColumnIndex(qsSIZE, qswDirs->currentIndex());
		qi64DetectedSize = 0;
		if (bDirectory) {
			iDirectoriesTotal++;

			// size for directory can be known too
			if (iColumnSize != -1) {
				qi64DetectedSize = qtwiFile->data(iColumnSize, Qt::UserRole).toLongLong();
				qi64TotalSize += qi64DetectedSize;
			} // if
			if (qtwiFile->isSelected()) {
				iDirectories++;
				qi64Size += qi64DetectedSize;
			} // if
		} else {
			iFilesTotal++;
			qi64DetectedSize = qtwiFile->data(iColumnSize, Qt::UserRole).toLongLong();
			qi64TotalSize += qi64DetectedSize;
			if (qtwiFile->isSelected()) {
				iFiles++;
				qi64Size += qi64DetectedSize;
			} // if
		} // if else
	} // for

	stTab->swWidgets.qsSelected = tr("%1 / %2 in %3 / %4 files and %5 / %6 directories").arg(GetSizeString(qi64Size)).arg(GetSizeString(qi64TotalSize)).arg(iFiles).arg(iFilesTotal).arg(iDirectories).arg(iDirectoriesTotal);
	ActualizeWidgets();
} // on_ctwTree_itemSelectionChanged

// space pressed in dir view
const void cPanel::on_ctwTree_KeyPressed(QKeyEvent *qkeEvent)
{
	cTreeWidget *ctwDir;
	int iI;
	QString qsName;
	sTab *stTab;

	ctwDir = static_cast<cTreeWidget *>(qswDirs->currentWidget());
	stTab = &qlTabs[qswDirs->currentIndex()];

	switch (qkeEvent->key()) {
		case Qt::Key_Backspace:
			stTab->cfsFileSystem->GoToUpDir();
			break;
		case Qt::Key_Space:
			// refresh content plugin values
			for (iI = 0; iI < stTab->qlColumns.count(); iI++) {
				if (stTab->qlColumns.at(iI).qsPlugin != qsNO) {
					cSettings::sColumn *scColumn;
					cFileSystem::sContentPluginRequest scprContent;

					scColumn = &stTab->qlColumns[iI];

					scprContent.qtwiFile = ctwDir->currentItem();
					scprContent.qsPlugin = scColumn->qsPlugin;
					scprContent.qsColumn = scColumn->qsIdentifier;
					scprContent.qsUnit = scColumn->qsUnit;
					scprContent.iColumn = iI;

					stTab->cfsFileSystem->GetContentPluginValue(scprContent);
				} // if
			} // for
			stTab->cfsFileSystem->RetreiveContentDelayedValues();

			if (stTab->cfsFileSystem->IsDir(ctwDir->currentItem())) {
				// proceed only with directory as current item
				int iColumnExtension;

				// check if there is native size column
				iColumnExtension = GetNativeColumnIndex(qsSIZE, qswDirs->currentIndex());
				if (iColumnExtension != -1) {
					qint64 qi64Size;

					// check if size already calculated
					qi64Size = ctwDir->currentItem()->data(iColumnExtension, Qt::UserRole).toLongLong();
					if (qi64Size == 0) {
						// not calculated yet
						qi64Size = stTab->cfsFileSystem->GetDirectorySize();

						// put size to data to count with it when selecting files
						ctwDir->currentItem()->setData(iColumnExtension, Qt::UserRole, qi64Size);
						// show the size in size column
						ctwDir->currentItem()->setText(iColumnExtension, GetSizeString(qi64Size));

						// refresh selected items size
						on_ctwTree_itemSelectionChanged();
					} // if
				} // if
			} // if
			break;
		case Qt::Key_Enter:
		case Qt::Key_Return:
			if (static_cast<bool>(qkeEvent->modifiers() & Qt::ControlModifier) && static_cast<bool>(qkeEvent->modifiers() & Qt::ShiftModifier)) {
				// ctrl+shift+enter -> copy filePath to command line
				qsName = stTab->cfsFileSystem->GetFilePath(ctwDir->currentItem());
			} else {
				// ctrl+enter -> copy fileName to command line
				qsName = stTab->cfsFileSystem->GetFileName(ctwDir->currentItem(), false);
			} // if else
			if (qsName.contains(' ')) {
				qsName = '"' + qsName + '"';
			} // if
			qcbCommand->setEditText(qcbCommand->currentText() + qsName);
			break;
		case Qt::Key_Delete:
			emit Delete();
			break;
		default:
			if (csSettings->GetQuickSearchEnabled() &&
				 static_cast<bool>(qkeEvent->modifiers() & Qt::ControlModifier) == csSettings->GetQuickSearchCtrl() &&
				 static_cast<bool>(qkeEvent->modifiers() & Qt::AltModifier) == csSettings->GetQuickSearchAlt() &&
				 static_cast<bool>(qkeEvent->modifiers() & Qt::ShiftModifier) == csSettings->GetQuickSearchShift()) {
				// quick search activated
				qleQuickSearch->show();
				QApplication::sendEvent(qleQuickSearch, qkeEvent);
			} else {
				// pass key to command line
				QApplication::sendEvent(qcbCommand, qkeEvent);
				qcbCommand->setFocus(Qt::OtherFocusReason);
			} // if else
	} // switch
} // on_ctwTree_KeyPressed

// click on header in tree (dir) view
const void cPanel::on_qhvTreeHeader_sectionClicked(int logicalIndex)
{
	Sort(qswDirs->currentIndex(), qlTabs.value(qswDirs->currentIndex()).cfsFileSystem->GetDirectoryContent(false));
} // on_qhvTreeHeader_sectionClicked

// tab bar's index changed
const void cPanel::on_qtbTab_currentChanged(int index)
{
	qswDirs->setCurrentIndex(index);

	if (!qlTabs.value(index).bValid) {
		RefreshContent();
	} // if

	ActualizeWidgets();
} // on_qtbTab_currentChanged

// search if quick searched file exists in current dir view
const bool cPanel::QuickSearch(const QString &qsNextChar, const eQuickSearchDirection &eqsdDirection)
{
	bool bOnStart;
	cTreeWidget *ctwDir;
	int iPos;
	QString qsFilename;

	ctwDir = static_cast<cTreeWidget *>(qswDirs->currentWidget());

	if (qleQuickSearch->maximumWidth() == 0) {
		// search for only one first character
		qleQuickSearch->clear();
	} // if
	qsFilename = qleQuickSearch->text() + qsNextChar;

	iPos = ctwDir->indexOfTopLevelItem(ctwDir->currentItem());
	if (qsNextChar.isEmpty() || qleQuickSearch->maximumWidth() == 0) {
		if (eqsdDirection == SearchDown) {
			iPos++;
		} else {
			iPos--;
		} // if else
	} // if

	bOnStart = false;
	while (true) {
		sTab *stTab;

		stTab = &qlTabs[qswDirs->currentIndex()];

		// correct position
		if (iPos > ctwDir->topLevelItemCount()) {
			iPos = 0;
		} else {
			if (iPos < 0) {
				iPos = ctwDir->topLevelItemCount() - 1;
			} // if
		} // if else

		if (stTab->cfsFileSystem->GetFileName(ctwDir->topLevelItem(iPos), false).startsWith(qsFilename)) {
			// found
			ctwDir->setCurrentItem(ctwDir->topLevelItem(iPos));
			return true;
		} // if

		if (ctwDir->topLevelItem(iPos) == ctwDir->currentItem()) {
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

// refresh all dir view contents
const void cPanel::RefreshAllContents()
{
	int iI;

	for (iI = 0; iI < qlTabs.count(); iI++) {
		if (qswDirs->currentIndex() == iI) {
			RefreshContent(iI);
		} else {
			qlTabs[iI].bValid = false;
		} // if else
	} // for
} // RefreshAllContents

// refresh all dir view headers
const void cPanel::RefreshAllHeaders()
{
	int iI;

	for (iI = 0; iI < qlTabs.count(); iI++) {
		if (qswDirs->currentIndex() == iI) {
			RefreshHeader(iI, true);
		} else {
			RefreshHeader(iI);
		} // if else
	} // for
} // RefreshAllHeaders

// refresh current dir content
const void cPanel::RefreshContent()
{
	RefreshContent(qswDirs->currentIndex());
} // RefreshContent

// refresh dir content
const void cPanel::RefreshContent(const int &iIndex, const bool &bRefresh /* true */)
{
	cTreeWidget *ctwDir;
	int iI;
	QList<QTreeWidgetItem *> qlFiles;
	sTab *stTab;

	ctwDir = static_cast<cTreeWidget *>(qswDirs->widget(iIndex));
	stTab = &qlTabs[iIndex];

	qlFiles = stTab->cfsFileSystem->GetDirectoryContent(bRefresh);

	// fill tree widget items
	for (iI = 0; iI < qlFiles.count(); iI++) {
		int iJ;
		QTreeWidgetItem *qtwiFile;

		qtwiFile = qlFiles.at(iI);
		for (iJ = 0; iJ < stTab->qlColumns.count(); iJ++) {
			cSettings::sColumn *scColumn;

			scColumn = &stTab->qlColumns[iJ];
			if (scColumn->qsPlugin == qsNO) {
				if (scColumn->qsIdentifier == qsICON) {
					qtwiFile->setIcon(iJ, stTab->cfsFileSystem->GetFileIcon(qtwiFile));
				} else {
					if (scColumn->qsIdentifier == qsNAME) {
						qtwiFile->setText(iJ, stTab->cfsFileSystem->GetFileName(qtwiFile));
					} else {
						if (scColumn->qsIdentifier == qsEXTENSION) {
							qtwiFile->setText(iJ, stTab->cfsFileSystem->GetFileExtension(qtwiFile));
						} else {
							if (scColumn->qsIdentifier == qsNAME_WITH_EXTENSION) {
								qtwiFile->setText(iJ, stTab->cfsFileSystem->GetFileNameWithExtension(qtwiFile));
							} else {
								if (scColumn->qsIdentifier == qsSIZE) {
									if (stTab->cfsFileSystem->IsDir(qtwiFile)) {
										qtwiFile->setText(iJ, tr("<DIR>"));
									} else {
										qint64 qi64Size;

										qi64Size = stTab->cfsFileSystem->GetFileSize(qtwiFile); 
										qtwiFile->setText(iJ, GetSizeString(qi64Size));
										qtwiFile->setData(iJ, Qt::UserRole, qi64Size);
									} // if else
								} else {
									if (scColumn->qsIdentifier == qsDATE_TIME) {
										QDateTime qdtLastModified;

										qdtLastModified = stTab->cfsFileSystem->GetLastModified(qtwiFile);
										qtwiFile->setText(iJ, GetDateTimeString(qdtLastModified));
										qtwiFile->setData(iJ, Qt::UserRole, qdtLastModified);
									}
#ifdef Q_WS_WIN
									else {
										qtwiFile->setText(iJ, stTab->cfsFileSystem->GetFileAttr(qtwiFile));
									} // if else
#endif
								} // if else
							} // if else
						} // if else
					} // if else
				} // if else
			} else {
				// plugin
				cFileSystem::sContentPluginRequest scprContent;

				scprContent.qtwiFile = qtwiFile;
				scprContent.qsPlugin = scColumn->qsPlugin;
				scprContent.qsColumn = scColumn->qsIdentifier;
				scprContent.qsUnit = scColumn->qsUnit;
				scprContent.iColumn = iJ;

				qtwiFile->setText(iJ, stTab->cfsFileSystem->GetContentPluginValue(scprContent));
			} // if else
		} // for
	} // for
	
	// sort and show files
	Sort(iIndex, qlFiles);
	stTab->bValid = true;

	// set directory went from
	if (!GoToFile(stTab->qsDirectoryIn)) {
		if (ctwDir->topLevelItemCount() > 0) {
			// focus to the first item
			ctwDir->setCurrentItem(ctwDir->topLevelItem(0));
		} // if
	} // if else
	stTab->qsDirectoryIn = stTab->cfsFileSystem->GetDirName();

	stTab->cfsFileSystem->RetreiveContentDelayedValues();

	AddHistory(iIndex);
} // RefreshContent

// refresh column's header
const void cPanel::RefreshHeader(const int &iIndex, const bool &bContent /* false */)
{
	bool bAutoStretch;
	cTreeWidget *ctwDir;
	int iI;
	QStringList qslColumns;
	QTreeWidgetItem *qtwiHeader;
	sTab *stTab;

	ctwDir = static_cast<cTreeWidget *>(qswDirs->widget(iIndex));
	stTab = &qlTabs[iIndex];

	// invalidate content of tab
	stTab->bValid = false;

	// show/hide header
	ShowHideHeader(iIndex);

	// clear previous header contents and fill new information
	stTab->qlColumns.clear();
	qslColumns = csSettings->GetColumnsInSet(stTab->qsColumnSet);
	// check for column count in current column set
	if (qslColumns.isEmpty() && stTab->qsColumnSet != qsFULL) {
		// no columns - switch to full column set
		stTab->qsColumnSet = qsFULL;
		qslColumns = csSettings->GetColumnsInSet(stTab->qsColumnSet);
	} // if
	if (qslColumns.isEmpty() && stTab->qsColumnSet == qsFULL) {
		// create default column set (Full) if not created yet or empty
		csSettings->CreateDefaultColumnSet();
		qslColumns = csSettings->GetColumnsInSet(stTab->qsColumnSet);
	} // if
	for (iI = 0; iI < qslColumns.count(); iI++) {
		cSettings::sColumn scColumn;

		scColumn = csSettings->GetColumnInfo(stTab->qsColumnSet, qslColumns.at(iI));
		// test if is native or plugin and loaded
		if (scColumn.qsPlugin == qsNO || ccpContentPlugin->Loaded(scColumn.qsPlugin)) {
			// ok -> add to column list
			stTab->qlColumns.append(scColumn);
		} // if
	} // for

	// set header according to qlColumns list
	qtwiHeader = new QTreeWidgetItem();
	for (iI = 0; iI < stTab->qlColumns.count(); iI++) {
		qtwiHeader->setText(iI, stTab->qlColumns.at(iI).qsName);
	} // for
	ctwDir->setColumnCount(stTab->qlColumns.count());
	ctwDir->setHeaderItem(qtwiHeader);

	// customize header
	// autostretch last section
	bAutoStretch = true;
	for (iI = 0; iI < stTab->qlColumns.count() - 1; iI++) {
		if (stTab->qlColumns.at(iI).iWidth == 0) {
			bAutoStretch = false;
			break;
		} // if
	} // for
	ctwDir->header()->setStretchLastSection(bAutoStretch);
	// set columns width
	for (iI = 0; iI < stTab->qlColumns.count(); iI++) {
		if (stTab->qlColumns.at(iI).iWidth == 0) {
			ctwDir->header()->setResizeMode(iI, QHeaderView::Stretch);
		} else {
			ctwDir->header()->resizeSection(iI, stTab->qlColumns.at(iI).iWidth);
		} // if else
	} // for

	if (bContent) {
		// refresh dir content according to new header
		RefreshContent(iIndex);
	} // if
} // RefreshHeader

// refresh tabs
const void cPanel::RefreshTabs() const
{
	int iI;

	for (iI = 0; iI < qlTabs.count(); iI++) {
		SetTabText(iI);
	} // for

	HideOrShowTabBar();
} // RefreshTabs

// reverse sort order
const void cPanel::ReverseOrder()
{
	SortBy(static_cast<cTreeWidget *>(qswDirs->currentWidget())->header()->sortIndicatorSection());
} // ReverseOrder

// save panel settings
const void cPanel::SaveSettings(const cSettings::ePosition &epPosition)
{
	int iI;
	QList<cSettings::sTabInfo> qlTabsToSave;

	// tabs
	for (iI = 0; iI < qlTabs.count(); iI++) {
		qlTabsToSave.append(GetTabSettings(iI));
	} // for

	csSettings->SetTabs(epPosition, qlTabsToSave);
} // SaveSettings

// select all files
const void cPanel::SelectAll() const
{
	cTreeWidget *ctwDir;
	int iI;
	QString qsMode;

	ctwDir = static_cast<cTreeWidget *>(qswDirs->currentWidget());
	qsMode = csSettings->GetSelectionMode();

	if (qsMode == qsONLY_FILES) {
		for (iI = 0; iI < ctwDir->topLevelItemCount(); iI++) {
			if (qlTabs.value(qswDirs->currentIndex()).cfsFileSystem->IsFile(ctwDir->topLevelItem(iI))) {
				ctwDir->topLevelItem(iI)->setSelected(true);
			} // if
		} // for
	} else {
		ctwDir->selectAll();
	} // if else
} // SelectAll

// selected another column set for actual directory view
const void cPanel::SetColumnSet(const QString &qsColumnSet)
{
	int iColumnName;

	qlTabs[qswDirs->currentIndex()].qsColumnSet = qsColumnSet;
	// change sort column to prevent sorting by nonexisting column number
	iColumnName = GetNativeColumnIndex(qsNAME, qswDirs->currentIndex());
	if (iColumnName == -1) {
		iColumnName = GetNativeColumnIndex(qsNAME_WITH_EXTENSION, qswDirs->currentIndex());
		if (iColumnName == -1) {
			iColumnName = 0;
		} // if
	} // if
	static_cast<cTreeWidget *>(qswDirs->currentWidget())->header()->setSortIndicator(iColumnName, Qt::AscendingOrder);
	// refresh header to show changes
	RefreshHeader(qswDirs->currentIndex(), true);
} // SetColumnSet

// set focus to current directory view
const void cPanel::SetFocus() const
{
	qswDirs->currentWidget()->setFocus(Qt::OtherFocusReason);
} // SetFocus

// set path by directory from history list
const void cPanel::SetHistoryDirectory(const int &iPosition)
{
	sTab *stTab;

	stTab = &qlTabs[qswDirs->currentIndex()];

	stTab->shHistory.iPosition = iPosition;
	SetPath(stTab->shHistory.qlLastPaths.at(iPosition).qsPath);
} // SetHistoryDirectory

const void cPanel::SetPath(const QString &qsPath)
{
	cFileControl::sPathInfo spiPathInfo;
	int iIndex;

	spiPathInfo = cfcFileControl->GetPathInfo(qsPath);

	iIndex = qcbDrive->findText(spiPathInfo.qsDrive);
	if (iIndex != -1) {
		// drive exists -> proceed with set path
		sTab *stTab;

		stTab = &qlTabs[qswDirs->currentIndex()];

		// change drive
		qcbDrive->blockSignals(true);
		qcbDrive->setCurrentIndex(iIndex);
		qcbDrive->blockSignals(false);

		// possible change of file system
		if (cfcFileControl->ChangeFileSystem(stTab->cfsFileSystem, spiPathInfo.qsDrive, qsPath)) {
			ConnectFileSystem(stTab->cfsFileSystem);
		} // if

		// set path
		stTab->cfsFileSystem->SetPath(spiPathInfo.qsDrive, spiPathInfo.qsRootPath, qsPath);
	} // if
} // SetPath

// switch tabs
const void cPanel::SetTabIndex(const int &iTabIndex) const
{
	qtbTab->setCurrentIndex(iTabIndex);
} // SetTabIndex

// set text in tab bar
const void cPanel::SetTabText(const int &iTabIndex) const
{
	qtbTab->setTabText(iTabIndex, qlTabs.at(iTabIndex).cfsFileSystem->GetTabText());
} // SetTabText

// show or hide header for specified tab
const void cPanel::ShowHideHeader(const int &iTabIndex) const
{
	static_cast<cTreeWidget *>(qswDirs->widget(iTabIndex))->header()->setVisible(csSettings->GetShowDirectoryViewHeader());
} // ShowHideHeader

// show or hide headers in all tabs
const void cPanel::ShowHideHeaders() const
{
	int iI;

	for (iI = 0; iI < qlTabs.count(); iI++) {
		ShowHideHeader(iI);
	} // for
} // ShowHideHeaders

// sort dir content and show
const void cPanel::Sort(const int &iIndex, QList<QTreeWidgetItem *> &qlToSort)
{
	cSettings::sColumn *scSortedColumn;
	cTreeWidget *ctwDir;
	int iColumnName, iI;
	QList<QTreeWidgetItem *> qlDirectories, qlFiles, qlMarked;
	sTab *stTab;

	ctwDir = static_cast<cTreeWidget *>(qswDirs->widget(iIndex));
	stTab = &qlTabs[iIndex];

	// remember marked items and clear tree
	while (ctwDir->topLevelItemCount() > 0) {
		if (ctwDir->topLevelItem(0)->isSelected()) {
			qlMarked.append(ctwDir->takeTopLevelItem(0));
		} else {
			ctwDir->takeTopLevelItem(0);
		} // if else
	} // while

	// split into directories and files
	for (iI = 0; iI < qlToSort.count(); iI++) {
		QTreeWidgetItem *qtwiFile;

		qtwiFile = qlToSort[iI];
		if (stTab->cfsFileSystem->IsDir(qtwiFile)) {
			qlDirectories.append(qtwiFile);
		} else {
			qlFiles.append(qtwiFile);
		} // if else
	} // for

	// sort at first by name if possible (to have sorted the rest by filename as second condition)
	iColumnName = GetNativeColumnIndex(qsNAME, iIndex);
	if (iColumnName == -1) {
		// or by name with extension
		iColumnName = GetNativeColumnIndex(qsNAME_WITH_EXTENSION, iIndex);
	} // if
	if (iColumnName != -1) {
		ssSort.iSortedColumn = iColumnName;
		ssSort.soSortOrder = Qt::AscendingOrder;
		ssSort.bCaseSensitive = csSettings->GetCaseSensitiveSorting();
		qStableSort(qlDirectories.begin(), qlDirectories.end(), &cPanel::TreeSortByString);
		qStableSort(qlFiles.begin(), qlFiles.end(), &cPanel::TreeSortByString);
	} // if

	// set sort informations for sorting functions
	ssSort.iSortedColumn = ctwDir->sortColumn();
	ssSort.soSortOrder = ctwDir->header()->sortIndicatorOrder();
	ssSort.bCaseSensitive = csSettings->GetCaseSensitiveSorting();
	scSortedColumn = &stTab->qlColumns[ssSort.iSortedColumn];
	if ((scSortedColumn->qsIdentifier == qsNAME && ssSort.soSortOrder == Qt::DescendingOrder)
		 || scSortedColumn->qsIdentifier == qsEXTENSION
		 || (scSortedColumn->qsIdentifier == qsNAME_WITH_EXTENSION && ssSort.soSortOrder == Qt::DescendingOrder)
		 || scSortedColumn->qsIdentifier == qsATTRIBUTES
		 || scSortedColumn->qsPlugin != qsNO) {
		// string sort
		qStableSort(qlDirectories.begin(), qlDirectories.end(), &cPanel::TreeSortByString);
		qStableSort(qlFiles.begin(), qlFiles.end(), &cPanel::TreeSortByString);
	} else {
		if (scSortedColumn->qsIdentifier == qsSIZE) {
			// qint64 sort
			qStableSort(qlDirectories.begin(), qlDirectories.end(), &cPanel::TreeSortByQInt64);
			qStableSort(qlFiles.begin(), qlFiles.end(), &cPanel::TreeSortByQInt64);
		} else {
			if (scSortedColumn->qsIdentifier == qsDATE_TIME) {
				// QDateTime sort
				qStableSort(qlDirectories.begin(), qlDirectories.end(), &cPanel::TreeSortByQDateTime);
				qStableSort(qlFiles.begin(), qlFiles.end(), &cPanel::TreeSortByQDateTime);
			} // if else
		} // if else
	} // if else

	// move ".." directory to the first position
	for (iI = 0; iI < qlDirectories.count(); iI++) {
		bool bFound;

		bFound = false;
		if (stTab->cfsFileSystem->GetFileName(qlDirectories.at(iI), false) == "..") {
			bFound = true;
		} // if

		if (bFound) {
			qlDirectories.move(iI, 0);
			break;
		} // if
	} // for

	// show the result
	ctwDir->addTopLevelItems(qlDirectories);
	ctwDir->addTopLevelItems(qlFiles);

	// mark items again
	for (iI = 0; iI < qlMarked.count(); iI++) {
		qlMarked.at(iI)->setSelected(true);
	} // for
} // Sort

// sort by specified column
const void cPanel::SortBy(const int &iColumn)
{
	cTreeWidget *ctwDir;
	Qt::SortOrder soSortOrder;

	ctwDir = static_cast<cTreeWidget *>(qswDirs->currentWidget());

	// change sort order
	if (ctwDir->header()->sortIndicatorSection() == iColumn) {
		soSortOrder = ctwDir->header()->sortIndicatorOrder();
		if (soSortOrder == Qt::AscendingOrder) {
			soSortOrder = Qt::DescendingOrder;
		} else {
			soSortOrder = Qt::AscendingOrder;
		} // if else
	} else {
		soSortOrder = Qt::AscendingOrder;
	} // if else
	ctwDir->header()->setSortIndicator(iColumn, soSortOrder);

	// sort again
	Sort(qswDirs->currentIndex(), qlTabs.value(qswDirs->currentIndex()).cfsFileSystem->GetDirectoryContent(false));
} // SortBy

// automatic actualizations
void cPanel::timerEvent(QTimerEvent *event)
{
	ActualizeDrives();
	ActualizeVolumeInfo();
} // timerEvent

// compare items by QDateTime
const bool cPanel::TreeSortByQDateTime(const QTreeWidgetItem *qtwiItem1, const QTreeWidgetItem *qtwiItem2)
{
	QDateTime qdtItem1, qdtItem2;

	qdtItem1 = qtwiItem1->data(ssSort.iSortedColumn, Qt::UserRole).toDateTime();
	qdtItem2 = qtwiItem2->data(ssSort.iSortedColumn, Qt::UserRole).toDateTime();

	if ((qdtItem1 < qdtItem2 && ssSort.soSortOrder == Qt::AscendingOrder) || (qdtItem1 <= qdtItem2 && ssSort.soSortOrder == Qt::DescendingOrder)) {
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
} // TreeSortByQDateTime

// compare items by qint64
const bool cPanel::TreeSortByQInt64(const QTreeWidgetItem *qtwiItem1, const QTreeWidgetItem *qtwiItem2)
{
	qint64 qi64Item1, qi64Item2;

	qi64Item1 = qtwiItem1->data(ssSort.iSortedColumn, Qt::UserRole).toLongLong();
	qi64Item2 = qtwiItem2->data(ssSort.iSortedColumn, Qt::UserRole).toLongLong();

	if ((qi64Item1 < qi64Item2 && ssSort.soSortOrder == Qt::AscendingOrder) || (qi64Item1 <= qi64Item2 && ssSort.soSortOrder == Qt::DescendingOrder)) {
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
} // TreeSortByQInt64

// compare items by text
const bool cPanel::TreeSortByString(const QTreeWidgetItem *qtwiItem1, const QTreeWidgetItem *qtwiItem2)
{
	QString qsItem1, qsItem2;

	if (ssSort.bCaseSensitive) {
		qsItem1 = qtwiItem1->text(ssSort.iSortedColumn);
		qsItem2 = qtwiItem2->text(ssSort.iSortedColumn);
	} else {
		qsItem1 = qtwiItem1->text(ssSort.iSortedColumn).toLower();
		qsItem2 = qtwiItem2->text(ssSort.iSortedColumn).toLower();
	} // if else

	if ((qsItem1 < qsItem2 && ssSort.soSortOrder == Qt::AscendingOrder) || (qsItem1 <= qsItem2 && ssSort.soSortOrder == Qt::DescendingOrder)) {
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
const void cPanel::UnselectAll() const
{
	static_cast<cTreeWidget *>(qswDirs->currentWidget())->clearSelection();
} // UnselectAll