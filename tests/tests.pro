#-------------------------------------------------
#
# Project created by QtCreator 2013-11-23T16:03:00
#
#-------------------------------------------------

QT       += testlib network widgets qml quick xml sql

QT       -= gui

TARGET = tst_request
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += tst_request.cpp \
    tst_httprange.cpp \
    main.cpp \
    tst_dlnarootfolder.cpp \
    tst_dlnafolder.cpp \
    tst_dlnamusictrack.cpp \
    tst_dlnavideoitem.cpp \
    tst_dlnacachedresources.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
DEFINES += PROFILING

include (../common/common.pri)
include (../network/network.pri)

HEADERS += \
    tst_request.h \
    tst_httprange.h \
    tst_dlnarootfolder.h \
    tst_dlnafolder.h \
    tst_dlnamusictrack.h \
    tst_dlnavideoitem.h \
    tst_dlnacachedresources.h
