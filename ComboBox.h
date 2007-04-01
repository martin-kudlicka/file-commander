/// extended QComboBox

#ifndef COMBO_BOX_H
#define COMBO_BOX_H

#include <QComboBox>

class cComboBox : public QComboBox
{
	Q_OBJECT

	private:
		void keyPressEvent(QKeyEvent *event);	///< key pressed in combo box

	signals:
		void EnterPressed();							///< command confirmed

}; // cComboBox

#endif