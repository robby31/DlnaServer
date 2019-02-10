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

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

TEMPLATE = app

DEFINES += SRCDIR=\\\"$$PWD/\\\"
DEFINES += PROFILING

INCLUDEPATH += $$(MYLIBRARY)/$$QT_VERSION/include/analyzer
LIBS += -L$$(MYLIBRARY)/$$QT_VERSION -l$$qtLibraryTarget(analyzer)

INCLUDEPATH += $$(MYLIBRARY)/$$QT_VERSION/include/QmlApplication
LIBS += -L$$(MYLIBRARY)/$$QT_VERSION -l$$qtLibraryTarget(QmlApplication)

INCLUDEPATH += $$(MYLIBRARY)/$$QT_VERSION/include/Streaming
LIBS += -L$$(MYLIBRARY)/$$QT_VERSION -l$$qtLibraryTarget(streaming)

INCLUDEPATH += $$(MYLIBRARY)/$$QT_VERSION/include/UpnpLibrary
LIBS += -L$$(MYLIBRARY)/$$QT_VERSION -l$$qtLibraryTarget(UpnpLibrary)

INCLUDEPATH += $$(MYLIBRARY)/$$QT_VERSION/include/multimedia
LIBS += -L$$(MYLIBRARY)/$$QT_VERSION -l$$qtLibraryTarget(mediadevice)

INCLUDEPATH += /opt/local/include
LIBS += -L/opt/local/lib -lavcodec -lavformat -lavutil -lswscale -lswresample

SOURCES += \
    main.cpp \
    tst_dlnarootfolder.cpp \
    tst_dlnafolder.cpp \
    tst_dlnamusictrack.cpp \
    tst_dlnavideoitem.cpp \
    tst_dlnacachedresources.cpp \
    tst_dlnayoutubevideo.cpp \
    tst_dlnanetworkvideo.cpp

HEADERS += \
    tst_dlnarootfolder.h \
    tst_dlnafolder.h \
    tst_dlnamusictrack.h \
    tst_dlnavideoitem.h \
    tst_dlnacachedresources.h \
    tst_dlnayoutubevideo.h \
    tst_dlnanetworkvideo.h
