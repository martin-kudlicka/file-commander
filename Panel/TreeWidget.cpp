#include "Panel/TreeWidget.h"

#include <QKeyEvent>

// constructor
cTreeWidget::cTreeWidget()
{
	connect(this, SIGNAL(itemSelectionChanged()), SLOT(on_ctwTree_itemSelectionChanged()));
} // cTreeWidget

// handle key processing
void cTreeWidget::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
		case Qt::Key_Insert:	setSelectionMode(MultiSelection);
									setCurrentItem(topLevelItem(indexOfTopLevelItem(currentItem()) + 1));
									setSelectionMode(ExtendedSelection);
									break;
		case Qt::Key_Space:	currentItem()->setSelected(!currentItem()->isSelected());
									emit SpacePressed(currentItem());
									break;
		default:					QTreeWidget::keyPressEvent(event);
	} // switch
} // keyPressEvent

// selection changed it directory view
void cTreeWidget::on_ctwTree_itemSelectionChanged()
{
	emit itemSelectionChanged(this);
} // on_ctwTree_itemSelectionChanged