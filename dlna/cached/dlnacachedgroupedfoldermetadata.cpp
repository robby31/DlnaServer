#include "dlnacachedgroupedfoldermetadata.h"

DlnaCachedGroupedFolderMetaData::DlnaCachedGroupedFolderMetaData(Logger* log, MediaLibrary *library, QString host, int port, QString name, int idType, QString where, QObject *parent):
    DlnaStorageFolder(log, host, port, parent),
    name(name),
    where(where),
    children()
{
    DlnaCachedFolderMetaData* child = 0;

    child = new DlnaCachedFolderMetaData(log, library, idType, "artist", "Artist",
                                         host, port,
                                         QString("title"), QString("ASC"),
                                         where, this);
    child->setId(QString("%1").arg(children.length()+1));
    children.append(child);
    child->setDlnaParent(this);

    child = new DlnaCachedFolderMetaData(log, library, idType, "album", "Album",
                                         host, port,
                                         QString("title"), QString("ASC"),
                                         where, this);
    child->setId(QString("%1").arg(children.length()+1));
    children.append(child);
    child->setDlnaParent(this);

    child = new DlnaCachedFolderMetaData(log, library, idType, "genre", "Genre",
                                         host, port,
                                         QString("title"), QString("ASC"),
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
