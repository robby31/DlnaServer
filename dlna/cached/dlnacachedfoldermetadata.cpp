#include "dlnacachedfoldermetadata.h"

DlnaCachedFolderMetaData::DlnaCachedFolderMetaData(Logger* log, MediaLibrary *library, int typeMedia, QString metaData, QString name, QString host, int port, QString orderedParam, QString sortOption, QString where, QObject *parent):
    DlnaStorageFolder(log, host, port, parent),
    library(library),
    metaData(metaData),
    name(name),
    typeMedia(typeMedia),
    query(),
    m_orderedParam(orderedParam),
    m_sortOption(sortOption),
    where(where),
    nbChildren(-1),
    m_nam(0)
{
    if (library) {
        query = library->getDistinctMetaData(typeMedia, metaData, where);
        if (query.last())
            nbChildren = query.at() + 1;
        else
            nbChildren = 0;
    }
}

DlnaResource *DlnaCachedFolderMetaData::getChild(int index, QObject *parent)
{
    QString whereQuery;
    if (!where.isEmpty())
        whereQuery = "and " + where;

    DlnaCachedFolder *child = 0;

    if (query.seek(index)) {
        QString name = query.value(0).toString();
        if (query.record().count()==2)
            name = query.value(1).toString();

        // metaData is null ?
        if (query.value(0).isNull())
            child = new DlnaCachedFolder(log(), library,
                                         QString("type=\"%2\" and %1 is null %3").arg(metaData).arg(typeMedia).arg(whereQuery),
                                         m_orderedParam,
                                         m_sortOption,
                                         QString("No %1").arg(metaData),
                                         host, port,
                                         false,
                                         parent != 0 ? parent : this);
        else
            child = new DlnaCachedFolder(log(), library,
                                         QString("type=\"%3\" and %1=\"%2\" %4").arg(metaData).arg(query.value(0).toString()).arg(typeMedia).arg(whereQuery),
                                         m_orderedParam,
                                         m_sortOption,
                                         name,
                                         host, port,
                                         false,
                                         parent != 0 ? parent : this);
    }

    if (child != 0) {
        child->setId(QString("%1").arg(index+1));
        child->setNetworkAccessManager(m_nam);
        child->setDlnaParent(this);
    }

    return child;
}
