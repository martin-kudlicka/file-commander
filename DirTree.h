/// directory representation in tree view

#ifndef DIR_TREE_H
#define DIR_TREE_H

#include <QTreeWidget>
#include "Plugins/Content.h"

class cDirTree
{
	public:
		enum eTreePanel {
			TreePanelLeft,											///< left panel
			TreePanelRight											///< right panel
		};

		cContent *ccContent;										///< access to content plugins
		eTreePanel etpPanel;										///< tree panel position
		QSettings *qsSettings;									///< main settings "file"
		QTreeWidget *qtwTreeWidget;							///< tree widget to be filled by this class

		void RefreshHeader();									///< refreshes header and directory content
																		/**< called when column settings changes */
		void SetPath(const QString qsPath);					///< sets path for view
																		/**< \param qsPath some drive path */

	private:
		/// column in dir tree view
		struct sColumn {
			QString qsName;										///< visible column name
			QString qsPlugin;										///< plugin filename or "no" if native
		};

		QList<sColumn> qlColumns;								///< current columns in dir view
		QString qsPath;											///< path on drive to display
		QTreeWidgetItem *qtwiHeader;							///< dir panel header

		void RefreshContent();									///< refreshes directory content
																		/**< is called when path for panel changes */
}; // cDirTree

#endif