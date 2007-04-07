#include "Panel/TreeWidget.h"

#include <QKeyEvent>

// constructor
cTreeWidget::cTreeWidget()
{
	connect(this, SIGNAL(itemSelectionChanged()), SLOT(on_ctwTree_itemSelectionChanged()));
} // cTreeWidget

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
		case Qt::Key_Insert:	setSelectionMode(MultiSelection);
									setCurrentItem(topLevelItem(indexOfTopLevelItem(currentItem()) + 1));
									setSelectionMode(ExtendedSelection);
									break;
		case Qt::Key_Space:	currentItem()->setSelected(!currentItem()->isSelected());
									emit KeyPressed(event, currentItem());
									break;
		default:					if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return || event->text() == "") {
										QTreeWidget::keyPressEvent(event);
									} else {
										emit KeyPressed(event, NULL);
									} // if else
	} // switch
} // keyPressEvent

// selection changed it directory view
void cTreeWidget::on_ctwTree_itemSelectionChanged()
{
	emit itemSelectionChanged(this);
} // on_ctwTree_itemSelectionChanged