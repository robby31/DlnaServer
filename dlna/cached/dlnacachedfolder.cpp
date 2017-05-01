#include "dlnacachedfolder.h"

DlnaCachedFolder::DlnaCachedFolder(Logger* log, MediaLibrary* library, QSqlQuery query, QString name, QString host, int port, bool cacheEnabled, int maxSize, QObject *parent):
    DlnaStorageFolder(log, host, port, parent),
    library(library),
    name(name),
    query(query),
    nbChildren(-1),
    cacheEnabled(cacheEnabled),
    cache(),
    limitSizeMax(maxSize),
    m_nam(0)
{
    refreshContent();
}

void DlnaCachedFolder::refreshContent()
{
    cache.clear();

    if (query.isSelect() && query.isActive())
    {
        query = QSqlQuery(query.executedQuery(), GET_DATABASE("MEDIA_DATABASE"));

        if (cacheEnabled) {
            nbChildren = 0;
            while (query.next()) {
                ++nbChildren;
                cache.append(query.value("id").toInt());
                if (nbChildren==limitSizeMax)
                    break;
            }
        } else {
            if (query.last())
                nbChildren = query.at() + 1;
            else
                nbChildren = 0;
            if (limitSizeMax>0 && nbChildren>limitSizeMax)
                nbChildren = limitSizeMax;
        }
    }

    emit dlnaContentUpdated();
}

DlnaResource *DlnaCachedFolder::getChild(int index, QObject *parent) {
    DlnaResource* child = 0;
    int id_media = -1;
    QString type_media;
    QString filename;

    if (cacheEnabled)
    {
        if (index>=0 && index<cache.count())
        {
            id_media = cache.at(index);
            QSqlQuery newQuery = library->getMedia(QString("media.id=%1").arg(id_media));
            if (newQuery.isActive() && newQuery.next())
            {
                type_media = newQuery.value("type_media").toString();
                filename = newQuery.value("filename").toString();
            }
        }
    } else {
        if (query.seek(index))
        {
            id_media = query.value("id").toInt();
            type_media = query.value("type_media").toString();
            filename = query.value("filename").toString();
        }
    }

    if (type_media == "audio")
    {
        child = new DlnaCachedMusicTrack(log(), library, id_media, host, port,
                                         parent != 0 ? parent : this);

    } else if (type_media == "video")
    {
        if (filename.startsWith("http"))
            child = new DlnaCachedNetworkVideo(log(), m_nam, library, id_media, host, port,
                                               parent != 0 ? parent : this);
        else
            child = new DlnaCachedVideo(log(), library, id_media, host, port,
                                        parent != 0 ? parent : this);

    } else {
        logWarning(QString("Unkwown format %1: %2").arg(type_media).arg(filename));
    }


    if (child != 0)
    {
        child->setId(QString("%1").arg(index+1));
        child->setDlnaParent(this);
    }

    return child;
}

