/// common archive file system methods

#ifndef ARCHIVE_COMMON_H
#define ARCHIVE_COMMON_H

#include <QtCore/QDateTime>
#include "FileSystem/Archive/InformationDialog.h"
#include <QtGui/QMessageBox>
#include <QtCore/QSemaphore>
#include <QtGui/QTreeWidget>
#include "Settings.h"
#include "Plugins/PackerPlugin.h"

class cArchiveCommon : public QObject
{
	Q_OBJECT

	public:
		static const cPackerPlugin::sPluginInfo GetPackerPluginInfo(const QString &qsPlugin, QHash<QString, cPackerPlugin::sPluginInfo> *qhPackerPlugins);
																						///< find packer plugin description (interface)
																						/**< \param qsPlugin plugin to find description (interface) for
																							  \param qhPackerPlugins packer plugin table
																							  \return plugins' description (interface) */
		static const void CreatePluginsTree(QTreeWidget *qtwPlugins, cSettings *csSettings);
																						///< create packer plugins tree to show in QTreeWidget
																						/**< \param qtwPlugins tree of packer plugins
																							  \param csSettings settings file */
		static const QString GetErrorString(const int &iError);		///< get error string from error code
																						/**< \param iError error code
																							  \return error string */
		static const void SelectDefaultPlugin(QTreeWidget *qtwPlugins, const cSettings *csSettings);
																						///< find default packer plugin in plugin's tree
																						/**< \param qtwPlugins tree of packer plugins
																							  \param csSettings settings file */
		static const void ShowInformationDialog(const cInformationDialog *cidInformationDialog, const enum QMessageBox::Icon &iIcon, const QString &qsInformation, QSemaphore *qsPause);
																						///< show information dialog about processed action
																						/**< \param cidInformationDialog information dialog
																							  \param iIcon dialog's icon
																							  \param qsInformation text to show
																							  \param qsPause thread pause */
		static const QDateTime ToQDateTime(const int &iDateTime);	///< converts packer plugin's date time format to QDateTime
																						/**< \param iDateTime packed file date time
																							  \return date time in Qt format */

	signals:
		void ShowInformationDialog2(const enum QMessageBox::Icon &iIcon, const QString &qsInformation) const;
																						///< show information dialog about processed action
																						/**< \param iIcon dialog's icon
																							  \param qsInformation text to show */
}; // cArchiveCommon

#endif
