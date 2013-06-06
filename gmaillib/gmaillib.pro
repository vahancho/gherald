TEMPLATE = lib
DESTDIR = ../lib
QT += network

CONFIG(debug) {
    DLLDESTDIR = ../bin/debug
}
CONFIG(release) {
    DLLDESTDIR = ../bin/release
}
CONFIG(debug, debug|release) {
    DLLDESTDIR = ../bin/debug
}
CONFIG(release, debug|release) {
    DLLDESTDIR = ../bin/release
}

DEFINES += GMAIL_LIB
INCLUDEPATH += .

DEPENDPATH += .
OBJECTS_DIR += obj

HEADERS = export.h gmail.h
SOURCES = gmail.cpp
