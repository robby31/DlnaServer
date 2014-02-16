INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

include (../../mediainfolib/mediainfolib.pri)

HEADERS += \
    ../multimedia/metadata.h \
    ../multimedia/transcodeprocess.h \
    ../multimedia/mencodertranscoding.h \
    ../multimedia/ffmpegtranscoding.h \
    ../multimedia/mediarenderer.h \
    ../multimedia/mediarenderermodel.h \
    ../multimedia/medialibrary.h

SOURCES += \
    ../multimedia/metadata.cpp \
    ../multimedia/transcodeprocess.cpp \
    ../multimedia/mencodertranscoding.cpp \
    ../multimedia/ffmpegtranscoding.cpp \
    ../multimedia/mediarenderer.cpp \
    ../multimedia/mediarenderermodel.cpp \
    ../multimedia/medialibrary.cpp



