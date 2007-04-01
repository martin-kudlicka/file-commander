#include "ComboBox.h"

#include <QKeyEvent>

// key pressed in combo box
void cComboBox::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
		emit EnterPressed();
		insertItem(0, currentText());
		setCurrentIndex(-1);
		setEditText("");
	} else {
		QComboBox::keyPressEvent(event);
	} // if else
} // keyPressEvent