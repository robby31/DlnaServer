INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/httpserver.h \
    $$PWD/request.h \
    $$PWD/requestlistmodel.h \
    ../network/httprange.h \
    ../network/elapsedtimer.h \
    ../network/reply.h \
    ../network/replydlnaitemcontent.h \
    $$PWD/httpclient.h

SOURCES += \
    $$PWD/httpserver.cpp \
    $$PWD/request.cpp \
    $$PWD/requestlistmodel.cpp \
    ../network/httprange.cpp \
    ../network/elapsedtimer.cpp \
    ../network/reply.cpp \
    ../network/replydlnaitemcontent.cpp \
    $$PWD/httpclient.cpp

include (../dlna/dlna.pri)
include (../../Upnp/UpnpLibrary/UpnpLibrary.prf)
