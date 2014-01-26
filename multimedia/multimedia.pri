INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

INCLUDEPATH += "../MediaInfoLib/Developpers/include"
LIBS += "/Users/doudou/workspace/DLNA_server/MediaInfoLib/libmediainfo.0.0.0.dylib"

HEADERS += \
    ../multimedia/metadata.h \
    ../multimedia/transcodeprocess.h \
    ../multimedia/mencodertranscoding.h \
    ../multimedia/ffmpegtranscoding.h \
    ../multimedia/mediarenderer.h \
    ../multimedia/mediarenderermodel.h

SOURCES += \
    ../multimedia/metadata.cpp \
    ../multimedia/transcodeprocess.cpp \
    ../multimedia/mencodertranscoding.cpp \
    ../multimedia/ffmpegtranscoding.cpp \
    ../multimedia/mediarenderer.cpp \
    ../multimedia/mediarenderermodel.cpp


