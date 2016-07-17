INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

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
    ../dlna/cached/dlnacachedfoldermetadata.h \
    ../dlna/dlnastoragefolder.h \
    ../dlna/dlnayoutubevideo.h \
    ../dlna/cached/dlnacachednetworkvideo.h \
    ../dlna/cached/dlnacachedgroupedfoldermetadata.h \
    $$PWD/cached/cachedrootfolderreaddirectory.h

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
    ../dlna/cached/dlnacachedfoldermetadata.cpp \
    ../dlna/dlnastoragefolder.cpp \
    ../dlna/dlnayoutubevideo.cpp \
    ../dlna/cached/dlnacachednetworkvideo.cpp \
    ../dlna/cached/dlnacachedgroupedfoldermetadata.cpp \
    $$PWD/cached/cachedrootfolderreaddirectory.cpp

include (../multimedia/multimedia.pri)
include (../youtube/youtube.pri)

