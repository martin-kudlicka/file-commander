#include "Panel/TreeWidget.h"

// constructor
cTreeWidget::cTreeWidget()
{
	connect(this, SIGNAL(itemSelectionChanged()), SLOT(on_ctwTree_itemSelectionChanged()));
} // cTreeWidget

// selection changed it directory view
void cTreeWidget::on_ctwTree_itemSelectionChanged()
{
	emit itemSelectionChanged(this);
} // on_ctwTree_itemSelectionChanged