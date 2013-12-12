INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/upnphelper.h \
    $$PWD/httpserver.h \
    $$PWD/request.h \
    $$PWD/requestlistmodel.h

SOURCES += \
    $$PWD/upnphelper.cpp \
    $$PWD/httpserver.cpp \
    $$PWD/request.cpp \
    $$PWD/requestlistmodel.cpp

include (../dlna/dlna.pri)
