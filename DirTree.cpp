#include "DirTree.h"

#include "Settings.h"

const QString qsNAME = "Name";
const QString cNO = "no";
const QString qsPLUGIN = "Plugin";

// refreshes directory content
void cDirTree::RefreshContent()
{
	// TODO RefreshContent
} // RefreshContent

// refreshes header and directory content
void cDirTree::RefreshHeader()
{
	int iI;
	QStringList qslColumns;

	if (etpPanel == TreePanelLeft) {
		qsSettings->beginGroup(qsMAIN_WINDOW__LEFT_PANEL__COLUMNS__);
	} else {
		qsSettings->beginGroup(qsMAIN_WINDOW__RIGHT_PANEL__COLUMNS__);
	} // if else
	qslColumns = qsSettings->childGroups();

	qlColumns.clear();
	// go through columns
	for (iI = 0; iI < qslColumns.count(); iI++) {
		sColumn scColumn;

		qsSettings->beginGroup(qslColumns.at(iI));
		scColumn.qsPlugin = qsSettings->value(qsPLUGIN).toString();
		scColumn.qsName = qsSettings->value(qsNAME).toString();
		// test if is native or plugin and loaded
		if (scColumn.qsPlugin == cNO || ccContent->Loaded(scColumn.qsPlugin)) {
			// ok -> add to column list
			qlColumns.append(scColumn);
		} // if
		qsSettings->endGroup();
	} // for

	qsSettings->endGroup();

	// set header according to qlColumns list
	qtwiHeader = new QTreeWidgetItem();	// TODO need do destroy this first?
	for (iI = 0; iI < qlColumns.count(); iI++) {
		qtwiHeader->setText(iI, qlColumns.at(iI).qsName);
	} // for
	qtwTreeWidget->setHeaderItem(qtwiHeader);

	RefreshContent();
} // RefreshHeader

// sets path for view
void cDirTree::SetPath(const QString qsPath)
{
	this->qsPath = qsPath;
	RefreshContent();
} // SetPath