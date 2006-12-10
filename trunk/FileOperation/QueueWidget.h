/// list of queued operations

#ifndef QUEUE_WIDGET_H
#define QUEUE_WIDGET_H

#include "ui_QueueWidget.h"

class cQueueWidget : public QWidget, private Ui::qwQueueWidget
{
	Q_OBJECT

	public:
		cQueueWidget();															///< constructor

	signals:
		void RemoveQueuedItems(QList<QListWidgetItem *> qlItems);	///< remove queued items (operations)
																						/**< \param qlItems operations to remove */

	private slots:
		void on_cFileOperation_AddIntoQueueList(QListWidgetItem *qlwiItem);
																						///< add new item into list
																						/**< \param qlwiItem new item (operation) */
		void on_qpbRemove_clicked(bool checked = false);				///< - button is clicked on
																						/**< \param checked true if button is checkable and checked */
}; // cQueueWidget

#endif