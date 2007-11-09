/// common routines for GUI operations

#ifndef GUI_COMMON_H
#define GUI_COMMON_H

#include "Settings.h"

class cGUICommon
{
	public:
		static const void SetComboBoxHistory(const cSettings::eHistoryType &ehtHistory, QComboBox *qcbComboBox, cSettings *csSettings);
}; // cGUICommon

#endif