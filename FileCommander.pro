CONFIG += qt warn_on
//RC_FILE = FileCommander.rc
FORMS = MainWindow.ui \
		  OptionsDialog.ui \
		  ListerMainWindow.ui \
		  ListerMainWindow/FindDialog.ui \
		  FindFilesDialog.ui \
		  FindFilesDialog/DrivesDialog.ui \
		  Panel/SelectDriveDialog.ui \
		  Panel/SelectFilesDialog.ui \
		  FileOperation/FileOperationDialog.ui \
		  FileOperation/CopyMoveDialog.ui \
		  FileOperation/CopyMoveWidget.ui \
		  FileOperation/QueueWidget.ui \
		  FileOperation/DeleteDialog.ui \
		  FileOperation/DeleteWidget.ui \
		  ArchiveOperation/UnpackFilesDialog.ui \
		  ArchiveOperation/ArchiveFilePropertiesDialog.ui \
		  Options/NewFavouriteDirectoryDialog.ui \
		  Common/About.ui
HEADERS = MainWindow.h \
		    OptionsDialog.h \
			 ListerMainWindow.h \
			 ListerMainWindow/FindDialog.h \
			 FindFilesDialog.h \
			 FindFilesDialog/DrivesDialog.h \
			 Panel.h \
			 Panel/TreeWidget.h \
			 Panel/ShellMenu.h \
			 Panel/Process.h \
			 Panel/SelectDriveDialog.h \
			 Panel/SelectFilesDialog.h \
			 FileOperation.h \
			 FileOperation/FileOperationDialog.h \
			 FileOperation/FileRoutine.h \
			 FileOperation/CopyMove.h \
			 FileOperation/CopyMoveDialog.h \
			 FileOperation/CopyMoveWidget.h \
			 FileOperation/CopyMoveConflict.h \
			 FileOperation/Rename.h \
			 FileOperation/Permission.h \
			 FileOperation/QueueWidget.h \
			 FileOperation/Delete.h \
			 FileOperation/DeleteDialog.h \
			 FileOperation/DeleteWidget.h \
			 FileOperation/DeleteNonEmptyDirectory.h \
			 FileOperation/Retry.h \
			 FileOperation/DiskSpace.h \
			 ArchiveOperation.h \
			 ArchiveOperation/UnpackFilesDialog.h \
			 ArchiveOperation/ArchiveFilePropertiesDialog.h \
			 Options/NewFavouriteDirectoryDialog.h \
			 Settings.h \
			 Plugins.h \
			 Plugins/ContentPlugin.h \
			 Plugins/ContentPluginDelayed.h \
			 Plugins/ContPlug.h \
			 Plugins/ListerPlugin.h \
			 Plugins/ListPlug.h \
			 Plugins/PackerPlugin.h \
			 Plugins/WCXHead.h \
			 Plugins/TotalCommanderInterface.h \
			 Common/System.h \
			 Common/About.h
win32 {
	 HEADERS += Panel/Win32/ShellContextMenu.h
}
SOURCES = FileCommander.cpp \
			 MainWindow.cpp \
			 OptionsDialog.cpp \
			 ListerMainWindow.cpp \
			 ListerMainWindow/FindDialog.cpp \
			 FindFilesDialog.cpp \
			 FindFilesDialog/DrivesDialog.cpp \
			 Panel.cpp \
			 Panel/TreeWidget.cpp \
			 Panel/ShellMenu.cpp \
			 Panel/Process.cpp \
			 Panel/SelectDriveDialog.cpp \
			 Panel/SelectFilesDialog.cpp \
			 FileOperation.cpp \
			 FileOperation/FileOperationDialog.cpp \
			 FileOperation/FileRoutine.cpp \
			 FileOperation/CopyMove.cpp \
			 FileOperation/CopyMoveDialog.cpp \
			 FileOperation/CopyMoveWidget.cpp \
			 FileOperation/CopyMoveConflict.cpp \
			 FileOperation/Rename.cpp \
			 FileOperation/Permission.cpp \
			 FileOperation/QueueWidget.cpp \
			 FileOperation/Delete.cpp \
			 FileOperation/DeleteDialog.cpp \
			 FileOperation/DeleteWidget.cpp \
			 FileOperation/DeleteNonEmptyDirectory.cpp \
			 FileOperation/Retry.cpp \
			 FileOperation/DiskSpace.cpp \
			 ArchiveOperation.cpp \
			 ArchiveOperation/UnpackFilesDialog.cpp \
			 ArchiveOperation/ArchiveFilePropertiesDialog.cpp \
			 Options/NewFavouriteDirectoryDialog.cpp \
			 Settings.cpp \
			 Plugins.cpp \
			 Plugins/ContentPlugin.cpp \
			 Plugins/ContentPluginDelayed.cpp \
			 Plugins/ListerPlugin.cpp \
			 Plugins/PackerPlugin.cpp \
			 Common/System.cpp \
			 Common/About.cpp
win32 {
	 SOURCES += Panel/Win32/ShellContextMenu.cpp
}
exists (File Commander.vcproj) {
	win32 {
		LIBS += User32.lib shell32.lib
	}
}