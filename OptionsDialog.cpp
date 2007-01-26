#include "OptionsDialog.h"

#include <QFileDialog>

const QString qsCONTENT = "Content";
const QString qsPLUGINS = "Plugins";

// add another plugin into tree
void cOptionsDialog::AddPluginIntoTree(const cSettings::sPlugin &spPlugin, QTreeWidget *qtwTree)
{
	QTreeWidgetItem *qtwiItem;

	// name
	qtwiItem = new QTreeWidgetItem(qtwTree);
	qtwiItem->setText(0, spPlugin.qsName);
	// enable/disable flag
	if (spPlugin.bEnabled) {
		qtwiItem->setCheckState(1, Qt::Checked);
	} else {
		qtwiItem->setCheckState(1, Qt::Unchecked);
	} // if else
} // AddPluginIntoTree

// build choices tree
void cOptionsDialog::CreateChoices()
{
	QTreeWidgetItem *qtwiChoice, *qtwiSubChoice;

	// plugins
	qtwiChoice = new QTreeWidgetItem(qtwChoices);
	qtwiChoice->setText(0, qsPLUGINS);
	qtwiSubChoice = new QTreeWidgetItem(qtwiChoice);
	qtwiSubChoice->setText(0, qsCONTENT);
} // CreateChoices

// constructor
cOptionsDialog::cOptionsDialog(QWidget *qmwParent, cSettings *csSettings)
{
	setParent(qmwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);

	this->csSettings = csSettings;

	qtwChoices->headerItem()->setHidden(true);
	CreateChoices();
	qtwChoices->expandAll();

	FillOptions();

	// select first item
	qtwChoices->setCurrentItem(qtwChoices->topLevelItem(0));
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
	qtwTree->resizeColumnToContents(0);
} // FillPluginsTree

// fill options with set settings
void cOptionsDialog::FillOptions()
{
	QList<cSettings::sPlugin> qlPlugins;

	// plugins
	// content
	qlPlugins = csSettings->GetPlugins(cSettings::ContentPlugins);
	FillPluginsTree(qlPlugins, qtwContentPlugins);
} // FillOptions

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
	// plugins
	if (current->text(0) == qsPLUGINS) {
		qswChoices->setCurrentIndex(iPLUGINS_PAGE);
	} else
		if (current->text(0) == qsCONTENT) {
			qswChoices->setCurrentIndex(iCONTENT_PLUGINS_PAGE);
		} // if else
} // on_qtwChoices_currentItemChanged

// selected content plugin changed
void cOptionsDialog::on_qtwContentPlugins_itemSelectionChanged()
{
	if (qtwContentPlugins->selectedItems().count() > 0) {
		qpbRemoveContentPlugin->setEnabled(true);
	} else {
		qpbRemoveContentPlugin->setEnabled(false);
	} // if else
} // on_qtwContentPlugins_itemSelectionChanged