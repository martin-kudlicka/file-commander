/// extended QTreeWidget

#ifndef C_TREE_WIDGET_H
#define C_TREE_WIDGET_H

#include <QtGui/QTreeWidget>
#include <QtCore/QUrl>

const QString qsMIME__SOURCE_FILE_SYSTEM = "text/source_file_system";

class cTreeWidget : public QTreeWidget
{
	Q_OBJECT

	public:
		/// drop action type
		enum eDropAction {
			CopyDropAction,																			///< copy files from source
			ChooseDropAction																			///< show choices copy/move
		};

		cTreeWidget();																					///< constructor

		const void StartDragFromPanel(const QList<QTreeWidgetItem *> &qlIgnore);	///< dragging will start from directory view
																											/**< \param qtwiIgnore items to ignore drag move on */
		const void StopDragFromPanel();															///< dragging from directory view ended

	private:
		bool bDraggingFromPanel;																	///< dragging from directory view flag
		QList<QTreeWidgetItem *> qlDragStart;													///< marked items on drag start
		QList<QTreeWidgetItem *> qlIgnore;														///< items to ignore drag move on
		QPoint qpDragStart;																			///< drag start position

		void dragEnterEvent(QDragEnterEvent *event);											///< drag enter event
																											/**< \param event event description */
		void dragLeaveEvent(QDragLeaveEvent *event);											///< drag leave event
																											/**< \param event event description */
		void dragMoveEvent(QDragMoveEvent *event);											///< drag move event
																											/**< \param event event description */
		void dropEvent(QDropEvent *event);														///< drop of object in dir view
																											/**< \param event event description */
		void focusInEvent(QFocusEvent *event);													///< dir view got focus
																											///< \param event incoming focus event */
		void keyPressEvent(QKeyEvent *event);													///< handle key processing
																											/**< \param event incoming key event */
		void mouseMoveEvent(QMouseEvent *event);												///< mouse move in dir view
																											/**< \param event event description */
		void mousePressEvent(QMouseEvent *event);												///< mouse click in dir view
																											/**< \param event event description */

	signals:
		void DragEvent() const;																		///< start dragging of selected objects
		void DropEvent(const cTreeWidget::eDropAction &edaAction, const QList<QUrl> &qlUrls, const QString &qsSourceFileSystem, QTreeWidgetItem *qtwiDroppedOn);
																											///< drop event occured
																											/**< \param edaAction action to do in this event with source
																												  \param qlUrls source objects location
																												  \param qsSourceFileSystem address of source file system in memory
																												  \param qtwiDroppedOn item dropped on */
		void GotFocus() const;																		///< dir view got focus
		void KeyPressed(QKeyEvent *qkeEvent) const;											///< key pressed in dir view
																											/**< \param qkeEvent key event description */
		void MoveEvent(QTreeWidgetItem *qtwiMovedOver) const;								///< dragging items
																											/**< \param qtwiMovedOver dragging over qtwiMovedOver item */
}; // cTreeWidget

#endif
