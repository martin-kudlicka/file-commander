/// file control

#ifndef FILE_CONTROL_H
#define FILE_CONTROL_H

#include "Settings.h"
#include <QtGui/QHBoxLayout>
#include <QtGui/QMainWindow>
#include "FileSystem.h"
#include "Plugins/ContentPlugin.h"
#include "Plugins/ListerPlugin.h"
#include "FileControl/FileOperationDialog.h"
#include <QtCore/QQueue>
#include "FileControl/QueueWidget.h"

class cFileControl : public QObject
{
	Q_OBJECT

	public:
		/// drive types
		enum eDriveType {
			Local																		///< local drive
		};

		/// drive information
		struct sDrive {
			QString qsPath;														///< path to drive's root
			eDriveType edtType;													///< drive type
			// TODO sDrive add drive type and show next to drive letters
		};
		/// path information
		struct sPathInfo {
			QString qsDrive;														///< path's drive
			QString qsRootPath;													///< drive's root path
			eDriveType edtType;													///< drive type
		};

		cFileControl(QMainWindow *qmwParent, QHBoxLayout *qhblOperations, cSettings *csSettings, cContentPlugin *ccpContentPlugin, cListerPlugin *clpListerPlugin);
																						///< constructor
																						/**< \param qmwParent parent window for dialogs
																							  \param qbnlOperations layout for background and queued operations
																							  \param csSettings application's configuration
																							  \param ccpContentPlugin content plugin interface
																							  \param clpListerPlugin lister plugin interface */

		const bool ChangeFileSystem(const cFileSystem *cfsFileSystem, const QString &qsDrive, const QString &qsPath) const;
																						///< change file system according to new drive
																						/**< \param cfsFileSystem currently set file system
																							  \param qsDrive drive to get file system for
																							  \param startup path */
		const void CloseFileSystem(cFileSystem *cfsFileSystem) const;
																						///< close used file system
																						/**< \param cfsFileSystem file system to close */
		const void CompareDirectories(cFileSystem *cfsLeft, cFileSystem *cfsRight) const;
																						///< compare directories in both panels
																						/**< \param cfsLeft left panel's file system
																							  \param cfsRight right panel's file system */
		const QString CreateDirectory(cFileSystem *cfsFileSystem) const;
																						///< create new directory
																						/**< \param cfsFileSystem file system to create new directory in
																							  \return new directory name */
		const void Edit(const cFileSystem *cfsFileSystem, const QList<QTreeWidgetItem *> qlSelectedFiles) const;
																						///< edit selected files
																						/**< \param cfsFileSystem file system
																							  \param qlSelectedFiles files to edit */
		const QList<QPair<QString, sDrive> > GetDrives() const;		///< get accessible drives
		cFileSystem *GetFileSystem(const QString &qsDrive, const QString &qsPath) const;
																						///< get file system for specified drive
																						/**< \param qsDrive drive to get file system for
																							  \param qsPath startup path
																							  \return file system handling class */
		const QPair<QString, sDrive> GetFirstDrive() const;			///< returns first accessible drive for application
																						/**< \param first accessible drive for application */
		const sPathInfo GetPathInfo(const QString &qsPath) const;	///< information about path
																						/**< \param qsPath path to detect information
																							  \return path's information */
		const void Operation(const cFileOperationDialog::eOperation &eoOperation, cFileSystem *cfsSource, QList<QTreeWidgetItem *> qlSource, const cFileSystem *cfsDestination);
																						///< file operation selected
																						/**< \param eoOperation type of operation
																							  \param cfsSource source file system
																							  \param qlSource selected source files
																							  \param cfsDestination destination file system */
		const void StartTerminal(const QString &qsPath) const;		///< start shell command window
																						/**< \param qsPath path to start shell in */
		const void View(const cFileSystem *cfsFileSystem, const QList<QTreeWidgetItem *> qlSelectedFiles) const;
																						///< view selected files
																						/**< \param cfsFileSystem file system
																							  \param qlSelectedFiles files to show */
		const void View(const cFileSystem *cfsFileSystem, QTreeWidgetItem *qtwiFile) const;
																						///< view selected file
																						/**< \param cfsFileSystem file system
																							  \param qtwiFile file to show */

	private:
		static const int iQUEUE_WIDGET_POS = 0;							///< position of queue widget on top layout

		/// operation description
		struct sOperation {
			cFileOperationDialog::eOperation eoType;						///< type of operation
			cFileSystem *cfsSource;												///< source file system
			cFileSystem *cfsDestination;										///< destination file system
			QString qsFilter;														///< filter for source files
		};
		/// count of file types
		struct sTypeCount {
			uint FileType;															///< file
			uint DirectoryType;													///< directory
		};

		cContentPlugin *ccpContentPlugin;									///< content plugin interface
		cFileSystem *cfsInQueue;												///< source file system processed in queue
		cListerPlugin *clpListerPlugin;										///< lister plugin interface
		cQueueWidget cqwQueue;													///< list of queued operations
		cSettings *csSettings;													///< application's configuration
		QHBoxLayout *qhblOperations;											///< background and queued operation windows
		QList<sOperation> qlOperations;										///< operations in processing
		QQueue<QPair<QListWidgetItem *, sOperation> > qqOperations;	///< queued operations
		QMainWindow *qmwParent;													///< parent window for dialogs

		const void Enqueue(const sOperation &soOperation);				///< place operation into queue
																						/**< \param soOperation operation description */
		const sTypeCount GetFilesTypeCount(const cFileSystem *cfsFileSystem, const QList<QTreeWidgetItem *> qlFiles) const;
																						///< count files type
																						/**< \param cfsFileSystem file system
																							  \param qlFiles files to count
																							  \return files type count */
		const void ProcessQueue();												///< process first queued operation

	signals:
		signals:
		void AddIntoQueueList(QListWidgetItem *qlwiItem) const;		///< add new item into queue list
																						/**< \param qlwiItem operation to queue */

	private slots:
		const void on_cFileSystem_OperationFinished(cFileSystem *cfsFileSystem);
																						///< file operation finished
																						/**< \param cfsFileSystem filesystem identifier */
		const void on_cqwQueue_RemoveQueuedItems(const QList<QListWidgetItem *> &qlItems);
																						///< remove queued items (operations)
																						/**< \param qlItems operations to remove */
}; // cFileControl

#endif
