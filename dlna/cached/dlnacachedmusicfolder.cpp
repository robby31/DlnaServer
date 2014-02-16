#include "dlnacachedmusicfolder.h"

DlnaCachedMusicFolder::DlnaCachedMusicFolder(Logger* log, MediaLibrary *library, QString host, int port, QObject *parent):
    DlnaStorageFolder(log, host, port, parent),
    library(library)
{
}

bool DlnaCachedMusicFolder::discoverChildren() {

    DlnaResource* child = 0;

    // TODO: set the parent of the QObject
    child = new DlnaCachedFolderMetaData(log, library, "artist", "Artist", host, port);
    addChild(child);

    // TODO: set the parent of the QObject
    child = new DlnaCachedFolderMetaData(log, library, "album", "Album", host, port);
    addChild(child);

    // TODO: set the parent of the QObject
    child = new DlnaCachedFolderMetaData(log, library, "genre", "Genre", host, port);
    addChild(child);

    return true;
}
