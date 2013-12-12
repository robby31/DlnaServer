INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

INCLUDEPATH += "/opt/local/include/taglib/"
LIBS += "/opt/local/lib/libtag.1.12.0.dylib"

HEADERS += \
    $$PWD/dlnaresource.h \
    $$PWD/dlnarootfolder.h \
    ../dlna/dlnamusictrack.h \
    ../dlna/dlnafolder.h

SOURCES += \
    $$PWD/dlnaresource.cpp \
    $$PWD/dlnarootfolder.cpp \
    ../dlna/dlnamusictrack.cpp \
    ../dlna/dlnafolder.cpp

