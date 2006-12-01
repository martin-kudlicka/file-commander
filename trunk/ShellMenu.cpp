#include "ShellMenu.h"

// show context menu
void cShellMenu::Show(const QPoint qpPoint)
{
#ifdef Q_WS_WIN
	POINT pPoint;

	pPoint.x = qpPoint.x();
	pPoint.y = qpPoint.y();
	cscmShellMenu.SetObjects(qslObjects);
	cscmShellMenu.ShowContextMenu(hwParent, pPoint);
#endif
} // Show