INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

QT += webkitwidgets

HEADERS += \
    $$PWD/dlnaresource.h \
    $$PWD/dlnarootfolder.h \
    $$PWD/dlnamusictrack.h \
    $$PWD/dlnafolder.h \
    $$PWD/dlnavideoitem.h \
    $$PWD/dlnaitem.h \
    $$PWD/cached/dlnacachedrootfolder.h \
    ../dlna/cached/dlnacachedfolder.h \
    ../dlna/cached/dlnacachedvideo.h \
    ../dlna/dlnavideofile.h \
    ../dlna/dlnamusictrackfile.h \
    ../dlna/cached/dlnacachedmusictrack.h \
    ../dlna/cached/dlnacachedmusicfolder.h \
    ../dlna/cached/dlnacachedfoldermetadata.h \
    ../dlna/dlnastoragefolder.h \
    ../dlna/cached/batchedrootfolder.h \
    ../dlna/dlnayoutubemusic.h \
    ../dlna/dlnayoutubevideo.h \
    ../dlna/cached/dlnacachednetworkvideo.h

SOURCES += \
    $$PWD/dlnaresource.cpp \
    $$PWD/dlnarootfolder.cpp \
    $$PWD/dlnamusictrack.cpp \
    $$PWD/dlnafolder.cpp \
    $$PWD/dlnavideoitem.cpp \
    $$PWD/dlnaitem.cpp \
    $$PWD/cached/dlnacachedrootfolder.cpp \
    ../dlna/cached/dlnacachedfolder.cpp \
    ../dlna/cached/dlnacachedvideo.cpp \
    ../dlna/dlnavideofile.cpp \
    ../dlna/dlnamusictrackfile.cpp \
    ../dlna/cached/dlnacachedmusictrack.cpp \
    ../dlna/cached/dlnacachedmusicfolder.cpp \
    ../dlna/cached/dlnacachedfoldermetadata.cpp \
    ../dlna/dlnastoragefolder.cpp \
    ../dlna/cached/batchedrootfolder.cpp \
    ../dlna/dlnayoutubemusic.cpp \
    ../dlna/dlnayoutubevideo.cpp \
    ../dlna/cached/dlnacachednetworkvideo.cpp

include (../multimedia/multimedia.pri)

