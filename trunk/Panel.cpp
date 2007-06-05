#include "Panel.h"

#include <QtCore/QDir>
#include <QtCore/QDateTime>
#include "Panel/Process.h"
#include <QtGui/QHeaderView>
#include <QtCore/QProcess>
#include "Panel/SelectDriveDialog.h"
#include <QtGui/QKeyEvent>
#include "FindFilesDialog.h"
#include <QtGui/QMenu>
#include <QtCore/QUrl>
#include <QtGui/QMessageBox>
#include <QtGui/QLineEdit>

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

	if (qhTabs.count() == 0 || !qhTabs.value(qswDir->currentIndex()).bValid) {
		// no tab created or not valid dir
		return;
	} // if

#ifdef Q_WS_WIN
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
	ctwTree->header()->setSortIndicatorShown(true);
	ctwTree->header()->setClickable(true);
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
	connect(ctwTree, SIGNAL(DropEvent(const cTreeWidget::eDropAction &, const QList<QUrl> &, QTreeWidgetItem *)), SLOT(on_ctwTree_DropEvent(const cTreeWidget::eDropAction &, const QList<QUrl> &, QTreeWidgetItem *)));
	connect(ctwTree, SIGNAL(DragEvent(cTreeWidget *)), SLOT(on_ctwTree_DragEvent(cTreeWidget *)));
	connect(ctwTree, SIGNAL(MoveEvent(QTreeWidgetItem *)), SLOT(on_ctwTree_MoveEvent(QTreeWidgetItem *)));

	// set tab properties
	stTab.qlColumns = new QList<cSettings::sColumn>;
	stTab.swWidgets = new sWidgets();
	stTab.swWidgets->qsDrive = stiTabInfo.qsDrive;
	stTab.swWidgets->qsPath = stiTabInfo.qsPath;
	stTab.sldLocalDirectory.qsPath = stiTabInfo.qsPath;
	stTab.qsColumnSet = stiTabInfo.qsColumnSet;
	stTab.elLocation = LocalDirectory;
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

// change current directory view path even to another drive
void cPanel::ChangePath(const QString &qsPath)
{
	QMap<QString, cFileRoutine::sDriveInfo> qmDrives;

	// change drive first
	qmDrives = cFileRoutine::GetDrives();
	QMapIterator<QString, cFileRoutine::sDriveInfo> qmiDrive(qmDrives);
	while (qmiDrive.hasNext()) {
		qmiDrive.next();

		if (qsPath.startsWith(qmiDrive.value().qsPath)) {
			qcbDrive->blockSignals(true);
			qcbDrive->setCurrentIndex(qcbDrive->findText(qmiDrive.key()));
			qcbDrive->blockSignals(false);
			// then change path
			qhTabs[qswDir->currentIndex()].sldLocalDirectory.qsPath = qsPath;
			if (qhTabs.value(qswDir->currentIndex()).elLocation == Archive) {
				qhTabs[qswDir->currentIndex()].elLocation = LocalDirectory;
			} // if
			SetPath(qsPath);
			break;
		} // if
	} // while
} // ChangePath

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
		qswDir->removeWidget(qswDir->widget(iTabIndex));
		qtbTab->removeTab(iTabIndex);

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
cPanel::cPanel(QMainWindow *qmwParent, QStackedWidget *qswPanel, QComboBox *qcbDrive, QLabel *qlDriveInfo, QTabBar *qtbTab, QLabel *qlPath, QLabel *qlSelected, cSettings *csSettings, cContent *ccContent, cPacker *cpPacker, QMap<QString, cFileRoutine::sDriveInfo> *qmDrives, QLabel *qlGlobalPath, QComboBox *qcbCommand, cFileOperation *cfoFileOperation, QLineEdit *qleQuickSearch)
{
	qswDir = qswPanel;
	this->qcbDrive = qcbDrive;
	this->qlDriveInfo = qlDriveInfo;
	this->qtbTab = qtbTab;
	this->qlPath = qlPath;
	this->qlSelected = qlSelected;
	this->csSettings = csSettings;
	this->ccContent = ccContent;
	this->cpPacker = cpPacker;
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
#ifdef Q_WS_WIN
	qcbDrive->view()->installEventFilter(this);
#endif
} // cPanel

// convert QDateTime to user defined format
QString cPanel::DateTime(const QDateTime &qdtDateTime)
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
} // DateTime

// create new tab by duplicate one
int cPanel::DuplicateTab(const int &iTabIndex)
{
	cSettings::sTabInfo stiTabInfo;

	stiTabInfo.qsColumnSet = qhTabs.value(iTabIndex).qsColumnSet;
	stiTabInfo.qsDrive = qhTabs.value(iTabIndex).swWidgets->qsDrive;
	stiTabInfo.qsPath = qhTabs.value(iTabIndex).sldLocalDirectory.qsPath;
	stiTabInfo.ssSort.iSortedColumn = static_cast<cTreeWidget *>(qswDir->widget(iTabIndex))->sortColumn();
	stiTabInfo.ssSort.soSortOrder = static_cast<cTreeWidget *>(qswDir->widget(iTabIndex))->header()->sortIndicatorOrder();

	return AddTab(stiTabInfo);
} // DuplicateTab

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
					case QEvent::FocusIn:
						qleQuickSearch->clear();
						if (csSettings->GetQuickSearchShowSearchWindow()) {
							qleQuickSearch->show();
						} // if
						return false;
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
								if (qswLastActive == qswDir) {
									qswLastActive->currentWidget()->setFocus(Qt::OtherFocusReason);
									QApplication::sendEvent(qswLastActive->currentWidget(), event);
									return true;
								} else {
									return false;
								} // if else
							case Qt::Key_Escape:
								if (qswLastActive == qswDir) {
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
										if (qswLastActive == qswDir) {
											qswLastActive->currentWidget()->setFocus(Qt::OtherFocusReason);
											return true;
										} else {
											return false;
										} // if else
									} // if else
								} // if else
						} // switch
					default:
						return false;
				} // switch
			} else {
#ifdef Q_WS_WIN
				if (watched == qcbDrive->view()) {
					// drives list
					if (event->type() == QEvent::KeyPress) {
						// if exists same drive as key pressed activate it
						if (qcbDrive->findText(static_cast<QKeyEvent *>(event)->text().toUpper()) != -1) {
							qcbDrive->setCurrentIndex(qcbDrive->findText(static_cast<QKeyEvent *>(event)->text().toUpper()));
							qcbDrive->hidePopup();
							static_cast<cTreeWidget *>(qswDir->currentWidget())->setFocus(Qt::OtherFocusReason);
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

// show custom list of files in current dir view
void cPanel::FeedToPanel(const QFileInfoList &qfilFiles)
{
	RefreshContent(qswDir->currentIndex(), qfilFiles);
} // FeedToPanel

// fill directory view item accodring to content of vData
void cPanel::FillDirViewItem(const int &iIndex, const eLocation &elType, QTreeWidgetItem *qtwiFile, const void *vData, QList<cContentDelayed::sParameters> *qlParameters)
{
	int iI;

	for (iI = 0; iI < qhTabs.value(iIndex).qlColumns->count(); iI++) {
		if (qhTabs.value(iIndex).qlColumns->at(iI).qsPlugin == qsNO) {
			// native
			if (qhTabs.value(iIndex).qlColumns->at(iI).qsIdentifier == qsICON) {
				// icon
				switch (elType) {
					case LocalDirectory:
						qtwiFile->setIcon(iI, qfipIcon.icon(*static_cast<const QFileInfo *>(vData)));
						break;
					case Archive:
						;// TODO FillDirViewItem archive filename icon
				} // switch
			} else {
				if (qhTabs.value(iIndex).qlColumns->at(iI).qsIdentifier == qsNAME) {
					// name
					QString qsName;

					switch (elType) {
						case LocalDirectory:
							if (static_cast<const QFileInfo *>(vData)->fileName() == "..") {
								// special handle for ".." directory to show both points
								qsName = "..";
							} else {
								qsName = static_cast<const QFileInfo *>(vData)->completeBaseName();
							} // if
							if (static_cast<const QFileInfo *>(vData)->isDir() && csSettings->GetShowBracketsAroundDirectoryName()) {
								qsName = '[' + qsName + ']';
							} // if
							break;
						case Archive:
							if (QFileInfo(static_cast<const tHeaderData *>(vData)->FileName).fileName() == "..") {
								// special handle for ".." directory to show both points
								qsName = "..";
							} else {
								qsName = QFileInfo(static_cast<const tHeaderData *>(vData)->FileName).completeBaseName();
							} // if
							if (static_cast<const tHeaderData *>(vData)->FileAttr & cPacker::iDIRECTORY && csSettings->GetShowBracketsAroundDirectoryName()) {
								qsName = '[' + qsName + ']';
							} // if
					} // switch
					qtwiFile->setText(iI, qsName);
				} else {
					if (qhTabs.value(iIndex).qlColumns->at(iI).qsIdentifier == qsEXTENSION) {
						// extension
						switch (elType) {
							case LocalDirectory:
								qtwiFile->setText(iI, static_cast<const QFileInfo *>(vData)->suffix());
								break;
							case Archive:
								qtwiFile->setText(iI, QFileInfo(static_cast<const tHeaderData *>(vData)->FileName).suffix());
						} // switch
					} else {
						if (qhTabs.value(iIndex).qlColumns->at(iI).qsIdentifier == qsSIZE) {
							// size
							switch (elType) {
								case LocalDirectory:
									if (static_cast<const QFileInfo *>(vData)->isDir()) {
										qtwiFile->setText(iI, tr("<DIR>"));
									} else {
										qtwiFile->setText(iI, GetSizeString(static_cast<const QFileInfo *>(vData)->size()));
									} // if else
									break;
								case Archive:
									if (static_cast<const tHeaderData *>(vData)->FileAttr & cPacker::iDIRECTORY) {
										qtwiFile->setText(iI, tr("<DIR>"));
									} else {
										qtwiFile->setText(iI, GetSizeString(static_cast<const tHeaderData *>(vData)->UnpSize));
									} // if else
							} // switch
						} else {
							if (qhTabs.value(iIndex).qlColumns->at(iI).qsIdentifier == qsDATE_TIME) {
								// date/time
								switch (elType) {
									case LocalDirectory:
										qtwiFile->setText(iI, DateTime(static_cast<const QFileInfo *>(vData)->lastModified()));
										break;
									case Archive:
										;// TODO FillDirViewItem archive filename datetime
								} // switch
							}
#ifdef Q_WS_WIN
							else {
								// attributes
								DWORD dwAttributes;
								QString qsAttributes;

								switch (elType ) {
									case LocalDirectory:
										dwAttributes = GetFileAttributes(reinterpret_cast<LPCWSTR>(static_cast<const QFileInfo *>(vData)->filePath().unicode()));
										break;
									case Archive:
										if (static_cast<const tHeaderData *>(vData)->FileAttr & cPacker::iREAD_ONLY) {
											dwAttributes = FILE_ATTRIBUTE_READONLY;
										} else {
											dwAttributes = 0;
										} // if else
										if (static_cast<const tHeaderData *>(vData)->FileAttr & cPacker::iHIDDEN) {
											dwAttributes |= FILE_ATTRIBUTE_HIDDEN;
										} // if
										if (static_cast<const tHeaderData *>(vData)->FileAttr & cPacker::iSYSTEM) {
											dwAttributes |= FILE_ATTRIBUTE_SYSTEM;
										} // if
										if (static_cast<const tHeaderData *>(vData)->FileAttr & cPacker::iARCHIVE) {
											dwAttributes |= FILE_ATTRIBUTE_ARCHIVE;
										} // if
								} // switch
								if (dwAttributes & FILE_ATTRIBUTE_READONLY) {
									qsAttributes = 'r';
								} // if
								if (dwAttributes & FILE_ATTRIBUTE_ARCHIVE) {
									qsAttributes += 'a';
								} // if
								if (dwAttributes & FILE_ATTRIBUTE_HIDDEN) {
									qsAttributes += 'h';
								} // if
								if (dwAttributes & FILE_ATTRIBUTE_SYSTEM) {
									qsAttributes += 's';
								} // if
								if (dwAttributes & FILE_ATTRIBUTE_COMPRESSED) {
									qsAttributes += 'c';
								} // if
								if (dwAttributes & FILE_ATTRIBUTE_ENCRYPTED) {
									qsAttributes += 'e';
								} // if

								qtwiFile->setText(iI, qsAttributes);
							} // if else
#endif
						} // if else
					} // if else
				} // if else
			} // if else
		} else {
			// plugin
			if (elType == LocalDirectory) {
				// but only for local directory files
				int iFlag;

				qtwiFile->setText(iI, ccContent->GetPluginValue(static_cast<const QFileInfo *>(vData)->filePath(), qhTabs.value(iIndex).qlColumns->at(iI).qsPlugin, qhTabs.value(iIndex).qlColumns->at(iI).qsIdentifier, qhTabs.value(iIndex).qlColumns->at(iI).qsUnit, &iFlag));
				if (iFlag == ft_delayed) {
					cContentDelayed::sParameters spParameters;

					// thread input
					spParameters.siInput.qsFilename = static_cast<const QFileInfo *>(vData)->filePath();
					spParameters.siInput.qsPlugin = qhTabs.value(iIndex).qlColumns->at(iI).qsPlugin;
					spParameters.siInput.qsColumn = qhTabs.value(iIndex).qlColumns->at(iI).qsIdentifier;
					spParameters.siInput.qsUnit = qhTabs.value(iIndex).qlColumns->at(iI).qsUnit;

					// thread output
					spParameters.soOutput.qtwiItem = qtwiFile;
					spParameters.soOutput.iColumn = iI;

					qlParameters->append(spParameters);
				} // if
			} // if
		} // if else
	} // for
} // FillDirViewItem

// columns for current dir view
QList<cSettings::sColumn> *cPanel::GetColumns()
{
	return qhTabs.value(qswDir->currentIndex()).qlColumns;
} // GetColumns

//< column set for current directory view
QString cPanel::GetColumnSet()
{
	return qhTabs.value(qswDir->currentIndex()).qsColumnSet;
} // GetColumnSet

// get content of the directory view
QHash<QTreeWidgetItem *, QFileInfo> cPanel::GetDirContent()
{
	return qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qhFiles;
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
	switch (qhTabs.value(qswDir->currentIndex()).elLocation) {
		case LocalDirectory:
			return qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qsPath;
		case Archive:
			return qhTabs.value(qswDir->currentIndex()).saArchive.qsPath;
	} // switch

	// shouldn't happen
	return QString();
} // GetPath

// get file infos of selected items
QFileInfoList cPanel::GetSelectedItemsFileList(const QDir::Filters &qfType /* QDir::Dirs | QDir::Files */)
{
	int iI;
	QFileInfoList qfilItems;
	QList<QTreeWidgetItem *> qlSelected;

	qlSelected = static_cast<cTreeWidget *>(qswDir->currentWidget())->selectedItems();
	for (iI = 0; iI < qlSelected.count(); iI++) {
		if (qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qhFiles.value(qlSelected.at(iI)).fileName() != "..") {
			// ignore ".." directories
			if ((qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qhFiles.value(qlSelected.at(iI)).isDir() && (qfType & QDir::Dirs))
				 || (qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qhFiles.value(qlSelected.at(iI)).isFile() && (qfType & QDir::Files))) {
				qfilItems.append(qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qhFiles.value(qlSelected.at(iI)));
			} // if
		} // if
	} // for

	return qfilItems;
} // GetSelectedItemsFileList

// get selected files and directories from current dir view
QStringList cPanel::GetSelectedItemsStringList()
{
	int iI;
	QList<QTreeWidgetItem *> qlSelected;
	QStringList qslSelected;

	qlSelected = static_cast<cTreeWidget *>(qswDir->currentWidget())->selectedItems();
	for (iI = 0; iI < qlSelected.count(); iI++) {
		qslSelected.append(QDir::toNativeSeparators(qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qhFiles.value(qlSelected.at(iI)).filePath()));
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
	QHashIterator<QTreeWidgetItem *, QFileInfo> qhiFiles(qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qhFiles);
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

	qdDir.setPath(qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qsPath);
	if(!qdDir.isRoot()) {
		if (qhTabs.value(qswDir->currentIndex()).elLocation == Archive) {
			qhTabs[qswDir->currentIndex()].elLocation = LocalDirectory;
		} // if
		SetPath(qdDir.rootPath());
	} // if
} // GoToRootDir

// go up one level (directory)
void cPanel::GoToUpDir()
{
	QDir qdDir;
	QString qsFrom;

	switch (qhTabs.value(qswDir->currentIndex()).elLocation) {
		case LocalDirectory:
			qdDir.setPath(qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qsPath);

			if(!qdDir.isRoot()) {				
				// remember directory going from
				qsFrom = QFileInfo(GetPath()).fileName();

				SetPath(qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qsPath + "/..");

				// find directory went from and set it as current
				QHashIterator<QTreeWidgetItem *, QFileInfo> qhiFile(qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qhFiles);
				while (qhiFile.hasNext()) {
					qhiFile.next();
					if (qhiFile.value().fileName() == qsFrom) {
						static_cast<QTreeWidget *>(qswDir->currentWidget())->setCurrentItem(qhiFile.key());
						break;
					} // if
				} // while
			} // if
			break;
		case Archive:
			qdDir.setPath(qhTabs.value(qswDir->currentIndex()).saArchive.qsPath);

			// remember directory going from
			qsFrom = QFileInfo(GetPath()).fileName();

			SetPath(qhTabs.value(qswDir->currentIndex()).saArchive.qsPath + "/..");

			// find directory went from and set it as current
			QHashIterator<QTreeWidgetItem *, tHeaderData> qhiFile(qhTabs.value(qswDir->currentIndex()).saArchive.qhFiles);
			while (qhiFile.hasNext()) {
				qhiFile.next();
				if (QFileInfo(qhiFile.value().FileName).fileName() == qsFrom) {
					static_cast<QTreeWidget *>(qswDir->currentWidget())->setCurrentItem(qhiFile.key());
					break;
				} // if
			} // while
	} // switch
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
	QHashIterator<QTreeWidgetItem *, QFileInfo> qhiFile(qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qhFiles);
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
		QList<QTreeWidgetItem *> qlFiles;
		QList<QUrl> qlUrls;
		QMimeData *qmdMimeData;

		for (iI = 0; iI < qfilFiles.count(); iI++) {
			qlUrls.append(QUrl::fromLocalFile(qfilFiles.at(iI)));
		} // for

		qmdMimeData = new QMimeData();
		qmdMimeData->setUrls(qlUrls);

		qdDrag = new QDrag(ctwSource);
		qdDrag->setMimeData(qmdMimeData);

		// get files from current directory to not be able drag on them
		QHashIterator<QTreeWidgetItem *, QFileInfo> qhiFiles(qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qhFiles);
		while (qhiFiles.hasNext()) {
			qhiFiles.next();
			if (qhiFiles.value().isFile()) {
				qlFiles.append(qhiFiles.key());
			} // if
		} // while

		static_cast<cTreeWidget *>(qswDir->currentWidget())->StartDragFromPanel(qlFiles);
		qdDrag->start(Qt::CopyAction | Qt::MoveAction);
		static_cast<cTreeWidget *>(qswDir->currentWidget())->StopDragFromPanel();
		qtwiLastMovedOver = NULL;
	} // if
} // on_ctwTree_DragEvent

// drop event occured
void cPanel::on_ctwTree_DropEvent(const cTreeWidget::eDropAction &edaAction, const QList<QUrl> &clUrls, QTreeWidgetItem *qtwiDroppedOn)
{
	cFileRoutine::eOperation eoOperation;
	int iI;
	QFileInfoList qfilFiles;
	QString qsDestination;

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

	if (qtwiDroppedOn && qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qhFiles.value(qtwiDroppedOn).isDir()) {
		qsDestination = qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qhFiles.value(qtwiDroppedOn).filePath();
	} else {
		qsDestination = GetPath();
	} // if else

	cfoFileOperation->Operate(eoOperation, qfilFiles, qsDestination);
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
	const tHeaderData *thdFile;

	qfiFile = qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qhFiles.value(item);
	thdFile = &qhTabs.value(qswDir->currentIndex()).saArchive.qhFiles.value(item);

	switch (qhTabs.value(qswDir->currentIndex()).elLocation) {
		case LocalDirectory:
			if (qfiFile.isDir()) {
				// double click on directory -> go into directory
				if (qfiFile.fileName() == "..") {
					GoToUpDir();
				} else {
					SetPath(qfiFile.filePath());
				} // if else
			} else {
				// double click on file
				// check if it's supported archive
				if (!OpenArchive(qfiFile)) {
					// else execute it
					cProcess::Execute(QString("\"%1\"").arg(qfiFile.filePath()), qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qsPath);
				} // if
			} // if else
			break;
		case Archive:
			if (thdFile->FileAttr & cPacker::iDIRECTORY) {
				// double click on directory -> go into directory
				if (QFileInfo(thdFile->FileName).fileName() == "..") {
					GoToUpDir();
				} else {
					SetPath(qhTabs.value(qswDir->currentIndex()).saArchive.qsPath + '/' + QFileInfo(thdFile->FileName).fileName());
				} // if else
			} else {
				// double click on file
				// TODO on_ctwTree_itemActivated archive file activated
				;
			} // if else
			;
	} // switch
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
		if (qhiTab.value().sldLocalDirectory.qhFiles.value(ctwTree->topLevelItem(iI)).isDir()) {
			iDirectoriesTotal++;
			if (ctwTree->topLevelItem(iI)->isSelected()) {
				int iColumnExtension;

				iDirectories++;
				iColumnExtension = GetNativeColumnIndex(qsSIZE, qswDir->currentIndex());
				if (iColumnExtension != -1) {
					// size for directory can be known too
					qi64Size += ctwTree->topLevelItem(iI)->data(iColumnExtension, Qt::UserRole).toInt();
				} // if
			} // if
		} else {
			iFilesTotal++;
			qi64TotalSize += qhiTab.value().sldLocalDirectory.qhFiles.value(ctwTree->topLevelItem(iI)).size();
			if (ctwTree->topLevelItem(iI)->isSelected()) {
				iFiles++;
				qi64Size += qhiTab.value().sldLocalDirectory.qhFiles.value(ctwTree->topLevelItem(iI)).size();
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
		case Qt::Key_Backspace:
			GoToUpDir();
			break;
		case Qt::Key_Space:
			qfiFile = qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qhFiles.value(qtwiItem);

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

			iColumnExtension = GetNativeColumnIndex(qsSIZE, qswDir->currentIndex());
			if (iColumnExtension == -1) {
				// no place to show occupied space
				return;
			} // if

			qfilFiles = cFileRoutine::GetSources(QFileInfoList() << qfiFile);
			qi64Size = 0;
			for (iI = 0; iI < qfilFiles.count(); iI++) {
				qi64Size += qfilFiles.at(iI).size();
			} // for

			// put size to data to count with it when selecting files
			qtwiItem->setData(iColumnExtension, Qt::UserRole, qi64Size);
			qtwiItem->setText(iColumnExtension, GetSizeString(qi64Size));
			break;
		case Qt::Key_Enter:
		case Qt::Key_Return:
			if (static_cast<bool>(qkeEvent->modifiers() & Qt::ControlModifier) && static_cast<bool>(qkeEvent->modifiers() & Qt::ShiftModifier)) {
				// ctrl+shift+enter -> copy filePath to command line
				qsName = qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qhFiles.value(qtwiItem).filePath();
			} else {
				qsName = qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qhFiles.value(qtwiItem).fileName();
				// ctrl+enter -> copy fileName to command line
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
				qleQuickSearch->setFocus(Qt::OtherFocusReason);
				QApplication::sendEvent(qleQuickSearch, qkeEvent);
			} else {
				// pass key to command line
				QApplication::sendEvent(qcbCommand, qkeEvent);
				qcbCommand->setFocus(Qt::OtherFocusReason);
			} // if else
	} // switch
} // on_ctwTree_KeyPressed

// dragging items
void cPanel::on_ctwTree_MoveEvent(QTreeWidgetItem *qtwiMovedOver)
{
	// unselect last item moved over
	if (static_cast<cTreeWidget *>(qswDir->currentWidget())->indexOfTopLevelItem(qtwiLastMovedOver) != -1) {
		qtwiLastMovedOver->setSelected(false);
	} // if

	// memorize current
	if (qtwiMovedOver && !qtwiMovedOver->isSelected()) {
		qtwiMovedOver->setSelected(true);
		qtwiLastMovedOver = qtwiMovedOver;
	} else {
		// will not be unselected next time
		qtwiLastMovedOver = NULL;
	} // if else
} // on_ctwTree_MoveEvent

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

			// switch drive always to local directory
			if (qhTabs.value(qswDir->currentIndex()).elLocation == Archive) {
				qhTabs[qswDir->currentIndex()].elLocation = LocalDirectory;
			} // if
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
		if (qhTabs.value(iI).elLocation == LocalDirectory && qhTabs.value(iI).sldLocalDirectory.qsPath == path) {
			if (qswDir->currentIndex() == iI) {
				// refresh immediately
				RefreshContent(iI);
			} else {
				// wait with refresh till swith to the tab
				qhTabs[qswDir->currentIndex()].bValid = false;
			} // if else
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
	if (!qhTabs.value(index).bValid) {
		RefreshContent();
	} // if
	ActualizeWidgets();
} // on_qtbTab_currentChanged

// timer's timeout
void cPanel::on_qtTimer_timeout()
{
	ActualizeVolumeInfo();
} // on_qtTimer_timeout

// try to open archive
bool cPanel::OpenArchive(const QFileInfo &qfiFile)
{
	int iI;
	QHash<QString, cPacker::sPluginInfo> qhPluginsInfo;
	QList<cSettings::sPlugin> qlPackerPlugins;

	qhPluginsInfo = cpPacker->GetPluginsInfo();
	qlPackerPlugins = csSettings->GetPlugins(cSettings::PackerPlugins);

	for (iI = 0; iI < qlPackerPlugins.count(); iI++) {
		if (qlPackerPlugins.at(iI).bEnabled) {
			QStringList qslExtensions;

			qslExtensions = qlPackerPlugins.at(iI).qsExtensions.split(';');
			if (qslExtensions.contains(qfiFile.suffix())) {
				HANDLE hArchive;
				tOpenArchiveData toadArchiveData;

				toadArchiveData.ArcName = new char[qfiFile.filePath().length() + 1];
				strcpy(toadArchiveData.ArcName, qfiFile.filePath().toLatin1().constData());
				toadArchiveData.OpenMode = PK_OM_LIST;

				hArchive = qhPluginsInfo.value(QFileInfo(qlPackerPlugins.at(iI).qsName).fileName()).toaOpenArchive(&toadArchiveData);

				if (hArchive) {
					// archive opened successfully
					qhTabs[qswDir->currentIndex()].saArchive.spiPlugin = qhPluginsInfo.value(QFileInfo(qlPackerPlugins.at(iI).qsName).fileName());
					qhTabs[qswDir->currentIndex()].saArchive.qsArchive = qfiFile.filePath();
					// set location in archive
					qhTabs[qswDir->currentIndex()].elLocation = Archive;
					// read archive files
					qhTabs[qswDir->currentIndex()].saArchive.qlFiles = ReadArchiveFiles(hArchive);
					qhTabs[qswDir->currentIndex()].saArchive.qsPath = '.';
					// close archive
					qhPluginsInfo.value(QFileInfo(qlPackerPlugins.at(iI).qsName).fileName()).tcaCloseArchive(hArchive);
					// show archive contents
					SetPath(qhTabs.value(qswDir->currentIndex()).saArchive.qsPath);

					return true;
				} // if

				delete toadArchiveData.ArcName;
			} // if
		} // if
	} // for

	return false;
} // OpenArchive

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

		if (qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qhFiles.value(static_cast<QTreeWidget *>(qswDir->currentWidget())->topLevelItem(iPos)).fileName().startsWith(qsFilename)) {
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

// read archive contents
QList<tHeaderData> cPanel::ReadArchiveFiles(const HANDLE &hArchive)
{
	QList<tHeaderData> qlFiles;
	tHeaderData thdHeaderData;

	// archive root ".." directory
	strcpy(thdHeaderData.FileName, "..");
	// TODO ReadArchiveFiles ".." archive file time as today
	thdHeaderData.FileAttr = cPacker::iDIRECTORY;
	qlFiles.append(thdHeaderData);

	while (!qhTabs.value(qswDir->currentIndex()).saArchive.spiPlugin.trhReadHeader(hArchive, &thdHeaderData)) {
		if (thdHeaderData.FileAttr & cPacker::iDIRECTORY) {
			// create ".." directory in each archive directory
			QString qsDirectory;
			tHeaderData thdDotDot;

			qsDirectory = QFileInfo(thdHeaderData.FileName).filePath() + "/..";
			strcpy(thdDotDot.FileName, qsDirectory.toLatin1().constData());
			// TODO ReadArchiveFiles ".." archive file time as today
			thdDotDot.FileAttr = cPacker::iDIRECTORY;
			qlFiles.append(thdDotDot);
		} // if

		qlFiles.append(thdHeaderData);
		qhTabs.value(qswDir->currentIndex()).saArchive.spiPlugin.tpfProcessFile(hArchive, PK_SKIP, NULL, NULL);
	} // while

	return qlFiles;
} // ReadArchiveFiles

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
	qsFrom = qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qhFiles.value(static_cast<QTreeWidget *>(qswDir->currentWidget())->currentItem()).fileName();

	RefreshContent(qswDir->currentIndex());

	// find file went from and set it as current
	QHashIterator<QTreeWidgetItem *, QFileInfo> qhiFile(qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qhFiles);
	while (qhiFile.hasNext()) {
		qhiFile.next();
		if (qhiFile.value().fileName() == qsFrom) {
			static_cast<QTreeWidget *>(qswDir->currentWidget())->clearSelection();
			static_cast<QTreeWidget *>(qswDir->currentWidget())->setCurrentItem(qhiFile.key());
			break;
		} // if
	} // while
} // RefreshContent

// refresh dir content with local directory files
void cPanel::RefreshContent(const int &iIndex, QFileInfoList qfilFiles)
{
	int iI;
	QList<cContentDelayed::sParameters> qlParameters;

	// interrupt delayed content processing
	emit InterruptContentDelayed();

	switch (qhTabs.value(iIndex).elLocation) {
		case LocalDirectory:
			// clear previous file contents
			qhTabs[iIndex].sldLocalDirectory.qhFiles.clear();

			if (qfilFiles.count() == 0) {
				// check path
				if (!PathExists(qhTabs.value(iIndex).sldLocalDirectory.qsPath)) {
					SetPath(qhTabs.value(iIndex).sldLocalDirectory.qsPath);
					return;
				} // if
				// get files
				qfilFiles = cFileRoutine::GetDirectoryContent(qhTabs.value(iIndex).sldLocalDirectory.qsPath, GetStandardFilters());
			} // if

			// go through files and add them into file list
			for (iI = 0; iI < qfilFiles.count(); iI++) {
				QTreeWidgetItem *qtwiFile;

				// omit some entries
				if (qfilFiles.at(iI).fileName() == ".") {
					continue;
				} // if

				// add to internal file list
				qtwiFile = new QTreeWidgetItem();
				qhTabs[iIndex].sldLocalDirectory.qhFiles.insert(qtwiFile, qfilFiles.at(iI));

				// fill columns
				FillDirViewItem(iIndex, LocalDirectory, qtwiFile, &qfilFiles.at(iI), &qlParameters);
			} // for
			break;
		case Archive:
			// clear previous file contents
			qhTabs[iIndex].saArchive.qhFiles.clear();

			// go through files and add them into file list
			for (iI = 0; iI < qhTabs.value(iIndex).saArchive.qlFiles.count(); iI++) {
				// check for path in archive
				if (QFileInfo(qhTabs.value(iIndex).saArchive.qlFiles.at(iI).FileName).path() == qhTabs.value(iIndex).saArchive.qsPath) {
					QTreeWidgetItem *qtwiFile;

					// add to internal file list
					qtwiFile = new QTreeWidgetItem();
					qhTabs[iIndex].saArchive.qhFiles.insert(qtwiFile, qhTabs.value(iIndex).saArchive.qlFiles.at(iI));

					// fill columns
					FillDirViewItem(iIndex, Archive, qtwiFile, &qhTabs.value(iIndex).saArchive.qlFiles.at(iI), NULL);
				} // if
			} // for
	} // switch

	// sort and show files
	Sort(iIndex);
	qhTabs[iIndex].bValid = true;

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
void cPanel::RefreshContent(const QFileInfoList &qfilFiles)
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
	qhTabs[iIndex].bValid = false;

	// clear previous header contents and fill new information
	qhTabs.value(iIndex).qlColumns->clear();
	qslColumns = csSettings->GetColumnsInSet(qhTabs.value(iIndex).qsColumnSet);
	// check for column count in current column set
	if (qslColumns.count() == 0 && qhTabs.value(iIndex).qsColumnSet != qsFULL) {
		// no columns - switch to full column set
		qhTabs[iIndex].qsColumnSet = qsFULL;
		qslColumns = csSettings->GetColumnsInSet(qhTabs.value(iIndex).qsColumnSet);
	} // if
	if (qslColumns.count() == 0 && qhTabs.value(iIndex).qsColumnSet == qsFULL) {
		// create default column set (Full) if not created yet or empty
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
	static_cast<cTreeWidget *>(qswDir->widget(iIndex))->setColumnCount(qhTabs.value(iIndex).qlColumns->count());
	static_cast<cTreeWidget *>(qswDir->widget(iIndex))->setHeaderItem(qtwiHeader);

	// customize header
	// autostretch last section
	bAutoStretch = true;
	for (iI = 0; iI < qhTabs.value(iIndex).qlColumns->count() - 1; iI++) {
		if (qhTabs.value(iIndex).qlColumns->at(iI).iWidth == 0) {
			bAutoStretch = false;
			break;
		} // if
	} // for
	static_cast<cTreeWidget *>(qswDir->widget(iIndex))->header()->setStretchLastSection(bAutoStretch);
	// set columns width
	for (iI = 0; iI < qhTabs.value(iIndex).qlColumns->count(); iI++) {
		if (qhTabs.value(iIndex).qlColumns->at(iI).iWidth == 0) {
			static_cast<cTreeWidget *>(qswDir->widget(iIndex))->header()->setResizeMode(iI, QHeaderView::Stretch);
		} else {
			static_cast<cTreeWidget *>(qswDir->widget(iIndex))->header()->resizeSection(iI, qhTabs.value(iIndex).qlColumns->at(iI).iWidth);
		} // if else
	} // for

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
		stiTab.qsPath = qhiTab.value().sldLocalDirectory.qsPath;
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

		QHashIterator<QTreeWidgetItem *, QFileInfo> qhiFile(qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qhFiles);
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
		QHashIterator<QTreeWidgetItem *, QFileInfo> qhiFiles(qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qhFiles);
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

// selected another column set for actual directory view
void cPanel::SetColumnSet(const QString &qsColumnSet)
{
	qhTabs[qswDir->currentIndex()].qsColumnSet = qsColumnSet;
	// change sort column to prevent sorting by nonexisting column number
	static_cast<cTreeWidget *>(qswDir->currentWidget())->header()->setSortIndicator(0, Qt::AscendingOrder);
	// refresh header to show changes
	RefreshHeader(qswDir->currentIndex(), true);
} // SetColumnSet

// set focus to current directory view
void cPanel::SetFocus()
{
	qswDir->currentWidget()->setFocus(Qt::OtherFocusReason);
} // SetFocus

// set new path for current dir view on selected drive
void cPanel::SetPath(const QString &qsPath)
{
	// remove old path from watcher
	qhTabs[qswDir->currentIndex()].bValid = false;
	qfswWatcher.removePath(qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qsPath);

	switch (qhTabs.value(qswDir->currentIndex()).elLocation) {
		case LocalDirectory:
#ifdef Q_WS_WIN
			// check new path
			if (!PathExists(qsPath)) {
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
						// stay in previous path if exists
						qcbDrive->blockSignals(true);
						qcbDrive->setCurrentIndex(qcbDrive->findText(qhTabs.value(qswDir->currentIndex()).swWidgets->qsDrive));
						qcbDrive->blockSignals(false);
						if (PathExists(qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qsPath)) {
							qfswWatcher.addPath(qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qsPath);
							qhTabs[qswDir->currentIndex()].bValid = true;
						} else {
							SetPath(qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qsPath);
						} // if
					} // if else
				} else {
					// maybe valid drive but invalid path
					QDir qdDir;

					qdDir.setPath(qsPath);
					qdDir.cdUp();
					if (qdDir.path() == qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qsPath) {
						// unsuccessful try to change to subdirectory
						qfswWatcher.addPath(qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qsPath);
						qhTabs[qswDir->currentIndex()].bValid = true;
					} else {
						// bad directory, try to go one dir up
						GoToUpDir();
					} // if else
				} // if else
			} else {		
#endif
				// path ok
				qhTabs.value(qswDir->currentIndex()).swWidgets->qsDrive = qcbDrive->currentText();
				qhTabs[qswDir->currentIndex()].sldLocalDirectory.qsPath = QDir::cleanPath(qsPath);
				// add new path to watcher
				qfswWatcher.addPath(qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qsPath);
				// modify last valid path
				qhLastPaths.insert(qcbDrive->currentText(), qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qsPath);
				// new path in widgets
				qhTabs[qswDir->currentIndex()].swWidgets->qsPath = qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qsPath;
				qlGlobalPath->setText(qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qsPath);
				// refresh directory view
				RefreshContent(qswDir->currentIndex());
#ifdef Q_WS_WIN
			} // if else
#endif
			break;
		case Archive:
			// set new path
			if (qsPath.endsWith("..")) {
				// go up one directory
				if (qhTabs.value(qswDir->currentIndex()).saArchive.qsPath == ".") {
					// go to local directory
					qhTabs[qswDir->currentIndex()].elLocation = LocalDirectory;
					SetPath(qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qsPath);
					return;
				} else {
					// go up one directory in archive
					qhTabs[qswDir->currentIndex()].saArchive.qsPath = QDir::cleanPath(qhTabs.value(qswDir->currentIndex()).saArchive.qsPath + "/..");
					if (qhTabs.value(qswDir->currentIndex()).saArchive.qsPath.endsWith("..")) {
						qhTabs[qswDir->currentIndex()].saArchive.qsPath = '.';
					} // if
				} // if else
			} else {
				// go into another directory in archive
				if (qsPath.startsWith('.') && qsPath != ".") {
					// skip "./"
					qhTabs[qswDir->currentIndex()].saArchive.qsPath = qsPath.mid(2);
				} else {
					qhTabs[qswDir->currentIndex()].saArchive.qsPath = qsPath;
				} // if else
			} // if else
			// new path in widgets
			qhTabs[qswDir->currentIndex()].swWidgets->qsPath = qhTabs.value(qswDir->currentIndex()).sldLocalDirectory.qsPath + '/' + QFileInfo(qhTabs.value(qswDir->currentIndex()).saArchive.qsArchive).fileName() + '/' + qhTabs.value(qswDir->currentIndex()).saArchive.qsPath;
			qlGlobalPath->setText(qhTabs.value(qswDir->currentIndex()).swWidgets->qsPath);
			// refresh directory view
			RefreshContent(qswDir->currentIndex());
	} // switch
} // SetPath

// set text in tab bar
void cPanel::SetTabText(const int &iTabIndex)
{
	QFileInfo qfiTabDir;
	QString qsTabText;

	if (csSettings->GetShowDriveLetter()) {
		qsTabText = qhTabs.value(iTabIndex).swWidgets->qsDrive + ": ";
	} // if

	switch (qhTabs.value(iTabIndex).elLocation) {
		case LocalDirectory:
			qfiTabDir = QFileInfo(qhTabs.value(iTabIndex).sldLocalDirectory.qsPath);
			if (qfiTabDir.fileName().isEmpty()) {
				qsTabText += '/';
			} else {
				qsTabText += qfiTabDir.fileName();
			} // if else
			break;
		case Archive:
			if (qhTabs.value(iTabIndex).saArchive.qsPath == ".") {
				qsTabText += QFileInfo(qhTabs.value(iTabIndex).saArchive.qsArchive).fileName();
			} else {
				qsTabText += QFileInfo(qhTabs.value(iTabIndex).saArchive.qsPath).fileName();
			} // if else
	} // switch

	qtbTab->setTabText(iTabIndex, qsTabText);
} // SetTabText

// sort dir content and show
void cPanel::Sort(const int &iIndex)
{
	int iColumnName, iI;
	QHashIterator<QTreeWidgetItem *, tHeaderData> qhiArchiveFile(qhTabs.value(iIndex).saArchive.qhFiles);
	QHashIterator<QTreeWidgetItem *, QFileInfo> qhiLocalDirectoryFile(qhTabs.value(iIndex).sldLocalDirectory.qhFiles);
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
	switch (qhTabs.value(iIndex).elLocation) {
		case LocalDirectory:
			while (qhiLocalDirectoryFile.hasNext()) {
				qhiLocalDirectoryFile.next();
				if (qhiLocalDirectoryFile.value().isDir()) {
					qlDirectories.append(qhiLocalDirectoryFile.key());
				} else {
					qlFiles.append(qhiLocalDirectoryFile.key());
				} // if else
			} // while
			break;
		case Archive:
			while (qhiArchiveFile.hasNext()) {
				qhiArchiveFile.next();
				if (qhiArchiveFile.value().FileAttr & cPacker::iDIRECTORY) {
					qlDirectories.append(qhiArchiveFile.key());
				} else {
					qlFiles.append(qhiArchiveFile.key());
				} // if else
			} // while
	} // switch

	// sort at first by name if possible (to have sorted the rest by filename as second condition)
	iColumnName = GetNativeColumnIndex(qsNAME, iIndex);
	if (iColumnName != -1) {
		ssSort.iSortedColumn = iColumnName;
		ssSort.soSortOrder = Qt::AscendingOrder;
		ssSort.bCaseSensitive = csSettings->GetCaseSensitiveSorting();
		qStableSort(qlDirectories.begin(), qlDirectories.end(), &cPanel::TreeSortByString);
		for (iI = 0; iI < qlDirectories.count(); iI++) {
			bool bFound;

			bFound = false;
			switch (qhTabs.value(iIndex).elLocation) {
				case LocalDirectory:
					if (qhTabs.value(iIndex).sldLocalDirectory.qhFiles.value(qlDirectories.at(iI)).fileName() == "..") {
						bFound = true;
					} // if
					break;
				case Archive:
					if (qhTabs.value(iIndex).saArchive.qhFiles.value(qlDirectories.at(iI)).FileName == "..") {
						bFound = true;
					} // if
			} // switch
			if (bFound) {
				qlDirectories.move(iI, 0);
				break;
			} // if
		} // for
		qStableSort(qlFiles.begin(), qlFiles.end(), &cPanel::TreeSortByString);
	} // if

	// set sort informations for sorting functions
	ssSort.iSortedColumn = static_cast<cTreeWidget *>(qswDir->widget(iIndex))->sortColumn();
	ssSort.soSortOrder = static_cast<cTreeWidget *>(qswDir->widget(iIndex))->header()->sortIndicatorOrder();
	ssSort.bCaseSensitive = csSettings->GetCaseSensitiveSorting();
	if ((qhTabs.value(iIndex).qlColumns->at(ssSort.iSortedColumn).qsIdentifier == qsNAME && ssSort.soSortOrder == Qt::DescendingOrder) || qhTabs.value(iIndex).qlColumns->at(ssSort.iSortedColumn).qsIdentifier == qsEXTENSION || qhTabs.value(iIndex).qlColumns->at(ssSort.iSortedColumn).qsPlugin != qsNO) {
		// let ".." be the first if possible
		bool bDotDotFirst;

		bDotDotFirst = false;
		switch (qhTabs.value(iIndex).elLocation) {
			case LocalDirectory:
				if (qhTabs.value(iIndex).sldLocalDirectory.qhFiles.value(qlDirectories.front()).fileName() == "..") {
					bDotDotFirst = true;
				} // if
				break;
			case Archive:
				if (QFileInfo(qhTabs.value(iIndex).saArchive.qhFiles.value(qlDirectories.front()).FileName).fileName() == "..") {
					bDotDotFirst = true;
				} // if
		} // switch
		if (bDotDotFirst) {
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
void cPanel::UnselectAll()
{
	static_cast<cTreeWidget *>(qswDir->currentWidget())->clearSelection();
} // UnselectAll
