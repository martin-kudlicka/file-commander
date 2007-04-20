#include "FileOperation/Delete.h"

#ifdef Q_WS_WIN
#include <Windows.h>
#endif

// constructor
cDelete::cDelete(QMainWindow *qmwParent, QHBoxLayout *qhblOperations
#ifdef Q_WS_WIN
					  , cSettings *csSettings)
#else
					  )
#endif
{
	this->qmwParent = qmwParent;
	this->qhblOperations = qhblOperations;
#ifdef Q_WS_WIN
	this->csSettings = csSettings;
#endif
	bCanceled = false;
	qi64TotalMaximum = 0;
} // cDelete

// create widget for background operation
void cDelete::CreateWidget()
{
	cdwWidget = new cDeleteWidget();
	qhblOperations->insertWidget(cFileRoutine::iQUEUED_OPERATION_POSITION, cdwWidget);
	connect(this, SIGNAL(SetSource(const QString &)), cdwWidget, SLOT(on_cDeleteWidget_SetSource(const QString &)));
	connect(this, SIGNAL(SetTotalMaximum(const qint64 &)), cdwWidget, SLOT(on_cDeleteWidget_SetTotalMaximum(const qint64 &)));
	connect(this, SIGNAL(SetTotalValue(const qint64 &)), cdwWidget, SLOT(on_cDeleteWidget_SetTotalValue(const qint64 &)));
	connect(cdwWidget, SIGNAL(Cancel()), SLOT(on_cd_OperationCanceled()));
} // CreateWidget

// start of delete operation
void cDelete::Delete(const QFileInfoList &qfilSource, const QString &qsFilter, const cFileRoutine::eWindow &eStyle)
{
	this->qfilSource = qfilSource;
	this->qsFilter = qsFilter;

	if (eStyle == cFileRoutine::ForegroundWindow) {
		cddDialog = new cDeleteDialog(qmwParent);
		cddDialog->setModal(true);
		cddDialog->show();
		connect(this, SIGNAL(SetSource(const QString &)), cddDialog, SLOT(on_cDeleteDialog_SetSource(const QString &)));
		connect(this, SIGNAL(SetTotalMaximum(const qint64 &)), cddDialog, SLOT(on_cDeleteDialog_SetTotalMaximum(const qint64 &)));
		connect(this, SIGNAL(SetTotalValue(const qint64 &)), cddDialog, SLOT(on_cDeleteDialog_SetTotalValue(const qint64 &)));
		connect(cddDialog, SIGNAL(Cancel()), SLOT(on_cd_OperationCanceled()));
		connect(cddDialog, SIGNAL(Background()), SLOT(on_cdDeleteDialog_Background()));
		cdwWidget = NULL;
	} else {
		CreateWidget();
		cddDialog = NULL;
	} // if else

#ifdef Q_WS_WIN
	// permission dialog
	cpPermission = new cPermission(qmwParent);
	connect(this, SIGNAL(ShowPermissionDialog(const QString &, const QString &)), cpPermission, SLOT(Show(const QString &, const QString &)));
	connect(cpPermission, SIGNAL(Finished(const cPermissionDialog::eChoice &)), SLOT(on_cpPermission_Finished(const cPermissionDialog::eChoice &)));
#endif

	start();
} // Delete

// delete operation was canceled
void cDelete::on_cd_OperationCanceled()
{
	bCanceled = true;
} // on_cd_OperationCanceled

// delete operation to background
void cDelete::on_cdDeleteDialog_Background()
{
	CreateWidget();
	emit SetTotalMaximum(qi64TotalMaximum);
	cddDialog->deleteLater();
	cddDialog = NULL;
} // on_cdDeleteDialog_Background

#ifdef Q_WS_WIN
// permission dialog closed with user response
void cDelete::on_cpPermission_Finished(const cPermissionDialog::eChoice &ecResponse)
{
	ecPermissionCurrent = ecResponse;
	qsPause.release();
} // on_cpPermission_Finished
#endif

// separate thread process
void cDelete::run()
{
	int iI;
	QFileInfoList qfilSources;
#ifdef Q_WS_WIN
	QString qsOverwrite;
#endif

	// gather source files
	qfilSources = cFileRoutine::GetSources(qfilSource, true, qsFilter);
	qi64TotalMaximum = qfilSources.count();

	emit SetTotalMaximum(qi64TotalMaximum);

#ifdef Q_WS_WIN
	// get default readonly overwrite permission
	qsOverwrite = csSettings->GetReadonlyFileOverwrite();
	if (qsOverwrite == qsASK) {
		ecPermission = cPermissionDialog::Ask;
	} else {
		if (qsOverwrite == qsYES_TO_ALL) {
			ecPermission = cPermissionDialog::YesToAll;
		} else {
			ecPermission = cPermissionDialog::NoToAll;
		} // if else
	} // if else
#endif

	// main process
	for (iI = qfilSources.count() - 1; iI >= 0; iI--) {
		QDir qdSource;

		if (cddDialog) {
			// name with path in dialog
			emit SetSource(qfilSources.at(iI).filePath());
		} else {
			// just name in widget
			emit SetSource(qfilSources.at(iI).fileName());
		} // if else

#ifdef Q_WS_WIN
		// check readonly permission
		ecPermissionCurrent = cPermissionDialog::Ask;
		if (GetFileAttributes(reinterpret_cast<LPCWSTR>(qfilSources.at(iI).filePath().unicode())) & FILE_ATTRIBUTE_READONLY) {
			if (ecPermission == cPermissionDialog::Ask) {
				emit ShowPermissionDialog(qfilSources.at(iI).fileName(), tr("is readonly."));
				// wait for answer
				qsPause.acquire();

				switch (ecPermissionCurrent) {
					case cPermissionDialog::YesToAll:	ecPermission = cPermissionDialog::YesToAll;
																	break;
					case cPermissionDialog::NoToAll:		ecPermission = cPermissionDialog::NoToAll;
																	break;
				} // switch

				if (ecPermissionCurrent == cPermissionDialog::Cancel) {
					break;
				} // if
			} // if
			if (ecPermission == cPermissionDialog::NoToAll || ecPermissionCurrent == cPermissionDialog::No) {
				continue;
			} else {
				// remove target file readonly permission
				SetFileAttributes(reinterpret_cast<LPCWSTR>(qfilSources.at(iI).filePath().unicode()), GetFileAttributes(reinterpret_cast<LPCWSTR>(qfilSources.at(iI).filePath().unicode())) & ~FILE_ATTRIBUTE_READONLY);
			} // if else
		} // if
#endif

		if (qfilSources.at(iI).isDir()) {
			qdSource.rmdir(qfilSources.at(iI).filePath());
		} else {
			qdSource.remove(qfilSources.at(iI).filePath());
		} // if else

		emit SetTotalValue(qi64TotalMaximum - iI);
	} // for

	// close dialog or widget
	if (cddDialog) {
		cddDialog->deleteLater();
	} else {
		cdwWidget->deleteLater();
	} // if else
} // run