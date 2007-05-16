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

// - button is clicked on
void cQueueWidget::on_qpbRemove_clicked(bool checked /* false */)
{
	emit RemoveQueuedItems(qlwOperations->selectedItems());
} // on_qpbRemove_clicked
