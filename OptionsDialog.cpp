#include "OptionsDialog.h"

#include <QFileDialog>
#include <QAction>
#include "Plugins/ContPlug.h"
#include "Options/NewColumnSetDialog.h"
#include <QHeaderView>
#include <QSpinBox>

const QString qsCOLUMN_SETS = QT_TR_NOOP("Column sets");
const QString qsCONTENT = QT_TR_NOOP("Content");
const QString qsDISPLAY = QT_TR_NOOP("Display");
const QString qsNATIVE = QT_TR_NOOP("native");
const QString qsNATIVE2 = QT_TR_NOOP("Native");
const QString qsPANELS = QT_TR_NOOP("Panels");
const QString qsPLUGINS = QT_TR_NOOP("Plugins");

// destructor
cOptionsDialog::~cOptionsDialog()
{
	qmNative->deleteLater();
	qmPlugins->deleteLater();
} // ~cOptionsDialog

// add new column to current column set
int cOptionsDialog::AddColumnToColumns(const cSettings::sColumn &scColumn)
{
	int iI, iToRow;
	QSpinBox *qsbWidth;
	QTableWidgetItem *qtwiItem;

	if (qtwColumns->currentRow() != -1) {
		iToRow = qtwColumns->currentRow();
		qtwColumns->insertRow(qtwColumns->currentRow());
	} else {
		qtwColumns->setRowCount(qtwColumns->rowCount() + 1);
		iToRow = qtwColumns->rowCount() - 1;
	} // if else

	// type
	if (scColumn.qsPlugin == qsNO) {
		qtwiItem = new QTableWidgetItem(qsNATIVE);
	} else {
		qtwiItem = new QTableWidgetItem(scColumn.qsPlugin);
	} // if else
	qtwColumns->setItem(iToRow, iTYPE_COLUMN, qtwiItem);
	// name
	qtwiItem = new QTableWidgetItem(scColumn.qsIdentifier);
	qtwColumns->setItem(iToRow, iNAME_COLUMN, qtwiItem);
	// unit
	qtwiItem = new QTableWidgetItem(scColumn.qsUnit);
	qtwColumns->setItem(iToRow, iUNIT_COLUMN, qtwiItem);
	// show
	qtwiItem = new QTableWidgetItem(scColumn.qsName);
	qtwColumns->setItem(iToRow, iSHOW_COLUMN, qtwiItem);
	// width
	qsbWidth = new QSpinBox();
	qsbWidth->setValue(scColumn.iWidth);
	qtwColumns->setCellWidget(iToRow, iWIDTH_COLUMN, qsbWidth);
	connect(qsbWidth, SIGNAL(valueChanged(int)), SLOT(on_qsbWidth_valueChanged(int)));

	for (iI = iToRow; iI < qtwColumns->rowCount(); iI++) {
		qtwColumns->setRowHeight(iI, qtwColumns->font().pointSize() + iROW_SPACE);
	} // for

	qtwColumns->resizeColumnsToContents();

	return iToRow;
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
} // CreateChoices

// constructor
cOptionsDialog::cOptionsDialog(QWidget *qmwParent, cSettings *csSettings, cContent *ccContent)
{
	setParent(qmwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);

	this->csSettings = csSettings;
	this->ccContent = ccContent;

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
	qtwColumns->setHorizontalHeaderLabels(qslHeader);
	qtwColumns->verticalHeader()->hide();
	qcbColumnSet->addItems(csSettings->GetColumnSets());

	// plugins
	// content
	FillPluginsTree(csSettings->GetPlugins(cSettings::ContentPlugins), qtwContentPlugins);
} // FillOptions

// column set changed
void cOptionsDialog::on_qcbColumnSet_currentIndexChanged(const QString &text)
{
	QStringList qslColumns;

	qslColumns = csSettings->GetColumnsInSet(text);

	qtwColumns->clearContents();
	qtwColumns->setRowCount(0);

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

	qslSelection = action->data().toString().split('|');

	if (qslSelection.at(0) == qsNATIVE2) {
		// native column
		// type
		scColumn.qsPlugin == qsNO;
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

	qtwColumns->setCurrentCell(AddColumnToColumns(scColumn), 0);

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

// column add button is clicked on in columns view
void cOptionsDialog::on_qpbColumnAdd_clicked(bool checked /* false */)
{
	qmColumns.popup(QCursor::pos());
} // on_qpbColumnAdd_clicked

// column down button is clicked on in columns view
void cOptionsDialog::on_qpbColumnDown_clicked(bool checked /* false */)
{
	int iI;

	for (iI = 0; iI < iCOLUMNS; iI++) {
		QTableWidgetItem *qtwiCurrent, *qtwiLower;

		qtwiCurrent = qtwColumns->takeItem(qtwColumns->currentRow(), iI);
		qtwiLower = qtwColumns->takeItem(qtwColumns->currentRow() + 1, iI);
		qtwColumns->setItem(qtwColumns->currentRow() + 1, iI, qtwiCurrent);
		qtwColumns->setItem(qtwColumns->currentRow(), iI, qtwiLower);
	} // for

	qtwColumns->setCurrentCell(qtwColumns->currentRow() + 1, 0);
} // on_qpbColumnDown_clicked

// column remove button is clicked on in columns view
void cOptionsDialog::on_qpbColumnRemove_clicked(bool checked /* false */)
{
	qtwColumns->removeRow(qtwColumns->currentRow());
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
	int iI;

	for (iI = 0; iI < iCOLUMNS; iI++) {
		QTableWidgetItem *qtwiCurrent, *qtwiUpper;

		qtwiCurrent = qtwColumns->takeItem(qtwColumns->currentRow(), iI);
		qtwiUpper = qtwColumns->takeItem(qtwColumns->currentRow() - 1, iI);
		qtwColumns->setItem(qtwColumns->currentRow() - 1, iI, qtwiCurrent);
		qtwColumns->setItem(qtwColumns->currentRow(), iI, qtwiUpper);
	} // for

	qtwColumns->setCurrentCell(qtwColumns->currentRow() - 1, 0);
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
	} else
		if (current->text(0) == qsDISPLAY) {
			qswChoices->setCurrentIndex(iDISPLAY_PAGE);
		} else
			if (current->text(0) == qsCOLUMN_SETS) {
				qswChoices->setCurrentIndex(iCOLUMN_SETS_PAGE);
			} else
				// plugins
				if (current->text(0) == qsPLUGINS) {
					qswChoices->setCurrentIndex(iPLUGINS_PAGE);
				} else
					if (current->text(0) == qsCONTENT) {
						qswChoices->setCurrentIndex(iCONTENT_PLUGINS_PAGE);
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
		for (iI = 0; iI < qhiPlugins.value().qlFields.count(); iI++) {
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
void cOptionsDialog::on_qtwColumns_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous)
{
	if (current && current->row() > 0) {
			qpbColumnUp->setEnabled(true);
	} else {
		qpbColumnUp->setEnabled(false);
	} // if else
	if (current && current->row() < qtwColumns->rowCount() - 1) {
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
		case Columns:	for (iI = 0; iI < qtwColumns->rowCount(); iI++) {
								cSettings::sColumn scColumn;

								// type
								if (qtwColumns->item(iI, iTYPE_COLUMN)->text() == qsNATIVE) {
									scColumn.qsPlugin = qsNO;
								} else {
									scColumn.qsPlugin = qtwColumns->item(iI, iTYPE_COLUMN)->text();
								} // if else
								// name
								scColumn.qsIdentifier = qtwColumns->item(iI, iNAME_COLUMN)->text();
								// unit
								if (qtwColumns->item(iI, iUNIT_COLUMN)) {
									scColumn.qsUnit = qtwColumns->item(iI, iUNIT_COLUMN)->text();
								} // if
								// show
								scColumn.qsName = qtwColumns->item(iI, iSHOW_COLUMN)->text();
								// width
								scColumn.iWidth = static_cast<QSpinBox *>(qtwColumns->cellWidget(iI, iWIDTH_COLUMN))->value();

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
} // SaveOptions