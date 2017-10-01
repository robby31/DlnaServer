INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

QT += multimedia

INCLUDEPATH += $$(MYLIBRARY)/$$QT_VERSION/include/UpnpLibrary
LIBS += -L$$(MYLIBRARY)/$$QT_VERSION -l$$qtLibraryTarget(UpnpLibrary)

INCLUDEPATH += $$(MYLIBRARY)/$$QT_VERSION/include/multimedia
LIBS += -L$$(MYLIBRARY)/$$QT_VERSION -l$$qtLibraryTarget(mediadevice)

#include (../../chromaprint_v11/acoustid/acoustid.pri)

HEADERS += \
    ../multimedia/mediarenderer.h \
    ../multimedia/mediarenderermodel.h \
    ../multimedia/medialibrary.h

SOURCES += \
    ../multimedia/mediarenderer.cpp \
    ../multimedia/mediarenderermodel.cpp \
    ../multimedia/medialibrary.cpp



