TEMPLATE = app
TARGET = gherald
QT += network xml

CONFIG(debug) {
    DESTDIR = ../bin/debug
}
CONFIG(release) {
    DESTDIR = ../bin/release
}
CONFIG(debug, debug|release) {
    DESTDIR = ../bin/debug
}
CONFIG(release, debug|release) {
    DESTDIR = ../bin/release
}

win32:CONFIG += embed_manifest_exe
INCLUDEPATH = ../..
INCLUDEPATH += .

# Define the application version and version string.
DEFINES += GHERALD_VERSION=0x0003020b
DEFINES += GHERALD_VERSION_STR=\\\"0.3.2b\\\"

DEPENDPATH += .
OBJECTS_DIR += obj

# Adds icon to the executable file on Windows.
win32:RC_FILE = application.rc

LIBS += -L../lib -lgmaillib

#Include file(s)
include(gherald.pri)

