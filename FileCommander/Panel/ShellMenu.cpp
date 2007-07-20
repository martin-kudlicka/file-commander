#include "Panel/ShellMenu.h"

#ifdef Q_WS_WIN
// constructor
cShellMenu::cShellMenu(HWND hwParent)
{
	this->hwParent = hwParent;
} // cShellMenu
#endif

// show context menu
#ifdef Q_WS_WIN
void cShellMenu::Show(const QStringList &qslObjects, const QPoint &qpPoint)
#else
void cShellMenu::Show(const QFileInfoList &qfilObjects, const QPoint &qpPoint)
#endif
{
#ifdef Q_WS_WIN
	POINT pPoint;

	pPoint.x = qpPoint.x();
	pPoint.y = qpPoint.y();
	if (qslObjects.isEmpty()) {
		cscmShellMenu.SetObjects("");
	} else {
		cscmShellMenu.SetObjects(qslObjects);
	} // if else
	cscmShellMenu.ShowContextMenu(hwParent, pPoint);
#else
	// TODO Show other operating systems
#endif
} // Show
