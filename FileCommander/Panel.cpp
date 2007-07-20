#include "Panel.h"

#include "Panel/TreeWidget.h"
#include <QtGui/QHeaderView>
#include <QtCore/QDateTime>

QStackedWidget *cPanel::qswLastActive;	///< last active panel (static class variable)

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

// add new tab with dir view
const int cPanel::AddTab(cSettings::sTabInfo &stiTabInfo, const bool &bStartUp /* false */)
{
	// TODO AddTab
	cTreeWidget *ctwTree;
	int /*iI, */iIndex;
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

	/*// connect signals to slots
	connect(ctwTree, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(on_ctwTree_customContextMenuRequested(const QPoint &)));
	connect(ctwTree, SIGNAL(itemActivated(QTreeWidgetItem *, int)), SLOT(on_ctwTree_itemActivated(QTreeWidgetItem *, int)));
	connect(ctwTree, SIGNAL(itemSelectionChanged(const cTreeWidget *)), SLOT(on_ctwTree_itemSelectionChanged(const cTreeWidget *)));
	connect(ctwTree, SIGNAL(KeyPressed(QKeyEvent *, QTreeWidgetItem *)), SLOT(on_ctwTree_KeyPressed(QKeyEvent *, QTreeWidgetItem *)));*/
	connect(ctwTree, SIGNAL(GotFocus()), SLOT(on_ctwTree_GotFocus()));
	/*connect(ctwTree, SIGNAL(DropEvent(const cTreeWidget::eDropAction &, const QList<QUrl> &, const QString &, const QString &, QTreeWidgetItem *)), SLOT(on_ctwTree_DropEvent(const cTreeWidget::eDropAction &, const QList<QUrl> &, const QString &, const QString &, QTreeWidgetItem *)));
	connect(ctwTree, SIGNAL(DragEvent()), SLOT(on_ctwTree_DragEvent()));
	connect(ctwTree, SIGNAL(MoveEvent(QTreeWidgetItem *)), SLOT(on_ctwTree_MoveEvent(QTreeWidgetItem *)));*/

	// set tab properties
	stTab.swWidgets.qsDrive = stiTabInfo.qsDrive;
	stTab.swWidgets.qsPath = stiTabInfo.qsPath;
	stTab.qsColumnSet = stiTabInfo.qsColumnSet;
	stTab.cfsFileSystem = cfcFileControl->GetFileSystem(stiTabInfo.qsDrive);

	/*// history
	stTab.shHistory.iPosition = stiTabInfo.shHistory.iPosition;
	for (iI = 0; iI < stiTabInfo.shHistory.qlLastPaths.count(); iI++) {
		sLastPath slpPanel;
		cSettings::sLastPath *slpSetting;

		slpSetting = &stiTabInfo.shHistory.qlLastPaths[iI];

		slpPanel.qsShow = slpSetting->qsShow;
		slpPanel.qsLocalDirectory = slpSetting->qsLocalDirectory;
		slpPanel.qsArchive = slpSetting->qsArchive;
		slpPanel.qsPathInArchive = slpSetting->qsPathInArchive;
		if (slpSetting->qsLocation == qsLOCAL_DIRECTORY) {
			slpPanel.elLocation = LocalDirectory;
		} else {
			slpPanel.elLocation = Archive;
		} // if else

		stTab.shHistory.qlLastPaths.append(slpPanel);
	} // for*/

	qlTabs.insert(iIndex, stTab);

	// add new tab into GUI
	qtbTab->blockSignals(true);
	qtbTab->insertTab(iIndex, QString());
	qtbTab->blockSignals(false);
	//SetTabText(iIndex);

	// set sorting
	ctwTree->header()->setSortIndicator(stiTabInfo.ssSort.iSortedColumn, stiTabInfo.ssSort.soSortOrder);

	// set header, refresh content only in first added tab
	RefreshHeader(iIndex, qlTabs.count() == 1);

	// connect signals to slots
	//connect(ctwTree->header(), SIGNAL(sectionClicked(int)), SLOT(on_qhvTreeHeader_sectionClicked(int)));

	HideOrShowTabBar();

	return iIndex;
} // AddTab

// constructor
cPanel::cPanel(QStackedWidget *qswDirs, QComboBox *qcbDrive, QLabel *qlDriveInfo, QTabBar *qtbTab, QLabel *qlPath, QLabel *qlSelected, cSettings *csSettings, cContentPlugin *ccpContentPlugin, QLabel *qlGlobalPath, QComboBox *qcbCommand, cFileControl *cfcFileControl, QLineEdit *qleQuickSearch)
{
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

	// automatic actualizations
	connect(&qtTimer, SIGNAL(timeout()), SLOT(on_qtTimer_timeout()));
	qtTimer.start(iTIMER_INTERVAL);
} // cPanel

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

// dir view got focus
const void cPanel::on_ctwTree_GotFocus()
{
	if (qswLastActive != qswDirs) {
		qswLastActive = qswDirs;
		qlGlobalPath->setText(qlTabs.at(qswDirs->currentIndex()).swWidgets.qsPath);
		emit GotFocus();
	} // if
} // on_ctwTree_GotFocus

// timer's action
const void cPanel::on_qtTimer_timeout() const
{
	ActualizeDrives();
} // on_qtTimer_timeout

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

// refresh dir content
const void cPanel::RefreshContent(const int &iIndex)
{
	// TODO RefreshContent
	cTreeWidget *ctwDir;
	int iI;
	QList<QTreeWidgetItem *> qlFiles;
	//QList<cContentPluginDelayed::sParameters> qlParameters;
	sTab *stTab;

	ctwDir = static_cast<cTreeWidget *>(qswDirs->widget(iIndex));
	stTab = &qlTabs[iIndex];

	qlFiles = qlTabs.at(iIndex).cfsFileSystem->GetDirectoryContent();

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
								QString qsNameWithExtension;

								qsNameWithExtension = stTab->cfsFileSystem->GetFileName(qtwiFile) + '.' + stTab->cfsFileSystem->GetFileExtension(qtwiFile);
								qtwiFile->setText(iJ, qsNameWithExtension);
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
			} /*else {
				// plugin
				if (elType == LocalDirectory) {
					// but only for local directory files
					int iFlag;

					qtwiFile->setText(iI, ccpContentPlugin->GetPluginValue(static_cast<const QFileInfo *>(vData)->filePath(), scColumn->qsPlugin, scColumn->qsIdentifier, qhTabs.value(iIndex).qlColumns->at(iI).qsUnit, &iFlag));
					if (iFlag == ft_delayed) {
						cContentPluginDelayed::sParameters spParameters;

						// thread input
						spParameters.siInput.qsFilename = static_cast<const QFileInfo *>(vData)->filePath();
						spParameters.siInput.qsPlugin = scColumn->qsPlugin;
						spParameters.siInput.qsColumn = scColumn->qsIdentifier;
						spParameters.siInput.qsUnit = scColumn->qsUnit;

						// thread output
						spParameters.soOutput.qtwiItem = qtwiFile;
						spParameters.soOutput.iColumn = iI;

						qlParameters->append(spParameters);
					} // if
				} // if
			} // if else*/
			/*if (scColumn->qsPlugin == qsNO) {
				// native
				if (scColumn->qsIdentifier == qsICON) {
					// icon
					switch (elType) {
						case LocalDirectory:
							qtwiFile->setIcon(iI, qfipIcon.icon(*static_cast<const QFileInfo *>(vData)));
							break;
						case Archive:
							if (static_cast<const tHeaderData *>(vData)->FileAttr & cPackerPlugin::iDIRECTORY) {
								qtwiFile->setIcon(iI, qfipIcon.icon(QFileIconProvider::Folder));
							} else {
								qtwiFile->setIcon(iI, qfipIcon.icon(QFileIconProvider::File));
							} // if else
					} // switch
				} else {
					if (scColumn->qsIdentifier == qsNAME) {
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
								if (static_cast<const tHeaderData *>(vData)->FileAttr & cPackerPlugin::iDIRECTORY && csSettings->GetShowBracketsAroundDirectoryName()) {
									qsName = '[' + qsName + ']';
								} // if
						} // switch
						qtwiFile->setText(iI, qsName);
					} else {
						if (scColumn->qsIdentifier == qsEXTENSION) {
							// extension
							switch (elType) {
								case LocalDirectory:
									qtwiFile->setText(iI, static_cast<const QFileInfo *>(vData)->suffix());
									break;
								case Archive:
									qtwiFile->setText(iI, QFileInfo(static_cast<const tHeaderData *>(vData)->FileName).suffix());
							} // switch
						} else {
							if (scColumn->qsIdentifier == qsNAME_WITH_EXTENSION) {
								// name with extension
								QString qsName;

								switch (elType) {
									case LocalDirectory:
										qsName = qsName = static_cast<const QFileInfo *>(vData)->fileName();
										if (static_cast<const QFileInfo *>(vData)->isDir() && csSettings->GetShowBracketsAroundDirectoryName()) {
											qsName = '[' + qsName + ']';
										} // if
										break;
									case Archive:
										qsName = QFileInfo(static_cast<const tHeaderData *>(vData)->FileName).fileName();
										if (static_cast<const tHeaderData *>(vData)->FileAttr & cPackerPlugin::iDIRECTORY && csSettings->GetShowBracketsAroundDirectoryName()) {
											qsName = '[' + qsName + ']';
										} // if
								} // switch
								qtwiFile->setText(iI, qsName);
							} else {
								if (scColumn->qsIdentifier == qsSIZE) {
									// size
									switch (elType) {
										case LocalDirectory:
											if (static_cast<const QFileInfo *>(vData)->isDir()) {
												qtwiFile->setText(iI, tr("<DIR>"));
											} else {
												qtwiFile->setText(iI, GetSizeString(static_cast<const QFileInfo *>(vData)->size()));
												qtwiFile->setData(iI, Qt::UserRole, static_cast<const QFileInfo *>(vData)->size());
											} // if else
											break;
										case Archive:
											if (static_cast<const tHeaderData *>(vData)->FileAttr & cPackerPlugin::iDIRECTORY) {
												qtwiFile->setText(iI, tr("<DIR>"));
											} else {
												qtwiFile->setText(iI, GetSizeString(static_cast<const tHeaderData *>(vData)->UnpSize));
												qtwiFile->setData(iI, Qt::UserRole, static_cast<const tHeaderData *>(vData)->UnpSize);
											} // if else
									} // switch
								} else {
									if (scColumn->qsIdentifier == qsDATE_TIME) {
										// date/time
										switch (elType) {
											case LocalDirectory:
												qtwiFile->setText(iI, DateTime(static_cast<const QFileInfo *>(vData)->lastModified()));
												qtwiFile->setData(iI, Qt::UserRole, static_cast<const QFileInfo *>(vData)->lastModified());
												break;
											case Archive:
												qtwiFile->setText(iI, DateTime(cArchiveOperation::ToQDateTime(static_cast<const tHeaderData *>(vData)->FileTime)));
												qtwiFile->setData(iI, Qt::UserRole, cArchiveOperation::ToQDateTime(static_cast<const tHeaderData *>(vData)->FileTime));
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
												if (static_cast<const tHeaderData *>(vData)->FileAttr & cPackerPlugin::iREAD_ONLY) {
													dwAttributes = FILE_ATTRIBUTE_READONLY;
												} else {
													dwAttributes = 0;
												} // if else
												if (static_cast<const tHeaderData *>(vData)->FileAttr & cPackerPlugin::iHIDDEN) {
													dwAttributes |= FILE_ATTRIBUTE_HIDDEN;
												} // if
												if (static_cast<const tHeaderData *>(vData)->FileAttr & cPackerPlugin::iSYSTEM) {
													dwAttributes |= FILE_ATTRIBUTE_SYSTEM;
												} // if
												if (static_cast<const tHeaderData *>(vData)->FileAttr & cPackerPlugin::iARCHIVE) {
													dwAttributes |= FILE_ATTRIBUTE_ARCHIVE;
												} // if
												break;
											default:
												dwAttributes = 0;
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
				} // if else
			} else {
			} // if else*/
		} // for
	} // for
	
	// sort and show files
	//Sort(iIndex);
	stTab->bValid = true;

	if (ctwDir->topLevelItemCount() > 0) {
		// focus to the first item
		ctwDir->setCurrentItem(ctwDir->topLevelItem(0));
	} // if

	/*if (qlParameters.count() > 0) {
		// start thread to query content plugins values
		ccpdContentPluginDelayed->Start(qlParameters);
	} // if*/

	//AddHistory(iIndex);
} // RefreshContent

// refresh column's header
const void cPanel::RefreshHeader(const int &iIndex, const bool &bContent /* false */)
{
	// TODO RefreshHeader
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

// show or hide header for specified tab
const void cPanel::ShowHideHeader(const int &iTabIndex) const
{
	static_cast<cTreeWidget *>(qswDirs->widget(iTabIndex))->header()->setVisible(csSettings->GetShowDirectoryViewHeader());
} // ShowHideHeader
