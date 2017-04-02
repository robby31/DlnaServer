INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/requestlistmodel.h \
    ../network/httprange.h

SOURCES += \
    $$PWD/requestlistmodel.cpp \
    ../network/httprange.cpp

include (../dlna/dlna.pri)
include (../../Upnp/UpnpLibrary/UpnpLibrary.prf)
