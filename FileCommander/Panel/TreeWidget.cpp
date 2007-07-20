#include "Panel/TreeWidget.h"

#include <QtGui/QKeyEvent>
#include <QtCore/QUrl>
#include <QtGui/QApplication>

// constructor
cTreeWidget::cTreeWidget()
{
	connect(this, SIGNAL(itemSelectionChanged()), SLOT(on_ctwTree_itemSelectionChanged()));
	setDragDropMode(QAbstractItemView::DragDrop);
	bDraggingFromPanel = false;
} // cTreeWidget

// drag enter event
void cTreeWidget::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasUrls() || event->mimeData()->hasFormat(qsMIME__ARCHIVE_INFORMATION)) {
		event->acceptProposedAction();
	} // if
} // dragEnterEvent

// drag leave event
void cTreeWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
	emit MoveEvent(NULL);
} // dragLeaveEvent

// drag move event
void cTreeWidget::dragMoveEvent(QDragMoveEvent *event)
{
	if (bDraggingFromPanel
		 && (qlDragStart.contains(itemAt(event->pos())) || !itemAt(event->pos()) || qlIgnore.contains(itemAt(event->pos())))) {
		event->ignore(visualItemRect(itemAt(event->pos())));
	} else {
		event->acceptProposedAction();
	} // if else

	emit MoveEvent(itemAt(event->pos()));
} // dragMoveEvent

// drop of object in dir view
void cTreeWidget::dropEvent(QDropEvent *event)
{
	if (event->mouseButtons() == Qt::LeftButton) {
		emit DropEvent(CopyDropAction, event->mimeData()->urls(), event->mimeData()->data(qsMIME__ARCHIVE_INFORMATION), event->mimeData()->data(qsMIME__ARCHIVE_FILES), itemAt(event->pos()));
	} else {
		emit DropEvent(ChooseDropAction, event->mimeData()->urls(), event->mimeData()->data(qsMIME__ARCHIVE_INFORMATION), event->mimeData()->data(qsMIME__ARCHIVE_FILES), itemAt(event->pos()));
	} // if else
	event->acceptProposedAction();
} // dropEvent

// dir view got focus
void cTreeWidget::focusInEvent(QFocusEvent *event)
{
	QTreeWidget::focusInEvent(event);
	emit GotFocus();
} // focusInEvent

// handle key processing
void cTreeWidget::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
		case Qt::Key_Insert:
			setSelectionMode(MultiSelection);
			setCurrentItem(topLevelItem(indexOfTopLevelItem(currentItem()) + 1));
			setSelectionMode(ExtendedSelection);
			break;
		case Qt::Key_Space:
			currentItem()->setSelected(!currentItem()->isSelected());
			emit KeyPressed(event, currentItem());
			break;
		default:
			if (((event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
				  && !(event->modifiers() & Qt::ControlModifier || event->modifiers() & Qt::ShiftModifier))
				 || event->text().isEmpty()) {
				QTreeWidget::keyPressEvent(event);
			} else {
				emit KeyPressed(event, currentItem());
			} // if else
	} // switch
} // keyPressEvent

// mouse move in dir view
void cTreeWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() & (Qt::LeftButton | Qt::RightButton)) {
		if ((event->pos() - qpDragStart).manhattanLength() >= QApplication::startDragDistance()) {
			emit DragEvent();
		} // if
	} // if
} // mouseMoveEvent

// mouse click in dir view
void cTreeWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton) {
		qpDragStart = event->pos();
	} // if

	QTreeWidget::mousePressEvent(event);
} // mousePressEvent

// selection changed it directory view
void cTreeWidget::on_ctwTree_itemSelectionChanged()
{
	emit itemSelectionChanged(this);
} // on_ctwTree_itemSelectionChanged

// dragging will start from directory view
void cTreeWidget::StartDragFromPanel(const QList<QTreeWidgetItem *> &qlIgnore)
{
	bDraggingFromPanel = true;
	qlDragStart = selectedItems();
	this->qlIgnore = qlIgnore;
} // StartDragFromPanel

// dragging from directory view ended
void cTreeWidget::StopDragFromPanel()
{
	bDraggingFromPanel = false;
} // StopDragFromPanel
