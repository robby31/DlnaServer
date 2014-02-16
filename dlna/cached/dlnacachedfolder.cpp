#include "dlnacachedfolder.h"

DlnaCachedFolder::DlnaCachedFolder(Logger* log, MediaLibrary* library, QString whereQuery, QString name, QString host, int port, QObject *parent):
    DlnaStorageFolder(log, host, port, parent),
    library(library),
    whereQuery(whereQuery),
    name(name)
{
}

bool DlnaCachedFolder::discoverChildren() {

    QSqlQuery query = library->getMedia(whereQuery);

    int fieldId = query.record().indexOf("id");
    int fieldFilename = query.record().indexOf("filename");
    int fieldType = query.record().indexOf("type_media");

    while (query.next()) {
        DlnaResource* child = 0;

        int id_media = query.value(fieldId).toInt();
        QString type_media = query.value(fieldType).toString();
        QString filename = query.value(fieldFilename).toString();

        if (type_media == "audio") {
            // TODO: set the parent of the QObject
            child = new DlnaCachedMusicTrack(log, filename, library, id_media, host, port);

        } else if (type_media == "video") {
            // TODO: set the parent of the QObject
            child = new DlnaCachedVideo(log, filename, library, id_media, host, port);

        } else {
            log->WARNING(QString("Unkwown format %1: %2").arg(type_media).arg(filename));
        }

        if (child != 0) {
            addChild(child);
        }
    }

    return true;
}

int DlnaCachedFolder::getChildrenSize() {
    if (!isDiscovered()) {
        return library->countMedia(whereQuery);
    } else {
        return getChildren().size();
    }
}
