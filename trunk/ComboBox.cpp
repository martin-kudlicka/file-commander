#include "ComboBox.h"

#include <QKeyEvent>

// key pressed in combo box
void cComboBox::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
		case Qt::Key_Down:
		case Qt::Key_Up:		emit KeyPressed(event);
									break;
		case Qt::Key_Enter:
		case Qt::Key_Return:	emit KeyPressed(event);
									insertItem(0, currentText());
									setCurrentIndex(-1);
									setEditText("");
									break;
		default:					QComboBox::keyPressEvent(event);
	} // switch
} // keyPressEvent