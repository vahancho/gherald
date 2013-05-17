message("You are running qmake on gherald.pro file.")

TEMPLATE = app
TARGET = gherald
QT += network xml

CONFIG(debug) {
    message(debug)
    DESTDIR = ./bin/debug
}
CONFIG(release) {
    message(release)
    DESTDIR = ./bin/release
}
CONFIG(debug, debug|release) {
    message("debug, debug|release")
    DESTDIR = ./bin/debug
}
CONFIG(release, debug|release) {
    message("release, debug|release")
    DESTDIR = ./bin/release
}

win32:CONFIG += embed_manifest_exe

# Define the application version and version string.
DEFINES += GHERALD_VERSION=0x0003020b
DEFINES += GHERALD_VERSION_STR=\\\"0.3.2b\\\"

INCLUDEPATH += ./GeneratedFiles \
               ./GeneratedFiles/release \
			   $(ProjectDir)/. \
               .

DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/release
OBJECTS_DIR += obj
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles

# Adds icon to the executable file on Windows.
win32:RC_FILE = application.rc

#Include file(s)
include(gherald.pri)
