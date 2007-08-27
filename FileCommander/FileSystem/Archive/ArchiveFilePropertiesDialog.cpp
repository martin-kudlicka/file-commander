#include "FileSystem/Archive/ArchiveFilePropertiesDialog.h"

#include <QtCore/QFileInfo>
#include "FileSystem/Archive/ArchiveCommon.h"
#include "Plugins/PackerPlugin.h"

cArchiveFilePropertiesDialog::cArchiveFilePropertiesDialog(QMainWindow *qmwParent, const tHeaderData &thdFile)
{
	QStringList qslAttributes;

	setParent(qmwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);

	// set dialog labels
	qlFileName->setText(QFileInfo(thdFile.FileName).fileName());
	qlUnpackedSize->setText(QString("%1 bytes").arg(thdFile.UnpSize));
	qlPackedSize->setText(QString("%1 bytes").arg(thdFile.PackSize));
	qlMethod->setText(QVariant(thdFile.Method).toString());
	qlCRC->setText(QString("%1").arg(thdFile.FileCRC, 0, 16));
	qlDateTime->setText(cArchiveCommon::ToQDateTime(thdFile.FileTime).toString());
	if (thdFile.FileAttr & cPackerPlugin::iREAD_ONLY) {
		qslAttributes.append(tr("read only"));
	} // if
	if (thdFile.FileAttr & cPackerPlugin::iHIDDEN) {
		qslAttributes.append(tr("hidden"));
	} // if
	if (thdFile.FileAttr & cPackerPlugin::iSYSTEM) {
		qslAttributes.append(tr("system"));
	} // if
	if (thdFile.FileAttr & cPackerPlugin::iVOLUME_ID) {
		qslAttributes.append(tr("volume ID"));
	} // if
	if (thdFile.FileAttr & cPackerPlugin::iDIRECTORY) {
		qslAttributes.append(tr("directory"));
	} // if
	if (thdFile.FileAttr & cPackerPlugin::iARCHIVE) {
		qslAttributes.append(tr("archive"));
	} // if
	if (thdFile.FileAttr & cPackerPlugin::iANY) {
		qslAttributes.append(tr("any"));
	} // if
	qlAttributes->setText(qslAttributes.join(", "));
} // cArchiveFilePropertiesDialog
