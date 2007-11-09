#include "GUICommon.h"

const void cGUICommon::SetComboBoxHistory(const cSettings::eHistoryType &ehtHistory, QComboBox *qcbComboBox, cSettings *csSettings)
{
	int iIndex;
	QString qsComboBox;

	qsComboBox = qcbComboBox->currentText();
	iIndex = qcbComboBox->findText(qsComboBox);
	if (iIndex > 0) {
		qcbComboBox->removeItem(iIndex);
	} // if
	if (iIndex != 0) {
		qcbComboBox->insertItem(0, qsComboBox);
		qcbComboBox->setEditText(qsComboBox);
	} // if
	csSettings->SetComboBoxHistory(ehtHistory, qcbComboBox);
} // SetComboBoxHistory