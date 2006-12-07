CONFIG += qt warn_on
//RC_FILE = FileCommander.rc
FORMS = MainWindow.ui
HEADERS = MainWindow.h \
			 Panel.h \
			 ShellMenu.h \
			 Process.h \
			 FileOperation.h \
			 Settings.h \
			 Plugins/Plugins.h \
			 Plugins/Content.h \
			 Plugins/ContPlug.h \
			 Plugins/TotalCommanderInterface.h \
			 Common/System.h
win32 {
	 HEADERS += Win32/ShellContextMenu.h
}
SOURCES = FileCommander.cpp \
			 MainWindow.cpp \
			 Panel.cpp \
			 ShellMenu.cpp \
			 Process.cpp \
			 FileOperation.cpp \
			 Settings.cpp \
			 Plugins/Plugins.cpp \
			 Plugins/Content.cpp \
			 Common/System.cpp
win32 {
	 SOURCES += Win32/ShellContextMenu.cpp
}
win32 {
	 LIBS += User32.lib shell32.lib
}