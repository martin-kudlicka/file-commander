#include "ListerMainWindow.h"

#include <QFileInfo>
#include <QTextCodec>
#include "ListerMainWindow/FindDialog.h"
#include <QPageSetupDialog>
#include <QFileDialog>

// destructor
cListerMainWindow::~cListerMainWindow()
{
	if (hwPlugin) {
		ClosePlugin();
	} // if

	delete qhiPlugins;
	if (qteContent && qteContent->document()) {
		qteContent->setDocument(NULL);
	} // if
	qteContent->deleteLater();
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

	// assign shortcuts
	qaOpen->setShortcut(QKeySequence(csSettings->GetShortcut(qsSHORTCUT__LISTER__FILE__OPEN)));
	qaSaveAs->setShortcut(QKeySequence(csSettings->GetShortcut(qsSHORTCUT__LISTER__FILE__SAVE_AS)));
	qaPrint->setShortcut(QKeySequence(csSettings->GetShortcut(qsSHORTCUT__LISTER__FILE__PRINT)));
	qaPrintSetup->setShortcut(QKeySequence(csSettings->GetShortcut(qsSHORTCUT__LISTER__FILE__PRINT_SETUP)));
	qaClose->setShortcut(QKeySequence(csSettings->GetShortcut(qsSHORTCUT__LISTER__FILE__CLOSE)));
	qaCopyAsText->setShortcut(QKeySequence(csSettings->GetShortcut(qsSHORTCUT__LISTER__EDIT__COPY_AS_TEXT)));
	qaSelectAll->setShortcut(QKeySequence(csSettings->GetShortcut(qsSHORTCUT__LISTER__EDIT__SELECT_ALL)));
	qaFind->setShortcut(QKeySequence(csSettings->GetShortcut(qsSHORTCUT__LISTER__EDIT__FIND)));
	qaFindNext->setShortcut(QKeySequence(csSettings->GetShortcut(qsSHORTCUT__LISTER__EDIT__FIND_NEXT)));
	qaText->setShortcut(QKeySequence(csSettings->GetShortcut(qsSHORTCUT__LISTER__OPTIONS__TEXT)));
	qaBinary->setShortcut(QKeySequence(csSettings->GetShortcut(qsSHORTCUT__LISTER__OPTIONS__BINARY)));
	qaHex->setShortcut(QKeySequence(csSettings->GetShortcut(qsSHORTCUT__LISTER__OPTIONS__HEX)));
	qaMultimedia->setShortcut(QKeySequence(csSettings->GetShortcut(qsSHORTCUT__LISTER__OPTIONS__MULTIMEDIA)));
	qaANSI->setShortcut(QKeySequence(csSettings->GetShortcut(qsSHORTCUT__LISTER__OPTIONS__ANSI)));
	qaASCII->setShortcut(QKeySequence(csSettings->GetShortcut(qsSHORTCUT__LISTER__OPTIONS__ASCII)));
	qaVariableCharWidth->setShortcut(QKeySequence(csSettings->GetShortcut(qsSHORTCUT__LISTER__OPTIONS__VARIABLE_CHAR_WIDTH)));
	qaWrapText->setShortcut(QKeySequence(csSettings->GetShortcut(qsSHORTCUT__LISTER__OPTIONS__WRAP_TEXT)));
	qaFitImageToWindow->setShortcut(QKeySequence(csSettings->GetShortcut(qsSHORTCUT__LISTER__OPTIONS__FIT_IMAGE_TO_WINDOW)));

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
	if (slLister.bWrapText) {
		qaWrapText->setChecked(true);
	} // if
	if (slLister.bFitImageToWindow) {
		qaFitImageToWindow->setChecked(true);
	} // if

	ShowContent(true);
} // cListerMainWindow

// lister window close
void cListerMainWindow::closeEvent(QCloseEvent *event)
{
	deleteLater();;
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
			hwPlugin = qhiPlugins->value().tllListLoad(centralwidget->winId(), QDir::toNativeSeparators(qsFile).toLatin1().data(), iFlags);
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
	if (qteContent) {
		// native
		qteContent->copy();
	} else  {
		// plugin
		if (qhiPlugins->value().tlscListSendCommand) {
			qhiPlugins->value().tlscListSendCommand(hwPlugin, lc_copy, 0);
		} // if
	} // if else
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
			if (cfdFind->qcbSearchBackwards->isChecked()) {
				ffFlags = QTextDocument::FindBackward;
			} else {
				ffFlags = 0;
			} // if
			if (cfdFind->qcbCaseSensitive->isChecked()) {
				ffFlags |= QTextDocument::FindCaseSensitively;
			} // if
			if (cfdFind->qcbWholeWords->isChecked()) {
				ffFlags |= QTextDocument::FindWholeWords;
			} // if
			qteContent->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);
			qteContent->find(qsSearchedText, ffFlags);
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
		qteContent->find(qsSearchedText, ffFlags);
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

// open selected
void cListerMainWindow::on_qaOpen_triggered(bool checked /* false */)
{
	QString qsFile;

	qsFile = QFileDialog::getOpenFileName(this);
	if (qsFile != "") {
		this->qsFile = qsFile;
		ShowContent();
	} // if
} // on_qaOpen_triggered

// print selected
void cListerMainWindow::on_qaPrint_triggered(bool checked /* false */)
{
	if (qteContent) {
		// native
		qtdDocument.print(&qpPrinter);
	} else {
		// plugin
		RECT rMargins;

		rMargins.bottom = qpPrinter.paperRect().bottom() - qpPrinter.pageRect().bottom();
		rMargins.left = qpPrinter.pageRect().left() - qpPrinter.paperRect().left();
		rMargins.right = qpPrinter.paperRect().right() - qpPrinter.pageRect().right();
		rMargins.top = qpPrinter.pageRect().top() - qpPrinter.paperRect().top();

		qhiPlugins->value().tlpListPrint(hwPlugin, qsFile.toLatin1().data(), qpPrinter.printerName().toLatin1().data(), 0, &rMargins);
	} // if else
} // on_qaPrint_triggered

// print setup selected
void cListerMainWindow::on_qaPrintSetup_triggered(bool checked /* false */)
{
	QPageSetupDialog qpsdPrint(&qpPrinter, this);

	qpsdPrint.exec();
} // on_qaPrintSetup_triggered

// save as selected
void cListerMainWindow::on_qaSaveAs_triggered(bool checked /* false */)
{
	QString qsFile;

	qsFile = QFileDialog::getSaveFileName(this, tr("Save as"), QFileInfo(this->qsFile).path());
	if (qsFile != "") {
		QFile::remove(qsFile);
		QFile::copy(this->qsFile, qsFile);
	} // if
} // on_qaSaveAs_triggered

// select all selected
void cListerMainWindow::on_qaSelectAll_triggered(bool checked /* false */)
{
	if (qteContent) {
		// native
		qteContent->selectAll();
	} else {
		// plugin
		if (qhiPlugins->value().tlscListSendCommand) {
			qhiPlugins->value().tlscListSendCommand(hwPlugin, lc_selectall, 0);
		} // if
	} // if else
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

	if (hwPlugin && !qhiPlugins->value().tlpListPrint) {
		qaPrint->setEnabled(false);
		qaPrintSetup->setEnabled(false);
	} else {
		qaPrint->setEnabled(true);
		qaPrintSetup->setEnabled(true);
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
		qteContent->setDocument(NULL);
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
			qtdDocument.setPlainText(Qt::codecForHtml(qbaFile)->toUnicode(qbaFile));
			qteContent->setDocument(&qtdDocument);
		} else {
			// TODO ShowContent Hex
		} // if else

		qfFile.close();
	} // if else

	PermitMenuActions();
} // ShowContent