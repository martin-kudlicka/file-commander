CONFIG += qt warn_on
//RC_FILE = FileCommander.rc
FORMS = MainWindow.ui \
		  FileOperation/FileOperationDialog.ui \
		  FileOperation/CopyMoveDialog.ui \
		  FileOperation/CopyMoveWidget.ui \
		  FileOperation/QueueWidget.ui
HEADERS = MainWindow.h \
			 Panel.h \
			 Panel/ShellMenu.h \
			 Panel/Process.h \
			 FileOperation.h \
			 FileOperation/FileOperationDialog.h \
			 FileOperation/FileRoutine.h \
			 FileOperation/CopyMove.h \
			 FileOperation/CopyMoveDialog.h \
			 FileOperation/CopyMoveWidget.h \
			 FileOperation/QueueWidget.h \
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
			 Panel.cpp \
			 Panel/ShellMenu.cpp \
			 Panel/Process.cpp \
			 FileOperation.cpp \
			 FileOperation/FileOperationDialog.cpp \
			 FileOperation/FileRoutine.cpp \
			 FileOperation/CopyMove.cpp \
			 FileOperation/CopyMoveDialog.cpp \
			 FileOperation/CopyMoveWidget.cpp \
			 FileOperation/QueueWidget.cpp \
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