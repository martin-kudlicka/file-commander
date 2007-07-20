/// get delayed plugin values

#ifndef CONTENT_PLUGIN_DELAYED_H
#define CONTENT_PLUGIN_DELAYED_H

#include <QtCore/QThread>
#include <QtGui/QTreeWidgetItem>
#include "Plugins/ContentPlugin.h"
#include <QtCore/QQueue>

class cContentPluginDelayed : public QThread
{
	Q_OBJECT

	public:
		/// thread input parameters
		struct sInput {
			QString qsFilename;											///< file to probe
			QString qsPlugin;												///< plugin to check with
			QString qsColumn;												///< plugin's column name
			QString qsUnit;												///< column's unit
		};

		/// thread output parameters
		struct sOutput {
			QTreeWidgetItem *qtwiItem;									///< dir view item
			int iColumn;													///< column index to update in qtwiItem
			QString qsValue;												///< value to update column with
		};

		/// thread parameters
		struct sParameters {
			sInput siInput;												///< input parameters
			sOutput soOutput;												///< output parameters
		};

		cContentPluginDelayed(cContentPlugin *ccpContentPlugin);
																				///< constructor
																				/**< \param ccpContentPlugin content plugins */

		const void Start(const QQueue<sParameters> &qqParameters);
																				///< start thread processing
																				/**< \param qqParameters description what to check */
		void run();															///< main thread

	private:
		bool bStop;															///< interrupt thread process
		cContentPlugin *ccpContentPlugin;							///< content plugins
		QQueue<sParameters> qqParameters;							///< description of what to check
		QString qsCurrentFile;											///< currently checked file
		QString qsCurrentPlugin;										///< currently used plugin

	signals:
		void GotColumnValue(const cContentPluginDelayed::sOutput &soOutput) const;
																				///< got golumn value from plugin
																				/**< \param soOutput information to update dir view */

	private slots:
		const void on_InterruptContentDelayed();					///< interrupt delayed content processing before refresh dir view content
}; // cContentPluginDelayed

#endif
