#include "Common/About.h"

// constructor
cAbout::cAbout(QMainWindow *qmwParent, const QString &qsApplication, const QString &qsVersion)
{
	setParent(qmwParent, windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
	setupUi(this);

	qlApplication->setText(qsApplication);
	qlVersion->setText(qsVersion);
	qlQt->setText(QString("%1 %2").arg("Qt ").arg(QT_VERSION_STR));
} // cAbout
