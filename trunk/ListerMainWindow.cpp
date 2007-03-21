#include "ListerMainWindow.h"

#include <QFileInfo>
#include <QTextCodec>
#include "ListerMainWindow/FindDialog.h"

// destructor
cListerMainWindow::~cListerMainWindow()
{
	if (hwPlugin) {
		ClosePlugin();
	} // if

	delete qhiPlugins;
	qteContent->deleteLater();

	deleteLater();
} // ~cListerMainWindow

// creates lister window
cListerMainWindow::cListerMainWindow(cSettings *csSettings, cLister *clLister, const QString &qsFile)
{
	cSettings::sLister slLister;
	// setup GUI
	setupUi(this);

	// variables
	this->csSettings = csSettings;
	this->clLister = clLister;
	this->qsFile = qsFile;

	qhPlugins = clLister->GetPluginsInfo();
	qhiPlugins = new QHashIterator<QString, cLister::sPluginInfo>(qhPlugins);
	hwPlugin = NULL;
	qteContent = NULL;

	// settings
	slLister = csSettings->GetListerSettings();
	if (slLister.qsCharSet == qsANSI) {
		qaANSI->setChecked(true);
	} else {
		if (slLister.qsCharSet == qsASCII) {
			qaASCII->setChecked(true);
		} else {
			qaVariableCharWidth->setChecked(true);
		} // if else
	} // if else
	if (slLister.qsWrapText == qsTRUE) {
		qaWrapText->setChecked(true);
	} // if
	if (slLister.qsFitImageToWindow == qsTRUE) {
		qaFitImageToWindow->setChecked(true);
	} // if

	ShowContent(true);
} // cListerMainWindow

// lister window close
void cListerMainWindow::closeEvent(QCloseEvent *event)
{
	delete this;
} // closeEvent

// destroy of plugin's window
void cListerMainWindow::ClosePlugin()
{
	if (qhiPlugins->value().tlcwListCloseWindow) {
		qhiPlugins->value().tlcwListCloseWindow(hwPlugin);
	}
#ifdef Q_WS_WIN
	else {
		DestroyWindow(hwPlugin);
	} // if else
#endif
	hwPlugin = NULL;
} // ClosePlugin

// create text edit control and set default parameters
void cListerMainWindow::CreateTextEdit()
{
	qteContent = new QTextEdit(this);
	resizeEvent(NULL);
	qteContent->show();

	qteContent->setReadOnly(true);
	on_qaWrapText_triggered(qaWrapText->isChecked());
} // CreateTextEdit

// find next usable plugin for file
bool cListerMainWindow::FindNextPlugin(const bool &bNextPlugin, const bool &bForceShow)
{
	int iFlags, iI;

	if (qhPlugins.count() == 0) {
		// no lister plugins
		return false;
	} // if

	if (bNextPlugin == false) {
		qhiPlugins->previous();
	} // if

	// set flags
	if (bForceShow) {
		iFlags = lcp_forceshow;
	} else {
		iFlags = 0;
	} // if else
	if (qaWrapText->isChecked()) {
		iFlags |= lcp_wraptext;
	} // if
	if (qaFitImageToWindow->isChecked()) {
		iFlags |= lcp_fittowindow;
	} // if
	if (qaANSI->isChecked()) {
		iFlags |= lcp_ansi;
	} else {
		if (qaASCII->isChecked()) {
			iFlags |= lcp_ascii;
		} else {
			iFlags |= lcp_variable;
		} // if else
	} // if else

	// search twice
	for (iI = 0; iI < 2; iI++) {
		while (qhiPlugins->hasNext()) {
			qhiPlugins->next();
			hwPlugin = qhiPlugins->value().tllListLoad(centralwidget->winId(), qsFile.toLatin1().data(), iFlags);
			if (hwPlugin) {
				resizeEvent(NULL);
				return true;
			} // if
		} // while

		qhiPlugins->toFront();
	} // for

	return false;
} // FindNextPlugin

// parameters for plugin
int cListerMainWindow::GetSendCommandParameters()
{
	int iParameters;

	if (qaANSI->isChecked()) {
		iParameters = lcp_ansi;
	} else {
		iParameters = 0;
	} // if else
	if (qaASCII->isChecked()) {
		iParameters |= lcp_ascii;
	} // if
	if (qaVariableCharWidth->isChecked()) {
		iParameters |= lcp_variable;
	} // if
	if (qaWrapText->isChecked()) {
		iParameters |= lcp_wraptext;
	} // if
	if (qaFitImageToWindow->isChecked()) {
		iParameters |= lcp_fittowindow;
	} // if

	return iParameters;
} // GetSendCommandParameters

// ANSI char set selected
void cListerMainWindow::on_qaANSI_triggered(bool checked /* false */)
{
	if (checked) {
		qaASCII->setChecked(false);
		qaVariableCharWidth->setChecked(false);
	} else {
		qaANSI->setChecked(true);
	} // if else

	if (hwPlugin) {
		if (qhiPlugins->value().tlscListSendCommand) {
			qhiPlugins->value().tlscListSendCommand(hwPlugin, lc_newparams, GetSendCommandParameters());
		} // if
	} // if
} // on_qaANSI_triggered

// ASCII char set selected
void cListerMainWindow::on_qaASCII_triggered(bool checked /* false */)
{
	if (checked) {
		qaANSI->setChecked(false);
		qaVariableCharWidth->setChecked(false);
	} else {
		qaASCII->setChecked(true);
	} // if else

	if (hwPlugin) {
		if (qhiPlugins->value().tlscListSendCommand) {
			qhiPlugins->value().tlscListSendCommand(hwPlugin, lc_newparams, GetSendCommandParameters());
		} // if
	} // if
} // on_qaASCII_triggered

// binary mode selected
void cListerMainWindow::on_qaBinary_triggered(bool checked /* false */)
{
	qaBinary->setChecked(true);
	qaText->setChecked(false);
	qaHex->setChecked(false);
	qaMultimedia->setChecked(false);

	ShowContent();
} // on_qaBinary_triggered

// copy as text selected
void cListerMainWindow::on_qaCopyAsText_triggered(bool checked /* false */)
{
	if (hwPlugin) {
		if (qhiPlugins->value().tlscListSendCommand) {
			qhiPlugins->value().tlscListSendCommand(hwPlugin, lc_copy, 0);
		} // if
	} // if
} // on_qaCopyAsText_triggered

// find selected
void cListerMainWindow::on_qaFind_triggered(bool checked /* false */)
{
	bool bPlugin;
	cFindDialog *cfdFind;

	if (qteContent) {
		// native
		bPlugin = false;
	} else {
		// plugin
		bPlugin = true;
	} // if else

	cfdFind = new cFindDialog(this, bPlugin);
	if (cfdFind->exec() == QDialog::Accepted) {
		qsSearchedText = cfdFind->qcbFind->currentText();

		if (qteContent) {
			// native
			// TODO on_qaFind_triggered native
		} else {
			// plugin

			iSearchFlags = lcs_findfirst;
			if (cfdFind->qcbCaseSensitive->isChecked()) {
				iSearchFlags |= lcs_matchcase;
			} // if
			if (cfdFind->qcbWholeWords->isChecked()) {
				iSearchFlags |= lcs_wholewords;
			} // if
			if (cfdFind->qcbSearchBackwards->isChecked()) {
				iSearchFlags |= lcs_backwards;
			} // if
			qhiPlugins->value().tlstListSearchText(hwPlugin, qsSearchedText.toLatin1().data(), iSearchFlags);
		} // if else
	} // if

	cfdFind->deleteLater();
} // on_qaFind_triggered

// find next selected
void cListerMainWindow::on_qaFindNext_triggered(bool checked /* false */)
{
	if (qteContent) {
		// native
		// TODO on_qaFindNext_triggered native
	} else {
		// plugin
		if (iSearchFlags & lcs_findfirst) {
			iSearchFlags ^= lcs_findfirst;
		} // if
		qhiPlugins->value().tlstListSearchText(hwPlugin, qsSearchedText.toLatin1().data(), iSearchFlags);
	} // if else
} // on_qaFindNext_triggered

// fit image to window selected
void cListerMainWindow::on_qaFitImageToWindow_triggered(bool checked /* false */)
{
	if (hwPlugin) {
		if (qhiPlugins->value().tlscListSendCommand) {
			qhiPlugins->value().tlscListSendCommand(hwPlugin, lc_newparams, GetSendCommandParameters());
		} // if
	} // if
} // on_qaFitImageToWindow_triggered

// hex mode selected
void cListerMainWindow::on_qaHex_triggered(bool checked /* false */)
{
	if (checked) {
		qaText->setChecked(false);
		qaBinary->setChecked(false);
		qaMultimedia->setChecked(false);
	} else {
		qaHex->setChecked(true);
	} // if else

	ShowContent();
} // on_qaHex_triggered

// multimedia mode selected
void cListerMainWindow::on_qaMultimedia_triggered(bool checked /* false */)
{
	if (checked) {
		qaText->setChecked(false);
		qaBinary->setChecked(false);
		qaHex->setChecked(false);
	} else {
		qaMultimedia->setChecked(true);
	} // if else

	ShowContent(true, true);
} // on_qaMultimedia_triggered

// select all selected
void cListerMainWindow::on_qaSelectAll_triggered(bool checked /* false */)
{
	if (hwPlugin) {
		if (qhiPlugins->value().tlscListSendCommand) {
			qhiPlugins->value().tlscListSendCommand(hwPlugin, lc_selectall, 0);
		} // if
	} // if
} // on_qaSelectAll_triggered

// text mode selected
void cListerMainWindow::on_qaText_triggered(bool checked /* false */)
{
	qaText->setChecked(true);
	qaBinary->setChecked(false);
	qaHex->setChecked(false);
	qaMultimedia->setChecked(false);

	ShowContent();
} // on_qaText_triggered

// variable char width selected
void cListerMainWindow::on_qaVariableCharWidth_triggered(bool checked /* false */)
{
	if (checked) {
		qaANSI->setChecked(false);
		qaASCII->setChecked(false);
	} else {
		qaVariableCharWidth->setChecked(true);
	} // if else

	if (hwPlugin) {
		if (qhiPlugins->value().tlscListSendCommand) {
			qhiPlugins->value().tlscListSendCommand(hwPlugin, lc_newparams, GetSendCommandParameters());
		} // if
	} // if
} // on_qaVariableCharWidth_triggered

// variable char width selected
void cListerMainWindow::on_qaWrapText_triggered(bool checked /* false */)
{
	if (qteContent) {
		// native
		if (qaText->isChecked() || qaBinary->isChecked()) {
			if (checked) {
				qteContent->setLineWrapMode(QTextEdit::WidgetWidth);
			} else {
				qteContent->setLineWrapMode(QTextEdit::NoWrap);
			} // if else
			// TODO on_qaWrapText_triggered QTextOption::WrapAnywhere
			/*if (qaText->isChecked()) {
				qteContent->setWordWrapMode(QTextOption::WordWrap);
			} else {
				qteContent->setWordWrapMode(QTextOption::WrapAnywhere);
			} // if else*/
		} else {
			// TODO on_qaWrapText_triggered Hex
		} // if else
	} else {
		// plugin
		if (qhiPlugins->value().tlscListSendCommand) {
			qhiPlugins->value().tlscListSendCommand(hwPlugin, lc_newparams, GetSendCommandParameters());
		} // if
	} // if else
} // on_qaWrapText_triggered

// enable/disable menu actions
void cListerMainWindow::PermitMenuActions()
{
	if (hwPlugin && !qhiPlugins->value().tlstListSearchText) {
		qaFind->setEnabled(false);
		qaFindNext->setEnabled(false);
	} else {
		qaFind->setEnabled(true);
		qaFindNext->setEnabled(true);
	} // if else
} // PermitMenuActions

// resize of lister window occurs
void cListerMainWindow::resizeEvent(QResizeEvent *event)
{
#ifdef Q_WS_WIN
	if (hwPlugin) {
		MoveWindow(hwPlugin, 0, 0, centralwidget->width(), centralwidget->height(), FALSE);
	} // if
#endif
	if (qteContent) {
		qteContent->setGeometry(centralwidget->geometry());
	} // if
} // resizeEvent

// show file content
void cListerMainWindow::ShowContent(const bool &bNextPlugin /* false */, const bool &bForceShow /* false */)
{
	if (qteContent) {
		qteContent->deleteLater();
		qteContent = NULL;
	} // if
	if (hwPlugin) {
		ClosePlugin();
	} // if

	if (qaMultimedia->isChecked()) {
		// multimedia
		if (!FindNextPlugin(bNextPlugin, bForceShow)) {
			// switch to another possible mode
			if (QFileInfo(qsFile).suffix() == "txt") {
				on_qaText_triggered();
			} else {
				on_qaBinary_triggered();
			} // if else
		} else {
			// send default parameters to plugin
			if (qhiPlugins->value().tlscListSendCommand) {
				qhiPlugins->value().tlscListSendCommand(hwPlugin, lc_newparams, GetSendCommandParameters());
			} // if
		} // if else
	} else {
		// text, binary, hex
		QByteArray qbaFile;
		QFile qfFile;

		CreateTextEdit();
		qfFile.setFileName(qsFile);
		qfFile.open(QIODevice::ReadOnly);

		// fill text edit
		qbaFile = qfFile.readAll();
		if (qaText->isChecked() || qaBinary->isChecked()) {
			qteContent->setPlainText(Qt::codecForHtml(qbaFile)->toUnicode(qbaFile));
		} else {
			// TODO ShowContent Hex
		} // if else

		qfFile.close();
	} // if else

	PermitMenuActions();
} // ShowContent