/// extended QTreeWidget

#ifndef TREE_WIDGET_H
#define TREE_WIDGET_H

#include <QtGui/QTreeWidget>

class cTreeWidget : public QTreeWidget
{
	Q_OBJECT

	private:
		void focusInEvent(QFocusEvent *event);			///< dir view got focus
																	///< \param event incoming focus event */
		void keyPressEvent(QKeyEvent *event);			///< handle key processing
																	/**< \param event incoming key event */

	signals:
		void GotFocus() const;								///< dir view got focus
		void KeyPressed(QKeyEvent *qkeEvent) const;	///< key pressed in dir view
																	/**< \param qkeEvent key event description */
}; // cTreeWidget

#endif
