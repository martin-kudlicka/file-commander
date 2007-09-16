CONFIG += qt warn_on
//RC_FILE = FileCommander.rc
INCLUDEPATH += .
UI_DIR = UI

CONFIG(debug, debug|release) {
	MOC_DIR = Debug/Moc
	OBJECTS_DIR = Debug/Obj
}
CONFIG(release, debug|release) {
	MOC_DIR = Release/Moc
	OBJECTS_DIR = Release/Obj
}

FORMS = MainWindow.ui \
		  FileControl/FileOperationDialog.ui \
		  FileControl/QueueWidget.ui \
		  FileControl/SelectDriveDialog.ui \
		  FileSystem/DeleteDialog.ui \
		  FileSystem/DeleteWidget.ui \
		  FileSystem/CopyMoveDialog.ui \
		  FileSystem/CopyMoveWidget.ui \
		  FileSystem/Archive/ArchiveFilePropertiesDialog.ui \
		  FileSystem/Archive/UnpackFilesDialog.ui \
		  Panel/SelectFilesDialog.ui \
		  FindFilesDialog.ui \
		  FindFilesDialog/DrivesDialog.ui \
		  OptionsDialog.ui \
		  OptionsDialog/NewFavouriteDirectoryDialog.ui \
		  ListerMainWindow.ui \
		  ListerMainWindow/FindDialog.ui \
		  Common/About.ui

HEADERS = MainWindow.h \
			 FileControl.h \
			 FileControl/Process.h \
			 FileControl/FileOperationDialog.h \
			 FileControl/QueueWidget.h \
			 FileControl/SelectDriveDialog.h \
			 FileSystem.h \
			 FileSystem/Local.h \
			 FileSystem/FileOperation.h \
			 FileSystem/DeleteDialog.h \
			 FileSystem/DeleteNonEmptyDirectory.h \
			 FileSystem/DeleteWidget.h \
			 FileSystem/Permission.h \
			 FileSystem/Retry.h \
			 FileSystem/CopyMoveConflict.h \
			 FileSystem/CopyMoveDialog.h \
			 FileSystem/CopyMoveWidget.h \
			 FileSystem/DiskSpace.h \
			 FileSystem/Rename.h \
			 FileSystem/Archive.h \
			 FileSystem/Local/ShellMenu.h \
			 FileSystem/Local/LocalDelete.h \
			 FileSystem/Local/LocalCommon.h \
			 FileSystem/Local/LocalCopyMove.h \
			 FileSystem/Archive/ArchiveFilePropertiesDialog.h \
			 FileSystem/Archive/ArchiveCommon.h \
			 FileSystem/Archive/ArchiveCopy.h \
			 FileSystem/Archive/Continue.h \
			 FileSystem/Archive/UnpackFilesDialog.h \
			 Panel.h \
			 Panel/TreeWidget.h \
			 Panel/SelectFilesDialog.h \
			 FindFilesDialog.h \
			 FindFilesDialog/DrivesDialog.h \
			 FindFilesDialog/FindFilesThread.h \
			 Settings.h \
			 OptionsDialog.h \
			 OptionsDialog/NewFavouriteDirectoryDialog.h \
			 ListerMainWindow.h \
			 ListerMainWindow/FindDialog.h \
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
	 HEADERS += FileSystem/Local/Win32/ShellContextMenu.h
}

SOURCES = FileCommander.cpp \
			 MainWindow.cpp \
			 FileControl.cpp \
			 FileControl/Process.cpp \
			 FileControl/FileOperationDialog.cpp \
			 FileControl/QueueWidget.cpp \
			 FileControl/SelectDriveDialog.cpp \
			 FileSystem.cpp \
			 FileSystem/Local.cpp \
			 FileSystem/FileOperation.cpp \
			 FileSystem/DeleteDialog.cpp \
			 FileSystem/DeleteNonEmptyDirectory.cpp \
			 FileSystem/DeleteWidget.cpp \
			 FileSystem/Permission.cpp \
			 FileSystem/Retry.cpp \
			 FileSystem/CopyMoveConflict.cpp \
			 FileSystem/CopyMoveDialog.cpp \
			 FileSystem/CopyMoveWidget.cpp \
			 FileSystem/DiskSpace.cpp \
			 FileSystem/Rename.cpp \
			 FileSystem/Archive.cpp \
			 FileSystem/Local/ShellMenu.cpp \
			 FileSystem/Local/LocalDelete.cpp \
			 FileSystem/Local/LocalCommon.cpp \
			 FileSystem/Local/LocalCopyMove.cpp \
			 FileSystem/Archive/ArchiveFilePropertiesDialog.cpp \
			 FileSystem/Archive/ArchiveCommon.cpp \
			 FileSystem/Archive/ArchiveCopy.cpp \
			 FileSystem/Archive/Continue.cpp \
			 FileSystem/Archive/UnpackFilesDialog.cpp \
			 Panel.cpp \
			 Panel/TreeWidget.cpp \
			 Panel/SelectFilesDialog.cpp \
			 FindFilesDialog.cpp \
			 FindFilesDialog/DrivesDialog.cpp \
			 FindFilesDialog/FindFilesThread.cpp \
			 Settings.cpp \
			 OptionsDialog.cpp \
			 OptionsDialog/NewFavouriteDirectoryDialog.cpp \
			 ListerMainWindow.cpp \
			 ListerMainWindow/FindDialog.cpp \
			 Plugins.cpp \
			 Plugins/ContentPlugin.cpp \
			 Plugins/ContentPluginDelayed.cpp \
			 Plugins/ListerPlugin.cpp \
			 Plugins/PackerPlugin.cpp \
			 Common/System.cpp \
			 Common/About.cpp
win32 {
	 SOURCES += FileSystem/Local/Win32/ShellContextMenu.cpp
}

exists (File Commander.vcproj) {
	win32 {
		LIBS += User32.lib shell32.lib
	}
}