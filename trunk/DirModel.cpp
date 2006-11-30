#include "DirModel.h"

// number of columns
int cDirModel::columnCount(const QModelIndex &parent) const
{
	// TODO columnCount
	return 0;
} // columnCount

// data located on QModelIndex
QVariant cDirModel::data(const QModelIndex &index, int role) const
{
	// TODO data
	return 0;
} // data

// index for specified location
QModelIndex cDirModel::index(int row, int column, const QModelIndex &parent) const
{
	// TODO index
	return QModelIndex();
} // index

// parent for specified index
QModelIndex cDirModel::parent(const QModelIndex &index) const
{
	// TODO parent
	return QModelIndex();
} // parent

// number of rows (childs)
int cDirModel::rowCount(const QModelIndex &parent) const
{
	// TODO rowCount
	return 0;
} // rowCount

// refreshes directory content
void cDirModel::RefreshContent()
{
	// TODO RefreshContent
} // RefreshContent

// refreshes header and directory content
void cDirModel::RefreshHeader()
{
	// TODO RefreshHeader

	RefreshContent();
} // RefreshHeader

// sets pointer to content plugins class
void cDirModel::SetContentPlugins(cContent *ccContent)
{
	this->ccContent = ccContent;
} // SetContentPlugins

// sets for which of panels is DirModel for
void cDirModel::SetPanel(const ePanel epPanel)
{
	this->epPanel = epPanel;
} // SetPanel

// sets path for model
void cDirModel::SetPath(const QString qsPath)
{
	this->qsPath = qsPath;
	RefreshContent();
} // SetPath

// sets main settings "file"
void cDirModel::SetSettings(QSettings *qsSettings)
{
	this->qsSettings = qsSettings;// set main settings "file"
} // SetSettings