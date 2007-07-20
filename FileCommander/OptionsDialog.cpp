#include "OptionsDialog.h"

#include <QtGui/QFileDialog>
#include <QtGui/QAction>
#include "Plugins/ContPlug.h"
#include <QtGui/QSpinBox>
#include <QtGui/QHeaderView>
#include <QtGui/QKeyEvent>
#include <QtGui/QInputDialog>
#include <QtGui/QFontDialog>
#include <QtGui/QDirModel>

const QString qsASCII = "ASCII";
#ifdef Q_WS_WIN
const QString qsATTRIBUTES = "Attributes";
#endif
const QString qsBYTES = "Bytes";
const QString qsDATE_TIME = "DateTime";
const QString qsEXTENSION = "Extension";
const QString qsEXTERNAL = "External";
const QString qsFILES_AND_DIRECTORIES = "FilesAndDirectories";
const QString qsGIGABYTES = "Gigabytes";
const QString qsICON = "Icon";
const QString qsKILOBYTES = "Kilobytes";
const QString qsMEGABYTES = "Megabytes";
const QString qsNAME_WITH_EXTENSION = "NameWithExtension";
const QString qsNO = "no";
const QString qsNO_TO_ALL = "NoToAll";
const QString qsOVERWRITE_ALL = "OverwriteAll";
const QString qsOVERWRITE_ALL_OLDER = "OverwriteAllOlder";
const QString qsSIZE = "Size";
const QString qsSKIP_ALL = "SkipAll";
const QString qsVARIABLE_CHAR_WIDTH = "VariableCharWidth";
const QString qsYES_TO_ALL = "YesToAll";

const QString qsNATIVE = QT_TR_NOOP("native");
const QString qsNATIVE2 = QT_TR_NOOP("Native");
const QString qsPLUGINS = QT_TR_NOOP("Plugins");

// shortcuts
const QStringList qslSHORTCUTS__LISTER__FILE = QStringList() << qsSHORTCUT__LISTER__FILE__OPEN
																				 << qsSHORTCUT__LISTER__FILE__SAVE_AS
																				 << qsSHORTCUT__LISTER__FILE__PRINT
																				 << qsSHORTCUT__LISTER__FILE__PRINT_SETUP
																				 << qsSHORTCUT__LISTER__FILE__CLOSE;
const QStringList qslSHORTCUTS__LISTER__EDIT = QStringList() << qsSHORTCUT__LISTER__EDIT__COPY_AS_TEXT
																				 << qsSHORTCUT__LISTER__EDIT__SELECT_ALL
																				 << qsSHORTCUT__LISTER__EDIT__FIND
																				 << qsSHORTCUT__LISTER__EDIT__FIND_NEXT;
const QStringList qslSHORTCUTS__LISTER__OPTIONS = QStringList() << qsSHORTCUT__LISTER__OPTIONS__TEXT
																					 << qsSHORTCUT__LISTER__OPTIONS__BINARY
																					 << qsSHORTCUT__LISTER__OPTIONS__HEX
																					 << qsSHORTCUT__LISTER__OPTIONS__MULTIMEDIA
																					 << qsSHORTCUT__LISTER__OPTIONS__ANSI
																					 << qsSHORTCUT__LISTER__OPTIONS__ASCII
																					 << qsSHORTCUT__LISTER__OPTIONS__VARIABLE_CHAR_WIDTH
																					 << qsSHORTCUT__LISTER__OPTIONS__WRAP_TEXT
																					 << qsSHORTCUT__LISTER__OPTIONS__FIT_IMAGE_TO_WINDOW;
const QStringList qslSHORTCUTS__MAIN_MENU__FILE = QStringList() << qsSHORTCUT__MAIN_MENU__FILE__UNPACK_SELECTED_FILES
																					 << qsSHORTCUT__MAIN_MENU__FILE__QUIT;
const QStringList qslSHORTCUTS__MAIN_MENU__MARK = QStringList() << qsSHORTCUT__MAIN_MENU__MARK__SELECT_GROUP
																					 << qsSHORTCUT__MAIN_MENU__MARK__UNSELECT_GROUP
																					 << qsSHORTCUT__MAIN_MENU__MARK__SELECT_ALL
																					 << qsSHORTCUT__MAIN_MENU__MARK__UNSELECT_ALL
																					 << qsSHORTCUT__MAIN_MENU__MARK__INVERT_SELECTION;
const QStringList qslSHORTCUTS__MAIN_MENU__COMMANDS = QStringList() << qsSHORTCUT__MAIN_MENU__COMMANDS__SEARCH
																						  << qsSHORTCUT__MAIN_MENU__COMMANDS__FAVOURITE_DIRECTORIES
																						  << qsSHORTCUT__MAIN_MENU__COMMANDS__HISTORY_DIRECTORY_LIST
																						  << qsSHORTCUT__MAIN_MENU__COMMANDS__BRANCH_VIEW
																						  << qsSHORTCUT__MAIN_MENU__COMMANDS__FULLSCREEN;
const QStringList qslSHORTCUTS__MAIN_MENU__PANEL = QStringList() << qsSHORTCUT__MAIN_MENU__PANEL__SORT_BY_FIRST_COLUMN
																					  << qsSHORTCUT__MAIN_MENU__PANEL__SORT_BY_SECOND_COLUMN
																					  << qsSHORTCUT__MAIN_MENU__PANEL__SORT_BY_THIRD_COLUMN
																					  << qsSHORTCUT__MAIN_MENU__PANEL__SORT_BY_FOURTH_COLUMN
																					  << qsSHORTCUT__MAIN_MENU__PANEL__REVERSE_ORDER
																					  << qsSHORTCUT__MAIN_MENU__PANEL__REFRESH;
const QStringList qslSHORTCUTS__MAIN_MENU__CONFIGURATION = QStringList() << qsSHORTCUT__MAIN_MENU__CONFIGURATION__OPTIONS
																								 << qsSHORTCUT__MAIN_MENU__CONFIGURATION__SAVE_POSITION
																								 << qsSHORTCUT__MAIN_MENU__CONFIGURATION__SAVE_SETTINGS;
const QStringList qslSHORTCUTS__OPERATIONS__DIALOG = QStringList() << qsSHORTCUT__OPERATIONS__DIALOG__ENQUEUE;
const QStringList qslSHORTCUTS__PANELS__DIRECTORY_VIEW = QStringList() << qsSHORTCUT__PANELS__DIRECTORY_VIEW__DRIVE_LEFT
																							  << qsSHORTCUT__PANELS__DIRECTORY_VIEW__DRIVE_RIGHT
																							  << qsSHORTCUT__PANELS__DIRECTORY_VIEW__HISTORY_BACK
																							  << qsSHORTCUT__PANELS__DIRECTORY_VIEW__HISTORY_FRONT;
const QStringList qslSHORTCUTS__PANELS__MAIN_BUTTON = QStringList() << qsSHORTCUT__PANELS__MAIN_BUTTON__TERMINAL
																						  << qsSHORTCUT__PANELS__MAIN_BUTTON__VIEW
																						  << qsSHORTCUT__PANELS__MAIN_BUTTON__EDIT
																						  << qsSHORTCUT__PANELS__MAIN_BUTTON__COPY
																						  << qsSHORTCUT__PANELS__MAIN_BUTTON__MOVE
																						  << qsSHORTCUT__PANELS__MAIN_BUTTON__NEW_DIRECTORY
																						  << qsSHORTCUT__PANELS__MAIN_BUTTON__DELETE;
const QStringList qslSHORTCUTS__PANELS__TABS = QStringList() << qsSHORTCUT__PANELS__TABS__DUPLICATE_TAB
																				 << qsSHORTCUT__PANELS__TABS__CLOSE_TAB
																				 << qsSHORTCUT__PANELS__TABS__CLOSE_ALL_OTHER_TABS;

// destructor
cOptionsDialog::~cOptionsDialog()
{
	qmNative->deleteLater();
	qmPlugins->deleteLater();
	qagToolBarActions->deleteLater();
	qtbToolBar->deleteLater();
} // ~cOptionsDialog

// add new column to current column set
QTreeWidgetItem *cOptionsDialog::AddColumnToColumns(const cSettings::sColumn &scColumn, const int &iPos /* INT_MAX */) const
{
	QLineEdit *qleShow;
	QSpinBox *qsbWidth;
	QTreeWidgetItem *qtwiItem;

	if (iPos == INT_MAX) {
		qtwiItem = new QTreeWidgetItem(qtwColumns);
	} else {
		qtwiItem = new QTreeWidgetItem();
		qtwColumns->insertTopLevelItem(iPos, qtwiItem);
	} // if else

	// type
	if (scColumn.qsPlugin == qsNO) {
		qtwiItem->setText(iTYPE_COLUMN, qsNATIVE);
	} else {
		qtwiItem->setText(iTYPE_COLUMN, scColumn.qsPlugin);
	} // if else
	// name
	qtwiItem->setText(iNAME_COLUMN, scColumn.qsIdentifier);
	// unit
	qtwiItem->setText(iUNIT_COLUMN, scColumn.qsUnit);
	// show
	qleShow = new QLineEdit(qtwColumns);
	qleShow->setText(scColumn.qsName);
	qtwColumns->setItemWidget(qtwiItem, iSHOW_COLUMN, qleShow);
	connect(qleShow, SIGNAL(textEdited(const QString &)), SLOT(on_qleShow_textEdited(const QString &)));
	// width
	qsbWidth = new QSpinBox(qtwColumns);
	qsbWidth->setValue(scColumn.iWidth);
	qtwColumns->setItemWidget(qtwiItem, iWIDTH_COLUMN, qsbWidth);
	connect(qsbWidth, SIGNAL(valueChanged(int)), SLOT(on_qsbWidth_valueChanged(int)));

	qtwColumns->resizeColumnToContents(iTYPE_COLUMN);
	qtwColumns->resizeColumnToContents(iUNIT_COLUMN);
	qtwColumns->resizeColumnToContents(iWIDTH_COLUMN);

	return qtwiItem;
} // AddColumnToColumns

// add another plugin into tree
const void cOptionsDialog::AddPluginIntoTree(const cSettings::sPlugin &spPlugin, QTreeWidget *qtwTree) const
{
	int iEnabledColumn;
	QTreeWidgetItem *qtwiItem;

	qtwTree->blockSignals(true);

	qtwiItem = new QTreeWidgetItem(qtwTree);
	// name
	qtwiItem->setText(iPLUGIN_NAME_COLUMN, spPlugin.qsName);
	// extensions for packer
	if (qtwTree == qtwPackerPlugins) {
		QLineEdit *qleExtensions;

		qleExtensions = new QLineEdit(qtwPackerPlugins);
		qtwPackerPlugins->setItemWidget(qtwiItem, iPLUGIN_EXTENSIONS_COLUMN, qleExtensions);
		qleExtensions->setText(spPlugin.qsExtensions);
	} // if
	// enable/disable flag
	if (qtwTree == qtwPackerPlugins) {
		iEnabledColumn = iPLUGIN_ENABLED_COLUMN + 1;
	} else {
		iEnabledColumn = iPLUGIN_ENABLED_COLUMN;
	} // if else
	if (spPlugin.bEnabled) {
		qtwiItem->setCheckState(iEnabledColumn, Qt::Checked);
	} else {
		qtwiItem->setCheckState(iEnabledColumn, Qt::Unchecked);
	} // if else

	qtwTree->resizeColumnToContents(iPLUGIN_NAME_COLUMN);

	qtwTree->blockSignals(false);
} // AddPluginIntoTree

// constructor
cOptionsDialog::cOptionsDialog(QWidget *qmwParent, cSettings *csSettings, cContentPlugin *ccpContentPlugin)
{
	setParent(qmwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);

	this->csSettings = csSettings;
	this->ccpContentPlugin = ccpContentPlugin;

	qpbColumnAdd->setMenu(&qmColumns);

	// remember original options
	qlOldOptions = csSettings->GetAllSettings();

	CreateToolBar();
	PrepareColumnsMenu();
	FillOptions();

	// connections
	connect(&qmColumns, SIGNAL(triggered(QAction *)), SLOT(on_qmColumns_triggered(QAction *)));

	qleShortcut->installEventFilter(this);

	// hide unrelated options
#ifndef Q_WS_WIN
	qcbDeleteToRecycleBin->deleteLater();
#endif

	// completers
	qcDirModel.setModel(new QDirModel(&qcDirModel));
	qleExternalViewer->setCompleter(&qcDirModel);
	qleExternalEditor->setCompleter(&qcDirModel);
	qleFavouriteSource->setCompleter(&qcDirModel);
	qleFavouriteTarget->setCompleter(&qcDirModel);
} // cConfigurationDialog

// create left toolbar for navigation
const void cOptionsDialog::CreateToolBar()
{
	// create toolbar
	qtbToolBar = new QToolBar();
	qtbToolBar->setOrientation(Qt::Vertical);
	qtbToolBar->setSizePolicy(QSizePolicy::Preferred, qtbToolBar->sizePolicy().verticalPolicy());
	static_cast<QHBoxLayout *>(layout())->insertWidget(0, qtbToolBar);

	// add actions
	qagToolBarActions = new QActionGroup(qtbToolBar);
	qaPanels = qtbToolBar->addAction(tr("Panels"));
	qaPanels->setCheckable(true);
	qagToolBarActions->addAction(qaPanels);
	qaOperations = qtbToolBar->addAction(tr("Operations"));
	qaOperations->setCheckable(true);
	qagToolBarActions->addAction(qaOperations);
	qaPlugins = qtbToolBar->addAction(tr("Plugins"));
	qaPlugins->setCheckable(true);
	qagToolBarActions->addAction(qaPlugins);
	qaOthers = qtbToolBar->addAction(tr("Others"));
	qaOthers->setCheckable(true);
	qagToolBarActions->addAction(qaOthers);

	// check first action as default
	qaPanels->setChecked(true);

	// connect
	connect(qagToolBarActions, SIGNAL(triggered(QAction *)), SLOT(on_qagToolBarActions_triggered(QAction *)));
} // CreateToolBar

// event filter
bool cOptionsDialog::eventFilter(QObject *watched, QEvent *event)
{
	if (watched == qleShortcut) {
		if (event->type() == QEvent::KeyPress) {
			QKeyEvent *qkeKey;
			QString qsKey;

			// get shortcut
			qkeKey = static_cast<QKeyEvent *>(event);
			if (qkeKey->modifiers() & Qt::ControlModifier) {
				qsKey = "Ctrl";
			} // if
			if (qkeKey->modifiers() & Qt::AltModifier) {
				if (!qsKey.isEmpty()) {
					qsKey += "+";
				} // if
				qsKey += "Alt";
			} // if
			if (qkeKey->modifiers() & Qt::ShiftModifier) {
				if (!qsKey.isEmpty()) {
					qsKey += "+";
				} // if
				qsKey += "Shift";
			} // if
			if (!qsKey.isEmpty()) {
				qsKey += "+";
			} // if
			/*if (qkeKey->modifiers() & Qt::KeypadModifier) {
				qsKey += "Num ";
			} // if*/

			if (QKeySequence(qkeKey->key()).toString().at(0) != 0x17C0) {
				// not only Ctrl, Alt or Shift keys pressed
				qsKey += QKeySequence(qkeKey->key()).toString();
				qleShortcut->setText(qsKey);
			} // if
				
			return true;
		} else {
			return false;
		} // if else
	} else {
		return QDialog::eventFilter(watched, event);
	} // if else
} // eventFilter

// add new favourite directory/submenu
const void cOptionsDialog::FavouriteAdd(const cNewFavouriteDirectoryDialog::eType &cnfdType)
{
	bool bInSubmenu;
	cNewFavouriteDirectoryDialog *cnfdFavourite;
	cNewFavouriteDirectoryDialog::ePosition epPosition;
	QString qsDirectory;

	// settings for new favourite directory dialog
	if (qtwFavouriteDirectories->currentItem()) {
		epPosition = cNewFavouriteDirectoryDialog::After;
		if (qtwFavouriteDirectories->currentItem()->type() == cNewFavouriteDirectoryDialog::Directory) {
			bInSubmenu = false;
		} else {
			bInSubmenu = true;
		} // if else
	} else {
		epPosition = cNewFavouriteDirectoryDialog::Disabled;
	} // if else

	cnfdFavourite = new cNewFavouriteDirectoryDialog(this, cnfdType, &qsDirectory, &epPosition, &bInSubmenu);
	if (cnfdFavourite->exec() == QDialog::Accepted) {
		QTreeWidgetItem *qtwiFavourite;
		sFavouriteDirectory sfdFavourite;

		if (epPosition == cNewFavouriteDirectoryDialog::Disabled) {
			// insert first item
			qtwiFavourite = new QTreeWidgetItem(qtwFavouriteDirectories, cnfdType);
		} else {
			// insert another item
			QTreeWidgetItem *qtwiParent;

			qtwiFavourite = new QTreeWidgetItem(cnfdType);

			if (bInSubmenu) {
				// in submenu
				qtwiParent = qtwFavouriteDirectories->currentItem();
				if (epPosition == cNewFavouriteDirectoryDialog::Before) {
					qtwiParent->insertChild(0, qtwiFavourite);
				} else {
					qtwiParent->addChild(qtwiFavourite);
				} // if else
			} else {
				// next to existing item
				qtwiParent = qtwFavouriteDirectories->currentItem()->parent();
				if (!qtwiParent) {
					qtwiParent = qtwFavouriteDirectories->invisibleRootItem();
				} // if
				if (epPosition == cNewFavouriteDirectoryDialog::Before) {
					qtwiParent->insertChild(qtwiParent->indexOfChild(qtwFavouriteDirectories->currentItem()), qtwiFavourite);
				} else {
					qtwiParent->insertChild(qtwiParent->indexOfChild(qtwFavouriteDirectories->currentItem()) + 1, qtwiFavourite);
				} // if else
			} // if else
		} // if else

		// set text for item
		qtwiFavourite->setText(0, qsDirectory);

		// add to favourite directory table
		if (cnfdType == cNewFavouriteDirectoryDialog::Directory) {
			sfdFavourite.bTarget = false;
			qhFavouriteDirectories.insert(qtwiFavourite, sfdFavourite);
		} // if

		// set cursor to new favourite
		qtwFavouriteDirectories->setCurrentItem(qtwiFavourite);

		qfToDo |= RefreshFavouriteDirectories;
	} // if

	cnfdFavourite->deleteLater();
} // FavouriteAdd

// fill options with set settings
const void cOptionsDialog::FillOptions()
{
	cSettings::sLister slLister;
	QString qsValue;
	QTreeWidgetItem *qtwiShortcutMain;

	// panels
	// display
	qcbShowSystemFiles->blockSignals(true);
	qcbShowSystemFiles->setChecked(csSettings->GetShowSystemFiles());
	qcbShowSystemFiles->blockSignals(false);
	qcbShowHiddenFiles->blockSignals(true);
	qcbShowHiddenFiles->setChecked(csSettings->GetShowHiddenFiles());
	qcbShowHiddenFiles->blockSignals(false);
	qsValue = csSettings->GetFileSizeIn();
	if (qsValue == qsBYTES) {
		qrbSizeBytes->blockSignals(true);
		qrbSizeBytes->setChecked(true);
		qrbSizeBytes->blockSignals(false);
	} else {
		if (qsValue == qsKILOBYTES) {
			qrbSizeKilobytes->blockSignals(true);
			qrbSizeKilobytes->setChecked(true);
			qrbSizeKilobytes->blockSignals(false);
		} else {
			if (qsValue == qsMEGABYTES) {
				qrbSizeMegabytes->blockSignals(true);
				qrbSizeMegabytes->setChecked(true);
				qrbSizeMegabytes->blockSignals(false);
			} else {
				if (qsValue == qsGIGABYTES) {
					qrbSizeGigabytes->blockSignals(true);
					qrbSizeGigabytes->setChecked(true);
					qrbSizeGigabytes->blockSignals(false);
				} else {
					qrbSizeDynamic->blockSignals(true);
					qrbSizeDynamic->setChecked(true);
					qrbSizeDynamic->blockSignals(false);
				} // if else
			} // if else
		} // if else
	} // if else
	qcbSquareBracketsAroundDirectoryName->blockSignals(true);
	qcbSquareBracketsAroundDirectoryName->setChecked(csSettings->GetShowBracketsAroundDirectoryName());
	qcbSquareBracketsAroundDirectoryName->blockSignals(false);
	qsValue = csSettings->GetSelectionMode();
	if (qsValue == qsONLY_FILES) {
		qrbSelectionOnlyFiles->setChecked(true);
	} else {
		qrbSelectionFilesAndDirectories->setChecked(true);
	} // if else
	qcbSaveSettingsOnQuit->setChecked(csSettings->GetSaveSettingsOnQuit());
	qleDateTimeDisplay->setText(csSettings->GetDateTimeDisplay());
	qcbCaseSensitiveSorting->setChecked(csSettings->GetCaseSensitiveSorting());
	qcbShowDirectoryViewHeader->setChecked(csSettings->GetShowDirectoryViewHeader());
	// tabs
	qcbConfirmCloseOfAllOtherTabs->setChecked(csSettings->GetConfirmCloseOfAllTabs());
	qcbShowTabBarWithOnlyOneTab->blockSignals(true);
	qcbShowTabBarWithOnlyOneTab->setChecked(csSettings->GetShowTabBarWithOnlyOneTab());
	qcbShowTabBarWithOnlyOneTab->blockSignals(false);
	qcbCloseTabOnDoubleClick->setChecked(csSettings->GetCloseTabOnDoubleClick());
	qcbNewTabByShortcutInForeground->setChecked(csSettings->GetNewTabByShortcutInForeground());
	qcbOpenNewTabNextToCurrentTab->setChecked(csSettings->GetOpenNewTabNextToCurrentTab());
	qcbShowDriveLetter->blockSignals(true);
	qcbShowDriveLetter->setChecked(csSettings->GetShowDriveLetter());
	qcbShowDriveLetter->blockSignals(false);
	// column sets
	qtwColumns->setHeaderLabels(QStringList() << tr("Type") << tr("Name") << tr("Unit") << tr("Show") << tr("Width"));
	qtwColumns->header()->setResizeMode(iNAME_COLUMN, QHeaderView::Stretch);
	qtwColumns->header()->setResizeMode(iSHOW_COLUMN, QHeaderView::Stretch);
	qtwColumns->header()->setStretchLastSection(false);
	qcbColumnSet->addItems(csSettings->GetColumnSets());
	// quick search
	qgbQuickSearch->setChecked(csSettings->GetQuickSearchEnabled());
	qcbQuickSearchCtrl->setChecked(csSettings->GetQuickSearchCtrl());
	qcbQuickSearchAlt->setChecked(csSettings->GetQuickSearchAlt());
	qcbQuickSearchShift->setChecked(csSettings->GetQuickSearchShift());
	qcbQuickSearchShowWindow->setChecked(csSettings->GetQuickSearchShowSearchWindow());

	// operations
	// viewer
	if (csSettings->GetViewerType() == qsINTERNAL) {
		qrbViewerInternal->setChecked(true);
	} else {
		qrbViewerExternal->setChecked(true);
	} // if else
	qleExternalViewer->setText(csSettings->GetExternalViewer());
	// editor
	qleExternalEditor->setText(csSettings->GetExternalEditor());
	// copy/move
	qsbCopyMoveBufferSize->setValue(csSettings->GetCopyMoveBufferSize());
	// delete
	qcbAskToDeleteNonEmptyDirectory->setChecked(csSettings->GetAskToDeleteNonEmptyDirectory());
#ifdef Q_WS_WIN
	qcbDeleteToRecycleBin->setChecked(csSettings->GetDeleteToRecycleBin());
#endif
	// confirmation
	qsValue = csSettings->GetFileOverwrite();
	if (qsValue == qsASK) {
		qrbOverwriteAsk->setChecked(true);
	} else {
		if (qsValue == qsOVERWRITE_ALL) {
			qrbOverwriteAll->setChecked(true);
		} else {
			if (qsValue == qsOVERWRITE_ALL_OLDER) {
				qrbOverwriteAllOlder->setChecked(true);
			} else {
				qrbOverwriteSkipAll->setChecked(true);
			} // if else
		} // if else
	} // if else
	qsValue = csSettings->GetReadonlyFileOverwrite();
	if (qsValue == qsASK) {
		qrbReadonlyOverwriteAsk->setChecked(true);
	} else {
		if (qsValue == qsYES_TO_ALL) {
			qrbReadonlyOverwriteYesToAll->setChecked(true);
		} else {
			qrbReadonlyOverwriteNoToAll->setChecked(true);
		} // if else
	} // if else

	// plugins
	// content
	qlePluginDateTimeDisplay->blockSignals(true);
	qlePluginDateTimeDisplay->setText(csSettings->GetPluginDateTimeDisplay());
	qlePluginDateTimeDisplay->blockSignals(false);
	qlePluginTimeDisplay->blockSignals(true);
	qlePluginTimeDisplay->setText(csSettings->GetPluginTimeDisplay());
	qlePluginTimeDisplay->blockSignals(false);
	FillPluginsTree(csSettings->GetPlugins(cSettings::ContentPlugins), qtwContentPlugins);
	// lister
	FillPluginsTree(csSettings->GetPlugins(cSettings::ListerPlugins), qtwListerPlugins);
	slLister = csSettings->GetListerSettings();
	if (slLister.qsCharSet == qsANSI) {
		qrbListerANSI->setChecked(true);
	} else {
		if (slLister.qsCharSet == qsASCII) {
			qrbListerASCII->setChecked(true);
		} else {
			qrbListerVariableCharWidth->setChecked(true);
		} // if else
	} // if else
	qcbListerWrapText->setChecked(slLister.bWrapText);
	qcbListerFitImageToWindow->setChecked(slLister.bFitImageToWindow);
	qfListerFont = csSettings->GetListerFont();
	// packer
	FillPluginsTree(csSettings->GetPlugins(cSettings::PackerPlugins), qtwPackerPlugins);
	qcbTreatArchivesLikeDirectories->setChecked(csSettings->GetTreatArchivesLikeDirectories());

	// others
	// favourite directories
	qtwFavouriteDirectories->header()->hide();
	SetFavouriteDirectories(qtwFavouriteDirectories->invisibleRootItem(), csSettings->GetFavouriteDirectories());
	qtwFavouriteDirectories->expandAll();
	// shortcuts
	qtwShortcutCategory->header()->hide();
	qtwiShortcutMain = new QTreeWidgetItem(qtwShortcutCategory);
	qtwiShortcutMain->setText(0, tr("Main menu"));
	qtwiShortcutFile = new QTreeWidgetItem(qtwiShortcutMain);
	qtwiShortcutFile->setText(0, tr("File"));
	qtwiShortcutFile->setData(0, Qt::UserRole, cSettings::MainMenuCategory);
	qtwiShortcutMark = new QTreeWidgetItem(qtwiShortcutMain);
	qtwiShortcutMark->setText(0, tr("Mark"));
	qtwiShortcutMark->setData(0, Qt::UserRole, cSettings::MainMenuCategory);
	qtwiShortcutCommands = new QTreeWidgetItem(qtwiShortcutMain);
	qtwiShortcutCommands->setText(0, tr("Commands"));
	qtwiShortcutCommands->setData(0, Qt::UserRole, cSettings::MainMenuCategory);
	qtwiShortcutPanel = new QTreeWidgetItem(qtwiShortcutMain);
	qtwiShortcutPanel->setText(0, tr("Panel"));
	qtwiShortcutPanel->setData(0, Qt::UserRole, cSettings::MainMenuCategory);
	qtwiShortcutConfiguration = new QTreeWidgetItem(qtwiShortcutMain);
	qtwiShortcutConfiguration->setText(0, tr("Configuration"));
	qtwiShortcutConfiguration->setData(0, Qt::UserRole, cSettings::MainMenuCategory);
	qtwiShortcutMain = new QTreeWidgetItem(qtwShortcutCategory);
	qtwiShortcutMain->setText(0, tr("Panels"));
	qtwiShortcutTabs = new QTreeWidgetItem(qtwiShortcutMain);
	qtwiShortcutTabs->setText(0, tr("Tabs"));
	qtwiShortcutTabs->setData(0, Qt::UserRole, cSettings::PanelsCategory);
	qtwiShortcutDirectoryView = new QTreeWidgetItem(qtwiShortcutMain);
	qtwiShortcutDirectoryView->setText(0, tr("Directory view"));
	qtwiShortcutDirectoryView->setData(0, Qt::UserRole, cSettings::PanelsCategory);
	qtwiShortcutMainButtons = new QTreeWidgetItem(qtwiShortcutMain);
	qtwiShortcutMainButtons->setText(0, tr("Main buttons"));
	qtwiShortcutMainButtons->setData(0, Qt::UserRole, cSettings::PanelsCategory);
	qtwiShortcutMain = new QTreeWidgetItem(qtwShortcutCategory);
	qtwiShortcutMain->setText(0, tr("Operations"));
	qtwiShortcutOperationsDialog = new QTreeWidgetItem(qtwiShortcutMain);
	qtwiShortcutOperationsDialog->setText(0, tr("Dialog"));
	qtwiShortcutOperationsDialog->setData(0, Qt::UserRole, cSettings::OperationsCategory);
	qtwiShortcutMain = new QTreeWidgetItem(qtwShortcutCategory);
	qtwiShortcutMain->setText(0, tr("Lister"));
	qtwiShortcutListerFile = new QTreeWidgetItem(qtwiShortcutMain);
	qtwiShortcutListerFile->setText(0, tr("File"));
	qtwiShortcutListerFile->setData(0, Qt::UserRole, cSettings::ListerCategory);
	qtwiShortcutListerEdit = new QTreeWidgetItem(qtwiShortcutMain);
	qtwiShortcutListerEdit->setText(0, tr("Edit"));
	qtwiShortcutListerEdit->setData(0, Qt::UserRole, cSettings::ListerCategory);
	qtwiShortcutListerOptions = new QTreeWidgetItem(qtwiShortcutMain);
	qtwiShortcutListerOptions->setText(0, tr("Options"));
	qtwiShortcutListerOptions->setData(0, Qt::UserRole, cSettings::ListerCategory);
	qtwShortcutCategory->expandAll();
	qtwShortcutItem->setHeaderLabels(QStringList() << tr("Item") << tr("Shortcut"));
	// miscellaneous
	qsbMaximumHistoryDirectoryListSize->setValue(csSettings->GetMaximumHistoryDirectoryListSize());
} // FillOptions

// fills plugin information into tree
const void cOptionsDialog::FillPluginsTree(const QList<cSettings::sPlugin> &qlPlugins, QTreeWidget *qtwTree) const
{
	int iI;
	QStringList qslHeader;

	qslHeader.append(tr("File path"));
	if (qtwTree == qtwPackerPlugins) {
		qslHeader.append(tr("Extensions"));
	} // if
	qslHeader.append(tr("Enabled"));
	qtwTree->setHeaderLabels(qslHeader);

	for (iI = 0; iI < qlPlugins.count(); iI++) {
		AddPluginIntoTree(qlPlugins.at(iI), qtwTree);
	} // for
} // FillPluginsTree

// fill shortcuts for selected category
const void cOptionsDialog::FillShortcutItems(const cSettings::eShortcutCategory &escCategory, const QStringList &qslItems) const
{
	int iI;

	qtwShortcutItem->clear();

	for (iI = 0; iI < qslItems.count(); iI++) {
		QTreeWidgetItem *qtwShortcut;
		QString *qsItem;

		qsItem = &const_cast<QString &>(qslItems[iI]);

		qtwShortcut = new QTreeWidgetItem(qtwShortcutItem);
		qtwShortcut->setText(0, *qsItem);
		qtwShortcut->setText(1, csSettings->GetShortcut(escCategory, *qsItem));
	} // for

	qtwShortcutItem->resizeColumnToContents(0);
} // FillShortcutItems

// get information about column from column set
const cSettings::sColumn cOptionsDialog::GetColumnInfo(QTreeWidgetItem *qtwiItem) const
{
	cSettings::sColumn scColumn;

	// type
	if (qtwiItem->text(iTYPE_COLUMN) == qsNATIVE) {
		scColumn.qsPlugin = qsNO;
	} else {
		scColumn.qsPlugin = qtwiItem->text(iTYPE_COLUMN);
	} // if else
	// name
	scColumn.qsIdentifier = qtwiItem->text(iNAME_COLUMN);
	// unit
	scColumn.qsUnit = qtwiItem->text(iUNIT_COLUMN);
	// show
	scColumn.qsName = static_cast<QLineEdit *>(qtwColumns->itemWidget(qtwiItem, iSHOW_COLUMN))->text();
	// width
	scColumn.iWidth = static_cast<QSpinBox *>(qtwColumns->itemWidget(qtwiItem, iWIDTH_COLUMN))->value();

	return scColumn;
} // GetColumnInfo

// collect favourite directories
QList<QPair<QString, cSettings::sFavouriteDirectory> > cOptionsDialog::GetFavouriteDirectories(QTreeWidgetItem *qtwiParent)
{
	int iI;
	QList<QPair<QString, cSettings::sFavouriteDirectory> > qlFavouriteDirectories;
	QList<QTreeWidgetItem *> qlChildren;

	qlChildren = qtwiParent->takeChildren();

	for (iI = 0; iI < qlChildren.count(); iI++) {
		cSettings::sFavouriteDirectory sfdFavouriteDirectory;
		QTreeWidgetItem *qtwiChild;

		qtwiChild = qlChildren[iI];

		if (qtwiChild->type() == cNewFavouriteDirectoryDialog::Directory) {
			sFavouriteDirectory *sfdCurrent;

			sfdCurrent = &qhFavouriteDirectories[qtwiChild];

			sfdFavouriteDirectory.qsSource = sfdCurrent->qsSource;
			sfdFavouriteDirectory.bTarget = sfdCurrent->bTarget;
			sfdFavouriteDirectory.qsTarget = sfdCurrent->qsTarget;
			sfdFavouriteDirectory.bSubmenu = false;
		} else {
			sfdFavouriteDirectory.bSubmenu = true;
			sfdFavouriteDirectory.qlChildFavourites = GetFavouriteDirectories(qtwiChild);
		} // if else

		qlFavouriteDirectories.append(QPair<QString, cSettings::sFavouriteDirectory>(qtwiChild->text(0), sfdFavouriteDirectory));
	} // for

	return qlFavouriteDirectories;
} // GetFavouriteDirectories

// get info about specified plugins
QList<cSettings::sPlugin> cOptionsDialog::GetPluginList(const QTreeWidget *qtwPlugins) const
{
	int iI;
	QList<cSettings::sPlugin> qlPlugins;

	for (iI = 0; iI < qtwPlugins->topLevelItemCount(); iI++) {
		cSettings::sPlugin spPlugin;
		int iEnabledColumn;

		// name
		spPlugin.qsName = qtwPlugins->topLevelItem(iI)->text(iPLUGIN_NAME_COLUMN);
		// extensions
		if (qtwPlugins == qtwPackerPlugins) {
			spPlugin.qsExtensions = static_cast<QLineEdit *>(qtwPlugins->itemWidget(qtwPlugins->topLevelItem(iI), iPLUGIN_EXTENSIONS_COLUMN))->text();
		} // if
		// enabled
		if (qtwPlugins == qtwPackerPlugins) {
			iEnabledColumn = iPLUGIN_ENABLED_COLUMN + 1;
		} else {
			iEnabledColumn = iPLUGIN_ENABLED_COLUMN;
		} // if else
		if (qtwPlugins->topLevelItem(iI)->checkState(iEnabledColumn) == Qt::Checked) {
			spPlugin.bEnabled = true;
		} else {
			spPlugin.bEnabled = false;
		} // if else

		qlPlugins.append(spPlugin);
	} // for

	return qlPlugins;
} // GetPluginList

// clicked on action in tool bar panel
const void cOptionsDialog::on_qagToolBarActions_triggered(QAction *qaAction) const
{
	if (qaAction == qaPanels) {
		qswTabs->setCurrentIndex(iPANELS_TAB);
	} else {
		if (qaAction == qaOperations) {
			qswTabs->setCurrentIndex(iOPERATIONS_TAB);
		} else {
			if (qaAction == qaPlugins) {
				qswTabs->setCurrentIndex(iPLUGINS_TAB);
			} else {
				qswTabs->setCurrentIndex(iOTHERS_TAB);
			} // if else
		} // if else
	} // if else
} // on_qagToolBarActions_triggered

// column set changed
const void cOptionsDialog::on_qcbColumnSet_currentIndexChanged(const QString &text) const
{
	QStringList qslColumns;

	qslColumns = csSettings->GetColumnsInSet(text);

	qtwColumns->clear();

	if (text.isEmpty()) {
		qpbColumnSetRemove->setEnabled(false);
	} else {
		int iI;

		for (iI = 0; iI < qslColumns.count(); iI++) {
			cSettings::sColumn scColumn;

			scColumn = csSettings->GetColumnInfo(text, qslColumns.at(iI));
			AddColumnToColumns(scColumn);
		} // for

		qpbColumnSetRemove->setEnabled(true);
	} // if else
} // on_qcbColumnSet_currentIndexChanged

// set target favourite directory too
const void cOptionsDialog::on_qcbFavouriteTargetDirectory_stateChanged(int state)
{
	qleFavouriteTarget->setEnabled(state == Qt::Checked);
	qpbFavouriteTargetBrowse->setEnabled(state == Qt::Checked);

	// write change to favourite directories table
	if (qtwFavouriteDirectories->currentItem() && qtwFavouriteDirectories->currentItem()->type() == cNewFavouriteDirectoryDialog::Directory) {
		qhFavouriteDirectories[qtwFavouriteDirectories->currentItem()].bTarget = state == Qt::Checked;
	} // if
} // on_qcbFavouriteTargetDirectory_toggled

// change of show directory header view
const void cOptionsDialog::on_qcbShowDirectoryViewHeader_stateChanged(int state)
{
	qfToDo |= ShowHideDirectoryViewHeader;
} // on_qcbShowDirectoryViewHeader_stateChanged

// change of show drive letter in tab bar
const void cOptionsDialog::on_qcbShowDriveLetter_stateChanged(int state)
{
	qfToDo |= RefreshTabs;
} // on_qcbShowDriveLetter_stateChanged

// change of show hidden files
const void cOptionsDialog::on_qcbShowHiddenFiles_stateChanged(int state)
{
	qfToDo |= RefreshContent;
} // on_qcbShowHiddenFiles_stateChanged

// change of show system files
const void cOptionsDialog::on_qcbShowSystemFiles_stateChanged(int state)
{
	qfToDo |= RefreshContent;
} // on_qcbShowSystemFiles_stateChanged

// change of show tab bar with only one tab
const void cOptionsDialog::on_qcbShowTabBarWithOnlyOneTab_stateChanged(int state)
{
	qfToDo |= RefreshTabs;
} // on_qcbShowTabBarWithOnlyOneTab_stateChanged

// change of show square brackets around directory name
const void cOptionsDialog::on_qcbSquareBracketsAroundDirectoryName_stateChanged(int state)
{
	qfToDo |= RefreshContent;
} // on_qcbSquareBracketsAroundDirectoryName_stateChanged

// changes accepted
const void cOptionsDialog::on_qdbbResponse_accepted()
{
	SaveOptions();
	done(qfToDo);
} // on_qdbbResponse_accepted

const void cOptionsDialog::on_qdbbResponse_rejected()
{
	csSettings->RestoreSettings(qlOldOptions);
	done(Nothing);
} // on_qdbbResponse_rejected

// date/time format changed
const void cOptionsDialog::on_qleDateTimeDisplay_textEdited(const QString &text)
{
	qfToDo |= RefreshContent;
} // on_qleDateTimeDisplay_textEdited

// source favourite directory path changed
const void cOptionsDialog::on_qleFavouriteSource_textChanged(const QString &text)
{
	if (qtwFavouriteDirectories->currentItem() && qtwFavouriteDirectories->currentItem()->type() == cNewFavouriteDirectoryDialog::Directory) {
		qhFavouriteDirectories[qtwFavouriteDirectories->currentItem()].qsSource = text;

		qfToDo |= RefreshFavouriteDirectories;
	} // if
} // on_qleFavouriteSource_textChanged

// target favourite directory path changed
const void cOptionsDialog::on_qleFavouriteTarget_textChanged(const QString &text)
{
	if (qtwFavouriteDirectories->currentItem() && qtwFavouriteDirectories->currentItem()->type() == cNewFavouriteDirectoryDialog::Directory) {
		qhFavouriteDirectories[qtwFavouriteDirectories->currentItem()].qsTarget = text;

		qfToDo |= RefreshFavouriteDirectories;
	} // if
} // on_qleFavouriteTarget_textChanged

// date/time format for plugin changed
const void cOptionsDialog::on_qlePluginDateTimeDisplay_textEdited(const QString &text)
{
	qfToDo |= RefreshContent;
} // on_qlePluginDateTimeDisplay_textEdited

// time format for plugin changed
const void cOptionsDialog::on_qlePluginTimeDisplay_textEdited(const QString &text)
{
	qfToDo |= RefreshContent;
} // on_qlePluginTimeDisplay_textEdited

// shortcut changed
const void cOptionsDialog::on_qleShortcut_textChanged(const QString &text)
{
	qtwShortcutItem->currentItem()->setText(1, text);
	csSettings->SetShortcut(static_cast<cSettings::eShortcutCategory>(qtwShortcutCategory->currentItem()->data(0, Qt::UserRole).toInt()), qtwShortcutItem->currentItem()->text(0), text);

	qfToDo |= ReassignShortcuts;
} // on_qleShortcut_textEdited

// changed column name visible in dir view
const void cOptionsDialog::on_qleShow_textEdited(const QString &text)
{
	SaveOption(Columns);

	qfToDo |= RefreshHeader;
} // on_qleShow_textEdited

// column selected into column set
const void cOptionsDialog::on_qmColumns_triggered(QAction *action)
{
	cSettings::sColumn scColumn;
	QStringList qslSelection;
	QTreeWidgetItem *qtwiNewColumn;

	qslSelection = action->data().toString().split('|');

	if (qslSelection.at(0) == qsNATIVE2) {
		// native column
		// type
		scColumn.qsPlugin = qsNO;
		// name
		scColumn.qsIdentifier = qslSelection.at(1);
		// unit
		// show
		scColumn.qsName = qslSelection.at(1);
	} else {
		// plugin column
		// type
		scColumn.qsPlugin = qslSelection.at(1);
		// name
		scColumn.qsIdentifier = qslSelection.at(2);
		// unit
		if (qslSelection.count() > 3) {
			scColumn.qsUnit = qslSelection.at(3);
		} // if
		// show
		scColumn.qsName = qslSelection.at(2);
	} // if else

	if (qtwColumns->currentItem()) {
		qtwiNewColumn = AddColumnToColumns(scColumn, qtwColumns->indexOfTopLevelItem(qtwColumns->currentItem()));
	} else {
		qtwiNewColumn = AddColumnToColumns(scColumn);
	} // if else
	qtwColumns->setCurrentItem(qtwiNewColumn);

	SaveOption(Columns);

	qfToDo |= RefreshHeader;
} // on_qmColumns_triggered

// add button is clicked on in content plugins
const void cOptionsDialog::on_qpbAddContentPlugin_clicked(bool checked /* false */)
{
	QString qsFile;

	qsFile = QFileDialog::getOpenFileName(this, tr("Select content plugin"), "/", "*.wdx");

	if (!qsFile.isEmpty()) {
		cSettings::sPlugin spPlugin;

		spPlugin.qsName = qsFile;
		spPlugin.bEnabled = true;
		AddPluginIntoTree(spPlugin, qtwContentPlugins);

		qfToDo |= ReloadPlugins;
	} // if
} // on_qpbAddContentPlugin_clicked

// add button is clicked on in lister plugins
const void cOptionsDialog::on_qpbAddListerPlugin_clicked(bool checked /* false */)
{
	QString qsFile;

	qsFile = QFileDialog::getOpenFileName(this, tr("Select lister plugin"), "/", "*.wlx");

	if (!qsFile.isEmpty()) {
		cSettings::sPlugin spPlugin;

		spPlugin.qsName = qsFile;
		spPlugin.bEnabled = true;
		AddPluginIntoTree(spPlugin, qtwListerPlugins);

		qfToDo |= ReloadPlugins;
	} // if
} // on_qpbAddListerPlugin_clicked

// add button is clicked on in packer plugins
const void cOptionsDialog::on_qpbAddPackerPlugin_clicked(bool checked /* false */)
{
	QString qsFile;

	qsFile = QFileDialog::getOpenFileName(this, tr("Select lister plugin"), "/", "*.wcx");

	if (!qsFile.isEmpty()) {
		cSettings::sPlugin spPlugin;
		QString qsInfFile;

		spPlugin.qsName = qsFile;
		spPlugin.bEnabled = true;
		// check for default extensions
		qsInfFile = QFileInfo(qsFile).path() + "/pluginst.inf";
		if (QFile::exists(qsInfFile)) {
			QSettings qsPackerSettings(qsInfFile, QSettings::IniFormat);

			qsPackerSettings.beginGroup("plugininstall");
			spPlugin.qsExtensions = qsPackerSettings.value("defaultextension").toStringList().join(";");
		} // if

		AddPluginIntoTree(spPlugin, qtwPackerPlugins);

		qfToDo |= ReloadPlugins;
	} // if
} // on_qpbAddPackerPlugin_clicked

// column down button is clicked on in columns view
const void cOptionsDialog::on_qpbColumnDown_clicked(bool checked /* false */)
{
	int iIndex;
	cSettings::sColumn scColumn;

	scColumn = GetColumnInfo(qtwColumns->currentItem());
	iIndex = qtwColumns->indexOfTopLevelItem(qtwColumns->currentItem());
	delete qtwColumns->currentItem();
	qtwColumns->setCurrentItem(AddColumnToColumns(scColumn, iIndex + 1));

	SaveOption(Columns);

	qfToDo |= RefreshHeader;
} // on_qpbColumnDown_clicked

// column remove button is clicked on in columns view
const void cOptionsDialog::on_qpbColumnRemove_clicked(bool checked /* false */)
{
	delete qtwColumns->currentItem();
	SaveOption(Columns);

	qfToDo |= RefreshHeader;
} // on_qpbColumnRemove_clicked

// column set add button is clicked on in columns view
const void cOptionsDialog::on_qpbColumnSetAdd_clicked(bool checked /* false */)
{
	QString qsName;

	qsName = QInputDialog::getText(this, tr("New column set"), NULL);

	if (!qsName.isEmpty()) {
		csSettings->CreateColumnSet(qsName);
		qcbColumnSet->addItem(qsName);
		qcbColumnSet->setCurrentIndex(qcbColumnSet->findText(qsName));

		qfToDo |= RefreshHeader | RefreshColumnSets;
	} // if
} // on_qpbColumnSetAdd_clicked

// column set remove button is clicked on in columns view
const void cOptionsDialog::on_qpbColumnSetRemove_clicked(bool checked /* false */)
{
	csSettings->RemoveColumnSet(qcbColumnSet->currentText());
	qcbColumnSet->removeItem(qcbColumnSet->currentIndex());

	qfToDo |= RefreshHeader | RefreshColumnSets;
} // on_qpbColumnSetRemove_clicked

// column up button is clicked on in columns view
const void cOptionsDialog::on_qpbColumnUp_clicked(bool checked /* false */)
{
	int iIndex;
	cSettings::sColumn scColumn;

	scColumn = GetColumnInfo(qtwColumns->currentItem());
	iIndex = qtwColumns->indexOfTopLevelItem(qtwColumns->currentItem());
	delete qtwColumns->currentItem();
	qtwColumns->setCurrentItem(AddColumnToColumns(scColumn, iIndex - 1));

	SaveOption(Columns);

	qfToDo |= RefreshHeader;
} // on_qpbColumnUp_clicked

// external editor browse button is clicked on
const void cOptionsDialog::on_qpbExternalEditorBrowse_clicked(bool checked /* false */)
{
	QString qsEditor;

	// TODO on_qpbExternalEditorBrowse_clicked other OS than Windows
#ifdef Q_WS_WIN
	qsEditor = QFileDialog::getOpenFileName(this, tr("Select editor"), "/", "*.exe");
#endif

	if (!qsEditor.isEmpty()) {
		if (qsEditor.contains(' ')) {
			qsEditor = '"' + qsEditor + '"';
		} // if
		qleExternalEditor->setText(qsEditor + " %1");
	} // if
} // on_qpbExternalEditorBrowse_clicked

// external viewer browse button is clicked on
const void cOptionsDialog::on_qpbExternalViewerBrowse_clicked(bool checked /* false */)
{
	QString qsViewer;

	// TODO on_qpbExternalViewerBrowse_clicked other OS than Windows
#ifdef Q_WS_WIN
	qsViewer = QFileDialog::getOpenFileName(this, tr("Select viewer"), "/", "*.exe");
#endif

	if (!qsViewer.isEmpty()) {
		if (qsViewer.contains(' ')) {
			qsViewer = '"' + qsViewer + '"';
		} // if
		qleExternalViewer->setText(qsViewer + " %1");
	} // if
} // on_qpbExternalViewerBrowse_clicked

// add favourite directory button is clicked on
const void cOptionsDialog::on_qpbFavouriteAddDirectory_clicked(bool checked /* false */)
{
	FavouriteAdd(cNewFavouriteDirectoryDialog::Directory);
} // on_qpbFavouriteAddDirectory_clicked

// add submenu button is clicked on
const void cOptionsDialog::on_qpbFavouriteAddSubmenu_clicked(bool checked /* false */)
{
	FavouriteAdd(cNewFavouriteDirectoryDialog::Submenu);
} // on_qpbFavouriteAddSubmenu_clicked

// remove favourite button is clicked on
const void cOptionsDialog::on_qpbFavouriteRemove_clicked(bool checked /* false */)
{
	// TODO on_qpbFavouriteRemove_clicked remove subitems from hash
	qhFavouriteDirectories.remove(qtwFavouriteDirectories->currentItem());
	delete qtwFavouriteDirectories->currentItem();

	qfToDo |= RefreshFavouriteDirectories;
} // on_qpbFavouriteRemove_clicked

// favourite source browse button is clicked on
const void cOptionsDialog::on_qpbFavouriteSourceBrowse_clicked(bool checked /* false */)
{
	QString qsDirectory;

	qsDirectory = QFileDialog::getExistingDirectory(this, tr("Select directory"), "/");

	if (!qsDirectory.isEmpty()) {
		qleFavouriteSource->setText(qsDirectory);
	} // if
} // on_qpbFavouriteSourceBrowse_clicked

// favourite target browse button is clicked on
const void cOptionsDialog::on_qpbFavouriteTargetBrowse_clicked(bool checked /* false */)
{
	QString qsDirectory;

	qsDirectory = QFileDialog::getExistingDirectory(this, tr("Select directory"), "/");

	if (!qsDirectory.isEmpty()) {
		qleFavouriteTarget->setText(qsDirectory);
	} // if
} // on_qpbFavouriteTargetBrowse_clicked

// change font in lister button is clicked on
const void cOptionsDialog::on_qpbListerChangeFont_clicked(bool checked /* false */)
{
	bool bOK;
	QFont qfFont;

	qfFont = QFontDialog::getFont(&bOK, qfListerFont, this, tr("Font"));

	if (bOK) {
		qfListerFont = qfFont;
	} // if
} // on_qpbListerChangeFont_clicked

// remove content plugin button is clicked on
const void cOptionsDialog::on_qpbRemoveContentPlugin_clicked(bool checked /* false */)
{
	delete qtwContentPlugins->selectedItems().at(0);

	qfToDo |= ReloadPlugins;
} // on_qpbRemoveContentPlugin_clicked

// remove lister plugin button is clicked on
const void cOptionsDialog::on_qpbRemoveListerPlugin_clicked(bool checked /* false */)
{
	delete qtwListerPlugins->selectedItems().at(0);

	qfToDo |= ReloadPlugins;
} // on_qpbRemoveListerPlugin_clicked

// remove packer plugin button is clicked on
const void cOptionsDialog::on_qpbRemovePackerPlugin_clicked(bool checked /* false */)
{
	delete qtwPackerPlugins->selectedItems().at(0);

	qfToDo |= ReloadPlugins;
} // on_qpbRemovePackerPlugin_clicked

// shortcut clear button is clicked on
const void cOptionsDialog::on_qpbShortcutClear_clicked(bool checked /* false */) const
{
	qleShortcut->clear();
} // on_qpbShortcutClear_clicked

// size in bytes status change
const void cOptionsDialog::on_qrbSizeBytes_toggled(bool checked)
{
	qfToDo |= RefreshContent;
} // on_qrbSizeBytes_toggled

// dynamic size status change
const void cOptionsDialog::on_qrbSizeDynamic_toggled(bool checked)
{
	qfToDo |= RefreshContent;
} // on_qrbSizeDynamic_toggled

// size in gigabytes status change
const void cOptionsDialog::on_qrbSizeGigabytes_toggled(bool checked)
{
	qfToDo |= RefreshContent;
} // on_qrbSizeGigabytes_toggled

// size in kilobytes status change
const void cOptionsDialog::on_qrbSizeKilobytes_toggled(bool checked)
{
	qfToDo |= RefreshContent;
} // on_qrbSizeKilobytes_toggled

// size in megabytes status change
const void cOptionsDialog::on_qrbSizeMegabytes_toggled(bool checked)
{
	qfToDo |= RefreshContent;
} // on_qrbSizeMegabytes_toggled

// changed width of column
const void cOptionsDialog::on_qsbWidth_valueChanged(int val)
{
	SaveOption(Columns);

	qfToDo |= RefreshHeader;
} // on_qsbWidth_valueChanged

// selected cell changed
const void cOptionsDialog::on_qtwColumns_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous) const
{
	if (current && qtwColumns->indexOfTopLevelItem(qtwColumns->currentItem()) > 0) {
			qpbColumnUp->setEnabled(true);
	} else {
		qpbColumnUp->setEnabled(false);
	} // if else
	if (current && qtwColumns->indexOfTopLevelItem(qtwColumns->currentItem()) < qtwColumns->topLevelItemCount() - 1) {
		qpbColumnDown->setEnabled(true);
	} else {
		qpbColumnDown->setEnabled(false);
	} // if else
} // on_qtwColumns_currentItemChanged

// selected column changed
const void cOptionsDialog::on_qtwColumns_itemSelectionChanged() const
{
	if (qtwColumns->selectedItems().count() > 0) {
		qpbColumnRemove->setEnabled(true);
	} else {
		qpbColumnRemove->setEnabled(false);
	} // if else
} // on_qtwColumns_itemSelectionChanged

// selected content plugin changed
const void cOptionsDialog::on_qtwContentPlugins_itemChanged(QTreeWidgetItem *item, int column)
{
	qfToDo |= ReloadPlugins;
} // on_qtwContentPlugins_itemChanged

// selected content plugin changed
const void cOptionsDialog::on_qtwContentPlugins_itemSelectionChanged() const
{
	if (qtwContentPlugins->selectedItems().count() > 0) {
		qpbRemoveContentPlugin->setEnabled(true);
	} else {
		qpbRemoveContentPlugin->setEnabled(false);
	} // if else
} // on_qtwContentPlugins_itemSelectionChanged

// another favourite directory is selected
const void cOptionsDialog::on_qtwFavouriteDirectories_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
	qpbFavouriteRemove->setEnabled(current);

	if (current && current->type() == cNewFavouriteDirectoryDialog::Directory) {
		sFavouriteDirectory *sfdCurrent;

		qleFavouriteSource->setEnabled(true);
		qpbFavouriteSourceBrowse->setEnabled(true);
		qcbFavouriteTargetDirectory->setEnabled(true);

		// set values for currently selected favourite directory
		sfdCurrent = &qhFavouriteDirectories[current];
		qleFavouriteSource->setText(sfdCurrent->qsSource);
		qcbFavouriteTargetDirectory->setChecked(sfdCurrent->bTarget);
		qleFavouriteTarget->setText(sfdCurrent->qsTarget);
	} else {
		qleFavouriteSource->setEnabled(false);
		qleFavouriteSource->clear();
		qpbFavouriteSourceBrowse->setEnabled(false);
		qcbFavouriteTargetDirectory->setChecked(false);
		qcbFavouriteTargetDirectory->setEnabled(false);
		qleFavouriteTarget->clear();
	} // if else
} // on_qtwFavouriteDirectories_currentItemChanged

// selected lister plugin changed
const void cOptionsDialog::on_qtwListerPlugins_itemChanged(QTreeWidgetItem *item, int column)
{
	qfToDo |= ReloadPlugins;
} // on_qtwListerPlugins_itemSelectionChanged

// selected lister plugin changed
const void cOptionsDialog::on_qtwListerPlugins_itemSelectionChanged() const
{
	if (qtwListerPlugins->selectedItems().count() > 0) {
		qpbRemoveListerPlugin->setEnabled(true);
	} else {
		qpbRemoveListerPlugin->setEnabled(false);
	} // if else
} // on_qtwListerPlugins_itemSelectionChanged

// selected packer plugin changed
const void cOptionsDialog::on_qtwPackerPlugins_itemChanged(QTreeWidgetItem *item, int column)
{
	qfToDo |= ReloadPlugins;
} // on_qtwPackerPlugins_itemChanged

// selected packer plugin changed
const void cOptionsDialog::on_qtwPackerPlugins_itemSelectionChanged() const
{
	if (qtwPackerPlugins->selectedItems().count() > 0) {
		qpbRemovePackerPlugin->setEnabled(true);
	} else {
		qpbRemovePackerPlugin->setEnabled(false);
	} // if else
} // on_qtwPackerPlugins_itemSelectionChanged

// selected shortcut category changed
const void cOptionsDialog::on_qtwShortcutCategory_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous) const
{
	qtwShortcutItem->clear();

	if (current == qtwiShortcutFile) {
		// main menu/file
		FillShortcutItems(cSettings::MainMenuCategory, qslSHORTCUTS__MAIN_MENU__FILE);
	} else {
		if (current == qtwiShortcutMark) {
			// main menu/mark
			FillShortcutItems(cSettings::MainMenuCategory, qslSHORTCUTS__MAIN_MENU__MARK);
		} else {
			if (current == qtwiShortcutCommands) {
				// main menu/commands
				FillShortcutItems(cSettings::MainMenuCategory, qslSHORTCUTS__MAIN_MENU__COMMANDS);
			} else {
				if (current == qtwiShortcutPanel) {
					// main menu/panel
					FillShortcutItems(cSettings::MainMenuCategory, qslSHORTCUTS__MAIN_MENU__PANEL);
				} else {
					if (current == qtwiShortcutConfiguration) {
						// main menu/configuration
						FillShortcutItems(cSettings::MainMenuCategory, qslSHORTCUTS__MAIN_MENU__CONFIGURATION);
					} else {
						if (current == qtwiShortcutTabs) {
							// panels/tabs
							FillShortcutItems(cSettings::PanelsCategory, qslSHORTCUTS__PANELS__TABS);
						} else {
							if (current == qtwiShortcutDirectoryView) {
								// panels/directory view
								FillShortcutItems(cSettings::PanelsCategory, qslSHORTCUTS__PANELS__DIRECTORY_VIEW);
							} else {
								if (current == qtwiShortcutMainButtons) {
									// panels/main buttons
									FillShortcutItems(cSettings::PanelsCategory, qslSHORTCUTS__PANELS__MAIN_BUTTON);
								} else {
									if (current == qtwiShortcutOperationsDialog) {
										// operations/dialog
										FillShortcutItems(cSettings::OperationsCategory, qslSHORTCUTS__OPERATIONS__DIALOG);
									} else {
										if (current == qtwiShortcutListerFile) {
											// lister/file
											FillShortcutItems(cSettings::ListerCategory, qslSHORTCUTS__LISTER__FILE);
										} else {
											if (current == qtwiShortcutListerEdit) {
												// lister/edit
												FillShortcutItems(cSettings::ListerCategory, qslSHORTCUTS__LISTER__EDIT);
											} else {
												if (current == qtwiShortcutListerOptions) {
													// lister/options
													FillShortcutItems(cSettings::ListerCategory, qslSHORTCUTS__LISTER__OPTIONS);
												} // if
											} // if else
										} // if else
									} // if else
								} // if else
							} // if else
						} // if else
					} // if else
				} // if else
			} // if else
		} // if else
	} // if else
} // on_qtwShortcutCategory_itemSelectionChanged

// selected shortcut changed
const void cOptionsDialog::on_qtwShortcutItem_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous) const
{
	// set shortcut line edit
	qleShortcut->blockSignals(true);
	if (current) {
		qleShortcut->setText(current->text(1));
		qleShortcut->setEnabled(true);
	} else {
		qleShortcut->clear();
		qleShortcut->setEnabled(false);
	} // if else
	qleShortcut->blockSignals(false);
} // on_qtwShortcutItem_currentItemChanged

// prepare context plugin menu
const void cOptionsDialog::PrepareColumnsMenu()
{
	QAction *qaAction, *qaNative, *qaPlugins;
	QHash<QString, cContentPlugin::sPluginInfo> qhPlugins;

	// main menu
	qaNative = qmColumns.addAction(qsNATIVE2);
	qaPlugins = qmColumns.addAction(qsPLUGINS);

	// native sub menu
	qmNative = new QMenu(&qmColumns);
	qaAction = qmNative->addAction(qsICON);
	qaAction->setData(QString("%1|%2").arg(qsNATIVE2).arg(qsICON));
	qaAction = qmNative->addAction(qsNAME);
	qaAction->setData(QString("%1|%2").arg(qsNATIVE2).arg(qsNAME));
	qaAction = qmNative->addAction(qsEXTENSION);
	qaAction->setData(QString("%1|%2").arg(qsNATIVE2).arg(qsEXTENSION));
	qaAction = qmNative->addAction(qsNAME_WITH_EXTENSION);
	qaAction->setData(QString("%1|%2").arg(qsNATIVE2).arg(qsNAME_WITH_EXTENSION));
	qaAction = qmNative->addAction(qsSIZE);
	qaAction->setData(QString("%1|%2").arg(qsNATIVE2).arg(qsSIZE));
	qaAction = qmNative->addAction(qsDATE_TIME);
	qaAction->setData(QString("%1|%2").arg(qsNATIVE2).arg(qsDATE_TIME));
#ifdef Q_WS_WIN
	qaAction = qmNative->addAction(qsATTRIBUTES);
	qaAction->setData(QString("%1|%2").arg(qsNATIVE2).arg(qsATTRIBUTES));
#endif
	qaNative->setMenu(qmNative);

	// plugins sub menu
	qmPlugins = new QMenu(&qmColumns);
	qhPlugins = ccpContentPlugin->GetPluginsInfo();
	if (qhPlugins.isEmpty()) {
		qmPlugins->setEnabled(false);
	} else {
		QHashIterator<QString, cContentPlugin::sPluginInfo> qhiPlugins(qhPlugins);
		while (qhiPlugins.hasNext()) {
			int iI;
			QAction *qaPlugin;
			QMenu *qmFields;

			qhiPlugins.next();
			qaPlugin = qmPlugins->addAction(qhiPlugins.key());

			qmFields = new QMenu(qmPlugins);
			for (iI = 0; iI < qhiPlugins.value().qlFields.count(); iI++) {
				cContentPlugin::sField *sfField;
				
				sfField = &const_cast<cContentPlugin::sPluginInfo &>(qhiPlugins.value()).qlFields[iI];
				if (sfField->iType != ft_fulltext) {
					// plugin's fields (columns)
					QAction *qaField;

					qaField = qmFields->addAction(sfField->qsName);

					if (!sfField->qsUnits.isEmpty() && sfField->iType != ft_multiplechoice) {
						// column's units
						int iJ;
						QMenu *qmUnits;
						QStringList qslUnits;

						qslUnits = sfField->qsUnits.split('|');

						qmUnits = new QMenu(qmFields);
						for (iJ = 0; iJ < qslUnits.count(); iJ++) {
							QAction *qaUnit;
							QString *qsUnit;

							qsUnit = &qslUnits[iJ];

							qaUnit = qmUnits->addAction(*qsUnit);
							qaUnit->setData(QString("%1|%2|%3|%4").arg(qsPLUGINS).arg(qhiPlugins.key()).arg(sfField->qsName).arg(*qsUnit));
						} // for

						qaField->setMenu(qmUnits);
					} else {
						qaField->setData(QString("%1|%2|%3").arg(qsPLUGINS).arg(qhiPlugins.key()).arg(sfField->qsName));
					} // if else
				} // if
			} // for
			qaPlugin->setMenu(qmFields);
		} // while
	} // if else
	qaPlugins->setMenu(qmPlugins);
} // PrepareColumnsMenu

// save specific changes into settings file
const void cOptionsDialog::SaveOption(const eOption &eoType) const
{
	int iI;
	QList<cSettings::sColumn> qlColumns;

	switch (eoType) {
		case Columns:
			for (iI = 0; iI < qtwColumns->topLevelItemCount(); iI++) {
				cSettings::sColumn scColumn;

				scColumn = GetColumnInfo(qtwColumns->topLevelItem(iI));
				qlColumns.append(scColumn);
			} // for

			csSettings->CreateColumnSet(qcbColumnSet->currentText(), qlColumns);
	} // switch
} // SaveOption

// save changes into application's settings file
const void cOptionsDialog::SaveOptions()
{
	cSettings::sLister slLister;
	QString qsValue;

	// panels
	// display
	csSettings->SetShowSystemFiles(qcbShowSystemFiles->isChecked());
	csSettings->SetShowHiddenFiles(qcbShowHiddenFiles->isChecked());
	csSettings->SetShowBracketsAroundDirectoryName(qcbSquareBracketsAroundDirectoryName->isChecked());
	if (qrbSizeBytes->isChecked()) {
		qsValue = qsBYTES;
	} else {
		if (qrbSizeKilobytes->isChecked()) {
			qsValue = qsKILOBYTES;
		} else {
			if (qrbSizeMegabytes->isChecked()) {
				qsValue = qsMEGABYTES;
			} else {
				if (qrbSizeGigabytes->isChecked()) {
					qsValue = qsGIGABYTES;
				} else {
					qsValue = qsDYNAMIC;
				} // if else
			} // if else
		} // if else
	} // if else
	csSettings->SetFileSizeIn(qsValue);
	if (qrbSelectionOnlyFiles->isChecked()) {
		qsValue = qsONLY_FILES;
	} else {
		qsValue = qsFILES_AND_DIRECTORIES;
	} // if else
	csSettings->SetSelectionMode(qsValue);
	csSettings->SetSaveSettingsOnQuit(qcbSaveSettingsOnQuit->isChecked());
	csSettings->SetDateTimeDisplay(qleDateTimeDisplay->text());
	csSettings->SetCaseSensitiveSorting(qcbCaseSensitiveSorting->isChecked());
	csSettings->SetShowDirectoryViewHeader(qcbShowDirectoryViewHeader->isChecked());
	// tabs
	csSettings->SetShowTabBarWithOnlyOneTab(qcbShowTabBarWithOnlyOneTab->isChecked());
	csSettings->SetConfirmCloseOfAllTabs(qcbConfirmCloseOfAllOtherTabs->isChecked());
	csSettings->SetCloseTabOnDoubleClick(qcbCloseTabOnDoubleClick->isChecked());
	csSettings->SetNewTabByShortcutInForeground(qcbNewTabByShortcutInForeground->isChecked());
	csSettings->SetOpenNewTabNextToCurrentTab(qcbOpenNewTabNextToCurrentTab->isChecked());
	csSettings->SetShowDriveLetter(qcbShowDriveLetter->isChecked());
	// quick search
	csSettings->SetQuickSearchEnabled(qgbQuickSearch->isChecked());
	csSettings->SetQuickSearchCtrl(qcbQuickSearchCtrl->isChecked());
	csSettings->SetQuickSearchAlt(qcbQuickSearchAlt->isChecked());
	csSettings->SetQuickSearchShift(qcbQuickSearchShift->isChecked());
	csSettings->SetQuickSearchShowSearchWindow(qcbQuickSearchShowWindow->isChecked());

	// operations
	// viewer
	if (qrbViewerInternal->isChecked()) {
		csSettings->SetViewerType(qsINTERNAL);
	} else {
		csSettings->SetViewerType(qsEXTERNAL);
	} // if else
	csSettings->SetExternalViewer(qleExternalViewer->text());
	// editor
	csSettings->SetExternalEditor(qleExternalEditor->text());
	// copy/move
	csSettings->SetCopyMoveBufferSize(qsbCopyMoveBufferSize->value());
	// delete
	csSettings->SetAskToDeleteNonEmptyDirectory(qcbAskToDeleteNonEmptyDirectory->isChecked());
#ifdef Q_WS_WIN
	csSettings->SetDeleteToRecycleBin(qcbDeleteToRecycleBin->isChecked());
#endif
	// confirmation
	if (qrbOverwriteAsk->isChecked()) {
		qsValue = qsASK;
	} else {
		if (qrbOverwriteAll->isChecked()) {
			qsValue = qsOVERWRITE_ALL;
		} else {
			if (qrbOverwriteAllOlder->isChecked()) {
				qsValue = qsOVERWRITE_ALL_OLDER;
			} else {
				qsValue = qsSKIP_ALL;
			} // if else
		} // if else
	} // if else
	csSettings->SetFileOverwrite(qsValue);
	if (qrbReadonlyOverwriteAsk->isChecked()) {
		qsValue = qsASK;
	} else {
		if (qrbReadonlyOverwriteYesToAll->isChecked()) {
			qsValue = qsYES_TO_ALL;
		} else {
			qsValue = qsNO_TO_ALL;
		} // if else
	} // if else
	csSettings->SetReadonlyFileOverwrite(qsValue);

	// plugins
	// content
	csSettings->SetPlugins(cSettings::ContentPlugins, GetPluginList(qtwContentPlugins));
	csSettings->SetPluginDateTimeDisplay(qlePluginDateTimeDisplay->text());
	csSettings->SetPluginTimeDisplay(qlePluginTimeDisplay->text());
	// lister
	csSettings->SetPlugins(cSettings::ListerPlugins, GetPluginList(qtwListerPlugins));
	if (qrbListerANSI->isChecked()) {
		slLister.qsCharSet = qsANSI;
	} else {
		if (qrbListerASCII->isChecked()) {
			slLister.qsCharSet = qsASCII;
		} else {
			slLister.qsCharSet = qsVARIABLE_CHAR_WIDTH;
		} // if else
	} // if else
	slLister.bWrapText = qcbListerWrapText->isChecked();
	slLister.bFitImageToWindow = qcbListerFitImageToWindow->isChecked();
	csSettings->SetListerSettings(slLister);
	csSettings->SetListerFont(qfListerFont);
	// packer
	csSettings->SetPlugins(cSettings::PackerPlugins, GetPluginList(qtwPackerPlugins));
	csSettings->SetTreatArchivesLikeDirectories(qcbTreatArchivesLikeDirectories->isChecked());

	// others
	// favourite directories
	csSettings->SetFavouriteDirectories(GetFavouriteDirectories(qtwFavouriteDirectories->invisibleRootItem()));
	// miscellaneous
	csSettings->SetMaximumHistoryDirectoryListSize(qsbMaximumHistoryDirectoryListSize->value());
} // SaveOptions

// fill favourite directories into tree widget
const void cOptionsDialog::SetFavouriteDirectories(QTreeWidgetItem *qtwiParent, QList<QPair<QString, cSettings::sFavouriteDirectory> > &qlFavouriteDirectories)
{
	int iI;

	for (iI = 0; iI < qlFavouriteDirectories.count(); iI++) {
		cNewFavouriteDirectoryDialog::eType etType;
		QPair<QString, cSettings::sFavouriteDirectory> *qpFavourite;
		QTreeWidgetItem *qtwiFavouriteDirectory;
		sFavouriteDirectory sfdFavouriteDirectory;

		qpFavourite = &qlFavouriteDirectories[iI];

		if (qpFavourite->second.bSubmenu) {
			etType = cNewFavouriteDirectoryDialog::Submenu;
		} else {
			etType = cNewFavouriteDirectoryDialog::Directory;
		} // if else

		qtwiFavouriteDirectory = new QTreeWidgetItem(qtwiParent, etType);
		qtwiFavouriteDirectory->setText(0, qpFavourite->first);

		if (etType == cNewFavouriteDirectoryDialog::Submenu) {
			SetFavouriteDirectories(qtwiFavouriteDirectory, qpFavourite->second.qlChildFavourites);
		} else {
			sfdFavouriteDirectory.qsSource = qpFavourite->second.qsSource;
			sfdFavouriteDirectory.bTarget = qpFavourite->second.bTarget;
			sfdFavouriteDirectory.qsTarget = qpFavourite->second.qsTarget;
		} // if else

		qhFavouriteDirectories.insert(qtwiFavouriteDirectory, sfdFavouriteDirectory);
	} // for
} // SetFavouriteDirectories
