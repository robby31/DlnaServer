#include "dlnacachedrootfolder.h"

DlnaCachedRootFolder::DlnaCachedRootFolder(QString host, int port, QObject *parent):
    DlnaRootFolder(host, port, parent),
    library(this),
    mimeDb(),
    rootFolder(host, port, this),
    recentlyPlayedChild(0),
    resumeChild(0),
    favoritesChild(0),
    lastAddedChild(0),
    youtube(0),
    m_nam(0)
{
    recentlyPlayedChild = new DlnaCachedFolder(&library,
                                               library.getMedia("last_played is not null", "last_played", "DESC"),
                                               "Recently Played", host, port, true, 200, this);
    addChild(recentlyPlayedChild);

    resumeChild = new DlnaCachedFolder(&library,
                                       library.getMedia("progress_played>0", "last_played", "DESC"),
                                       "Resume", host, port, true, 200, this);
    addChild(resumeChild);

    lastAddedChild = new DlnaCachedFolder(&library,
                                          library.getMedia("addedDate is not null", "addedDate", "DESC"),
                                          "Last Added", host, port, true, 200, this);
    addChild(lastAddedChild);

    favoritesChild = new DlnaCachedFolder(&library,
                                          library.getMedia("counter_played>0", "counter_played", "DESC"),
                                          "Favorites", host, port, true, 200, this);
    addChild(favoritesChild);

    QSqlQuery query = library.getMediaType();
    while (query.next()) {
        int id_type = query.value(0).toInt();
        QString typeMedia = query.value(1).toString();

        if (typeMedia == "video")
        {
            QString where = QString("media.type=%1 and is_reachable=1 and filename like '%youtube%'").arg(id_type);
            youtube = new DlnaCachedGroupedFolderMetaData(&library, host, port,
                                                          "YOUTUBE", this);
            youtube->addFolder("SELECT DISTINCT artist.id, artist.name FROM media LEFT OUTER JOIN artist ON media.artist=artist.id WHERE " + where + " ORDER BY artist.name",
                               "SELECT media.id, media.filename, type.name AS type_media, media.last_modified, media.counter_played "
                               "from media "
                               "LEFT OUTER JOIN type ON media.type=type.id "
                               "WHERE " + where + " and artist%1 "
                               "ORDER BY title ASC",
                               "Artist");
            youtube->addFolder("SELECT DISTINCT album.id, album.name FROM media LEFT OUTER JOIN album ON media.album=album.id WHERE " + where + " ORDER BY album.name",
                               "SELECT media.id, media.filename, type.name AS type_media, media.last_modified, media.counter_played "
                               "from media "
                               "LEFT OUTER JOIN type ON media.type=type.id "
                               "WHERE " + where + " and album%1 "
                               "ORDER BY title ASC",
                               "Album");
            youtube->addFolder("SELECT DISTINCT genre.id, genre.name FROM media LEFT OUTER JOIN genre ON media.genre=genre.id WHERE " + where + "ORDER BY genre.name",
                               "SELECT media.id, media.filename, type.name AS type_media, media.last_modified, media.counter_played "
                               "from media "
                               "LEFT OUTER JOIN type ON media.type=type.id "
                               "WHERE " + where + " and genre%1 "
                               "ORDER BY title ASC",
                               "Genre");
            addChild(youtube);
        }

        if (typeMedia == "audio")
        {
            DlnaCachedGroupedFolderMetaData* child;
            child = new DlnaCachedGroupedFolderMetaData(&library, host, port,
                                                        "Music", this);
            QString where = QString("media.type=%1 and is_reachable=1").arg(id_type);

            child->addFolder("SELECT DISTINCT artist.id, artist.name FROM media LEFT OUTER JOIN artist ON media.artist=artist.id WHERE " + where + " ORDER BY artist.name",
                             "SELECT media.id, media.filename, type.name AS type_media, media.last_modified, media.counter_played "
                             "from media "
                             "LEFT OUTER JOIN type ON media.type=type.id "
                             "WHERE " + where + " and artist%1 "
                             "ORDER BY album, disc, trackposition ASC",
                             "Artist");

            child->addFolder("SELECT DISTINCT album.id, album.name FROM media LEFT OUTER JOIN album ON media.album=album.id WHERE " + where + " ORDER BY album.name",
                             "SELECT media.id, media.filename, type.name AS type_media, media.last_modified, media.counter_played "
                             "from media "
                             "LEFT OUTER JOIN type ON media.type=type.id "
                             "WHERE " + where + " and album%1 "
                             "ORDER BY disc, trackposition ASC",
                             "Album");

            child->addFolder("SELECT DISTINCT genre.id, genre.name FROM media LEFT OUTER JOIN genre ON media.genre=genre.id WHERE " + where + " ORDER BY genre.name",
                             "SELECT media.id, media.filename, type.name AS type_media, media.last_modified, media.counter_played "
                             "from media "
                             "LEFT OUTER JOIN type ON media.type=type.id "
                             "WHERE " + where + " and genre%1 "
                             "ORDER BY album, disc, trackposition ASC",
                             "Genre");

            child->addFolder("SELECT DISTINCT artist.id, artist.name FROM media LEFT OUTER JOIN album ON media.album=album.id LEFT OUTER JOIN artist ON album.artist=artist.id WHERE " + where + " ORDER BY artist.name",
                             "SELECT media.id, media.filename, type.name AS type_media, media.last_modified, media.counter_played "
                             "from media "
                             "LEFT OUTER JOIN type ON media.type=type.id "
                             "LEFT OUTER JOIN album ON media.album=album.id "
                             "LEFT OUTER JOIN artist ON album.artist=artist.id "
                             "WHERE " + where + " and album.artist%1 "
                             "ORDER BY album, disc, trackposition ASC",
                             "Album Artist");
            addChild(child);
        } else {
            DlnaCachedFolder* child = new DlnaCachedFolder(&library,
                                                           library.getMedia(QString("type='%1'").arg(id_type), "title", "ASC"),
                                                           typeMedia, host, port, false, -1, this);
            addChild(child);
        }
    }

    addChild(&rootFolder);
}

bool DlnaCachedRootFolder::addFolderSlot(QString path)
{
    if (QFileInfo(path).isDir())
    {
        // scan the folder in background
        CachedRootFolderReadDirectory *readDirectoryWorker = new CachedRootFolderReadDirectory(QDir(path));
        QThreadPool::globalInstance()->start(readDirectoryWorker);

        rootFolder.addFolder(path);

        emit folderAddedSignal(path);
        return true;
    }

    emit error_addFolder(path);
    return false;
}

void DlnaCachedRootFolder::addNetworkLink(const QString &url)
{
    addResource(QUrl(url));
}

void DlnaCachedRootFolder::addResource(QUrl url)
{
    DlnaYouTubeVideo *movie = new DlnaYouTubeVideo(host, port, this);
    connect(movie, SIGNAL(streamUrlDefined(QString)), this, SLOT(networkLinkAnalyzed(QString)));
    movie->setNetworkAccessManager(m_nam);
    movie->setUrl(url.toString());
}


void DlnaCachedRootFolder::networkLinkAnalyzed(const QString &streamingUrl)
{
    Q_UNUSED(streamingUrl)

    DlnaYouTubeVideo *movie = qobject_cast<DlnaYouTubeVideo*>(sender());
    if (movie)
    {
        QHash<QString, QVariant> data;
        QHash<QString, QVariant> data_album;
        QHash<QString, QVariant> data_artist;

        data.insert("filename", movie->url());
        data.insert("type", "video");

        data.insert("title", movie->metaDataTitle());
        data.insert("duration", movie->metaDataDuration());
        data.insert("resolution", movie->resolution());
        data.insert("samplerate", movie->samplerate());
        data.insert("channelcount", movie->channelCount());
        data.insert("audiolanguages", movie->audioLanguages().join(","));
        data.insert("subtitlelanguages", movie->subtitleLanguages().join(","));
        data.insert("framerate", movie->framerate());
        data.insert("bitrate", movie->bitrate());
        data.insert("format", movie->metaDataFormat());
        data.insert("mime_type", movie->mimeType());

        if (movie->metaDataTitle().isEmpty())
        {
            qCritical() << QString("unable to add resource %1, title is empty").arg(movie->url().toString());
            emit error_addNetworkLink(movie->url().toString());
        }
        else
        {
            if (movie->metaDataDuration()<=0)
                qWarning() << QString("invalid duration %3 for %1 (%2).").arg(movie->metaDataTitle()).arg(movie->url().toString()).arg(movie->metaDataDuration());
            if (movie->resolution().isEmpty())
                qWarning() << QString("invalid resolution %3 for %1 (%2).").arg(movie->metaDataTitle()).arg(movie->url().toString()).arg(movie->resolution());

            if (!data.isEmpty())
            {
                qDebug() << QString("Resource to add: %1").arg(movie->metaDataTitle());
                if (!library.add_media(data, data_album, data_artist))
                {
                    qCritical() << QString("unable to add or update resource %1 (%2)").arg(movie->url().toString()).arg("video");
                    emit error_addNetworkLink(movie->metaDataTitle());
                }
                else
                {
                    lastAddedChild->needRefresh();
                    emit linkAdded(movie->metaDataTitle());
                }
            }
        }

        movie->deleteLater();
    }
    else
    {
        qCritical() << "invalid sender" << sender();
    }
}


void DlnaCachedRootFolder::networkLinkError(const QString &message)
{
    qCritical() << "ERROR, link not added" << message;

    DlnaYouTubeVideo *movie = qobject_cast<DlnaYouTubeVideo*>(sender());
    if (movie)
    {
        emit error_addNetworkLink(movie->url().toString());
        movie->deleteLater();
    }
    else
    {
        qCritical() << "invalid sender" << sender();
    }
}

void DlnaCachedRootFolder::addResource(QFileInfo fileinfo) {
//    // check meta data
//    library.checkMetaData(fileinfo);

    if (!library.contains(fileinfo))
    {
        QString mime_type = mimeDb.mimeTypeForFile(fileinfo).name();

        QHash<QString, QVariant> data;
        QHash<QString, QVariant> data_album;
        QHash<QString, QVariant> data_artist;

        data.insert("filename", fileinfo.absoluteFilePath());
        data.insert("type", mime_type.split("/").at(0));
        data.insert("mime_type", mime_type);
        data.insert("last_modified", fileinfo.lastModified());

        if (mime_type.startsWith("audio/"))
        {
            DlnaMusicTrackFile track(fileinfo.absoluteFilePath(), host, port);

            data.insert("title", track.metaDataTitle());
            data.insert("genre", track.metaDataGenre());
            data.insert("trackposition", track.metaDataTrackPosition());
            data.insert("disc", track.metaDataDisc());
            data.insert("duration", track.metaDataDuration());
            data.insert("samplerate", track.samplerate());
            data.insert("channelcount", track.channelCount());
            data.insert("picture", track.getByteAlbumArt());
            data.insert("format", track.metaDataFormat());
            data.insert("bitrate", track.bitrate());

            if (!track.metaDataAlbum().isEmpty())
            {
                data_album["name"] = track.metaDataAlbum();
                data_album["artist"] = track.metaDataAlbumArtist();
                int year = track.metaDataYear();
                if (year != -1)
                    data_album["year"] = year;
            }

            if (!track.metaDataPerformer().isEmpty())
            {
                data_artist["name"] = track.metaDataPerformer();
                if (!track.metaDataPerformerSort().isEmpty())
                    data_artist["sortname"] = track.metaDataPerformerSort();
            }
        }
        else if (mime_type.startsWith("video/"))
        {
            DlnaVideoFile movie(fileinfo.absoluteFilePath(), host, port);

            data.insert("duration", movie.metaDataDuration());
            data.insert("resolution", movie.resolution());
            data.insert("samplerate", movie.samplerate());
            data.insert("channelcount", movie.channelCount());
            data.insert("audiolanguages", movie.audioLanguages().join(","));
            data.insert("subtitlelanguages", movie.subtitleLanguages().join(","));
            data.insert("framerate", movie.framerate());
            data.insert("bitrate", movie.bitrate());
            data.insert("format", movie.metaDataFormat());

            if (!movie.metaDataAlbum().isEmpty())
            {
                data_album["name"] = movie.metaDataAlbum();
                data_album["artist"] = movie.metaDataAlbumArtist();
                int year = movie.metaDataYear();
                if (year != -1)
                    data_album["year"] = year;
            }

            if (!movie.metaDataPerformer().isEmpty())
            {
                data_artist["name"] = movie.metaDataPerformer();
                if (!movie.metaDataPerformerSort().isEmpty())
                    data_artist["sortname"] = movie.metaDataPerformerSort();
            }
        }
        else
        {
            qDebug() << "resource not added to library: " << mime_type << ", " << fileinfo.absoluteFilePath();
            data.clear();
        }

        if (!data.isEmpty())
        {
            if (!library.add_media(data, data_album, data_artist))
                qCritical() << QString("unable to add or update resource %1 (%2)").arg(fileinfo.absoluteFilePath()).arg(mime_type);
        }
    }
}

void DlnaCachedRootFolder::readDirectory(QDir folder)
{
    QFileInfoList files = folder.entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries);

    foreach(const QFileInfo &fileinfo, files) {
        QString currentPath = fileinfo.absoluteFilePath();

        if(fileinfo.isDir()) {
            readDirectory(currentPath);
        } else if (fileinfo.isFile()) {
            addResource(fileinfo);
        }
    }

    lastAddedChild->needRefresh();
}

void DlnaCachedRootFolder::updateLibrary(const QString &filename, const QHash<QString, QVariant> &data)
{
    if (!library.updateFromFilename(filename, data))
    {
        qCritical() << "unable to update library" << filename << data;
    }
    recentlyPlayedChild->needRefresh();
    resumeChild->needRefresh();
    favoritesChild->needRefresh();
}

void DlnaCachedRootFolder::updateLibraryFromId(const int &id, const QHash<QString, QVariant> &data)
{
    if (!library.updateFromId(id, data))
        qCritical() << QString("Unable to update library: media id %1").arg(id);

    recentlyPlayedChild->needRefresh();
    resumeChild->needRefresh();
    favoritesChild->needRefresh();

    if (data.contains("filename"))
    {
        // refresh the media
        if (data["filename"].toString().startsWith("http"))
            addResource(QUrl(data["filename"].toString()));
        else
            addResource(QFileInfo(data["filename"].toString()));
    }
}

void DlnaCachedRootFolder::incrementCounterPlayed(const QString &filename)
{
    if (!library.incrementCounterPlayed(filename))
        qCritical() << QString("Unable to update counter played: %1").arg(filename);
    recentlyPlayedChild->needRefresh();
    resumeChild->needRefresh();
    favoritesChild->needRefresh();
}

void DlnaCachedRootFolder::setNetworkAccessManager(QNetworkAccessManager *nam)
{
    m_nam = nam;

    if (recentlyPlayedChild)
        recentlyPlayedChild->setNetworkAccessManager(nam);

    if (resumeChild)
        resumeChild->setNetworkAccessManager(nam);

    if (favoritesChild)
        favoritesChild->setNetworkAccessManager(nam);

    if (lastAddedChild)
        lastAddedChild->setNetworkAccessManager(nam);

    if (youtube)
        youtube->setNetworkAccessManager(nam);
    else
        qCritical() << "Unable to set NetWorkManager for Youtube.";
}

void DlnaCachedRootFolder::reloadLibrary(const QStringList &localFolder)
{
    // save network media
    QList<QString> networkMedia;
    QSqlQuery query(GET_DATABASE("MEDIA_DATABASE"));
    if (query.exec("SELECT filename from media WHERE filename like 'http%' and is_reachable=1")) {
        while (query.next())
            networkMedia.append(query.value("filename").toString());
    } else {
        qCritical() << QString("Unable to load network media: %1").arg(query.lastError().text());
    }

    QString newDatabaseName = QString("%1.new").arg(library.databaseName());
    qWarning() << "RELOAD" << newDatabaseName;

    if (library.resetLibrary(newDatabaseName))
    {
        bool res = true;

        // load local folder
        foreach (const QString &folder, localFolder)
            if (!addFolderSlot(folder))
                res = false;

        // load network media
//        foreach (const QString &url, networkMedia)
//            if (!addNetworkLink(url))
//                res = false;

        if (!res)
            qCritical() << "Library reloaded with errors.";
        else
            qInfo() << "Library reloaded.";
    }
    else
    {
        qCritical() << "Unable to reload library";
    }
}
