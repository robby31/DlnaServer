#include "dlnacachedfoldermetadata.h"

DlnaCachedFolderMetaData::DlnaCachedFolderMetaData(Logger* log, MediaLibrary *library, int typeMedia, QString metaData, QString name, QString host, int port, QObject *parent):
    DlnaStorageFolder(log, host, port, parent),
    library(library),
    metaData(metaData),
    name(name),
    typeMedia(typeMedia),
    query(),
    nbChildren(-1)
{
    if (library) {
        query = library->getDistinctMetaData(typeMedia, metaData);
        if (query.last())
            nbChildren = query.at() + 1;
        else
            nbChildren = 0;
    }
}

DlnaResource *DlnaCachedFolderMetaData::getChild(int index, QObject *parent) {

    DlnaResource *child = 0;

    if (query.seek(index)) {
        // metaData is null ?
        if (query.value(0).isNull())
            child = new DlnaCachedFolder(log, library,
                                         QString("type=\"%2\" and %1 is null").arg(metaData).arg(typeMedia),
                                         QString("album"),
                                         QString("ASC"),
                                         QString("No %1").arg(metaData),
                                         host, port,
                                         false,
                                         parent != 0 ? parent : this);
        else
            child = new DlnaCachedFolder(log, library,
                                         QString("type=\"%3\" and %1=\"%2\"").arg(metaData).arg(query.value(0).toString()).arg(typeMedia),
                                         QString("album"),
                                         QString("ASC"),
                                         query.value(1).toString(),
                                         host, port,
                                         false,
                                         parent != 0 ? parent : this);
    }

    if (child != 0) {
        child->setId(QString("%1").arg(index+1));
        child->setDlnaParent(this);
    }

    return child;
}
