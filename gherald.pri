#Header files
HEADERS += ./gui/dlgLogin.h \
           ./gui/dlgOptions.h \
           ./gui/notifier.h \
           ./gui/trayIcon.h \
           ./core/defaultManager.h \
           ./core/versionManager.h \
           ./core/login.h \
           ./parser/mailEntry.h \
           ./parser/parser.h \
           ./parser/atomParser.h \
           ./strings/strings.h \
           ./strings/guiStrings.h

#Source files
SOURCES += ./main.cpp \
           ./gui/dlgLogin.cpp \
           ./gui/dlgOptions.cpp \
           ./gui/notifier.cpp \
           ./gui/trayIcon.cpp \
           ./core/defaultManager.cpp \
           ./core/versionManager.cpp \
           ./core/login.cpp \
           ./parser/mailEntry.cpp \
           ./parser/parser.cpp \
           ./parser/atomParser.cpp \
           ./strings/strings.cpp \
           ./strings/guiStrings.cpp

#Resource file(s)
RESOURCES += ./resources/gherald.qrc
