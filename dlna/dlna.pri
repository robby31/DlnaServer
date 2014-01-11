INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/dlnaresource.h \
    $$PWD/dlnarootfolder.h \
    ../dlna/dlnamusictrack.h \
    ../dlna/dlnafolder.h \
    ../dlna/dlnavideoitem.h

SOURCES += \
    $$PWD/dlnaresource.cpp \
    $$PWD/dlnarootfolder.cpp \
    ../dlna/dlnamusictrack.cpp \
    ../dlna/dlnafolder.cpp \
    ../dlna/dlnavideoitem.cpp

include (../multimedia/multimedia.pri)

