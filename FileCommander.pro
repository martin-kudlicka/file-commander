CONFIG += qt warn_on
//RC_FILE = FileCommander.rc
FORMS = MainWindow.ui
HEADERS = MainWindow.h \
			 DirModel.h \
			 DirModelItem.h \
			 Plugins/Plugins.h \
			 Plugins/Content.h \
			 Plugins/ContPlug.h \
			 Plugins/TotalCommanderInterface.h \
			 Settings.h \
			 Common/System.h
SOURCES = FileCommander.cpp \
			 MainWindow.cpp \
			 DirModel.cpp \
			 DirModelItem.cpp \
			 Plugins/Plugins.cpp \
			 Plugins/Content.cpp \
			 Common/System.cpp