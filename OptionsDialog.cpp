#include "OptionsDialog.h"

#include <QFileDialog>
#include <QAction>
#include "Plugins/ContPlug.h"
#include "Options/NewColumnSetDialog.h"
#include <QSpinBox>
#include <QHeaderView>

const QString qsCOLUMN_SETS = QT_TR_NOOP("Column sets");
const QString qsCONFIRMATION = QT_TR_NOOP("Confirmation");
const QString qsCONTENT = QT_TR_NOOP("Content");
const QString qsDISPLAY = QT_TR_NOOP("Display");
const QString qsNATIVE = QT_TR_NOOP("native");
const QString qsNATIVE2 = QT_TR_NOOP("Native");
const QString qsOTHERS = QT_TR_NOOP("Others");
const QString qsPANELS = QT_TR_NOOP("Panels");
const QString qsPLUGINS = QT_TR_NOOP("Plugins");

// destructor
cOptionsDialog::~cOptionsDialog()
{
	qmNative->deleteLater();
	qmPlugins->deleteLater();
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
	qsbWidth = new QSpinBox();
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

// build choices tree
void cOptionsDialog::CreateChoices()
{
	QTreeWidgetItem *qtwiChoice, *qtwiSubChoice;

	// panels
	qtwiChoice = new QTreeWidgetItem(qtwChoices);
	qtwiChoice->setText(0, qsPANELS);
	qtwiSubChoice = new QTreeWidgetItem(qtwiChoice);
	qtwiSubChoice->setText(0, qsDISPLAY);
	qtwiSubChoice = new QTreeWidgetItem(qtwiChoice);
	qtwiSubChoice->setText(0, qsCOLUMN_SETS);

	// plugins
	qtwiChoice = new QTreeWidgetItem(qtwChoices);
	qtwiChoice->setText(0, qsPLUGINS);
	qtwiSubChoice = new QTreeWidgetItem(qtwiChoice);
	qtwiSubChoice->setText(0, qsCONTENT);

	// others
	qtwiChoice = new QTreeWidgetItem(qtwChoices);
	qtwiChoice->setText(0, qsOTHERS);
	qtwiSubChoice = new QTreeWidgetItem(qtwiChoice);
	qtwiSubChoice->setText(0, qsCONFIRMATION);
} // CreateChoices

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

	PrepareColumnsMenu();

	// left menu
	qtwChoices->headerItem()->setHidden(true);
	CreateChoices();
	qtwChoices->expandAll();

	// submenus
	FillOptions();

	// select first item
	qtwChoices->setCurrentItem(qtwChoices->topLevelItem(0));

	// connections
	connect(&qmColumns, SIGNAL(triggered(QAction *)), SLOT(on_qmColumns_triggered(QAction *)));
} // cConfigurationDialog

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

// fill options with set settings
void cOptionsDialog::FillOptions()
{
	QString qsValue;
	QStringList qslHeader;

	// panels
	// display
	qsValue = csSettings->GetValue(cSettings::ShowSystemFiles);
	if (qsValue == qsTRUE) {
		qcbShowSystemFiles->setChecked(true);
	} // if
	qsValue = csSettings->GetValue(cSettings::ShowHiddenFiles);
	if (qsValue == qsTRUE) {
		qcbShowHiddenFiles->setChecked(true);
	} // if
	// column sets
	qtwColumns->setColumnCount(iCOLUMNS);
	qslHeader.append(tr("Type"));
	qslHeader.append(tr("Name"));
	qslHeader.append(tr("Unit"));
	qslHeader.append(tr("Show"));
	qslHeader.append(tr("Width"));
	qtwColumns->setHeaderLabels(qslHeader);
	qtwColumns->header()->setResizeMode(iNAME_COLUMN, QHeaderView::Stretch);
	qtwColumns->header()->setResizeMode(iSHOW_COLUMN, QHeaderView::Stretch);
	qtwColumns->header()->setStretchLastSection(false);
	qcbColumnSet->addItems(csSettings->GetColumnSets());

	// plugins
	qlePluginTimeDisplay->setText(csSettings->GetPluginTimeDisplay());
	// content
	FillPluginsTree(csSettings->GetPlugins(cSettings::ContentPlugins), qtwContentPlugins);

	// others
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
} // FillOptions

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
void cOptionsDialog::on_qdbbRespond_accepted()
{
	SaveOptions();
	accept();
} // on_qdbbRespond_accepted

void cOptionsDialog::on_qdbbRespond_rejected()
{
	csSettings->RestoreSettings(qlOldOptions);
	reject();
} // on_qdbbRespond_rejected

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

// choice change
void cOptionsDialog::on_qtwChoices_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
	// panels
	if (current->text(0) == qsPANELS) {
		qswChoices->setCurrentIndex(iPANELS_PAGE);
	} else {
		if (current->text(0) == qsDISPLAY) {
			qswChoices->setCurrentIndex(iDISPLAY_PAGE);
		} else {
			if (current->text(0) == qsCOLUMN_SETS) {
				qswChoices->setCurrentIndex(iCOLUMN_SETS_PAGE);
			} else {
				// plugins
				if (current->text(0) == qsPLUGINS) {
					qswChoices->setCurrentIndex(iPLUGINS_PAGE);
				} else {
					if (current->text(0) == qsCONTENT) {
						qswChoices->setCurrentIndex(iCONTENT_PLUGINS_PAGE);
					} else {
						// others
						if (current->text(0) == qsOTHERS) {
							qswChoices->setCurrentIndex(iOTHERS_PAGE);
						} else {
							if (current->text(0) == qsCONFIRMATION) {
								qswChoices->setCurrentIndex(iCONFIRMATION_PAGE);
							} // if
						} // if else
					} // if else
				} // if else
			} // if else
		} // if else
	} // if else
} // on_qtwChoices_currentItemChanged

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
	int iI;
	QList<cSettings::sPlugin> qlPlugins;
	QString qsValue;

	// panels
	// display
	if (qcbShowSystemFiles->isChecked()) {
		qsValue = qsTRUE;
	} else {
		qsValue = qsFALSE;
	} // if else
	csSettings->SetValue(cSettings::ShowSystemFiles, qsValue);
	if (qcbShowHiddenFiles->isChecked()) {
		qsValue = qsTRUE;
	} else {
		qsValue = qsFALSE;
	} // if else
	csSettings->SetValue(cSettings::ShowHiddenFiles, qsValue);

	// plugins
	csSettings->SetPluginTimeDisplay(qlePluginTimeDisplay->text());
	// content
	for (iI = 0; iI < qtwContentPlugins->topLevelItemCount(); iI++) {
		cSettings::sPlugin spPlugin;

		spPlugin.qsName = qtwContentPlugins->topLevelItem(iI)->text(iPLUGIN_NAME_COLUMN);
		if (qtwContentPlugins->topLevelItem(iI)->checkState(iPLUGIN_ENABLED_COLUMN) == Qt::Checked) {
			spPlugin.bEnabled = true;
		} else {
			spPlugin.bEnabled = false;
		} // if else

		qlPlugins.append(spPlugin);
	} // for
	csSettings->SetPlugins(cSettings::ContentPlugins, qlPlugins);

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