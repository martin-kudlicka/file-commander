CONFIG += qt warn_on
//RC_FILE = FileCommander.rc
FORMS = MainWindow.ui
HEADERS = MainWindow.h \
			 Panel.h \
			 Settings.h \
			 Plugins/Plugins.h \
			 Plugins/Content.h \
			 Plugins/ContPlug.h \
			 Plugins/TotalCommanderInterface.h \
			 Common/System.h
SOURCES = FileCommander.cpp \
			 MainWindow.cpp \
			 Panel.cpp \
			 Settings.cpp \
			 Plugins/Plugins.cpp \
			 Plugins/Content.cpp \
			 Common/System.cpp