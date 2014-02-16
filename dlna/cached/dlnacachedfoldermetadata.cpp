#include "dlnacachedfoldermetadata.h"

DlnaCachedFolderMetaData::DlnaCachedFolderMetaData(Logger* log, MediaLibrary *library, QString metaData, QString name, QString host, int port, QObject *parent):
    DlnaStorageFolder(log, host, port, parent),
    library(library),
    metaData(metaData),
    name(name)
{
}

bool DlnaCachedFolderMetaData::discoverChildren() {

    QSqlQuery query = library->getAllMetaData(metaData);

    while (query.next()) {
        DlnaResource* child = 0;

        child = new DlnaCachedFolder(log, library, QString("%1=\"%2\"").arg(metaData).arg(query.value(0).toString()), query.value(1).toString(), host, port);

        addChild(child);
    }

    return true;
}

int DlnaCachedFolderMetaData::getChildrenSize() {
    if (!isDiscovered()) {
        return library->countAllMetaData(metaData);
    } else {
        return getChildren().size();
    }
}
