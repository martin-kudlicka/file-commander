/// background copy/move window

#ifndef COPY_MOVE_WIDGET_H
#define COPY_MOVE_WIDGET_H

#include "ui_CopyMoveWidget.h"

class cCopyMoveWidget : public QWidget, private Ui::qwCopyMove
{
	public:
		cCopyMoveWidget(); ///< constructor
}; // cCopyMoveWidget

#endif