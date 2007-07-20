/// extended QTreeWidget

#ifndef TREE_WIDGET_H
#define TREE_WIDGET_H

#include <QtGui/QTreeWidget>

const QString qsMIME__ARCHIVE_FILES = "text/archive_files";
const QString qsMIME__ARCHIVE_INFORMATION = "text/archive_information";

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

		void StartDragFromPanel(const QList<QTreeWidgetItem *> &qlIgnore);
																								///< dragging will start from directory view
																								/**< \param qtwiIgnore items to ignore drag move on */
		void StopDragFromPanel();														///< dragging from directory view ended

	private:
		bool bDraggingFromPanel;														///< dragging from directory view flag
		QPoint qpDragStart;																///< drag start position
		QList<QTreeWidgetItem *> qlDragStart;										///< marked items on drag start
		QList<QTreeWidgetItem *> qlIgnore;											///< items to ignore drag move on

		void dragEnterEvent(QDragEnterEvent *event);								///< drag enter event
																								/**< \param event event description */
		void dragLeaveEvent(QDragLeaveEvent *event);								///< drag leave event
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
		void DragEvent();																	///< start dragging of selected objects
		void DropEvent(const cTreeWidget::eDropAction &edaAction, const QList<QUrl> &clUrls, const QString &qsArchiveInformatio, const QString &qsArchiveFiles, QTreeWidgetItem *qtwiDroppedOn);
																								///< drop event occured
																								/**< \param edaAction action to do in this event with source
																									  \param clUrls source objects location
																									  \param qsArchiveInformation address of archive information structure
																									  \param qsArchiveFiles files to extract from archive
																									  \param qtwiDroppedOn item dropped on */
		void GotFocus();																	///< dir view got focus
		void itemSelectionChanged(const cTreeWidget *ctwTree);				///< selection changed it directory view
																								/**< \param ctwTree tree that changes selection */
		void KeyPressed(QKeyEvent *qkeEvent, QTreeWidgetItem *qtwiItem);	///< key pressed in dir view
																								/**< \param qtwiItem item space pressed on */
		void MoveEvent(QTreeWidgetItem *qtwiMovedOver);					///< dragging items
																								/**< \param qtwiMovedOver dragging over qtwiMovedOver item */

	private slots:
		void on_ctwTree_itemSelectionChanged();									///< selection changed it directory view
}; // cTreeWidget

#endif