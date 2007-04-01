/// extended QComboBox

#ifndef COMBO_BOX_H
#define COMBO_BOX_H

#include <QComboBox>

class cComboBox : public QComboBox
{
	Q_OBJECT

	private:
		void keyPressEvent(QKeyEvent *event);	///< key pressed in combo box
															/**< \param event key event description */

	signals:
		void KeyPressed(QKeyEvent *qkeEvent);	///< key pressed in command combo box
															/**< \param qkeEvent key event description */

}; // cComboBox

#endif