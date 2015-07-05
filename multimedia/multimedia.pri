INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

include (../../qffmpeg/qffmpeg.pri)
#include (../../chromaprint_v11/acoustid/acoustid.pri)

HEADERS += \
    ../multimedia/transcodeprocess.h \
    ../multimedia/mencodertranscoding.h \
    ../multimedia/ffmpegtranscoding.h \
    ../multimedia/mediarenderer.h \
    ../multimedia/mediarenderermodel.h \
    ../multimedia/medialibrary.h \
    ../multimedia/streamingfile.h \
    $$PWD/device.h

SOURCES += \
    ../multimedia/transcodeprocess.cpp \
    ../multimedia/mencodertranscoding.cpp \
    ../multimedia/ffmpegtranscoding.cpp \
    ../multimedia/mediarenderer.cpp \
    ../multimedia/mediarenderermodel.cpp \
    ../multimedia/medialibrary.cpp \
    ../multimedia/streamingfile.cpp \
    $$PWD/device.cpp



