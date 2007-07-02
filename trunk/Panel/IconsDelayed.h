/// show icons as delayed content

#ifndef ICONS_DELAYED_H
#define ICONS_DELAYED_H

#include <QtCore/QThread>
#include <QtGui/QTreeWidgetItem>

class cIconsDelayed : public QThread
{
	Q_OBJECT

	public:
		/// thread input parameters
		struct sInput {
			QString qsFilename;											///< file to extract icon from
		};
		/// thread output parameters
		struct sOutput {
			QTreeWidgetItem *qtwiItem;									///< dir view item
			int iColumn;													///< column index to update in qtwiItem
			QIcon qiIcon;													///< icon to show
		};
		/// thread parameters
		struct sParameters {
			sInput siInput;												///< input parameters
			sOutput soOutput;												///< output parameters
		};

		cIconsDelayed();													///< constructor

		void Start(const QList<sParameters> &qlParameters);	///< start thread processing
																				/**< \param qlParameters icon list description */
		void run();															///< main thread

	private:
		bool bStop;															///< interrupt thread process
		QList<sParameters> qlParameters;								///< icon list description

	signals:
		void GotIcon(const cIconsDelayed::sOutput &soOutput);	///< got column value (icon)
																				/**< \param soOutput information to update dir view */

	private slots:
		void on_InterruptIconsDelayed();								///< interrupt delayed icons processing before refresh dir view content
}; // cIconsDelayed

#endif
