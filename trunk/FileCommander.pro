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
		  FileOperation/DeleteWidget.ui
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
			 FileOperation/Retry.h \
			 FileOperation/DiskSpace.h \
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
			 FileOperation/Retry.cpp \
			 FileOperation/DiskSpace.cpp \
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