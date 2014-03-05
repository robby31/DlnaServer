#include "dlnacachedfoldermetadata.h"

DlnaCachedFolderMetaData::DlnaCachedFolderMetaData(Logger* log, MediaLibrary *library, int typeMedia, QString metaData, QString name, QString host, int port, QObject *parent):
    DlnaStorageFolder(log, host, port, parent),
    library(library),
    metaData(metaData),
    name(name),
    typeMedia(typeMedia)
{
}

bool DlnaCachedFolderMetaData::discoverChildren() {

    // Add child when metaData is null
    DlnaResource* child = 0;
    child = new DlnaCachedFolder(log, library, QString("type=\"%2\" and %1 is null").arg(metaData).arg(typeMedia), QString("No %1").arg(metaData), host, port);
    addChild(child);

    // Add child for each distinct value taken by metaData
    QSqlQuery query = library->getDistinctMetaData(typeMedia, metaData);

    while (query.next()) {
        DlnaResource* child = 0;

        child = new DlnaCachedFolder(log, library, QString("type=\"%3\" and %1=\"%2\"").arg(metaData).arg(query.value(0).toString()).arg(typeMedia), query.value(1).toString(), host, port);

        addChild(child);
    }

    return true;
}

int DlnaCachedFolderMetaData::getChildrenSize() {
    if (!isDiscovered()) {
        return library->countDistinctMetaData(typeMedia, metaData)+1;
    } else {
        return getChildren().size();
    }
}
