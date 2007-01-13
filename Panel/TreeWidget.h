/// extended QTreeWidget

#ifndef TREE_WIDGET_H
#define TREE_WIDGET_H

#include <QTreeWidget>

class cTreeWidget : public QTreeWidget
{
	Q_OBJECT

	public:
		cTreeWidget();															///< constructor

	signals:
		void itemSelectionChanged(const cTreeWidget *ctwTree);	///< selection changed it directory view
																					/**< \param ctwTree tree that changes selection */

	private slots:
		void on_ctwTree_itemSelectionChanged();						///< selection changed it directory view
}; // cTreeWidget

#endif