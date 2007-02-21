#include "FileOperation/CopyMove.h"

#ifdef Q_WS_WIN
#include <Windows.h>
#endif
#include <QDateTime>

// constructor
cCopyMove::cCopyMove(QMainWindow *qmwParent, QHBoxLayout *qhblOperations)
{
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
	bCanceled = false;
	qi64TotalMaximum = 0;
} // cCopyMove

// copy file
void cCopyMove::Copy(const QString &qsSource, const QString &qsDestination, qint64 *qi64TotalValue)
{
	QByteArray qbaData;
	QFile qfDestination, qfSource;
	qint64 qi64CurrentValue;

	qfSource.setFileName(qsSource);
	qfDestination.setFileName(qsDestination);
	qfSource.open(QIODevice::ReadOnly);
	if (ecCurrent == cCopyMoveConflictDialog::Append) {
		qfDestination.open(QIODevice::Append);
	} else {
		qfDestination.open(QIODevice::WriteOnly);
	} // if else

	// set progress bar
	qi64CurrentMaximum = qfSource.size();
	emit SetCurrentMaximum(qi64CurrentMaximum);

	// copy
	qi64CurrentValue = 0;
	while(!qfSource.atEnd() && !bCanceled) {
		qbaData = qfSource.read(qi64BUFFER_SIZE);
		qfDestination.write(qbaData);
		qi64CurrentValue += qbaData.size();
		*qi64TotalValue += qbaData.size();
		emit SetCurrentValue(qi64CurrentValue);
		emit SetTotalValue(*qi64TotalValue);
		QApplication::processEvents();
	} // while
} // Copy

// start of copy or move operation
void cCopyMove::CopyMove(const cFileRoutine::eOperation &eoOperation, const QFileInfoList &qfilSource, const QString &qsDestination, const cFileRoutine::eWindow &eStyle)
{
	this->eoOperation = eoOperation;
	this->qfilSource = qfilSource;
	this->qsDestination = qsDestination;

	// information windows
	if (eStyle == cFileRoutine::ForegroundWindow) {
		ccmdDialog = new cCopyMoveDialog(qmwParent);
		switch (eoOperation) {
			case cFileRoutine::CopyOperation:	ccmdDialog->setWindowTitle(tr("Copy"));
															break;
			case cFileRoutine::MoveOperation:	ccmdDialog->setWindowTitle(tr("Move"));
															break;
		} // switch
		ccmdDialog->setModal(true);
		ccmdDialog->show();
		connect(this, SIGNAL(SetCurrentMaximum(const qint64 &)), ccmdDialog, SLOT(on_cCopyMove_SetCurrentMaximum(const qint64 &)));
		connect(this, SIGNAL(SetCurrentValue(const qint64 &)), ccmdDialog, SLOT(on_cCopyMove_SetCurrentValue(const qint64 &)));
		connect(this, SIGNAL(SetDestination(const QString &)), ccmdDialog, SLOT(on_cCopyMove_SetDestination(const QString &)));
		connect(this, SIGNAL(SetSource(const QString &)), ccmdDialog, SLOT(on_cCopyMove_SetSource(const QString &)));
		connect(this, SIGNAL(SetTotalMaximum(const qint64 &)), ccmdDialog, SLOT(on_cCopyMove_SetTotalMaximum(const qint64 &)));
		connect(this, SIGNAL(SetTotalValue(const qint64 &)), ccmdDialog, SLOT(on_cCopyMove_SetTotalValue(const qint64 &)));
		connect(ccmdDialog, SIGNAL(Cancel()), SLOT(on_ccm_OperationCanceled()));
		connect(ccmdDialog, SIGNAL(Background()), SLOT(on_ccmdCopyMoveDialog_Background()));
		ccmwWidget = NULL;
	} else {
		CreateWidget();
		ccmdDialog = NULL;
	} // if else

	// conclict dialog
	ccmcConflict = new cCopyMoveConflict(qmwParent);
	connect(this, SIGNAL(ShowConflictDialog(const QString &, const QFileInfo &, const QFileInfo &)), ccmcConflict, SLOT(Show(const QString &, const QFileInfo &, const QFileInfo &)));
	connect(ccmcConflict, SIGNAL(Finished(const cCopyMoveConflictDialog::eChoice &)), SLOT(on_ccmcConflict_Finished(const cCopyMoveConflictDialog::eChoice &)));

	start();
} // CopyMove

// create widget for background operation
void cCopyMove::CreateWidget()
{
	ccmwWidget = new cCopyMoveWidget();
	qhblOperations->insertWidget(cFileRoutine::iQUEUED_OPERATION_POSITION, ccmwWidget);
	connect(this, SIGNAL(SetCurrentMaximum(const qint64 &)), ccmwWidget, SLOT(on_cCopyMove_SetCurrentMaximum(const qint64 &)));
	connect(this, SIGNAL(SetCurrentValue(const qint64 &)), ccmwWidget, SLOT(on_cCopyMove_SetCurrentValue(const qint64 &)));
	connect(this, SIGNAL(SetDestination(const QString &)), ccmwWidget, SLOT(on_cCopyMove_SetDestination(const QString &)));
	connect(this, SIGNAL(SetSource(const QString &)), ccmwWidget, SLOT(on_cCopyMove_SetSource(const QString &)));
	connect(this, SIGNAL(SetTotalMaximum(const qint64 &)), ccmwWidget, SLOT(on_cCopyMove_SetTotalMaximum(const qint64 &)));
	connect(this, SIGNAL(SetTotalValue(const qint64 &)), ccmwWidget, SLOT(on_cCopyMove_SetTotalValue(const qint64 &)));
	connect(ccmwWidget, SIGNAL(Cancel()), SLOT(on_ccm_OperationCanceled()));
} // CreateWidget

// return file name modified by wildcard
QString cCopyMove::GetWildcardedName(const QFileInfo &qfiFile, const QString &qsSourcePath, const QString &qsDestination)
{
	int iI;
	QString qsNewFilename;
	QStringList qslOutputs, qslPatterns, qslStrings;

	if (!qsDestination.contains('*') && !qsDestination.contains('?')) {
		// no wildcard in file name
		return qsDestination;
	} // if

	// preparation
	qslPatterns.append(QFileInfo(qsDestination).completeBaseName());
	qslPatterns.append(QFileInfo(qsDestination).suffix());
	qslStrings.append(qfiFile.completeBaseName());
	qslStrings.append(qfiFile.suffix());
	qslOutputs.append("");
	qslOutputs.append("");

	// apply patterns on strings
	for (iI = 0; iI <= 1; iI++) {
		int iPatternPos, iStringPos;

		iStringPos = 0;
		for (iPatternPos = 0; iPatternPos < qslPatterns[iI].length(); iPatternPos++) {
			if (qslPatterns[iI].at(iPatternPos) == '*') {
				// copy rest of the source name
				int iJ;

				for (iJ = iStringPos; iJ < qslStrings[iI].length(); iJ++) {
					qslOutputs[iI] += qslStrings[iI].at(iStringPos);
					iStringPos++;
				} // for
			} else
				if (qslPatterns[iI].at(iPatternPos) == '?') {
					// copy one character of the source name
					if (iStringPos < qslStrings[iI].length()) {
						qslOutputs[iI] += qslStrings[iI].at(iStringPos);
						iStringPos++;
					} // if
				} else {
					// copy character from pattern
					qslOutputs[iI] += qslPatterns[iI].at(iPatternPos);
					iStringPos++;
				} // if else
		} // while
	} // for

	qsNewFilename = qfiFile.path() + '/' + qslOutputs[0] + '.' + qslOutputs[1];
	return QFileInfo(qsDestination).path() + qsNewFilename.mid(qsSourcePath.length());
} // GetWildcardedName

// copy or move operation was canceled
void cCopyMove::on_ccm_OperationCanceled()
{
	bCanceled = true;
} // on_ccm_OperationCanceled

// dialog closed with user response
void cCopyMove::on_ccmcConflict_Finished(const cCopyMoveConflictDialog::eChoice &ecResponse)
{
	ecCurrent = ecResponse;
	qsConflict.release();
} // on_ccmcConflict_Finished

// move operation to background
void cCopyMove::on_ccmdCopyMoveDialog_Background()
{
	CreateWidget();
	emit SetSource(qsSource);
	emit SetDestination(qsTarget);
	emit SetCurrentMaximum(qi64CurrentMaximum);
	emit SetTotalMaximum(qi64TotalMaximum);
	ccmdDialog->deleteLater();
	ccmdDialog = NULL;
} // on_ccmdCopyMoveDialog_Background

// thread code
void cCopyMove::run()
{
	int iI;
	QDir qdDir;
	QFileInfoList qfilSources;
	qint64 qi64TotalValue;
	QString qsSourcePath;

	// gather source files and source path
	qfilSources = cFileRoutine::GetSources(qfilSource);
	qsSourcePath = qfilSource.at(0).path();

	// prepare progress bars
	for (iI = 0; iI < qfilSources.count(); iI++) {
		qi64TotalMaximum += qfilSources.at(iI).size();
	} // for
	emit SetTotalMaximum(qi64TotalMaximum);

	// main process
	ecConflict = cCopyMoveConflictDialog::Nothing;
	qi64TotalValue = 0;
	for (iI = 0; iI < qfilSources.count() && !bCanceled; iI++) {
		// show file names
		qsSource = qfilSources.at(iI).filePath();
		qsTarget = GetWildcardedName(qfilSources.at(iI).filePath(), qsSourcePath, qsDestination);
		if (ccmdDialog) {
			// name with path in dialog
			emit SetSource(qsSource);
			emit SetDestination(qsTarget);
		} else {
			// just name in widget
			emit SetSource(qfilSources.at(iI).fileName());
			emit SetDestination(QFileInfo(qsTarget).fileName());
		} // if else

		if (qfilSources.at(iI).isDir()) {
			qdDir.mkpath(QFileInfo(qsTarget).filePath());
		} else {
			emit SetCurrentValue(0);

			// conflict solving
			ecCurrent = cCopyMoveConflictDialog::Nothing;
			if (QFile::exists(qsTarget)) {
				if (ecConflict == cCopyMoveConflictDialog::Nothing) {
					// no permanent conflict answer yet
					QString qsOperation;

					if (eoOperation == cFileRoutine::CopyOperation) {
						qsOperation = tr("Copy");
					} else {
						qsOperation = tr("Move");
					} // if else
					emit ShowConflictDialog(qsOperation, QFileInfo(qsSource), QFileInfo(qsTarget));
					// wait for answer
					qsConflict.acquire();
					// solve conflict
					// cancel
					if (ecCurrent == cCopyMoveConflictDialog::Cancel) {
						break;
					} // if
					// other permanent answers
					switch (ecCurrent) {
						case cCopyMoveConflictDialog::SkipAll:					ecConflict = cCopyMoveConflictDialog::SkipAll;
																							break;
						case cCopyMoveConflictDialog::OverwriteAll:			ecConflict = cCopyMoveConflictDialog::OverwriteAll;
																							break;
						case cCopyMoveConflictDialog::OverwriteAllOlder:	ecConflict = cCopyMoveConflictDialog::OverwriteAllOlder;
																							break;
					} // switch
				} // if

				if (ecConflict == cCopyMoveConflictDialog::SkipAll || ecCurrent == cCopyMoveConflictDialog::Skip) {
					// skip or skip all -> move onto next file
					continue;
				} else {
					if (ecConflict == cCopyMoveConflictDialog::OverwriteAll || ecCurrent == cCopyMoveConflictDialog::Overwrite) {
						// overwrite, overwrite all -> delete target file
						QFile::remove(qsTarget);
					} else {
						if (ecConflict == cCopyMoveConflictDialog::OverwriteAllOlder) {
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
			} // if

			// create destination path
			qdDir.mkpath(QFileInfo(qsTarget).path());
			switch (eoOperation) {
				case cFileRoutine::CopyOperation:	Copy(qfilSources.at(iI).filePath(), qsTarget, &qi64TotalValue);
																if (bCanceled) {
																	// delete unfinished file
																	QFile::remove(qsTarget);
																} else {
																	// set target permissions as source permissions
#ifdef Q_WS_WIN
																	DWORD dwAttributes;

																	dwAttributes = GetFileAttributes(reinterpret_cast<LPCWSTR>(qfilSources.at(iI).filePath().unicode()));
																	SetFileAttributes(reinterpret_cast<LPCWSTR>(qsTarget.unicode()), dwAttributes | FILE_ATTRIBUTE_ARCHIVE);
#else
																	QFile::setPermissions(qsTarget, QFile::permissions(qfilSources.at(iI).filePath()));
#endif
																} // if else
																break;
				case cFileRoutine::MoveOperation:	if (ecCurrent == cCopyMoveConflictDialog::Append) {
																	Copy(qfilSources.at(iI).filePath(), qsTarget, &qi64TotalValue);
																	QFile::remove(qfilSources.at(iI).filePath());
																} else {
																	QFile::rename(qfilSources.at(iI).filePath(), qsTarget);
																	qi64TotalValue += qfilSources.at(iI).size();
																	emit SetTotalValue(qi64TotalValue);
																} // if else
																if (qsSourcePath != qfilSources.at(iI).path()) {
																	qdDir.rmdir(qfilSources.at(iI).path());
																} // if
																break;
			} // switch
		} // if else
	} // for

	// remove source directory when moving files
	if (eoOperation == cFileRoutine::MoveOperation) {
		qdDir.rmdir(qfilSource.at(0).filePath());
	} // if

	// close dialog or widget
	if (ccmdDialog) {
		ccmdDialog->deleteLater();
	} else {
		ccmwWidget->deleteLater();
	} // if else
	ccmcConflict->deleteLater();
} // run