#include "ArchiveOperation.h"

#include "FileOperation/FileOperationDialog.h"
#include "FileOperation/CopyMoveDialog.h"
#include "FileOperation.h"

// constructor
cArchiveOperation::cArchiveOperation(QMainWindow *qmwParent, cSettings *csSettings)
{
	this->qmwParent = qmwParent;
	this->csSettings = csSettings;
} // cArchiveOperation

// extract files from archive to local directory
void cArchiveOperation::ExtractFiles(const cPanel::sArchive &saSourceArchive, const QList<tHeaderData> &qlSourceSelected, QString &qsDestination)
{
	cCopyMoveConflict::eChoice ecConflict;
	cCopyMoveDialog ccmdDialog(qmwParent, true);
	cDiskSpace::eChoice ecDiskSpace;
#ifdef Q_WS_WIN
	cPermission::eChoice ecPermission;
#endif
	cRetry::eChoice ecRetry;
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

	// preparation
	emit SetTotalMaximum(qlExtract.count());
	ecConflict = cCopyMove::GetDefaultOverwriteMode(csSettings);
#ifdef Q_WS_WIN
	ecPermission = cCopyMove::GetDefaultReadonlyOverwritePermission(csSettings);
#endif
	ecRetry = cRetry::Ask;
	ecDiskSpace = cDiskSpace::Ask;

	// open archive
	toadArchiveData.ArcName = new char[saSourceArchive.qsArchive.length() + 1];
	strcpy(toadArchiveData.ArcName, saSourceArchive.qsArchive.toLatin1().constData());
	toadArchiveData.OpenMode = PK_OM_EXTRACT;
	hArchive = saSourceArchive.spiPlugin.toaOpenArchive(&toadArchiveData);

#ifdef Q_WS_WIN
	// set callback function
	saSourceArchive.spiPlugin.tspdpSetProcessDataProc(hArchive, &cArchiveOperation::ProcessDataProc);
#endif

	// extract files
	while (!saSourceArchive.spiPlugin.trhReadHeader(hArchive, &thdHeaderData)) {
		int iI;

		for (iI = 0; iI < qlExtract.count(); iI++) {
			if (!strcmp(qlExtract.at(iI).FileName, thdHeaderData.FileName)) {
				// extract file
				cPermission::eChoice ecPermissionCurrent;
				cRetry::eChoice ecRetryCurrent;
				QString qsSource, qsTarget;
				cFileRoutine::sDiskSpace sdsDiskSpace;

				qsSource = saSourceArchive.qsArchive + '/' + thdHeaderData.FileName;
				qsTarget = cFileRoutine::GetWildcardedName(QFileInfo(thdHeaderData.FileName), saSourceArchive.qsPath, qsDestination);

				emit SetSource(qsSource);
				emit SetDestination(qsTarget);

				// check disk space on target
				sdsDiskSpace = cFileRoutine::GetDiskSpace(QFileInfo(qsTarget).path());
				if (sdsDiskSpace.qi64Free < thdHeaderData.UnpSize) {
					cDiskSpace::eChoice ecDiskSpaceCurrent;
					
					ecDiskSpaceCurrent = cDiskSpace::Ask;

					if (ecDiskSpace == cDiskSpace::Ask) {
						// disk space dialog
						cDiskSpace cdsDiskSpace;

						ecDiskSpaceCurrent = cdsDiskSpace.Exec(qsSource, thdHeaderData.UnpSize, sdsDiskSpace.qi64Free);

						// solve
						switch (ecDiskSpaceCurrent) {
							case cDiskSpace::YesToAll:
								ecDiskSpace = cDiskSpace::YesToAll;
								break;
							case cDiskSpace::SkipAll:
								ecDiskSpace = cDiskSpace::SkipAll;
							default:
								;
						} // switch
					} // if

					if (ecDiskSpace == cDiskSpace::SkipAll || ecDiskSpaceCurrent == cDiskSpace::Skip) {
						// skip current file
						continue;
					} else {
						if (ecDiskSpaceCurrent == cDiskSpace::No) {
							// cancel
							break;
						} // if
					} // if else
				} // if

				// conflict solving
				if (QFile::exists(qsTarget)) {
					cCopyMoveConflict::eChoice ecConflictCurrent;

					ecConflictCurrent = cCopyMoveConflict::Ask;

					if (ecConflict == cCopyMoveConflict::Ask) {
						while (true) {
							// no permanent conflict answer yet
							cCopyMoveConflict ccmcConflictDialog;

							// conflict dialog
							ecConflictCurrent = ccmcConflictDialog.Exec(tr("Extract"), QFileInfo(qsSource), QFileInfo(qsTarget));

							// solve conflict
							switch (ecConflictCurrent) {
								case cCopyMoveConflict::SkipAll:
									ecConflict = cCopyMoveConflict::SkipAll;
									break;
								case cCopyMoveConflict::OverwriteAll:
									ecConflict = cCopyMoveConflict::OverwriteAll;
									break;
								case cCopyMoveConflict::OverwriteAllOlder:
									ecConflict = cCopyMoveConflict::OverwriteAllOlder;
								default:
									;
							} // switch

							// rename dialog
							if (ecConflictCurrent == cCopyMoveConflict::Rename) {
								cRename crRenameDialog;
								QString qsNewFilename;

								// rename
								qsNewFilename = crRenameDialog.Exec(QFileInfo(qsTarget).fileName());

								if (!qsNewFilename.isEmpty()) {
									// new file name typed
									qsTarget = QFileInfo(qsTarget).path() + '/' + qsNewFilename;
									if (!QFile::exists(qsTarget)) {
										// rename ok, continue
										break;
									} // if
								} else {
									// cancel
									ecConflictCurrent = cCopyMoveConflict::Cancel;
									break;
								} // if else
							} else {
								// no rename
								break;
							} // if else
						} // while
						if (ecConflictCurrent == cCopyMoveConflict::Cancel) {
							// cancel
							break;
						} // if
					} // if

					if (ecConflict == cCopyMoveConflict::SkipAll || ecConflictCurrent == cCopyMoveConflict::Skip) {
						// skip or skip all -> move onto next file
						continue;
					} else {
						if (ecConflict == cCopyMoveConflict::OverwriteAll || ecConflictCurrent == cCopyMoveConflict::Overwrite) {
							// overwrite, overwrite all -> delete target file
							QFile::remove(qsTarget);
						} else {
							if (ecConflict == cCopyMoveConflict::OverwriteAllOlder) {
								// overwrite all older
								if (QFileInfo(qsSource).lastModified() > QFileInfo(qsTarget).lastModified()) {
									// target file is older -> delete it
									QFile::remove(qsTarget);
								} else {
									// target file is newer -> move onto next file
									continue;
								} // if else
							} // if
						} // if else
					} // if else
				} // if*/

#ifdef Q_WS_WIN
				// check readonly permission
				ecPermissionCurrent = cPermission::Ask;
				if (QFile::permissions(qsTarget) & QFile::ReadOther) {
					QFile::Permissions pPermissions;

					pPermissions = QFile::permissions(qsTarget);

					if (ecPermission == cPermission::Ask) {
						cPermission cpPermissionDialog;

						// show permission dialog
						ecPermissionCurrent = cpPermissionDialog.Exec(QFile(qsTarget).fileName(), tr("is readonly."));

						switch (ecPermissionCurrent) {
							case cPermission::YesToAll:
								ecPermission = cPermission::YesToAll;
								break;
							case cPermission::NoToAll:
								ecPermission = cPermission::NoToAll;
							default:
								;
						} // switch

						if (ecPermissionCurrent == cPermission::Cancel) {
							break;
						} // if
					} // if
					if (ecPermission == cPermission::NoToAll || ecPermissionCurrent == cPermission::No) {
						continue;
					} else {
						// remove target file readonly permission
						pPermissions ^= QFile::ReadOther;
						QFile::setPermissions(qsTarget, pPermissions);
					} // if else
				} // if
#endif

				// extract file
				while (true) {
					bool bExtractSuccess;

					bExtractSuccess = !saSourceArchive.spiPlugin.tpfProcessFile(hArchive, PK_EXTRACT, NULL, qsTarget.toLatin1().data());

					if (!bExtractSuccess) {
						if (ecRetry != cRetry::SkipAll) {
							cRetry crRetryDialog;

							ecRetryCurrent = crRetryDialog.Exec(tr("Can't extract following file:"), qsSource);

							if (ecRetryCurrent == cRetry::SkipAll) {
								// memorize permanent answer
								ecRetry = cRetry::SkipAll;
							} // if
						} // if
						if (ecRetry == cRetry::SkipAll || ecRetryCurrent == cRetry::Skip || ecRetryCurrent == cRetry::Abort) {
							// skip this file
							break;
						} // if
						// else try once more
					} else {
						// successfuly copied/moved
						break;
					} // if else
				} // while

				if (ecRetryCurrent == cRetry::Abort) {
					// process aborted
					break;
				} // if

				emit SetTotalValue(iI);
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
cFileOperation::sObjects cArchiveOperation::GetCount(const QList<tHeaderData> &qlArchive)
{
	int iI;
	cFileOperation::sObjects soCount;

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
	cFileOperation::sObjects soObjects;

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

#ifdef Q_WS_WIN
// callback progress function
int __stdcall cArchiveOperation::ProcessDataProc(char *cFileName, int iSize)
{
	// TODO ProcessDataProc
	return 1;
} // ProcessDataProc
#endif