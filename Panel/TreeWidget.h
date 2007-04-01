/// extended QTreeWidget

#ifndef TREE_WIDGET_H
#define TREE_WIDGET_H

#include <QTreeWidget>

class cTreeWidget : public QTreeWidget
{
	Q_OBJECT

	public:
		cTreeWidget();															///< constructor

	private:
		void focusInEvent(QFocusEvent *event);							///< dir view got focus
																					///< \param event incoming focus event */
		void keyPressEvent(QKeyEvent *event);							///< handle key processing
																					/**< \param event incoming key event */

	signals:
		void GotFocus();														///< dir view got focus
		void itemSelectionChanged(const cTreeWidget *ctwTree);	///< selection changed it directory view
																					/**< \param ctwTree tree that changes selection */
		void SpacePressed(QTreeWidgetItem *qtwiItem);				///< space pressed in dir view
																					/**< \param qtwiItem item space pressed on */

	private slots:
		void on_ctwTree_itemSelectionChanged();						///< selection changed it directory view
}; // cTreeWidget

#endif