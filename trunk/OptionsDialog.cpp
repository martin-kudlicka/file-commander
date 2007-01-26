#include "OptionsDialog.h"

#include <QTreeWidgetItem>
#include <QHeaderView>

const QString qsCONTENT = "Content";
const QString qsPLUGINS = "Plugins";

// build settings tree with choices
void cOptionsDialog::BuildTree()
{
	QTreeWidgetItem *qtwiChoice, *qtwiSubChoice;

	// plugins
	qtwiChoice = new QTreeWidgetItem(qtwChoices);
	qtwiChoice->setText(0, qsPLUGINS);
	qtwiSubChoice = new QTreeWidgetItem(qtwiChoice);
	qtwiSubChoice->setText(0, qsCONTENT);
} // BuildTree

// constructor
cOptionsDialog::cOptionsDialog(QWidget *qmwParent, cSettings *csSettings)
{
	setParent(qmwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);

	this->csSettings = csSettings;

	qtwChoices->headerItem()->setHidden(true);
	BuildTree();
	qtwChoices->expandAll();

	FillSettings();

	// select first item
	qtwChoices->setCurrentItem(qtwChoices->topLevelItem(0));
} // cConfigurationDialog

// fills plugin information into table
void cOptionsDialog::FillPluginsTable(const QList<cSettings::sPlugin> &qlPlugins, QTableWidget *qtwTable)
{
	int iI;
	QStringList qslHeader;

	qtwTable->setRowCount(qlPlugins.count());
	qtwTable->setColumnCount(2);
	for (iI = 0; iI < qlPlugins.count(); iI++) {
		QTableWidgetItem *qtwiItem;

		// name
		qtwiItem = new QTableWidgetItem(qlPlugins.at(iI).qsName);
		qtwiItem->setFlags(Qt::ItemIsEnabled);
		qtwTable->setItem(iI, 0, qtwiItem);
		// enable/disable flag
		qtwiItem = new QTableWidgetItem();
		qtwiItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
		if (qlPlugins.at(iI).bEnabled) {
			qtwiItem->setCheckState(Qt::Checked);
		} else {
			qtwiItem->setCheckState(Qt::Unchecked);
		} // if else
		qtwTable->setItem(iI, 1, qtwiItem);
	} // for

	qslHeader.append(tr("File path"));
	qslHeader.append(tr("Enabled"));
	qtwTable->setHorizontalHeaderLabels(qslHeader);
	qtwTable->verticalHeader()->hide();
	qtwTable->resizeColumnsToContents();
} // FillPluginsTable

// fill options with set settings
void cOptionsDialog::FillSettings()
{
	QList<cSettings::sPlugin> qlPlugins;

	// plugins
	// content
	qlPlugins = csSettings->GetPlugins(cSettings::ContentPlugins);
	FillPluginsTable(qlPlugins, qtwContentPlugins);
} // FillSettings

// choice change
void cOptionsDialog::on_qtwChoices_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
	// plugins
	if (current->text(0) == qsPLUGINS) {
		qswChoices->setCurrentIndex(iPLUGINS_PAGE);
	} else
		if (current->text(0) == qsCONTENT) {
			qswChoices->setCurrentIndex(iCONTENT_PLUGINS_PAGE);
		} // if else
} // on_qtwChoices_currentItemChanged