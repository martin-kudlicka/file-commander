/// dir model to handle disk contents

#ifndef DIR_MODEL_H
#define DIR_MODEL_H

#include <QString>
#include <QAbstractItemModel>
#include "Plugins/Content.h"
#include "DirModelItem.h"

class cDirModel : public QAbstractItemModel
{
	public:
		enum ePanel {
							PanelLeft,														///< left panel
							PanelRight														///< right panel
						};

		void RefreshContent();															///< refreshes directory content
																								/**< is called when path for panel changes */
		void RefreshHeader();															///< refreshes header and directory content
																								/**< called when column settings changes */
		void SetContentPlugins(cContent *ccContent);								///< sets pointer to content plugins class
																								/**< \param ccContent content plugin class pointer */
		void SetPanel(const ePanel epPanel);										///< sets for which of panels is DirModel for
																								/**< \param epPanel panel type */
		void SetPath(const QString qsPath);											///< sets path for model
																								/**< \param qsPath some drive path */
		void SetSettings(QSettings *qsSettings);									///< sets main settings "file"
																								/**< \param qsSettings settings "file" */

	private:
		cContent *ccContent;																///< access to content plugins
		cDirModelItem *cdmiRoot;														///< column's header
		ePanel epPanel;																	///< sets this panel as left or right
		QSettings *qsSettings;															///< main settings "file"
		QString qsPath;																	///< path on drive to display

		int columnCount(const QModelIndex &parent = QModelIndex()) const;	///< number of columns
																								/**< \param parent
																									  \return number of columns in dir model */
		QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
																								///< data located on QModelIndex
																								/**< \param	index
																									  \param role
																									  \return data on specified index */
		QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
																								///< index for specified location
																								/**< \param	row
																									  \param column
																									  \param parent
																									  \return index for specified location */
		QModelIndex parent(const QModelIndex &index) const;					///< parent for specified index
																								/**< \param index
																									  \return parent's index */
		int rowCount(const QModelIndex &parent = QModelIndex()) const;		///< number of rows (childs)
																								/**< \param parent
																									  \return row count (childs) of parent */
}; // cDirModel

#endif
