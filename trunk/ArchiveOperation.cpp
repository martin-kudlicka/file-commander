#include "ArchiveOperation.h"

#include "FileOperation/FileOperationDialog.h"
#include "FileOperation.h"
#include <QtGui/QMessageBox>
#include "ArchiveOperation/UnpackFilesDialog.h"

bool cArchiveOperation::bCanceled;					///< operation in progress is canceled (static class variable)
qint64 cArchiveOperation::qi64CurrentValue;		///< current file progress (static class variable)
qint64 cArchiveOperation::qi64TotalValue;			///< total progress (static class variable)
cCopyMoveDialog *cArchiveOperation::ccmdDialog;	///< copy/move (progress) dialog (static class variable)

// destructor
cArchiveOperation::~cArchiveOperation()
{
	ccmdDialog->deleteLater();
} // ~cArchiveOperation

// constructor
cArchiveOperation::cArchiveOperation(QMainWindow *qmwParent, cSettings *csSettings)
{
	this->qmwParent = qmwParent;
	this->csSettings = csSettings;

	ccmdDialog = new cCopyMoveDialog(qmwParent, true);
	bCanceled = false;
} // cArchiveOperation

// check existing destination file conflict
cCopyMove::eCheckResult cArchiveOperation::CheckConflict(const QFileInfo &qfiSource, QString *qsTarget, cCopyMoveConflict::eChoice *ecConflictCurrent, cCopyMoveConflict::eChoice *ecConflict)
{
	if (QFile::exists(*qsTarget)) {
		*ecConflictCurrent = cCopyMoveConflict::Ask;

		if (*ecConflict == cCopyMoveConflict::Ask) {
			while (true) {
				// no permanent conflict answer yet
				cCopyMoveConflict ccmcConflictDialog;

				// conflict dialog
				*ecConflictCurrent = ccmcConflictDialog.Exec(tr("Extract"), qfiSource, QFileInfo(*qsTarget));

				// solve conflict
				switch (*ecConflictCurrent) {
					case cCopyMoveConflict::SkipAll:
						*ecConflict = cCopyMoveConflict::SkipAll;
						break;
					case cCopyMoveConflict::OverwriteAll:
						*ecConflict = cCopyMoveConflict::OverwriteAll;
						break;
					case cCopyMoveConflict::OverwriteAllOlder:
						*ecConflict = cCopyMoveConflict::OverwriteAllOlder;
					default:
						;
				} // switch

				// rename dialog
				if (*ecConflictCurrent == cCopyMoveConflict::Rename) {
					cRename crRenameDialog;
					QString qsNewFilename;

					// rename
					qsNewFilename = crRenameDialog.Exec(QFileInfo(*qsTarget).fileName());

					if (!qsNewFilename.isEmpty()) {
						// new file name typed
						*qsTarget = QFileInfo(*qsTarget).path() + '/' + qsNewFilename;
						if (!QFile::exists(*qsTarget)) {
							// rename ok, continue
							break;
						} // if
					} else {
						// cancel
						*ecConflictCurrent = cCopyMoveConflict::Cancel;
						return cCopyMove::Cancel;
					} // if else
				} else {
					// no rename
					return cCopyMove::Cancel;
				} // if else
			} // while
			if (*ecConflictCurrent == cCopyMoveConflict::Cancel) {
				// cancel
				return cCopyMove::Cancel;
			} // if
		} // if

		if (*ecConflict == cCopyMoveConflict::SkipAll || *ecConflictCurrent == cCopyMoveConflict::Skip) {
			// skip or skip all -> move onto next file
			return cCopyMove::NextFile;
		} else {
			if (*ecConflict == cCopyMoveConflict::OverwriteAll || *ecConflictCurrent == cCopyMoveConflict::Overwrite) {
				// overwrite, overwrite all -> delete target file
				QFile::remove(*qsTarget);
			} else {
				if (*ecConflict == cCopyMoveConflict::OverwriteAllOlder) {
					// overwrite all older
					if (qfiSource.lastModified() > QFileInfo(*qsTarget).lastModified()) {
						// target file is older -> delete it
						QFile::remove(*qsTarget);
					} else {
						// target file is newer -> move onto next file
						return cCopyMove::NextFile;
					} // if else
				} // if
			} // if else
		} // if else
	} // if

	return cCopyMove::Nothing;
} // CheckConflict

// continue after unsuccessfull file extraction
void cArchiveOperation::CheckContinue(const QString &qsSource, eContinue *ecContinueCurrent, eContinue *ecContinue)
{
	if (*ecContinue != YesToAll) {
		QMessageBox qmbContinue;
		QPushButton *qpbNo, *qpbYes, *qpbYesToAll;

		// prepare dialog
		qmbContinue.setIcon(QMessageBox::Warning);
		qmbContinue.setWindowTitle(tr("Continue"));
		qmbContinue.setText(tr("Error while extracting file\n%2\nContinue?").arg(qsSource));
		qpbYes = qmbContinue.addButton(tr("&Yes"), QMessageBox::NoRole);
		qpbNo = qmbContinue.addButton(tr("&No"), QMessageBox::YesRole);
		qpbYesToAll = qmbContinue.addButton(tr("Yes to &all"), QMessageBox::YesRole);

		qmbContinue.exec();

		if (qmbContinue.clickedButton() == qpbYes) {
			*ecContinueCurrent = Yes;
		} else {
			if (qmbContinue.clickedButton() == qpbNo) {
				*ecContinueCurrent = No;
			} else {
				*ecContinueCurrent = YesToAll;
			} // if else
		} // if else

		if (*ecContinueCurrent == YesToAll) {
			// memorize permanent answer
			*ecContinue = YesToAll;
		} // if
	} // if
} // CheckContinue

// check disk space
cCopyMove::eCheckResult cArchiveOperation::CheckDiskSpace(const QString &qsSource, const QString &qsTargetPath, const int &iUnpackedSize, cDiskSpace::eChoice *ecDiskSpaceCurrent, cDiskSpace::eChoice *ecDiskSpace)
{
	cFileRoutine::sDiskSpace sdsDiskSpace;

	sdsDiskSpace = cFileRoutine::GetDiskSpace(qsTargetPath);
	if (sdsDiskSpace.qi64Free < iUnpackedSize) {
		*ecDiskSpaceCurrent = cDiskSpace::Ask;

		if (*ecDiskSpace == cDiskSpace::Ask) {
			// disk space dialog
			cDiskSpace cdsDiskSpace;

			*ecDiskSpaceCurrent = cdsDiskSpace.Exec(qsSource, iUnpackedSize, sdsDiskSpace.qi64Free);

			// solve
			switch (*ecDiskSpaceCurrent) {
				case cDiskSpace::YesToAll:
					*ecDiskSpace = cDiskSpace::YesToAll;
					break;
				case cDiskSpace::SkipAll:
					*ecDiskSpace = cDiskSpace::SkipAll;
				default:
					;
			} // switch
		} // if

		if (*ecDiskSpace == cDiskSpace::SkipAll || *ecDiskSpaceCurrent == cDiskSpace::Skip) {
			// skip current file
			return cCopyMove::NextFile;
		} else {
			if (*ecDiskSpaceCurrent == cDiskSpace::No) {
				// cancel
				return cCopyMove::Cancel;
			} // if
		} // if else
	} // if

	return cCopyMove::Nothing;
} // CheckDiskSpace

#ifdef Q_WS_WIN
// check target file permission
cCopyMove::eCheckResult cArchiveOperation::CheckPermission(const QString &qsTarget, cPermission::eChoice *ecPermissionCurrent, cPermission::eChoice *ecPermission)
{
	*ecPermissionCurrent = cPermission::Ask;
	if (QFile::permissions(qsTarget) & QFile::ReadOther) {
		QFile::Permissions pPermissions;

		pPermissions = QFile::permissions(qsTarget);

		if (*ecPermission == cPermission::Ask) {
			cPermission cpPermissionDialog;

			// show permission dialog
			*ecPermissionCurrent = cpPermissionDialog.Exec(QFile(qsTarget).fileName(), tr("is readonly."));

			switch (*ecPermissionCurrent) {
				case cPermission::YesToAll:
					*ecPermission = cPermission::YesToAll;
					break;
				case cPermission::NoToAll:
					*ecPermission = cPermission::NoToAll;
				default:
					;
			} // switch

			if (*ecPermissionCurrent == cPermission::Cancel) {
				return cCopyMove::Cancel;
			} // if
		} // if
		if (*ecPermission == cPermission::NoToAll || *ecPermissionCurrent == cPermission::No) {
			return cCopyMove::NextFile;
		} else {
			// remove target file readonly permission
			pPermissions ^= QFile::ReadOther;
			QFile::setPermissions(qsTarget, pPermissions);
		} // if else
	} // if

	return cCopyMove::Nothing;
} // CheckPermission
#endif

// extract files from archive to local directory
void cArchiveOperation::ExtractFiles(const sArchive &saSourceArchive, const QList<tHeaderData> &qlSourceSelected, QString &qsDestination, const QString &qsFilter /* QString() */, const bool &bFullPath /* true */)
{
	eContinue ecContinue;
	cCopyMoveConflict::eChoice ecConflict;
	cDiskSpace::eChoice ecDiskSpace;
#ifdef Q_WS_WIN
	cPermission::eChoice ecPermission;
#endif
	HANDLE hArchive;
	int iI;
	qint64 qi64TotalMaximum;
	QList<tHeaderData> qlExtract;
	tOpenArchiveData toadArchiveData;

	// prepare dialog
	ccmdDialog->setWindowTitle(tr("Extract"));
	ccmdDialog->setModal(true);
	ccmdDialog->show();
	connect(ccmdDialog, SIGNAL(Cancel()), SLOT(on_ccmdDialog_OperationCanceled()));

	// collect files to extract
	qlExtract = GetAllArchiveFiles(qlSourceSelected, saSourceArchive.qlFiles);
	qi64TotalValue = 0;
	qi64TotalMaximum = 0;
	for (iI = 0; iI < qlExtract.count(); iI++) {
		if (!(qlExtract.at(iI).FileAttr & cPackerPlugin::iDIRECTORY)) {
			qi64TotalMaximum += qlExtract.at(iI).UnpSize;
		} // if
	} // for

	// preparation
	ccmdDialog->qpbTotal->setMaximum(qi64TotalMaximum);
	ecConflict = cCopyMove::GetDefaultOverwriteMode(csSettings);
#ifdef Q_WS_WIN
	ecPermission = cCopyMove::GetDefaultReadonlyOverwritePermission(csSettings);
#endif
	ecContinue = Ask;
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
	while (true) {
		eContinue ecContinueCurrent;
		cCopyMoveConflict::eChoice ecConflictCurrent;
		cDiskSpace::eChoice ecDiskSpaceCurrent;
		cPermission::eChoice ecPermissionCurrent;
		tHeaderData thdHeaderData;

		memset(&thdHeaderData, 0, sizeof(tHeaderData));
		if (saSourceArchive.spiPlugin.trhReadHeader(hArchive, &thdHeaderData)) {
			// no other file in archive
			break;
		} // if

		for (iI = 0; iI < qlExtract.count(); iI++) {
			if (!strcmp(qlExtract.at(iI).FileName, thdHeaderData.FileName) && cFileRoutine::SuitsFilter(thdHeaderData.FileName, qsFilter)) {
				// extract file
				QString qsSource, qsTarget;

				qsSource = saSourceArchive.qsArchive + '/' + thdHeaderData.FileName;
				if (bFullPath) {
					qsTarget = thdHeaderData.FileName;
				} else {
					qsTarget = QFileInfo(thdHeaderData.FileName).fileName();
				} // if else
				qsTarget = cFileRoutine::GetWildcardedName(QFileInfo(qsTarget), saSourceArchive.qsPath, qsDestination);

				ccmdDialog->qlSource->setText(qsSource);
				ccmdDialog->qlDestination->setText(qsTarget);
				qi64CurrentValue = 0;
				ccmdDialog->qpbCurrent->setMaximum(thdHeaderData.UnpSize);

				if (thdHeaderData.FileAttr & cPackerPlugin::iDIRECTORY) {
					if (bFullPath) {
						QDir qdDir;

						qdDir.mkpath(qsTarget);
					} // if
					saSourceArchive.spiPlugin.tpfProcessFile(hArchive, PK_SKIP, NULL, NULL);
				} else {
					cCopyMove::eCheckResult ecrCheck;
					int iExtract;
					QDir qdDir;

					// check disk space on target
					ecrCheck = CheckDiskSpace(qsSource, QFileInfo(qsTarget).path(), thdHeaderData.UnpSize, &ecDiskSpaceCurrent, &ecDiskSpace);
					if (ecrCheck == cCopyMove::NextFile) {
						continue;
					} else {
						if (ecrCheck == cCopyMove::Cancel) {
							break;
						} // if
					} // if else

					// conflict solving
					ecrCheck = CheckConflict(QFileInfo(qsSource), &qsTarget, &ecConflictCurrent, &ecConflict);
					if (ecrCheck == cCopyMove::NextFile) {
						continue;
					} else {
						if (ecrCheck == cCopyMove::Cancel) {
							break;
						} // if
					} // if else

#ifdef Q_WS_WIN
					// check readonly permission
					ecrCheck = CheckPermission(qsTarget, &ecPermissionCurrent, &ecPermission);
					if (ecrCheck == cCopyMove::NextFile) {
						continue;
					} else {
						if (ecrCheck == cCopyMove::Cancel) {
							break;
						} // if
					} // if else
#endif

					// make destination path if doesn't exist
					qdDir.mkpath(QFileInfo(qsTarget).path());

					// extract file
					iExtract = saSourceArchive.spiPlugin.tpfProcessFile(hArchive, PK_EXTRACT, NULL, qsTarget.toLatin1().data());

					if (iExtract) {
						if (bCanceled) {
							// user abort
							ecContinueCurrent = No;
						} else {
							// other fault
							CheckContinue(qsSource, &ecContinueCurrent, &ecContinue);
						} // if else
					} // if
				} // if else

				// extracted or interrupted
				break;
			} else {
				// skip file
				if (iI == qlExtract.count() - 1) {
					saSourceArchive.spiPlugin.tpfProcessFile(hArchive, PK_SKIP, NULL, NULL);
				} // if
			} // if else
		} // for

		if (ecDiskSpaceCurrent == cDiskSpace::No || ecConflictCurrent == cCopyMoveConflict::Cancel || ecPermissionCurrent == cPermission::Cancel || ecContinueCurrent == No) {
			// process aborted
			break;
		} // if
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

		qlFiles.append(thdFile);

		if (thdFile.FileAttr & cPackerPlugin::iDIRECTORY) {
			// selected item is directory -> add all files from this directory too
			int iJ;

			for (iJ = 0; iJ < qlAll.count(); iJ++) {
				tHeaderData thdArchiveFile;

				thdArchiveFile = qlAll.at(iJ);
				if (QString(thdArchiveFile.FileName).startsWith(thdFile.FileName) && strcmp(thdArchiveFile.FileName, thdFile.FileName) && !QString(thdArchiveFile.FileName).endsWith("/..")) {
					qlFiles.append(thdArchiveFile);
				} // if
			} // for
		} // if
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
		if (qlArchive.at(iI).FileAttr & cPackerPlugin::iDIRECTORY) {
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

// try to open archive file
bool cArchiveOperation::OpenArchive(const QFileInfo &qfiFile, sArchive *saArchive, cSettings *csSettings, cPackerPlugin *cppPackerPlugin)
{
	int iI;
	QHash<QString, cPackerPlugin::sPluginInfo> qhPluginsInfo;
	QList<cSettings::sPlugin> qlPackerPlugins;

	qhPluginsInfo = cppPackerPlugin->GetPluginsInfo();
	qlPackerPlugins = csSettings->GetPlugins(cSettings::PackerPlugins);

	for (iI = 0; iI < qlPackerPlugins.count(); iI++) {
		if (qlPackerPlugins.at(iI).bEnabled) {
			QStringList qslExtensions;

			qslExtensions = qlPackerPlugins.at(iI).qsExtensions.split(';');
			if (qslExtensions.contains(qfiFile.suffix(), Qt::CaseInsensitive)) {
				HANDLE hArchive;
				tOpenArchiveData toadArchiveData;

				toadArchiveData.ArcName = new char[qfiFile.filePath().length() + 1];
				strcpy(toadArchiveData.ArcName, QDir::toNativeSeparators(qfiFile.filePath()).toLatin1().constData());
				toadArchiveData.OpenMode = PK_OM_LIST;

				hArchive = qhPluginsInfo.value(QFileInfo(qlPackerPlugins.at(iI).qsName).fileName()).toaOpenArchive(&toadArchiveData);

				delete toadArchiveData.ArcName;

				if (hArchive) {
					// archive opened successfully
					saArchive->spiPlugin = qhPluginsInfo.value(QFileInfo(qlPackerPlugins.at(iI).qsName).fileName());
					saArchive->qsArchive = qfiFile.filePath();
					// read archive files
					saArchive->qlFiles = ReadArchiveFiles(hArchive, saArchive->spiPlugin);
					saArchive->qsPath.clear();
					// close archive
					qhPluginsInfo.value(QFileInfo(qlPackerPlugins.at(iI).qsName).fileName()).tcaCloseArchive(hArchive);

					return true;
				} // if
			} // if
		} // if
	} // for

	return false;
} // OpenArchive

// manipulate with archive files
void cArchiveOperation::Operate(const eOperation &eoOperation, const sArchive &saSourceArchive, const QList<tHeaderData> &qlSourceSelected, QString &qsDestination)
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
			if (!(qlSourceSelected.at(0).FileAttr & cPackerPlugin::iDIRECTORY)) {
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

	// operate
	switch (euaAction) {
		case cFileOperationDialog::CancelAction:
			return;
		case cFileOperationDialog::OkAction:
			switch (eoOperation) {
				case Extract:
					ExtractFiles(saSourceArchive, qlSourceSelected, qsDestination);
			} // switch
		default:
			;
	} // switch
} // Operate

#ifdef Q_WS_WIN
// callback progress function
int __stdcall cArchiveOperation::ProcessDataProc(char *cFileName, int iSize)
{
	qi64CurrentValue += iSize;
	qi64TotalValue += iSize;
	ccmdDialog->qpbCurrent->setValue(qi64CurrentValue);
	ccmdDialog->qpbTotal->setValue(qi64TotalValue);
	QApplication::processEvents();

	return !bCanceled;
} // ProcessDataProc
#endif

// read archive contents
QList<tHeaderData> cArchiveOperation::ReadArchiveFiles(const HANDLE &hArchive, const cPackerPlugin::sPluginInfo &spiPlugin)
{
	QList<QString> qlDirectories;
	QList<tHeaderData> qlFiles;
	tHeaderData thdHeaderData;

	// archive root ".." directory
	strcpy(thdHeaderData.FileName, "..");
	thdHeaderData.FileTime = ToPackerDateTime(QDateTime::currentDateTime());
	thdHeaderData.FileAttr = cPackerPlugin::iDIRECTORY;
	qlFiles.append(thdHeaderData);

	qlDirectories.append(".");
	memset(&thdHeaderData, 0, sizeof(tHeaderData));
	while (!spiPlugin.trhReadHeader(hArchive, &thdHeaderData)) {
		if (!qlDirectories.contains(QDir::toNativeSeparators(QFileInfo(thdHeaderData.FileName).path()))
			|| thdHeaderData.FileAttr & cPackerPlugin::iDIRECTORY && !qlDirectories.contains(QDir::toNativeSeparators(thdHeaderData.FileName))) {
			// create directory and ".." directory in each archive directory
			QString qsDirectory, qsDotDot;
			tHeaderData thdDirectory, thdDotDot;

			if (thdHeaderData.FileAttr & cPackerPlugin::iDIRECTORY) {
				qsDirectory = thdHeaderData.FileName;
			} else {
				qsDirectory = QFileInfo(thdHeaderData.FileName).path();
			} // if else

			qsDotDot = qsDirectory + "/..";

			// create directory
			strcpy(thdDirectory.FileName, qsDirectory.toLatin1().constData());
			thdDirectory.FileTime = ToPackerDateTime(QDateTime::currentDateTime());
			thdDirectory.FileAttr = cPackerPlugin::iDIRECTORY;
			qlFiles.append(thdDirectory);

			qlDirectories.append(QDir::toNativeSeparators(qsDirectory));

			// create ".." directory
			strcpy(thdDotDot.FileName, qsDotDot.toLatin1().constData());
			thdDotDot.FileTime = ToPackerDateTime(QDateTime::currentDateTime());
			thdDotDot.FileAttr = cPackerPlugin::iDIRECTORY;
			qlFiles.append(thdDotDot);
		} // if

		if (!(thdHeaderData.FileAttr & cPackerPlugin::iDIRECTORY)) {
			qlFiles.append(thdHeaderData);
		} // if

		spiPlugin.tpfProcessFile(hArchive, PK_SKIP, NULL, NULL);
		memset(&thdHeaderData, 0, sizeof(tHeaderData));
	} // while

	return qlFiles;
} // ReadArchiveFiles

// select all root archive files and directories
QList<tHeaderData> cArchiveOperation::SelectAllRoot(const QList<tHeaderData> &qlArchive)
{
	int iI;
	QList<tHeaderData> qlRootFiles;

	for (iI = 0; iI < qlArchive.count(); iI++) {
		if (QFileInfo(qlArchive.at(iI).FileName).path() == "." && strcmp(qlArchive.at(iI).FileName, "..")) {
			qlRootFiles.append(qlArchive.at(iI));
		} // if
	} // for

	return qlRootFiles;
} // SelectAllRoot

// converts Qt's date time format to packer's
int cArchiveOperation::ToPackerDateTime(const QDateTime &qdtDateTime)
{
	int iDateTime;

	iDateTime = (qdtDateTime.date().year() - 1980) << 25;
	iDateTime |= qdtDateTime.date().month() << 21;
	iDateTime |= qdtDateTime.date().day() << 16;
	iDateTime |= qdtDateTime.time().hour() << 11;
	iDateTime |= qdtDateTime.time().minute() << 5;
	iDateTime |= qdtDateTime.time().second() / 2;

	return iDateTime;
} // ToPackerDateTime

// converts packer plugin's date time format to QDateTime
QDateTime cArchiveOperation::ToQDateTime(const int &iDateTime)
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

// unpack selected archives
void cArchiveOperation::UnpackSelectedFiles(const QFileInfoList &qfilArchives, const QString &qsDestination, cPackerPlugin *cppPackerPlugin)
{
	cUnpackFilesDialog cufdUnpackDialog(qmwParent, qsDestination, csSettings);

	if (qfilArchives.isEmpty()) {
		// no archive to unpack
		return;
	} // if

	if (cufdUnpackDialog.exec() == QDialog::Accepted) {
		int iI;

		for (iI = 0; iI < qfilArchives.count(); iI++) {
			sArchive saArchive;

			if (OpenArchive(qfilArchives.at(iI), &saArchive, csSettings, cppPackerPlugin)) {
				// archive file supported
				QString qsPath;

				qsPath = QDir::cleanPath(qsDestination);
				if (cufdUnpackDialog.qcbSeparatedSubdirectory->isChecked()) {
					QDir qdDir;

					qsPath += '/' + qfilArchives.at(iI).completeBaseName();
					qdDir.mkpath(qsPath);
				} // if
				qsPath += "/*.*";

				ExtractFiles(saArchive, SelectAllRoot(saArchive.qlFiles), qsPath, cufdUnpackDialog.qcbFilter->currentText(), cufdUnpackDialog.qcbUnpackWithFullPath->isChecked());
			} else {
				// unsupported archive file or not archive file
				QMessageBox::warning(qmwParent, tr("Unpack archive"), tr("Archive %1 not supported.").arg(qfilArchives.at(iI).fileName()));
			} // if else
		} // for
	} // if
} // UnpackSelectedFiles
