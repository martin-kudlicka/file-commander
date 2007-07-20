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

FORMS = MainWindow.ui

HEADERS = MainWindow.h \
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
			 Common/System.h
//win32 {
//	 HEADERS += Panel/Win32/ShellContextMenu.h
//}

SOURCES = FileCommander.cpp \
			 MainWindow.cpp \
			 Settings.cpp \
			 Plugins.cpp \
			 Plugins/ContentPlugin.cpp \
			 Plugins/ContentPluginDelayed.cpp \
			 Plugins/ListerPlugin.cpp \
			 Plugins/PackerPlugin.cpp \
			 Common/System.cpp
//win32 {
//	 SOURCES += Panel/Win32/ShellContextMenu.cpp
//}

exists (File Commander.vcproj) {
	win32 {
		LIBS += User32.lib shell32.lib
	}
}