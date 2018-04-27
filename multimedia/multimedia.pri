INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

QT += multimedia

INCLUDEPATH += $$(MYLIBRARY)/$$QT_VERSION/include/analyzer
LIBS += -L$$(MYLIBRARY)/$$QT_VERSION -l$$qtLibraryTarget(analyzer)

INCLUDEPATH += $$(MYLIBRARY)/$$QT_VERSION/include/UpnpLibrary
LIBS += -L$$(MYLIBRARY)/$$QT_VERSION -l$$qtLibraryTarget(UpnpLibrary)

INCLUDEPATH += $$(MYLIBRARY)/$$QT_VERSION/include/multimedia
LIBS += -L$$(MYLIBRARY)/$$QT_VERSION -l$$qtLibraryTarget(mediadevice)

INCLUDEPATH += /opt/local/include
LIBS += -L/opt/local/lib -lavcodec -lavformat -lavutil -lswscale -lswresample

DEFINES += USE_AVRESAMPLE
LIBS += -L/opt/local/lib -lavresample

DYLD_LIBRARY_PATH += /opt/local/lib

#include (../../chromaprint_v11/acoustid/acoustid.pri)

HEADERS += \
    $$PWD/mediarenderer.h \
    $$PWD/mediarenderermodel.h \
    $$PWD/medialibrary.h

SOURCES += \
    $$PWD/mediarenderer.cpp \
    $$PWD/mediarenderermodel.cpp \
    $$PWD/medialibrary.cpp



