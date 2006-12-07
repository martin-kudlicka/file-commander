/// "native" shell menu

#ifndef SHELL_MENU_H
#define SHELL_MENU_H

#include <QPoint>
#ifdef Q_WS_WIN
#include "Win32/ShellContextMenu.h"
#endif

class cShellMenu
{
	public:
#ifdef Q_WS_WIN
		HWND hwParent;							///< parent window handle (Windows)
#endif

		void Show(const QStringList qslObjects, const QPoint qpPoint);
													///< show context menu
													/**< \param qslObjects objects for context menu
														  \param qpPoint coordinates of context menu */

	private:
#ifdef Q_WS_WIN
		cShellContextMenu cscmShellMenu;	///< Windows context menu
#endif
}; // cShellMenu

#endif
