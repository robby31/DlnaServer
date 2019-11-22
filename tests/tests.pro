#-------------------------------------------------
#
# Project created by QtCreator 2013-11-23T16:03:00
#
#-------------------------------------------------

QT       += testlib network widgets qml quick xml sql

QT      -= gui

TARGET = tst_request
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += sdk_no_version_check

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

TEMPLATE = app

DEFINES += SRCDIR=\\\"$$PWD/\\\"
DEFINES += PROFILING

INCLUDEPATH += $$(MYLIBRARY)/include/analyzer
LIBS += -L$$(MYLIBRARY)/lib -l$$qtLibraryTarget(analyzer)

INCLUDEPATH += $$(MYLIBRARY)/include/QmlApplication
LIBS += -L$$(MYLIBRARY)/lib -l$$qtLibraryTarget(QmlApplication)

INCLUDEPATH += $$(MYLIBRARY)/include/Streaming
LIBS += -L$$(MYLIBRARY)/lib -l$$qtLibraryTarget(streaming)

INCLUDEPATH += $$(MYLIBRARY)/include/UpnpLibrary
LIBS += -L$$(MYLIBRARY)/lib -l$$qtLibraryTarget(UpnpLibrary)

INCLUDEPATH += $$(MYLIBRARY)/include/multimedia
LIBS += -L$$(MYLIBRARY)/lib -l$$qtLibraryTarget(mediadevice)

INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib -lavcodec -lavformat -lavutil -lswscale -lswresample

SOURCES += \
    main.cpp \
    tst_dlnarootfolder.cpp \
    tst_dlnafolder.cpp \
    tst_dlnamusictrack.cpp \
    tst_dlnavideoitem.cpp \
    tst_dlnacachedresources.cpp \
    tst_dlnanetworkvideo.cpp \
    dlna_check_functions.cpp

HEADERS += \
    tst_dlnarootfolder.h \
    tst_dlnafolder.h \
    tst_dlnamusictrack.h \
    tst_dlnavideoitem.h \
    tst_dlnacachedresources.h \
    tst_dlnanetworkvideo.h \
    dlna_check_functions.h

