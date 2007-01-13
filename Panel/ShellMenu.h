/// "native" shell menu

#ifndef SHELL_MENU_H
#define SHELL_MENU_H

#include <QPoint>
#ifdef Q_WS_WIN
#include "Panel/Win32/ShellContextMenu.h"
#endif

class cShellMenu
{
	public:
#ifdef Q_WS_WIN
		cShellMenu(HWND hwParent);			///< constructor
#endif

#ifdef Q_WS_WIN
		void Show(const QStringList &qslObjects, const QPoint &qpPoint);
													///< show context menu
													/**< \param qslObjects objects for context menu
														  \param qpPoint coordinates of context menu */
#else
		void Show(const QFileInfoList &qfilObjects, const QPoint &qpPoint);
													///< show context menu
													/**< \param qfilObjects objects for context menu
														  \param qpPoint coordinates of context menu */
#endif

	private:
#ifdef Q_WS_WIN
		HWND hwParent;							///< handle to parent window

		cShellContextMenu cscmShellMenu;	///< Windows context menu
#endif
}; // cShellMenu

#endif
