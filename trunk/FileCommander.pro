CONFIG += qt warn_on
//RC_FILE = FileCommander.rc
FORMS = MainWindow.ui \
		  OptionsDialog.ui \
		  Options/NewColumnSetDialog.ui \
		  NewDirectoryDialog.ui \
		  FileOperation/FileOperationDialog.ui \
		  FileOperation/CopyMoveDialog.ui \
		  FileOperation/CopyMoveWidget.ui \
		  FileOperation/QueueWidget.ui \
		  FileOperation/DeleteDialog.ui \
		  FileOperation/DeleteWidget.ui
HEADERS = MainWindow.h \
		    OptionsDialog.h \
		    Options/NewColumnSetDialog.h \
			 NewDirectoryDialog.h \
			 Panel.h \
			 Panel/TreeWidget.h \
			 Panel/ShellMenu.h \
			 Panel/Process.h \
			 FileOperation.h \
			 FileOperation/FileOperationDialog.h \
			 FileOperation/FileRoutine.h \
			 FileOperation/CopyMove.h \
			 FileOperation/CopyMoveDialog.h \
			 FileOperation/CopyMoveWidget.h \
			 FileOperation/QueueWidget.h \
			 FileOperation/Delete.h \
			 FileOperation/DeleteDialog.h \
			 FileOperation/DeleteWidget.h \
			 Settings.h \
			 Plugins.h \
			 Plugins/Content.h \
			 Plugins/ContPlug.h \
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
			 Panel.cpp \
			 Panel/TreeWidget.cpp \
			 Panel/ShellMenu.cpp \
			 Panel/Process.cpp \
			 FileOperation.cpp \
			 FileOperation/FileOperationDialog.cpp \
			 FileOperation/FileRoutine.cpp \
			 FileOperation/CopyMove.cpp \
			 FileOperation/CopyMoveDialog.cpp \
			 FileOperation/CopyMoveWidget.cpp \
			 FileOperation/QueueWidget.cpp \
			 FileOperation/Delete.cpp \
			 FileOperation/DeleteDialog.cpp \
			 FileOperation/DeleteWidget.cpp \
			 Settings.cpp \
			 Plugins.cpp \
			 Plugins/Content.cpp \
			 Common/System.cpp
win32 {
	 SOURCES += Panel/Win32/ShellContextMenu.cpp
}
win32 {
	 LIBS += User32.lib shell32.lib
}