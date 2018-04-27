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
    $$PWD/cached/dlnacachedfolder.h \
    $$PWD/cached/dlnacachedvideo.h \
    $$PWD/dlnavideofile.h \
    $$PWD/dlnamusictrackfile.h \
    $$PWD/cached/dlnacachedmusictrack.h \
    $$PWD/cached/dlnacachedfoldermetadata.h \
    $$PWD/dlnastoragefolder.h \
    $$PWD/dlnayoutubevideo.h \
    $$PWD/cached/dlnacachednetworkvideo.h \
    $$PWD/cached/dlnacachedgroupedfoldermetadata.h

SOURCES += \
    $$PWD/dlnaresource.cpp \
    $$PWD/dlnarootfolder.cpp \
    $$PWD/dlnamusictrack.cpp \
    $$PWD/dlnafolder.cpp \
    $$PWD/dlnavideoitem.cpp \
    $$PWD/dlnaitem.cpp \
    $$PWD/cached/dlnacachedrootfolder.cpp \
    $$PWD/cached/dlnacachedfolder.cpp \
    $$PWD/cached/dlnacachedvideo.cpp \
    $$PWD/dlnavideofile.cpp \
    $$PWD/dlnamusictrackfile.cpp \
    $$PWD/cached/dlnacachedmusictrack.cpp \
    $$PWD/cached/dlnacachedfoldermetadata.cpp \
    $$PWD/dlnastoragefolder.cpp \
    $$PWD/dlnayoutubevideo.cpp \
    $$PWD/cached/dlnacachednetworkvideo.cpp \
    $$PWD/cached/dlnacachedgroupedfoldermetadata.cpp

include (../multimedia/multimedia.pri)
include (../youtube/youtube.pri)

