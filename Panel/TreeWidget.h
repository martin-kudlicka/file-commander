/// extended QTreeWidget

#ifndef TREE_WIDGET_H
#define TREE_WIDGET_H

#include <QtGui/QTreeWidget>

class cTreeWidget : public QTreeWidget
{
	Q_OBJECT

	public:
		/// drop action type
		enum eDropAction {
			CopyDropAction,																///< copy files from source
			ChooseDropAction																///< show choices copy/move
		};

		cTreeWidget();																		///< constructor

		void StartDragFromPanel();														///< dragging will start from directory view
		void StopDragFromPanel();														///< dragging from directory view ended

	private:
		bool bDraggingFromPanel;														///< dragging from directory view flag
		QPoint qpDragStart;																///< drag start position

		void dragEnterEvent(QDragEnterEvent *event);								///< drag enter event
																								/**< \param event event description */
		void dragMoveEvent(QDragMoveEvent *event);								///< drag move event
																								/**< \param event event description */
		void dropEvent(QDropEvent *event);											///< drop of object in dir view
																								/**< \param event event description */
		void focusInEvent(QFocusEvent *event);										///< dir view got focus
																								///< \param event incoming focus event */
		void keyPressEvent(QKeyEvent *event);										///< handle key processing
																								/**< \param event incoming key event */
		void mouseMoveEvent(QMouseEvent *event);									///< mouse move in dir view
																								/**< \param event event description */
		void mousePressEvent(QMouseEvent *event);									///< mouse click in dir view
																								/**< \param event event description */

	signals:
		void DragEvent(cTreeWidget *ctwSource);									///< start dragging of selected objects
																								/**< \param ctwSource source dir view */
		void DropEvent(const cTreeWidget::eDropAction &edaAction, const QList<QUrl> &clUrls, QTreeWidgetItem *qtwiDroppedOn);
																								///< drop event occured
																								/**< \param edaAction action to do in this event with source
																									  \param clUrls source objects location
																									  \param qtwiDroppedOn item dropped on */
		void GotFocus();																	///< dir view got focus
		void itemSelectionChanged(const cTreeWidget *ctwTree);				///< selection changed it directory view
																								/**< \param ctwTree tree that changes selection */
		void KeyPressed(QKeyEvent *qkeEvent, QTreeWidgetItem *qtwiItem);	///< key pressed in dir view
																								/**< \param qtwiItem item space pressed on */

	private slots:
		void on_ctwTree_itemSelectionChanged();									///< selection changed it directory view
}; // cTreeWidget

#endif
