#include "Common/System.h"

#include <QtCore/QSettings>
#include <QtCore/QDir>

// settings path for application
const QString cSystem::GetApplicationSettingsPath(const QString &qsApplication)
{
	QDir qdDir;
	QString qsPath;
	
	qsPath = GetIsshouSettingsPath();
	qdDir.mkdir(qsPath + "/" + qsApplication);
	return qsPath + "/" + qsApplication;
} // GetApplicationSettingsPath

// common program files path
const QString cSystem::GetCommonFilesPath()
{
	// TODO GetCommonFilesPath - Windows only
	QString qsResult;

#ifdef Q_WS_WIN
	QSettings qsRegistry("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion", QSettings::NativeFormat);

	qsResult = qsRegistry.value("CommonFilesDir").toString();
#endif

	return qsResult;
} // GetCommonFilesPath

// first available drive in system
const QString cSystem::GetFirstDrive()
{
	QFileInfoList qfilDrives;

	qfilDrives = QDir::drives();
	return qfilDrives.at(0).path();
} // GetFirstDrive

// Isshou/Common/QT path
const QString cSystem::GetIsshouCommonPath()
{
	QString qsResult;

	qsResult = GetCommonFilesPath();
	qsResult += "/Isshou";

	return qsResult;
} // GetIsshouCommonPath

// settings path for applications
const QString cSystem::GetIsshouSettingsPath()
{
	QDir qdDir;
	QFileInfo qfiFile;
	QSettings qsSettings(QSettings::IniFormat, QSettings::UserScope, qsCOMPANY);
	
	qfiFile.setFile(qsSettings.fileName());
	qdDir.mkdir(qfiFile.absolutePath() + "/" + qsCOMPANY);
	
	return qfiFile.absolutePath() + "/" + qsCOMPANY;
} // GetIsshouSettingsPath

// program files path
const QString cSystem::GetProgramFilesPath()
{
	// TODO GetProgramFilesPath - Windows only
	QString qsResult;

#ifdef Q_WS_WIN
	QSettings qsRegistry("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion", QSettings::NativeFormat);

	qsResult = qsRegistry.value("ProgramFilesDir").toString();
#endif

	return qsResult;
} // GetProgramFilesPath
