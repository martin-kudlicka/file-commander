#include "ArchiveOperation.h"

#include "FileOperation/FileOperationDialog.h"
#include "FileOperation/CopyMoveDialog.h"

// constructor
cArchiveOperation::cArchiveOperation(QMainWindow *qmwParent, cSettings *csSettings)
{
	this->qmwParent = qmwParent;
	this->csSettings = csSettings;
} // cArchiveOperation

// extract files from archive to local directory
void cArchiveOperation::ExtractFiles(const cPanel::sArchive &saSourceArchive, const QList<tHeaderData> &qlSourceSelected, QString &qsDestination)
{
	cCopyMoveDialog ccmdDialog(qmwParent, true);
	HANDLE hArchive;
	QList<tHeaderData> qlExtract;
	tHeaderData thdHeaderData;
	tOpenArchiveData toadArchiveData;

	// prepare dialog
	ccmdDialog.setWindowTitle(tr("Extract"));
	ccmdDialog.setModal(true);
	ccmdDialog.show();
	connect(this, SIGNAL(SetCurrentMaximum(const qint64 &)), &ccmdDialog, SLOT(on_cCopyMove_SetCurrentMaximum(const qint64 &)));
	connect(this, SIGNAL(SetCurrentValue(const qint64 &)), &ccmdDialog, SLOT(on_cCopyMove_SetCurrentValue(const qint64 &)));
	connect(this, SIGNAL(SetDestination(const QString &)), &ccmdDialog, SLOT(on_cCopyMove_SetDestination(const QString &)));
	connect(this, SIGNAL(SetSource(const QString &)), &ccmdDialog, SLOT(on_cCopyMove_SetSource(const QString &)));
	connect(this, SIGNAL(SetTotalMaximum(const qint64 &)), &ccmdDialog, SLOT(on_cCopyMove_SetTotalMaximum(const qint64 &)));
	connect(this, SIGNAL(SetTotalValue(const qint64 &)), &ccmdDialog, SLOT(on_cCopyMove_SetTotalValue(const qint64 &)));
	connect(&ccmdDialog, SIGNAL(Cancel()), SLOT(on_ccmdDialog_OperationCanceled()));

	// collect files to extract
	qlExtract = GetAllArchiveFiles(qlSourceSelected, saSourceArchive.qlFiles);

	// open archive
	toadArchiveData.ArcName = new char[saSourceArchive.qsArchive.length() + 1];
	strcpy(toadArchiveData.ArcName, saSourceArchive.qsArchive.toLatin1().constData());
	toadArchiveData.OpenMode = PK_OM_EXTRACT;
	hArchive = saSourceArchive.spiPlugin.toaOpenArchive(&toadArchiveData);

	// extract files
	while (!saSourceArchive.spiPlugin.trhReadHeader(hArchive, &thdHeaderData)) {
		int iI;

		for (iI = 0; iI < qlExtract.count(); iI++) {
			if (!strcmp(qlExtract.at(iI).FileName, thdHeaderData.FileName)) {
				// extract file
			} else {
				// skip file
				saSourceArchive.spiPlugin.tpfProcessFile(hArchive, PK_SKIP, NULL, NULL);
			} // if else
		} // for
	} // while

	// close archive
	saSourceArchive.spiPlugin.tcaCloseArchive(hArchive);

	delete toadArchiveData.ArcName;
} // ExtractFiles

// gather all archive files that are selected
QList<tHeaderData> cArchiveOperation::GetAllArchiveFiles(const QList<tHeaderData> &qlSelected, const QList<tHeaderData> &qlAll)
{
	int iI;
	QList<tHeaderData> qlFiles;

	for (iI = 0; iI < qlSelected.count(); iI++) {
		tHeaderData thdFile = qlSelected.at(iI);

		if (thdFile.FileAttr & cPacker::iDIRECTORY) {
			// selected item is directory -> add all files from this directory
			int iJ;

			for (iJ = 0; iJ < qlAll.count(); iJ++) {
				tHeaderData thdArchiveFile;

				thdArchiveFile = qlAll.at(iJ);
				if (QString(thdArchiveFile.FileName).startsWith(thdFile.FileName) && strcmp(thdArchiveFile.FileName, thdFile.FileName) && !QString(thdArchiveFile.FileName).endsWith("/..")) {
					qlFiles.append(thdArchiveFile);
				} // if
			} // for
		} else {
			// add just file
			qlFiles.append(thdFile);
		} // if else
	} // for

	return qlFiles;
} // GetAllArchiveFiles

// count of objects
cArchiveOperation::sObjects cArchiveOperation::GetCount(const QList<tHeaderData> &qlArchive)
{
	int iI;
	sObjects soCount;

	soCount.Directories = 0;
	soCount.Files = 0;
	for (iI = 0; iI < qlArchive.count(); iI++) {
		if (qlArchive.at(iI).FileAttr & cPacker::iDIRECTORY) {
			soCount.Directories++;
		} else {
			soCount.Files++;
		} // if else
	} // for

	return soCount;
} // GetCount

// operation canceled
void cArchiveOperation::on_ccmdDialog_OperationCanceled()
{
	bCanceled = true;
} // on_ccmdDialog_OperationCanceled

// manipulate with archive files
void cArchiveOperation::Operate(const eOperation &eoOperation, const cPanel::sArchive &saSourceArchive, const QList<tHeaderData> &qlSourceSelected, QString &qsDestination)
{
	cFileOperationDialog cfodDialog(qmwParent, csSettings);
	cFileOperationDialog::eUserAction euaAction;
	QString qsFilter;
	sObjects soObjects;

	if (qlSourceSelected.isEmpty()) {
		// no items selected
		return;
	} // if

	// prepare destination path for dialog
	if (eoOperation == Extract) {
		if (qlSourceSelected.count() == 1) {
			// one file selected
			if (qlSourceSelected.at(0).FileAttr ^ cPacker::iDIRECTORY) {
				qsDestination += '/' + QFileInfo(qlSourceSelected.at(0).FileName).fileName();
			} // if
		} else {
			// many files selected
			qsDestination += "/*.*";
		} // if else
	} // if

	soObjects = GetCount(qlSourceSelected);

	euaAction = cFileOperationDialog::CancelAction;
	switch (eoOperation) {
		case Extract:
			euaAction = cfodDialog.ShowDialog(cFileRoutine::CopyOperation,
														 tr("&Extract %1 files and %2 directories.").arg(soObjects.Files).arg(soObjects.Directories), &qsDestination, &qsFilter, true);
	} // switch

	// correct destination path
	if (eoOperation == Extract) {
		QDir qdDir;

		qdDir.setPath(qsDestination);
		if (qdDir.exists()) {
			qsDestination = QDir::cleanPath(qsDestination) + "/*.*";
		} // if
	} // if

	bCanceled = false;

	// operate
	switch (euaAction) {
		case cFileOperationDialog::CancelAction:
			return;
		case cFileOperationDialog::OkAction:
			switch (eoOperation) {
				case Extract:
					ExtractFiles(saSourceArchive, qlSourceSelected, qsDestination);
			} // switch
	} // switch
} // Operate