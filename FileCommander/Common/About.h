/// about dialog

#ifndef ABOUT_H
#define ABOUT_H

#include "ui_About.h"

#include <QtGui/QMainWindow>

class cAbout : public QDialog, public Ui::qdAbout
{
	Q_OBJECT

	public:
		cAbout(QMainWindow *qmwParent, const QString &qsApplication, const QString &qsVersion);	///< constructor
																																/**< \param qmwParent parent window
																																	  \param qsApplication application name
																																	  \param qsVersion application's version */
}; // cAbout

#endif
