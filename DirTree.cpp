#include "DirTree.h"

#include "Settings.h"
#include <QDir>
#include <QDateTime>

const QString qsDATE = "Date";
const QString qsEXT = "Ext";
const QString qsICON = "Icon";
const QString qsNAME = "Name";
const QString qsNO = "no";
const QString qsPLUGIN = "Plugin";

// refreshes directory content
void cDirTree::RefreshContent()
{
	int iI;
	QDir qdDir(qsPath);
	QFileInfoList qfilFiles;

	// get file list in qsPath
	qfilFiles = qdDir.entryInfoList();

	// go through files and fill dir panel and add them into file list (qhFiles)
	qtwTreeWidget->clear();
	qhFiles.clear();
	for (iI = 0; iI < qfilFiles.count(); iI++) {
		int iJ;
		QTreeWidgetItem *qtwiFile;

		// add to internal file list
		qtwiFile = new QTreeWidgetItem(qtwTreeWidget);
		qhFiles.insert(qtwiFile, qfilFiles.at(iI));

		// fill columns
		for (iJ = 0; iJ < qlColumns.count(); iJ++) {
			if (qlColumns.at(iJ).qsPlugin == qsNO) {
				// native
				if (qlColumns.at(iJ).qsName == qsICON) {
					qtwiFile->setIcon(iJ, qfipIcon.icon(qfilFiles.at(iI)));
				} else
					if (qlColumns.at(iJ).qsName == qsNAME) {
						qtwiFile->setText(iJ, qfilFiles.at(iI).fileName());
					} else
						if (qlColumns.at(iJ).qsName == qsEXT) {
							qtwiFile->setText(iJ, qfilFiles.at(iI).suffix());
						} else
							if (qlColumns.at(iJ).qsName == qsDATE) {
								qtwiFile->setText(iJ, qfilFiles.at(iI).lastModified().toString());
							} // if else
			} else {
				// plugin
				// TODO get plugin column value
			} // if else
		} // for
	} // for
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
		if (scColumn.qsPlugin == qsNO || ccContent->Loaded(scColumn.qsPlugin)) {
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

	// refresh content according to header
	RefreshContent();
} // RefreshHeader

// sets path for view
void cDirTree::SetPath(const QString qsPath)
{
	this->qsPath = qsPath;
	RefreshContent();
} // SetPath