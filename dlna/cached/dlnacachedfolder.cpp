#include "dlnacachedfolder.h"

DlnaCachedFolder::DlnaCachedFolder(Logger* log, MediaLibrary* library, QString whereQuery, QString name, QString host, int port, QObject *parent):
    DlnaStorageFolder(log, host, port, parent),
    library(library),
    whereQuery(whereQuery),
    name(name),
    query(),
    nbChildren(-1)
{
    if (library)
        query = library->getMedia(whereQuery);
}

DlnaResource *DlnaCachedFolder::getChild(int index, QObject *parent) {
    DlnaResource* child = 0;

    if (query.seek(index)) {

        int fieldId = query.record().indexOf("id");
        int fieldFilename = query.record().indexOf("filename");
        int fieldType = query.record().indexOf("type_media");

        int id_media = query.value(fieldId).toInt();
        QString type_media = query.value(fieldType).toString();
        QString filename = query.value(fieldFilename).toString();

        if (type_media == "audio") {
            child = new DlnaCachedMusicTrack(log, filename, library, id_media, host, port,
                                             parent != 0 ? parent : this);

        } else if (type_media == "video") {
            child = new DlnaCachedVideo(log, filename, library, id_media, host, port,
                                        parent != 0 ? parent : this);

        } else {
            log->Warning(QString("Unkwown format %1: %2").arg(type_media).arg(filename));
        }

    }

    if (child != 0) {
        child->setId(QString("%1").arg(index+1));
        child->setDlnaParent(this);
    }

    return child;
}

int DlnaCachedFolder::getChildrenSize() {
    if (nbChildren == -1 and library)
        nbChildren = library->countMedia(whereQuery);
    return nbChildren;
}
