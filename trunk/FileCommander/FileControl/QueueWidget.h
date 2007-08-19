/// list of queued operations

#ifndef QUEUE_WIDGET_H
#define QUEUE_WIDGET_H

#include "ui_QueueWidget.h"

class cQueueWidget : public QWidget, private Ui::qwQueueWidget
{
	Q_OBJECT

	public:
		cQueueWidget();																	///< constructor

	signals:
		void RemoveQueuedItems(const QList<QListWidgetItem *> &qlItems) const;	///< remove queued items (operations)
																								/**< \param qlItems operations to remove */

	private slots:
		const void on_cFileOperation_AddIntoQueueList(QListWidgetItem *qlwiItem) const;
																								///< add new item into list
																								/**< \param qlwiItem new item (operation) */
		const void on_qlwOperations_itemSelectionChanged() const;			///< different queued operations selected
		const void on_qpbRemove_clicked(bool checked = false) const;		///< - button is clicked on
																								/**< \param checked true if button is checkable and checked */
}; // cQueueWidget

#endif
