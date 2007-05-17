#include "FileOperation/QueueWidget.h"

// constructor
cQueueWidget::cQueueWidget()
{
	setupUi(this);
} // cQueueWidget

// add new item into list
void cQueueWidget::on_cFileOperation_AddIntoQueueList(QListWidgetItem *qlwiItem)
{
	qlwOperations->addItem(qlwiItem);
} // on_cFileOperation_addItem

// different queued operations selected
void cQueueWidget::on_qlwOperations_itemSelectionChanged()
{
	if (qlwOperations->selectedItems().count() > 0) {
		qpbRemove->setEnabled(true);
	} else {
		qpbRemove->setEnabled(false);
	} // if else
} // on_qlwOperations_itemSelectionChanged

// - button is clicked on
void cQueueWidget::on_qpbRemove_clicked(bool checked /* false */)
{
	emit RemoveQueuedItems(qlwOperations->selectedItems());
} // on_qpbRemove_clicked
