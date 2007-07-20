#ifndef SYSTEM_H
#define SYSTEM_H

#include <QtCore/QString>

const QString qsCOMPANY = "Isshou";

/// system functions

class cSystem
{
	public:
		static QString GetApplicationSettingsPath(const QString &qsApplication);	///< settings path for application
																											/**< \param qsApplication application's name
																												  \return settings path for application */
		static QString GetCommonFilesPath();													///< common program files path
																											/**< \return system common files path */
		static QString GetFirstDrive();															///< first available drive in system
																											/**< \return first available drive in system */
		static QString GetIsshouCommonPath();													///< Isshou/Common/QT path
																											/**< \return Isshou/Common/QT path */
		static QString GetIsshouSettingsPath();												///< settings path for applications
																											/**< \return settings path for applications */
		static QString GetProgramFilesPath();													///< program files path
																											/**< \return program files path */
}; // cSystem

#endif
