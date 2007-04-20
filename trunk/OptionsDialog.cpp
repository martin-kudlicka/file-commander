#include "OptionsDialog.h"

#include <QFileDialog>
#include <QAction>
#include "Plugins/ContPlug.h"
#include "Options/NewColumnSetDialog.h"
#include <QSpinBox>
#include <QHeaderView>
#include <QKeyEvent>

const QString qsNATIVE = QT_TR_NOOP("native");
const QString qsNATIVE2 = QT_TR_NOOP("Native");
const QString qsPLUGINS = QT_TR_NOOP("Plugins");
// shortcuts
const QStringList qslSHORTCUTS__LISTER__FILE = QStringList() << qsSHORTCUT__LISTER__FILE__OPEN <<
																					 qsSHORTCUT__LISTER__FILE__SAVE_AS <<
																					 qsSHORTCUT__LISTER__FILE__PRINT <<
																					 qsSHORTCUT__LISTER__FILE__PRINT_SETUP <<
																					 qsSHORTCUT__LISTER__FILE__CLOSE;
const QStringList qslSHORTCUTS__LISTER__EDIT = QStringList() << qsSHORTCUT__LISTER__EDIT__COPY_AS_TEXT <<
																					 qsSHORTCUT__LISTER__EDIT__SELECT_ALL <<
																					 qsSHORTCUT__LISTER__EDIT__FIND <<
																					 qsSHORTCUT__LISTER__EDIT__FIND_NEXT;
const QStringList qslSHORTCUTS__LISTER__OPTIONS = QStringList() << qsSHORTCUT__LISTER__OPTIONS__TEXT <<
																						 qsSHORTCUT__LISTER__OPTIONS__BINARY <<
																						 qsSHORTCUT__LISTER__OPTIONS__HEX <<
																						 qsSHORTCUT__LISTER__OPTIONS__MULTIMEDIA <<
																						 qsSHORTCUT__LISTER__OPTIONS__ANSI <<
																						 qsSHORTCUT__LISTER__OPTIONS__ASCII <<
																						 qsSHORTCUT__LISTER__OPTIONS__VARIABLE_CHAR_WIDTH <<
																						 qsSHORTCUT__LISTER__OPTIONS__WRAP_TEXT <<
																						 qsSHORTCUT__LISTER__OPTIONS__FIT_IMAGE_TO_WINDOW;
const QStringList qslSHORTCUTS__MAIN_MENU__FILE = QStringList() << qsSHORTCUT__MAIN_MENU__FILE__QUIT;
const QStringList qslSHORTCUTS__MAIN_MENU__MARK = QStringList() << qsSHORTCUT__MAIN_MENU__MARK__SELECT_GROUP <<
																						 qsSHORTCUT__MAIN_MENU__MARK__UNSELECT_GROUP <<
																						 qsSHORTCUT__MAIN_MENU__MARK__SELECT_ALL <<
																						 qsSHORTCUT__MAIN_MENU__MARK__UNSELECT_ALL <<
																						 qsSHORTCUT__MAIN_MENU__MARK__INVERT_SELECTION;
const QStringList qslSHORTCUTS__MAIN_MENU__COMMANDS = QStringList() << qsSHORTCUT__MAIN_MENU__COMMANDS__SEARCH;
const QStringList qslSHORTCUTS__MAIN_MENU__SHOW = QStringList() << qsSHORTCUT__MAIN_MENU__SHOW__FULLSCREEN <<
																						 qsSHORTCUT__MAIN_MENU__SHOW__REFRESH;
const QStringList qslSHORTCUTS__MAIN_MENU__CONFIGURATION = QStringList() << qsSHORTCUT__MAIN_MENU__CONFIGURATION__OPTIONS <<
																									 qsSHORTCUT__MAIN_MENU__CONFIGURATION__SAVE_POSITION <<
																									 qsSHORTCUT__MAIN_MENU__CONFIGURATION__SAVE_SETTINGS;
const QStringList qslSHORTCUTS__OPERATIONS__DIALOG = QStringList() << qsSHORTCUT__OPERATIONS__DIALOG__ENQUEUE;
const QStringList qslSHORTCUTS__PANELS__DIRECTORY_VIEW = QStringList() << qsSHORTCUT__PANELS__DIRECTORY_VIEW__DRIVE_LEFT <<
																								  qsSHORTCUT__PANELS__DIRECTORY_VIEW__DRIVE_RIGHT;
const QStringList qslSHORTCUTS__PANELS__MAIN_BUTTON = QStringList() << qsSHORTCUT__PANELS__MAIN_BUTTON__VIEW <<
																							  qsSHORTCUT__PANELS__MAIN_BUTTON__EDIT <<
																							  qsSHORTCUT__PANELS__MAIN_BUTTON__COPY <<
																							  qsSHORTCUT__PANELS__MAIN_BUTTON__MOVE <<
																							  qsSHORTCUT__PANELS__MAIN_BUTTON__NEW_DIRECTORY <<
																							  qsSHORTCUT__PANELS__MAIN_BUTTON__DELETE;

// destructor
cOptionsDialog::~cOptionsDialog()
{
	qmNative->deleteLater();
	qmPlugins->deleteLater();
	qagToolBarActions->deleteLater();
	qtbToolBar->deleteLater();
} // ~cOptionsDialog

// add new column to current column set
QTreeWidgetItem *cOptionsDialog::AddColumnToColumns(const cSettings::sColumn &scColumn, const int &iPos /* INT_MAX */)
{
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
	qtwiItem->setText(iSHOW_COLUMN, scColumn.qsName);
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
void cOptionsDialog::AddPluginIntoTree(const cSettings::sPlugin &spPlugin, QTreeWidget *qtwTree)
{
	QTreeWidgetItem *qtwiItem;

	// name
	qtwiItem = new QTreeWidgetItem(qtwTree);
	qtwiItem->setText(iPLUGIN_NAME_COLUMN, spPlugin.qsName);
	// enable/disable flag
	if (spPlugin.bEnabled) {
		qtwiItem->setCheckState(iPLUGIN_ENABLED_COLUMN, Qt::Checked);
	} else {
		qtwiItem->setCheckState(iPLUGIN_ENABLED_COLUMN, Qt::Unchecked);
	} // if else

	qtwTree->resizeColumnToContents(iPLUGIN_NAME_COLUMN);
} // AddPluginIntoTree

// constructor
cOptionsDialog::cOptionsDialog(QWidget *qmwParent, cSettings *csSettings, cContent *ccContent)
{
	setParent(qmwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);

	this->csSettings = csSettings;
	this->ccContent = ccContent;

	qpbColumnAdd->setMenu(&qmColumns);

	// remember original options
	qlOldOptions = csSettings->GetAllSettings();

	CreateToolBar();
	PrepareColumnsMenu();
	FillOptions();

	// connections
	connect(&qmColumns, SIGNAL(triggered(QAction *)), SLOT(on_qmColumns_triggered(QAction *)));

	qleShortcut->installEventFilter(this);
} // cConfigurationDialog

// create left toolbar for navigation
void cOptionsDialog::CreateToolBar()
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
				if (qsKey != "") {
					qsKey += "+";
				} // if
				qsKey += "Alt";
			} // if
			if (qkeKey->modifiers() & Qt::ShiftModifier) {
				if (qsKey != "") {
					qsKey += "+";
				} // if
				qsKey += "Shift";
			} // if
			if (qsKey != "") {
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

// fill options with set settings
void cOptionsDialog::FillOptions()
{
	cSettings::sLister slLister;
	QString qsValue;
	QTreeWidgetItem *qtwiShortcutMain;

	// panels
	// display
	if (csSettings->GetShowSystemFiles()) {
		qcbShowSystemFiles->setChecked(true);
	} // if
	if (csSettings->GetShowHiddenFiles()) {
		qcbShowHiddenFiles->setChecked(true);
	} // if
	qsValue = csSettings->GetFileSizeIn();
	if (qsValue == qsBYTES) {
		qrbSizeBytes->setChecked(true);
	} else {
		if (qsValue == qsKILOBYTES) {
			qrbSizeKilobytes->setChecked(true);
		} else {
			if (qsValue == qsMEGABYTES) {
				qrbSizeMegabytes->setChecked(true);
			} else {
				if (qsValue == qsGIGABYTES) {
					qrbSizeGigabytes->setChecked(true);
				} else {
					qrbSizeDynamic->setChecked(true);
				} // if else
			} // if else
		} // if else
	} // if else
	if (csSettings->GetShowBracketsAroundDirectoryName()) {
		qcbSquareBracketsAroundDirectoryName->setChecked(true);
	} // if
	qsValue = csSettings->GetSelectionMode();
	if (qsValue == qsONLY_FILES) {
		qrbSelectionOnlyFiles->setChecked(true);
	} else {
		qrbSelectionFilesAndDirectories->setChecked(true);
	} // if else
	if (csSettings->GetSaveSettingsOnQuit()) {
		qcbSaveSettingsOnQuit->setChecked(true);
	} // if
	// column sets
	qtwColumns->setColumnCount(iCOLUMNS);
	qtwColumns->setHeaderLabels(QStringList() << tr("Type") << tr("Name") << tr("Unit") << tr("Show") << tr("Width"));
	qtwColumns->header()->setResizeMode(iNAME_COLUMN, QHeaderView::Stretch);
	qtwColumns->header()->setResizeMode(iSHOW_COLUMN, QHeaderView::Stretch);
	qtwColumns->header()->setStretchLastSection(false);
	qcbColumnSet->addItems(csSettings->GetColumnSets());

	// operations
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

	// others
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
	qtwiShortcutShow = new QTreeWidgetItem(qtwiShortcutMain);
	qtwiShortcutShow->setText(0, tr("Show"));
	qtwiShortcutShow->setData(0, Qt::UserRole, cSettings::MainMenuCategory);
	qtwiShortcutConfiguration = new QTreeWidgetItem(qtwiShortcutMain);
	qtwiShortcutConfiguration->setText(0, tr("Configuration"));
	qtwiShortcutConfiguration->setData(0, Qt::UserRole, cSettings::MainMenuCategory);
	qtwiShortcutMain = new QTreeWidgetItem(qtwShortcutCategory);
	qtwiShortcutMain->setText(0, tr("Panels"));
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

	// plugins
	// general
	qlePluginTimeDisplay->setText(csSettings->GetPluginTimeDisplay());
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
	if (slLister.bWrapText) {
		qcbListerWrapText->setChecked(true);
	} // if
	if (slLister.bFitImageToWindow) {
		qcbListerFitImageToWindow->setChecked(true);
	} // if
	// content
	FillPluginsTree(csSettings->GetPlugins(cSettings::ContentPlugins), qtwContentPlugins);
	// lister
	FillPluginsTree(csSettings->GetPlugins(cSettings::ListerPlugins), qtwListerPlugins);
} // FillOptions

// fills plugin information into tree
void cOptionsDialog::FillPluginsTree(const QList<cSettings::sPlugin> &qlPlugins, QTreeWidget *qtwTree)
{
	int iI;
	QStringList qslHeader;

	qslHeader.append(tr("File path"));
	qslHeader.append(tr("Enabled"));
	qtwTree->setHeaderLabels(qslHeader);

	for (iI = 0; iI < qlPlugins.count(); iI++) {
		AddPluginIntoTree(qlPlugins.at(iI), qtwTree);
	} // for
} // FillPluginsTree

// fill shortcuts for selected category
void cOptionsDialog::FillShortcutItems(const cSettings::eShortcutCategory &escCategory, const QStringList &qslItems)
{
	int iI;

	qtwShortcutItem->clear();

	for (iI = 0; iI < qslItems.count(); iI++) {
		QTreeWidgetItem *qtwShortcut;

		qtwShortcut = new QTreeWidgetItem(qtwShortcutItem);
		qtwShortcut->setText(0, qslItems.at(iI));
		qtwShortcut->setText(1, csSettings->GetShortcut(escCategory, qslItems.at(iI)));
	} // for

	qtwShortcutItem->resizeColumnToContents(0);
} // FillShortcutItems

// get information about column from column set
cSettings::sColumn cOptionsDialog::GetColumnInfo(QTreeWidgetItem *qtwiItem)
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
	scColumn.qsName = qtwiItem->text(iSHOW_COLUMN);
	// width
	scColumn.iWidth = static_cast<QSpinBox *>(qtwColumns->itemWidget(qtwiItem, iWIDTH_COLUMN))->value();

	return scColumn;
} // GetColumnInfo

// get info about specified plugins
QList<cSettings::sPlugin> cOptionsDialog::GetPluginList(const QTreeWidget *qtwPlugins)
{
	int iI;
	QList<cSettings::sPlugin> qlPlugins;

	for (iI = 0; iI < qtwPlugins->topLevelItemCount(); iI++) {
		cSettings::sPlugin spPlugin;

		spPlugin.qsName = qtwPlugins->topLevelItem(iI)->text(iPLUGIN_NAME_COLUMN);
		if (qtwPlugins->topLevelItem(iI)->checkState(iPLUGIN_ENABLED_COLUMN) == Qt::Checked) {
			spPlugin.bEnabled = true;
		} else {
			spPlugin.bEnabled = false;
		} // if else

		qlPlugins.append(spPlugin);
	} // for

	return qlPlugins;
} // GetPluginList

// clicked on action in tool bar panel
void cOptionsDialog::on_qagToolBarActions_triggered(QAction *qaAction)
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
void cOptionsDialog::on_qcbColumnSet_currentIndexChanged(const QString &text)
{
	QStringList qslColumns;

	qslColumns = csSettings->GetColumnsInSet(text);

	qtwColumns->clear();

	if (text == "") {
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

// changes accepted
void cOptionsDialog::on_qdbbResponse_accepted()
{
	SaveOptions();
	accept();
} // on_qdbbResponse_accepted

void cOptionsDialog::on_qdbbResponse_rejected()
{
	csSettings->RestoreSettings(qlOldOptions);
	reject();
} // on_qdbbResponse_rejected

// shortcut changed
void cOptionsDialog::on_qleShortcut_textChanged(const QString &text)
{
	qtwShortcutItem->currentItem()->setText(1, text);
	csSettings->SetShortcut(static_cast<cSettings::eShortcutCategory>(qtwShortcutCategory->currentItem()->data(0, Qt::UserRole).toInt()), qtwShortcutItem->currentItem()->text(0), text);
} // on_qleShortcut_textEdited

// column selected into column set
void cOptionsDialog::on_qmColumns_triggered(QAction *action)
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
} // on_qmColumns_triggered

// add button is clicked on in content plugins
void cOptionsDialog::on_qpbAddContentPlugin_clicked(bool checked /* false */)
{
	QString qsFile;

	qsFile = QFileDialog::getOpenFileName(this, tr("Select content plugin"), "/", "*.wdx");

	if (qsFile != "") {
		cSettings::sPlugin spPlugin;

		spPlugin.qsName = qsFile;
		spPlugin.bEnabled = true;
		AddPluginIntoTree(spPlugin, qtwContentPlugins);
	} // if
} // on_qpbAddContentPlugin_clicked

// add button is clicked on in lister plugins
void cOptionsDialog::on_qpbAddListerPlugin_clicked(bool checked /* false */)
{
	QString qsFile;

	qsFile = QFileDialog::getOpenFileName(this, tr("Select lister plugin"), "/", "*.wlx");

	if (qsFile != "") {
		cSettings::sPlugin spPlugin;

		spPlugin.qsName = qsFile;
		spPlugin.bEnabled = true;
		AddPluginIntoTree(spPlugin, qtwListerPlugins);
	} // if
} // on_qpbAddListerPlugin_clicked

// column down button is clicked on in columns view
void cOptionsDialog::on_qpbColumnDown_clicked(bool checked /* false */)
{
	int iIndex;
	cSettings::sColumn scColumn;

	scColumn = GetColumnInfo(qtwColumns->currentItem());
	iIndex = qtwColumns->indexOfTopLevelItem(qtwColumns->currentItem());
	delete qtwColumns->currentItem();
	qtwColumns->setCurrentItem(AddColumnToColumns(scColumn, iIndex + 1));

	SaveOption(Columns);
} // on_qpbColumnDown_clicked

// column remove button is clicked on in columns view
void cOptionsDialog::on_qpbColumnRemove_clicked(bool checked /* false */)
{
	delete qtwColumns->currentItem();
	SaveOption(Columns);
} // on_qpbColumnRemove_clicked

// column set add button is clicked on in columns view
void cOptionsDialog::on_qpbColumnSetAdd_clicked(bool checked /* false */)
{
	cNewColumnSetDialog cncsdDialog(this);

	if (cncsdDialog.exec() == QDialog::Accepted) {
		csSettings->CreateColumnSet(cncsdDialog.qleColumnSet->text());
		qcbColumnSet->addItem(cncsdDialog.qleColumnSet->text());
		qcbColumnSet->setCurrentIndex(qcbColumnSet->findText(cncsdDialog.qleColumnSet->text()));
	} // if
} // on_qpbColumnSetAdd_clicked

// column set remove button is clicked on in columns view
void cOptionsDialog::on_qpbColumnSetRemove_clicked(bool checked /* false */)
{
	csSettings->RemoveColumnSet(qcbColumnSet->currentText());
	qcbColumnSet->removeItem(qcbColumnSet->currentIndex());
} // on_qpbColumnSetRemove_clicked

// column up button is clicked on in columns view
void cOptionsDialog::on_qpbColumnUp_clicked(bool checked /* false */)
{
	int iIndex;
	cSettings::sColumn scColumn;

	scColumn = GetColumnInfo(qtwColumns->currentItem());
	iIndex = qtwColumns->indexOfTopLevelItem(qtwColumns->currentItem());
	delete qtwColumns->currentItem();
	qtwColumns->setCurrentItem(AddColumnToColumns(scColumn, iIndex - 1));

	SaveOption(Columns);
} // on_qpbColumnUp_clicked

// remove content plugin button is clicked on
void cOptionsDialog::on_qpbRemoveContentPlugin_clicked(bool checked /* false */)
{
	delete qtwContentPlugins->selectedItems().at(0);
} // on_qpbRemoveContentPlugin_clicked

// remove lister plugin button is clicked on
void cOptionsDialog::on_qpbRemoveListerPlugin_clicked(bool checked /* false */)
{
	delete qtwListerPlugins->selectedItems().at(0);
} // on_qpbRemoveListerPlugin_clicked

// prepare context plugin menu
void cOptionsDialog::PrepareColumnsMenu()
{
	QAction *qaAction, *qaNative, *qaPlugins;
	QHash<QString, cContent::sPluginInfo> qhPlugins;

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
	qaAction = qmNative->addAction(qsDATE);
	qaAction->setData(QString("%1|%2").arg(qsNATIVE2).arg(qsDATE));
	qaNative->setMenu(qmNative);

	// plugins sub menu
	qmPlugins = new QMenu(&qmColumns);
	qhPlugins = ccContent->GetPluginsInfo();
	QHashIterator<QString, cContent::sPluginInfo> qhiPlugins(qhPlugins);
	while (qhiPlugins.hasNext()) {
		int iI;
		QAction *qaPlugin;
		QMenu *qmFields;

		qhiPlugins.next();
		qaPlugin = qmPlugins->addAction(qhiPlugins.key());

		qmFields = new QMenu(qmPlugins);
		for (iI = 0; iI < qhiPlugins.value().qlFields.count() && qhiPlugins.value().qlFields.at(iI).iType != ft_fulltext; iI++) {
			// plugin's fields (columns)
			QAction *qaField;

			qaField = qmFields->addAction(qhiPlugins.value().qlFields.at(iI).qsName);

			if (qhiPlugins.value().qlFields.at(iI).qsUnits != "" && qhiPlugins.value().qlFields.at(iI).iType != ft_multiplechoice) {
				// column's units
				int iJ;
				QMenu *qmUnits;
				QStringList qslUnits;

				qslUnits = qhiPlugins.value().qlFields.at(iI).qsUnits.split('|');

				qmUnits = new QMenu(qmFields);
				for (iJ = 0; iJ < qslUnits.count(); iJ++) {
					QAction *qaUnit;

					qaUnit = qmUnits->addAction(qslUnits.at(iJ));
					qaUnit->setData(QString("%1|%2|%3|%4").arg(qsPLUGINS).arg(qhiPlugins.key()).arg(qhiPlugins.value().qlFields.at(iI).qsName).arg(qslUnits.at(iJ)));
				} // for

				qaField->setMenu(qmUnits);
			} else {
				qaField->setData(QString("%1|%2|%3").arg(qsPLUGINS).arg(qhiPlugins.key()).arg(qhiPlugins.value().qlFields.at(iI).qsName));
			} // if else
		} // for
		qaPlugin->setMenu(qmFields);
	} // while
	qaPlugins->setMenu(qmPlugins);
} // PrepareColumnsMenu

// changed width of column
void cOptionsDialog::on_qsbWidth_valueChanged(int val)
{
	SaveOption(Columns);
} // on_qsbWidth_valueChanged

// selected cell changed
void cOptionsDialog::on_qtwColumns_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
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
void cOptionsDialog::on_qtwColumns_itemSelectionChanged()
{
	if (qtwColumns->selectedItems().count() > 0) {
		qpbColumnRemove->setEnabled(true);
	} else {
		qpbColumnRemove->setEnabled(false);
	} // if else
} // on_qtwColumns_itemSelectionChanged

// selected content plugin changed
void cOptionsDialog::on_qtwContentPlugins_itemSelectionChanged()
{
	if (qtwContentPlugins->selectedItems().count() > 0) {
		qpbRemoveContentPlugin->setEnabled(true);
	} else {
		qpbRemoveContentPlugin->setEnabled(false);
	} // if else
} // on_qtwContentPlugins_itemSelectionChanged

// selected lister plugin changed
void cOptionsDialog::on_qtwListerPlugins_itemSelectionChanged()
{
	if (qtwListerPlugins->selectedItems().count() > 0) {
		qpbRemoveListerPlugin->setEnabled(true);
	} else {
		qpbRemoveListerPlugin->setEnabled(false);
	} // if else
} // on_qtwListerPlugins_itemSelectionChanged

// selected shortcut category changed
void cOptionsDialog::on_qtwShortcutCategory_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
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
				if (current == qtwiShortcutShow) {
					// main menu/show
					FillShortcutItems(cSettings::MainMenuCategory, qslSHORTCUTS__MAIN_MENU__SHOW);
				} else {
					if (current == qtwiShortcutConfiguration) {
						// main menu/configuration
						FillShortcutItems(cSettings::MainMenuCategory, qslSHORTCUTS__MAIN_MENU__CONFIGURATION);
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
} // on_qtwShortcutCategory_itemSelectionChanged

// selected shortcut changed
void cOptionsDialog::on_qtwShortcutItem_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
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

// save specific changes into settings file
void cOptionsDialog::SaveOption(const eOption &eoType)
{
	int iI;
	QList<cSettings::sColumn> qlColumns;

	switch (eoType) {
		case Columns:	for (iI = 0; iI < qtwColumns->topLevelItemCount(); iI++) {
								cSettings::sColumn scColumn;

								scColumn = GetColumnInfo(qtwColumns->topLevelItem(iI));
								qlColumns.append(scColumn);
							} // for

							csSettings->CreateColumnSet(qcbColumnSet->currentText(), qlColumns);
							break;
	} // switch
} // SaveOption

// save changes into application's settings file
void cOptionsDialog::SaveOptions()
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

	// plugins
	// content
	csSettings->SetPlugins(cSettings::ContentPlugins, GetPluginList(qtwContentPlugins));
	// lister
	csSettings->SetPlugins(cSettings::ListerPlugins, GetPluginList(qtwListerPlugins));
	// general
	csSettings->SetPluginTimeDisplay(qlePluginTimeDisplay->text());
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

	// others
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
} // SaveOptions