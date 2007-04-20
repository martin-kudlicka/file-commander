CONFIG += qt warn_on
//RC_FILE = FileCommander.rc
FORMS = MainWindow.ui \
		  OptionsDialog.ui \
		  Options/NewColumnSetDialog.ui \
		  NewDirectoryDialog.ui \
		  ListerMainWindow.ui \
		  ListerMainWindow/FindDialog.ui \
		  FindFilesDialog.ui \
		  FindFilesDialog/DrivesDialog.ui \
		  FindFilesDialog/SaveFindSettingsDialog.ui \
		  Panel/SelectDriveDialog.ui \
		  Panel/SelectFilesDialog.ui \
		  FileOperation/FileOperationDialog.ui \
		  FileOperation/CopyMoveDialog.ui \
		  FileOperation/CopyMoveWidget.ui \
		  FileOperation/CopyMoveConflictDialog.ui \
		  FileOperation/RenameDialog.ui \
		  FileOperation/PermissionDialog.ui \
		  FileOperation/QueueWidget.ui \
		  FileOperation/DeleteDialog.ui \
		  FileOperation/DeleteWidget.ui \
		  FileOperation/RetryDialog.ui
HEADERS = MainWindow.h \
		    OptionsDialog.h \
		    Options/NewColumnSetDialog.h \
			 NewDirectoryDialog.h \
			 ListerMainWindow.h \
			 ListerMainWindow/FindDialog.h \
			 FindFilesDialog.h \
			 FindFilesDialog/DrivesDialog.h \
			 FindFilesDialog/SaveFindSettingsDialog.h \
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
			 FileOperation/CopyMoveConflictDialog.h \
			 FileOperation/Rename.h \
			 FileOperation/RenameDialog.h \
			 FileOperation/Permission.h \
			 FileOperation/PermissionDialog.h \
			 FileOperation/QueueWidget.h \
			 FileOperation/Delete.h \
			 FileOperation/DeleteDialog.h \
			 FileOperation/DeleteWidget.h \
			 FileOperation/RetryDialog.h \
			 FileOperation/Retry.h \
			 Settings.h \
			 Plugins.h \
			 Plugins/Content.h \
			 Plugins/ContentDelayed.h \
			 Plugins/ContPlug.h \
			 Plugins/Lister.h \
			 Plugins/TotalCommanderInterface.h \
			 Common/System.h
win32 {
	 HEADERS += Panel/Win32/ShellContextMenu.h
}
SOURCES = FileCommander.cpp \
			 MainWindow.cpp \
			 OptionsDialog.cpp \
			 Options/NewColumnSetDialog.cpp \
			 NewDirectoryDialog.cpp \
			 ListerMainWindow.cpp \
			 ListerMainWindow/FindDialog.cpp \
			 FindFilesDialog.cpp \
			 FindFilesDialog/DrivesDialog.cpp \
			 FindFilesDialog/SaveFindSettingsDialog.cpp \
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
			 FileOperation/CopyMoveConflictDialog.cpp \
			 FileOperation/Rename.cpp \
			 FileOperation/RenameDialog.cpp \
			 FileOperation/Permission.cpp \
			 FileOperation/PermissionDialog.cpp \
			 FileOperation/QueueWidget.cpp \
			 FileOperation/Delete.cpp \
			 FileOperation/DeleteDialog.cpp \
			 FileOperation/DeleteWidget.cpp \
			 FileOperation/RetryDialog.cpp \
			 FileOperation/Retry.cpp \
			 Settings.cpp \
			 Plugins.cpp \
			 Plugins/Content.cpp \
			 Plugins/ContentDelayed.cpp \
			 Plugins/Lister.cpp \
			 Common/System.cpp
win32 {
	 SOURCES += Panel/Win32/ShellContextMenu.cpp
}
win32 {
	 LIBS += User32.lib shell32.lib
}