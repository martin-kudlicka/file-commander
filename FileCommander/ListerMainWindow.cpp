#include "ListerMainWindow.h"

#include <QtCore/QFileInfo>
#include <QtCore/QTextCodec>
#include "ListerMainWindow/FindDialog.h"
#include <QtGui/QPageSetupDialog>
#include <QtGui/QFileDialog>
#ifdef Q_WS_WIN
#include <QtGui/QKeyEvent>
#endif

// destructor
cListerMainWindow::~cListerMainWindow()
{
	delete qhiPlugins;
} // ~cListerMainWindow

// creates lister window
cListerMainWindow::cListerMainWindow(cSettings *csSettings, cListerPlugin *clpListerPlugin, const QString &qsFile)
{
	cSettings::sLister slLister;

	// setup GUI
	setupUi(this);

	// variables
	this->csSettings = csSettings;
	this->clpListerPlugin = clpListerPlugin;
	this->qsFile = qsFile;

	qhPlugins = clpListerPlugin->GetPluginsInfo();
	qhiPlugins = new QHashIterator<QString, cListerPlugin::sPluginInfo>(qhPlugins);
	hwPlugin = NULL;
	qteContent->setDocument(&qtdDocument);

	bMenuBarVisible = false;

	// assign shortcuts
	qaOpen->setShortcut(QKeySequence(csSettings->GetShortcut(cSettings::ListerCategory, qsSHORTCUT__LISTER__FILE__OPEN)));
	qaSaveAs->setShortcut(QKeySequence(csSettings->GetShortcut(cSettings::ListerCategory, qsSHORTCUT__LISTER__FILE__SAVE_AS)));
	qaPrint->setShortcut(QKeySequence(csSettings->GetShortcut(cSettings::ListerCategory, qsSHORTCUT__LISTER__FILE__PRINT)));
	qaPrintSetup->setShortcut(QKeySequence(csSettings->GetShortcut(cSettings::ListerCategory, qsSHORTCUT__LISTER__FILE__PRINT_SETUP)));
	qaClose->setShortcut(QKeySequence(csSettings->GetShortcut(cSettings::ListerCategory, qsSHORTCUT__LISTER__FILE__CLOSE)));
	qaCopyAsText->setShortcut(QKeySequence(csSettings->GetShortcut(cSettings::ListerCategory, qsSHORTCUT__LISTER__EDIT__COPY_AS_TEXT)));
	qaSelectAll->setShortcut(QKeySequence(csSettings->GetShortcut(cSettings::ListerCategory, qsSHORTCUT__LISTER__EDIT__SELECT_ALL)));
	qaFind->setShortcut(QKeySequence(csSettings->GetShortcut(cSettings::ListerCategory, qsSHORTCUT__LISTER__EDIT__FIND)));
	qaFindNext->setShortcut(QKeySequence(csSettings->GetShortcut(cSettings::ListerCategory, qsSHORTCUT__LISTER__EDIT__FIND_NEXT)));
	qaText->setShortcut(QKeySequence(csSettings->GetShortcut(cSettings::ListerCategory, qsSHORTCUT__LISTER__OPTIONS__TEXT)));
	qaBinary->setShortcut(QKeySequence(csSettings->GetShortcut(cSettings::ListerCategory, qsSHORTCUT__LISTER__OPTIONS__BINARY)));
	qaHex->setShortcut(QKeySequence(csSettings->GetShortcut(cSettings::ListerCategory, qsSHORTCUT__LISTER__OPTIONS__HEX)));
	qaMultimedia->setShortcut(QKeySequence(csSettings->GetShortcut(cSettings::ListerCategory, qsSHORTCUT__LISTER__OPTIONS__MULTIMEDIA)));
	qaANSI->setShortcut(QKeySequence(csSettings->GetShortcut(cSettings::ListerCategory, qsSHORTCUT__LISTER__OPTIONS__ANSI)));
	qaASCII->setShortcut(QKeySequence(csSettings->GetShortcut(cSettings::ListerCategory, qsSHORTCUT__LISTER__OPTIONS__ASCII)));
	qaVariableCharWidth->setShortcut(QKeySequence(csSettings->GetShortcut(cSettings::ListerCategory, qsSHORTCUT__LISTER__OPTIONS__VARIABLE_CHAR_WIDTH)));
	qaWrapText->setShortcut(QKeySequence(csSettings->GetShortcut(cSettings::ListerCategory, qsSHORTCUT__LISTER__OPTIONS__WRAP_TEXT)));
	qaFitImageToWindow->setShortcut(QKeySequence(csSettings->GetShortcut(cSettings::ListerCategory, qsSHORTCUT__LISTER__OPTIONS__FIT_IMAGE_TO_WINDOW)));

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

	// event filter
	qmbMenu->installEventFilter(this);

	ShowContent(true);
} // cListerMainWindow

// lister window close
void cListerMainWindow::closeEvent(QCloseEvent *event)
{
	if (hwPlugin) {
		ClosePlugin();
	} // if
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

#ifdef Q_WS_WIN
// event filter
bool cListerMainWindow::eventFilter(QObject *watched, QEvent *event)
{
	if (watched == qmbMenu) {
		if (!bMenuBarVisible && event->type() == QEvent::Resize && hwPlugin) {
			// to "fix" later shown menu bar
			resizeEvent(NULL);
			// UpdateWindow, InvalidateRect and WM_PAINT don't work -> hide and show
			ShowWindow(hwPlugin, SW_HIDE);
			ShowWindow(hwPlugin, SW_SHOW);
			if (qmbMenu->height() > 0) {
				bMenuBarVisible = true;
			} // if
		} // if
		return false;
	} else {
		return QMainWindow::eventFilter(watched, event);
	} // if else
} // eventFilter
#endif

// find next usable plugin for file
bool cListerMainWindow::FindNextPlugin(const bool &bNextPlugin, const bool &bForceShow)
{
	int iFlags, iI;

	if (qhPlugins.isEmpty()) {
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
			hwPlugin = qhiPlugins->value().tllListLoad(winId(), QDir::toNativeSeparators(qsFile).toLatin1().data(), iFlags);
			if (hwPlugin) {
#ifdef Q_WS_WIN
				resizeEvent(NULL);
#endif
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

#ifdef Q_WS_WIN
// key pressed in lister
void cListerMainWindow::keyPressEvent(QKeyEvent *event)
{
	// send key only once (plugin sends some key back again?)
	if (hwPlugin && event->nativeScanCode()) {
		if (event->modifiers() & Qt::AltModifier) {
			if (event->key() != Qt::Key_Alt) {
				// not only Alt key
				BYTE bNewKeys[256], bOldKeys[256];

				memset(&bNewKeys, 0, 256);
				GetKeyboardState(static_cast<PBYTE>(bOldKeys));
				bNewKeys[VK_MENU] = 0x80;
				SetKeyboardState(bNewKeys);
				PostMessage(hwPlugin, WM_SYSKEYDOWN, event->nativeVirtualKey(), 0x20000000);
				PostMessage(hwPlugin, WM_SYSKEYUP, event->nativeVirtualKey(), 0xE0000000);
				SetKeyboardState(bOldKeys);
			} // if
		} else {
			PostMessage(hwPlugin, WM_KEYDOWN, event->nativeVirtualKey(), 0);
			PostMessage(hwPlugin, WM_KEYUP, event->nativeVirtualKey(), 0xC0000000);
		} // if else
	} // if
} // keyPressEvent
#endif

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
		// plugin
		if (qhiPlugins->value().tlscListSendCommand) {
			qhiPlugins->value().tlscListSendCommand(hwPlugin, lc_copy, 0);
		} // if
	} else {
		// native
		qteContent->copy();
	} // if else
} // on_qaCopyAsText_triggered

// find selected
void cListerMainWindow::on_qaFind_triggered(bool checked /* false */)
{
	bool bPlugin;
	cFindDialog *cfdFind;

	if (hwPlugin) {
		// plugin
		bPlugin = true;
	} else {
		// native
		bPlugin = false;
	} // if else

	cfdFind = new cFindDialog(this, bPlugin);
	if (cfdFind->exec() == QDialog::Accepted) {
		qsSearchedText = cfdFind->qcbFind->currentText();

		if (hwPlugin) {
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
		} else {
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
		} // if else
	} // if

	cfdFind->deleteLater();
} // on_qaFind_triggered

// find next selected
void cListerMainWindow::on_qaFindNext_triggered(bool checked /* false */)
{
	if (hwPlugin) {
		// plugin
		if (iSearchFlags & lcs_findfirst) {
			iSearchFlags ^= lcs_findfirst;
		} // if
		qhiPlugins->value().tlstListSearchText(hwPlugin, qsSearchedText.toLatin1().data(), iSearchFlags);
	} else {
		// native
		qteContent->find(qsSearchedText, ffFlags);
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
	if (!qsFile.isEmpty()) {
		this->qsFile = qsFile;
		ShowContent();
	} // if
} // on_qaOpen_triggered

// print selected
void cListerMainWindow::on_qaPrint_triggered(bool checked /* false */)
{
	if (hwPlugin) {
		// plugin
		RECT rMargins;

		rMargins.bottom = qpPrinter.paperRect().bottom() - qpPrinter.pageRect().bottom();
		rMargins.left = qpPrinter.pageRect().left() - qpPrinter.paperRect().left();
		rMargins.right = qpPrinter.paperRect().right() - qpPrinter.pageRect().right();
		rMargins.top = qpPrinter.pageRect().top() - qpPrinter.paperRect().top();

		qhiPlugins->value().tlpListPrint(hwPlugin, qsFile.toLatin1().data(), qpPrinter.printerName().toLatin1().data(), 0, &rMargins);
	} else {
		// native
		qtdDocument.print(&qpPrinter);
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
	if (!qsFile.isEmpty()) {
		QFile::remove(qsFile);
		QFile::copy(this->qsFile, qsFile);
	} // if
} // on_qaSaveAs_triggered

// select all selected
void cListerMainWindow::on_qaSelectAll_triggered(bool checked /* false */)
{
	if (hwPlugin) {
		// plugin
		if (qhiPlugins->value().tlscListSendCommand) {
			qhiPlugins->value().tlscListSendCommand(hwPlugin, lc_selectall, 0);
		} // if
	} else {
		// native
		qteContent->selectAll();
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
	if (hwPlugin) {
		// plugin
		if (qhiPlugins->value().tlscListSendCommand) {
			qhiPlugins->value().tlscListSendCommand(hwPlugin, lc_newparams, GetSendCommandParameters());
		} // if
	} else {
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

#ifdef Q_WS_WIN
// resize of lister window occurs
void cListerMainWindow::resizeEvent(QResizeEvent *event)
{
	if (hwPlugin) {
		MoveWindow(hwPlugin, 0, qmbMenu->height(), width(), height() - qmbMenu->height(), FALSE);
	} // if
} // resizeEvent
#endif

// show file content
void cListerMainWindow::ShowContent(const bool &bNextPlugin /* false */, const bool &bForceShow /* false */)
{
	if (hwPlugin) {
		ClosePlugin();
	} else {
		qtdDocument.clear();
	} // if else

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
			centralwidget->hide();
			// send default parameters to plugin
			if (qhiPlugins->value().tlscListSendCommand) {
				qhiPlugins->value().tlscListSendCommand(hwPlugin, lc_newparams, GetSendCommandParameters());
			} // if
		} // if else
	} else {
		// text, binary, hex
		QByteArray qbaFile;
		QFile qfFile;

		centralwidget->show();
		qteContent->setFocus(Qt::OtherFocusReason);
		on_qaWrapText_triggered(qaWrapText->isChecked());
		qfFile.setFileName(qsFile);
		qfFile.open(QIODevice::ReadOnly);

		// fill text edit
		qbaFile = qfFile.readAll();

		if (qaText->isChecked() || qaBinary->isChecked()) {
			qtdDocument.setDefaultFont(csSettings->GetListerFont());
			qtdDocument.setPlainText(Qt::codecForHtml(qbaFile)->toUnicode(qbaFile));
		} else {
			// TODO ShowContent Hex
		} // if else

		qfFile.close();
	} // if else

	PermitMenuActions();
} // ShowContent
