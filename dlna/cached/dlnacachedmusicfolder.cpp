#include "dlnacachedmusicfolder.h"

DlnaCachedMusicFolder::DlnaCachedMusicFolder(Logger* log, MediaLibrary *library, QString host, int port, int idType, QObject *parent):
    DlnaStorageFolder(log, host, port, parent),
    library(library),
    idType(idType)
{
}

bool DlnaCachedMusicFolder::discoverChildren() {

    DlnaResource* child = 0;

    // TODO: set the parent of the QObject
    child = new DlnaCachedFolderMetaData(log, library, idType, "artist", "Artist", host, port);
    addChild(child);

    // TODO: set the parent of the QObject
    child = new DlnaCachedFolderMetaData(log, library, idType, "album", "Album", host, port);
    addChild(child);

    // TODO: set the parent of the QObject
    child = new DlnaCachedFolderMetaData(log, library, idType, "genre", "Genre", host, port);
    addChild(child);

    return true;
}
