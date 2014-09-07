INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/upnphelper.h \
    $$PWD/httpserver.h \
    $$PWD/request.h \
    $$PWD/requestlistmodel.h \
    ../network/httprange.h \
    ../network/elapsedtimer.h \
    ../network/reply.h

SOURCES += \
    $$PWD/upnphelper.cpp \
    $$PWD/httpserver.cpp \
    $$PWD/request.cpp \
    $$PWD/requestlistmodel.cpp \
    ../network/httprange.cpp \
    ../network/elapsedtimer.cpp \
    ../network/reply.cpp

include (../dlna/dlna.pri)
