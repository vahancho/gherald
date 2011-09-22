message("You are running qmake on gherald.pro file.")

TEMPLATE = app
TARGET = gherald
DESTDIR = ./release
QT += network xml
CONFIG += release
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

#Include file(s)
include(gherald.pri)
