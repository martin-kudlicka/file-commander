#include "Panel/TreeWidget.h"

#include <QtGui/QKeyEvent>

// dir view got focus
void cTreeWidget::focusInEvent(QFocusEvent *event)
{
	QTreeWidget::focusInEvent(event);
	emit GotFocus();
} // focusInEvent

// handle key processing
void cTreeWidget::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
		case Qt::Key_Insert:
			setSelectionMode(MultiSelection);
			setCurrentItem(topLevelItem(indexOfTopLevelItem(currentItem()) + 1));
			setSelectionMode(ExtendedSelection);
			break;
		case Qt::Key_Space:
			currentItem()->setSelected(!currentItem()->isSelected());
			emit KeyPressed(event);
			break;
		default:
			if (((event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
				  && !(event->modifiers() & Qt::ControlModifier || event->modifiers() & Qt::ShiftModifier))
				 || event->text().isEmpty()) {
				QTreeWidget::keyPressEvent(event);
			} else {
				emit KeyPressed(event);
			} // if else
	} // switch
} // keyPressEvent
