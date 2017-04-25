INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/requestlistmodel.h

SOURCES += \
    $$PWD/requestlistmodel.cpp

include (../dlna/dlna.pri)
include (../../Upnp/UpnpLibrary/UpnpLibrary.prf)
