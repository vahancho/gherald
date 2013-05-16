message("You are running qmake on gherald.pro file.")

TEMPLATE = app
TARGET = gherald
DESTDIR = ./release
QT += network xml

CONFIG += release
win32:CONFIG += embed_manifest_exe

# Define the application version and version string.
DEFINES += GHERALD_VERSION=0x0003020b
DEFINES += GHERALD_VERSION_STR=\\\"0.3.2b\\\"

INCLUDEPATH += ./GeneratedFiles \
               ./GeneratedFiles/release \
			   $(ProjectDir)/. \
               .
win32:LIBS += -lqtmain -lQtCore4 -lQtGui4 -lQtNetwork4
unix:LIBS += -lQtCore -lQtGui -lQtNetwork

DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/release
OBJECTS_DIR += release
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles

# Adds icon to the executable file on Windows.
win32:RC_FILE = application.rc

#Include file(s)
include(gherald.pri)
