#include "FileControl/QueueWidget.h"

// constructor
cQueueWidget::cQueueWidget()
{
	setupUi(this);
} // cQueueWidget

// add new item into list
const void cQueueWidget::on_cFileOperation_AddIntoQueueList(QListWidgetItem *qlwiItem) const
{
	qlwOperations->addItem(qlwiItem);
} // on_cFileOperation_addItem

// different queued operations selected
const void cQueueWidget::on_qlwOperations_itemSelectionChanged() const
{
	if (qlwOperations->selectedItems().count() > 0) {
		qpbRemove->setEnabled(true);
	} else {
		qpbRemove->setEnabled(false);
	} // if else
} // on_qlwOperations_itemSelectionChanged

// - button is clicked on
const void cQueueWidget::on_qpbRemove_clicked(bool checked /* false */) const
{
	emit RemoveQueuedItems(qlwOperations->selectedItems());
} // on_qpbRemove_clicked
