#include "FileSystem/Archive/ArchiveCommon.h"

#include "Plugins/WCXHead.h"
#include <QtCore/QFileInfo>

// create packer plugins tree to show in QTreeWidget
const void cArchiveCommon::CreatePluginsTree(QTreeWidget *qtwPlugins, cSettings *csSettings)
{
	int iI;
	QList<cSettings::sPlugin> qlPlugins;

	qlPlugins = csSettings->GetPlugins(cSettings::PackerPlugins);
	for (iI = 0; iI < qlPlugins.count(); iI++) {
		const cSettings::sPlugin *spPlugin;

		spPlugin = &qlPlugins.at(iI);
		if (spPlugin->bEnabled) {
			int iJ;
			QStringList qslExtensions;
			QTreeWidgetItem *qtwiPlugin;

			qtwiPlugin = new QTreeWidgetItem(qtwPlugins);
			qtwiPlugin->setText(0, QFileInfo(spPlugin->qsName).completeBaseName());
			qtwiPlugin->setFlags(qtwiPlugin->flags() ^ Qt::ItemIsSelectable);

			qslExtensions = spPlugin->qsExtensions.split(';');
			for (iJ = 0; iJ < qslExtensions.count(); iJ++) {
				QTreeWidgetItem *qtwiExtension;

				qtwiExtension = new QTreeWidgetItem(qtwiPlugin);
				qtwiExtension->setText(0, qslExtensions.at(iJ));
			} // for
		} // if
	} // for

	qtwPlugins->expandAll();
} // CreatePluginsTree

// get error string from error code
const QString cArchiveCommon::GetErrorString(const int &iError)
{
	switch (iError) {
		case E_END_ARCHIVE:
			return QT_TR_NOOP("no more files in archive");
		case E_NO_MEMORY:
			return QT_TR_NOOP("not enough memory");
		case E_BAD_DATA:
			return QT_TR_NOOP("data is bad");
		case E_BAD_ARCHIVE:
			return QT_TR_NOOP("CRC error in archive data");
		case E_UNKNOWN_FORMAT:
			return QT_TR_NOOP("archive format unknown");
		case E_EOPEN:
			return QT_TR_NOOP("Cannot open existing file");
		case E_ECREATE:
			return QT_TR_NOOP("cannot create file");
		case E_ECLOSE:
			return QT_TR_NOOP("error closing file");
		case E_EREAD:
			return QT_TR_NOOP("error reading from file");
		case E_EWRITE:
			return QT_TR_NOOP("error writing to file");
		case E_SMALL_BUF:
			return QT_TR_NOOP("buffer too small");
		case E_EABORTED:
			return QT_TR_NOOP("function aborted by user");
		case E_NO_FILES:
			return QT_TR_NOOP("no files found");
		case E_TOO_MANY_FILES:
			return QT_TR_NOOP("too many files to pack");
		case E_NOT_SUPPORTED:
			return QT_TR_NOOP("function not supported");
	} // switch
	
	return QString();
} // ///< get error string from error code

// find default packer plugin in plugin's tree
const void cArchiveCommon::SelectDefaultPlugin(QTreeWidget *qtwPlugins, const cSettings *csSettings)
{
	cSettings::sDefaultPackerPlugin sdppDefault;
	QList<QTreeWidgetItem *> qlPlugins;
	QTreeWidgetItem *qtwiPlugin;

	sdppDefault = csSettings->GetPackerDefaultPlugin();

	// find plugin
	qlPlugins = qtwPlugins->findItems(sdppDefault.qsPlugin, Qt::MatchExactly);
	if (qlPlugins.count() > 0) {
		// default plugin found
		qtwiPlugin = qlPlugins.at(0);
	} else {
		// default plugin not found
		if (qtwPlugins->topLevelItemCount() > 0) {
			// select first possible plugin
			qtwiPlugin = qtwPlugins->topLevelItem(0);
		} else {
			qtwiPlugin = NULL;
		} // if else
	} // if else

	// find extension
	if (qtwiPlugin) {
		bool bFound;
		int iI;
		QTreeWidgetItem *qtwiExtension;

		bFound = false;
		for (iI = 0; iI < qtwiPlugin->childCount(); iI++) {
			qtwiExtension = qtwiPlugin->child(iI);
			if (qtwiExtension->text(0) == sdppDefault.qsExtension) {
				// default extension found
				bFound = true;
				break;
			} // if
		} // for

		if (!bFound) {
			// no default extension found -> select first possible one
			qtwiExtension = qtwiPlugin->child(0);
		} // if

		qtwPlugins->setCurrentItem(qtwiExtension);
	} // if
} // SelectDefaultPlugin

// show information dialog about processed action
const void cArchiveCommon::ShowInformationDialog(const cInformationDialog *cidInformationDialog, const enum QMessageBox::Icon &iIcon, const QString &qsInformation, QSemaphore *qsPause)
{
	cArchiveCommon cacDialog;

	// show information dialog
	connect(&cacDialog, SIGNAL(ShowInformationDialog2(const enum QMessageBox::Icon &, const QString &)), cidInformationDialog, SLOT(Show(const enum QMessageBox::Icon &, const QString &)));
	emit cacDialog.ShowInformationDialog2(iIcon, qsInformation);
	// wait for answer
	qsPause->acquire();
} // ShowInformationDialog

// converts packer plugin's date time format to QDateTime
const QDateTime cArchiveCommon::ToQDateTime(const int &iDateTime)
{
	QDate qdDate;
	QDateTime qdtDateTime;
	QTime qtTime;

	qdDate.setDate((iDateTime >> 25) + 1980, (iDateTime >> 21) & 0xF, (iDateTime >> 16) & 0x1F);
	qtTime.setHMS((iDateTime >> 11) & 0x1F, (iDateTime >> 5) & 0x3F, (iDateTime & 0x1F) * 2);

	qdtDateTime.setDate(qdDate);
	qdtDateTime.setTime(qtTime);

	return qdtDateTime;
} // ToQDateTime
