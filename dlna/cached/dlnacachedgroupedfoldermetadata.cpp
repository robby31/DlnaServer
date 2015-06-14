#include "dlnacachedgroupedfoldermetadata.h"

DlnaCachedGroupedFolderMetaData::DlnaCachedGroupedFolderMetaData(Logger* log, MediaLibrary *library, QString host, int port, QString name, int idType, QString where, QObject *parent):
    DlnaStorageFolder(log, host, port, parent),
    library(library),
    typeMedia(idType),
    name(name),
    where(where),
    children()
{
}

void DlnaCachedGroupedFolderMetaData::addFolder(QString metaData, QString name, QString orderedParam, QString sortOption)
{
    DlnaCachedFolderMetaData* child;

    child = new DlnaCachedFolderMetaData(log(), library,
                                         typeMedia, metaData, name,
                                         host, port,
                                         orderedParam, sortOption,
                                         where, this);
    child->setId(QString("%1").arg(children.length()+1));
    children.append(child);
    child->setDlnaParent(this);
}

DlnaResource *DlnaCachedGroupedFolderMetaData::getChild(int index, QObject *parent)
{
    Q_UNUSED(parent)

    if (index>=0 && index<children.count())
        return children.at(index);
    else
        return 0;
}

void DlnaCachedGroupedFolderMetaData::setNetworkAccessManager(QNetworkAccessManager *manager)
{
    foreach (DlnaCachedFolderMetaData *child, children)
        child->setNetworkAccessManager(manager);
}
