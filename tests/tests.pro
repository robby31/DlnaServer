#-------------------------------------------------
#
# Project created by QtCreator 2013-11-23T16:03:00
#
#-------------------------------------------------

QT       += testlib network widgets qml quick xml

QT       -= gui

TARGET = tst_request
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += tst_request.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

include (../common/common.pri)
include (../network/network.pri)

HEADERS += \
    tst_request.h
