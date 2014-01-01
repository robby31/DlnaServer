INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

INCLUDEPATH += "../MediaInfoLib/Developpers/include"
LIBS += "/Users/doudou/workspace/DLNA_server/MediaInfoLib/libmediainfo.0.0.0.dylib"

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

