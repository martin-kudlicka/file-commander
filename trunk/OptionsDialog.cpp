#include "OptionsDialog.h"

#include <QFileDialog>
#include <QHeaderView>
#include <QAction>
#include "Plugins/ContPlug.h"

const QString qsCOLUMN_SETS = QT_TR_NOOP("Column sets");
const QString qsCONTENT = QT_TR_NOOP("Content");
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
	qtwiSubChoice  = new QTreeWidgetItem(qtwiChoice);
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
	qtwColumns->verticalHeader()->hide();
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

	for (iI = 0; iI < qlPlugins.count(); iI++) {
		AddPluginIntoTree(qlPlugins.at(iI), qtwTree);
	} // for

	qslHeader.append(tr("File path"));
	qslHeader.append(tr("Enabled"));
	qtwTree->setHeaderLabels(qslHeader);
} // FillPluginsTree

// fill options with set settings
void cOptionsDialog::FillOptions()
{
	QStringList qslHeader;

	// panels
	// column sets
	qtwColumns->setColumnCount(4);
	qslHeader.append(tr("Type"));
	qslHeader.append(tr("Name"));
	qslHeader.append(tr("Unit"));
	qslHeader.append(tr("Show"));
	qtwColumns->setHorizontalHeaderLabels(qslHeader);
	qcbColumnSet->addItems(csSettings->GetColumnSets());

	// plugins
	// content
	FillPluginsTree(csSettings->GetPlugins(cSettings::ContentPlugins), qtwContentPlugins);
} // FillOptions

// column set changed
void cOptionsDialog::on_qcbColumnSet_currentIndexChanged(const QString &text)
{
	int iI;
	QStringList qslColumns;

	qslColumns = csSettings->GetColumnsInSet(text);
	qtwColumns->setRowCount(qslColumns.count());

	for (iI = 0; iI < qslColumns.count(); iI++) {
		cSettings::sColumn scColumn;
		QTableWidgetItem *qtwiItem;

		scColumn = csSettings->GetColumnInfo(text, qslColumns.at(iI));

		// type
		if (scColumn.qsPlugin == qsNO) {
			qtwiItem = new QTableWidgetItem(qsNATIVE);
		} else {
			qtwiItem = new QTableWidgetItem(scColumn.qsPlugin);
		} // if else
		qtwColumns->setItem(iI, iTYPE_COLUMN, qtwiItem);
		// name
		qtwiItem = new QTableWidgetItem(scColumn.qsIdentifier);
		qtwColumns->setItem(iI, iNAME_COLUMN, qtwiItem);
		// unit
		qtwiItem = new QTableWidgetItem(scColumn.qsUnit);
		qtwColumns->setItem(iI, iUNIT_COLUMN, qtwiItem);
		// show
		qtwiItem = new QTableWidgetItem(scColumn.qsName);
		qtwColumns->setItem(iI, iSHOW_COLUMN, qtwiItem);

		qtwColumns->setRowHeight(iI, qtwColumns->font().pointSize() + iROW_SPACE);
	} // for

	qtwColumns->resizeColumnsToContents();
} // on_qcbColumnSet_currentIndexChanged

// column add button is clicked on in columns view
void cOptionsDialog::on_qpbColumnAdd_clicked(bool checked /* false */)
{
	qmColumns.popup(QCursor::pos());
} // on_qpbColumnAdd_clicked

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
	QStringList qslSelection;

	qslSelection = action->data().toString().split('|');
	qtwColumns->setRowCount(qtwColumns->rowCount() + 1);

	if (qslSelection.at(0) == qsNATIVE2) {
		// native column
		QTableWidgetItem *qtwiItem;

		// type
		qtwiItem = new QTableWidgetItem(qsNATIVE);
		qtwColumns->setItem(qtwColumns->rowCount() - 1, iTYPE_COLUMN, qtwiItem);
		// name
		qtwiItem = new QTableWidgetItem(qslSelection.at(1));
		qtwColumns->setItem(qtwColumns->rowCount() - 1, iNAME_COLUMN, qtwiItem);
		// unit
		// show
		qtwiItem = new QTableWidgetItem(qslSelection.at(1));
		qtwColumns->setItem(qtwColumns->rowCount() - 1, iSHOW_COLUMN, qtwiItem);
	} else {
		// plugin column
		QTableWidgetItem *qtwiItem;

		// type
		qtwiItem = new QTableWidgetItem(qslSelection.at(1));
		qtwColumns->setItem(qtwColumns->rowCount() - 1, iTYPE_COLUMN, qtwiItem);
		// name
		qtwiItem = new QTableWidgetItem(qslSelection.at(2));
		qtwColumns->setItem(qtwColumns->rowCount() - 1, iNAME_COLUMN, qtwiItem);
		// unit
		if (qslSelection.count() > 3) {
			qtwiItem = new QTableWidgetItem(qslSelection.at(3));
			qtwColumns->setItem(qtwColumns->rowCount() - 1, iUNIT_COLUMN, qtwiItem);
		} // if
		// show
		qtwiItem = new QTableWidgetItem(qslSelection.at(2));
		qtwColumns->setItem(qtwColumns->rowCount() - 1, iSHOW_COLUMN, qtwiItem);
	} // if else

	qtwColumns->setRowHeight(qtwColumns->rowCount() - 1, qtwColumns->font().pointSize() + iROW_SPACE);
	qtwColumns->resizeColumnsToContents();

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
		if (current->text(0) == qsCOLUMN_SETS) {
			qswChoices->setCurrentIndex(iCOLUMN_SETS_PAGE);
		} else
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