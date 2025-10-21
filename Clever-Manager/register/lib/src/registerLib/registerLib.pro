#-------------------------------------------------
#
# Project created by QtCreator 2016-10-08T14:55:23
#
#-------------------------------------------------

QT       += network
QT       += gui

TARGET = registerLib
TEMPLATE = lib

DEFINES += REGISTERLIB_LIBRARY
CONFIG += qaxcontainer

SOURCES += softwareregistration.cpp

HEADERS += softwareregistration.h\
        registerlib_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
