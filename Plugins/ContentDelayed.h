/// get delayed plugin values

#ifndef CONTENT_DELAYED_H
#define CONTENT_DELAYED_H

#include <QtCore/QThread>
#include <QtGui/QTreeWidgetItem>
#include "Plugins/Content.h"

class cContentDelayed : public QThread
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

		cContentDelayed(cContent *ccContent);						///< constructor
																				/**< \param ccContent content plugins */

		void Start(const QList<sParameters> &qlParameters);	///< start thread processing
																				/**< \param qlParameters description what to check */
		void run();															///< main thread

	private:
		bool bStop;															///< interrupt thread process
		cContent *ccContent;												///< content plugins
		QList<sParameters> qlParameters;								///< description what to check
		QString qsCurrentFile;											///< currently checked file
		QString qsCurrentPlugin;										///< currently used plugin

	signals:
		void GotColumnValue(const cContentDelayed::sOutput &soOutput);	
																				///< got golumn value from plugin
																				/**< \param soOutput information to update dir view */

	private slots:
		void on_InterruptContentDelayed();							///< interrupt delayed content processing before refresh dir view content
}; // cContentDelayed

#endif
