#include "ShellMenu.h"

// show context menu
void cShellMenu::Show(const QStringList qslObjects, const QPoint qpPoint)
{
#ifdef Q_WS_WIN
	POINT pPoint;

	pPoint.x = qpPoint.x();
	pPoint.y = qpPoint.y();
	if (qslObjects.count() == 0) {
		cscmShellMenu.SetObjects("");
	} else {
		cscmShellMenu.SetObjects(qslObjects);
	} // if else
	cscmShellMenu.ShowContextMenu(hwParent, pPoint);
#endif
} // Show