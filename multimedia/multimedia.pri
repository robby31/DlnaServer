INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

QT += multimedia

include (../../Upnp/UpnpLibrary/UpnpLibrary.prf)
include (../../multimedia/mediadevice/mediadevice.prf)
#include (../../chromaprint_v11/acoustid/acoustid.pri)

HEADERS += \
    ../multimedia/mediarenderer.h \
    ../multimedia/mediarenderermodel.h \
    ../multimedia/medialibrary.h

SOURCES += \
    ../multimedia/mediarenderer.cpp \
    ../multimedia/mediarenderermodel.cpp \
    ../multimedia/medialibrary.cpp



