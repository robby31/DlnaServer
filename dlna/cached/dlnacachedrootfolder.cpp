#include "dlnacachedrootfolder.h"

DlnaCachedRootFolder::DlnaCachedRootFolder(Logger* log, QSqlDatabase *database, QString host, int port, QObject *parent):
    DlnaRootFolder(log, host, port, parent),
    library(log, database, this),
    mimeDb(),
    rootFolder(log, host, port, this),
    recentlyPlayedChild(0),
    resumeChild(0),
    favoritesChild(0),
    lastAddedChild(0),
    youtube(0),
    m_nam(0)
{
    connect(this, SIGNAL(resetLibrarySignal()), this, SLOT(resetLibrarySlot()));

    recentlyPlayedChild = new DlnaCachedFolder(log, &library,
                                               QString("last_played is not null"),
                                               QString("last_played"),
                                               QString("DESC"),
                                               "Recently Played", host, port, true, this);
    addChild(recentlyPlayedChild);

    resumeChild = new DlnaCachedFolder(log, &library,
                                       QString("progress_played>0"),
                                       QString("last_played"),
                                       QString("DESC"),
                                       "Resume", host, port, true, this);
    addChild(resumeChild);

    lastAddedChild = new DlnaCachedFolder(log, &library,
                                          QString("addedDate is not null"),
                                          QString("addedDate"),
                                          QString("DESC"),
                                          "Last Added", host, port, true, this);
    lastAddedChild->setLimitSizeMax(200);
    addChild(lastAddedChild);

    favoritesChild = new DlnaCachedFolder(log, &library,
                                          QString("counter_played>0"),
                                          QString("counter_played"),
                                          QString("DESC"),
                                          "Favorites", host, port, true, this);
    addChild(favoritesChild);

    QSqlQuery query = library.getMediaType();
    while (query.next()) {
        int id_type = query.value(0).toInt();
        QString typeMedia = query.value(1).toString();

        if (typeMedia == "video") {
            youtube = new DlnaCachedGroupedFolderMetaData(log, &library, host, port,
                                                          "YOUTUBE", id_type, "filename like '%youtube%'", this);
            addChild(youtube);
        }

        if (typeMedia == "audio") {
            DlnaCachedMusicFolder* child = new DlnaCachedMusicFolder(log, &library, host, port, id_type, this);
            addChild(child);
        } else {
            DlnaCachedFolder* child = new DlnaCachedFolder(log, &library,
                                                           QString("type='%1'").arg(id_type),
                                                           QString("title"),
                                                           QString("ASC"),
                                                           typeMedia, host, port, false, this);
            addChild(child);
        }
    }

    addChild(&rootFolder);
}

bool DlnaCachedRootFolder::addFolderSlot(QString path)
{
    if (QFileInfo(path).isDir()) {
        readDirectory(QDir(path));

        rootFolder.addFolder(path);

        emit folderAddedSignal(path);
        return true;
    }

    emit error_addFolder(path);
    return false;
}

bool DlnaCachedRootFolder::addNetworkLink(const QString &url)
{
    if (addResource(QUrl(url))) {
        lastAddedChild->needRefresh();
        emit linkAdded(url);
        return true;
    }

    emit error_addNetworkLink(url);
    return false;
}

void DlnaCachedRootFolder::checkNetworkLink()
{
    int nb = 0;
    logInfo("CHECK NETWORK LINK started");

    QSqlQuery query = getAllNetworkLinks();
    while (query.next()) {
        ++nb;

        QString url(query.value("filename").toString());
        bool isReachable = query.value("is_reachable").toBool();

        DlnaYouTubeVideo movie(log(), host, port);
        if (m_nam)
        {
            movie.moveToThread(m_nam->thread());
            movie.setNetworkAccessManager(m_nam);
        }
        movie.setUrl(url);
        bool res = movie.waitUrl(30000);

        if (!(res && movie.isValid()))
        {
            if (!res)
                logWarning("TIMEOUT");

            if (isReachable)
            {
                logError(QString("link %1 is broken, title: %2").arg(query.value("filename").toString()).arg(query.value("title").toString()));

                if (res && !movie.unavailableMessage().isEmpty())
                {
                    logWarning(QString("PUT OFFLINE %1, %2, %3").arg(query.value("id").toString()).arg(query.value("title").toString()).arg(query.value("artist").toString()));

                    QHash<QString, QVariant> data;
                    data["is_reachable"] = QVariant(0);

                    library.updateFromFilename(url, data);
                }
            }
            else
            {
                logWarning(QString("link %1 is still unreachable, title: %2").arg(query.value("filename").toString()).arg(query.value("title").toString()));
            }
        }
        else
        {
            if (!isReachable)
            {
                logWarning(QString("PUT ONLINE %1, %2, %3").arg(query.value("id").toString()).arg(query.value("title").toString()).arg(query.value("artist").toString()));

                QHash<QString, QVariant> data;
                data["is_reachable"] = QVariant(1);

                library.updateFromFilename(url, data);
            }

            // refresh data
//            addResource(QUrl(url));
        }
    }

    logInfo(QString("%1 links checked.").arg(nb));
}

bool DlnaCachedRootFolder::addResource(QUrl url)
{
    QHash<QString, QVariant> data;

    data.insert("filename", url.toString());
    data.insert("type", "video");
//    data.insert("last_modified", fileinfo.lastModified());

    DlnaYouTubeVideo movie(log(), host, port);
    if (m_nam)
    {
        movie.moveToThread(m_nam->thread());
        movie.setNetworkAccessManager(m_nam);
    }
    movie.setUrl(url.toString());

    bool res = movie.waitUrl(30000);
    if (!(res && movie.isValid()))
    {
        logError(QString("unable to add resource %1, media is not valid").arg(url.toString()));
    }
    else
    {
        data.insert("title", movie.metaDataTitle());
        data.insert("duration", movie.metaDataDuration());
        data.insert("resolution", movie.resolution());
        data.insert("samplerate", movie.samplerate());
        data.insert("channelcount", movie.channelCount());
        data.insert("audiolanguages", movie.audioLanguages().join(","));
        data.insert("subtitlelanguages", movie.subtitleLanguages().join(","));
        data.insert("framerate", movie.framerate());
        data.insert("bitrate", movie.bitrate());
        data.insert("format", movie.metaDataFormat());
        data.insert("mime_type", movie.mimeType());

        if (movie.metaDataTitle().isEmpty()) {
            logError(QString("unable to add resource %1, title is empty").arg(url.toString()));
        } else {
            if (movie.metaDataDuration()<=0)
                logWarning(QString("invalid duration %3 for %1 (%2).").arg(movie.metaDataTitle()).arg(url.toString()).arg(movie.metaDataDuration()));
            if (movie.resolution().isEmpty())
                logWarning(QString("invalid resolution %3 for %1 (%2).").arg(movie.metaDataTitle()).arg(url.toString()).arg(movie.resolution()));

            if (!data.isEmpty()) {
                logDebug(QString("Resource to add: %1").arg(movie.metaDataTitle()));
                if (!library.add_media(data)) {
                    logError(QString("unable to add or update resource %1 (%2)").arg(url.toString().arg("video")));
                } else {
                    return true;
                }
            }
        }
    }

    return false;
}

void DlnaCachedRootFolder::addResource(QFileInfo fileinfo) {
//    // check meta data
//    library.checkMetaData(fileinfo);

    if (!library.contains(fileinfo))
    {
        QString mime_type = mimeDb.mimeTypeForFile(fileinfo).name();

        QHash<QString, QVariant> data;

        data.insert("filename", fileinfo.absoluteFilePath());
        data.insert("type", mime_type.split("/").at(0));
        data.insert("mime_type", mime_type);
        data.insert("last_modified", fileinfo.lastModified());

        if (mime_type.startsWith("audio/"))
        {
            DlnaMusicTrackFile track(log(), fileinfo.absoluteFilePath(), host, port);

            data.insert("title", track.metaDataTitle());
            data.insert("album", track.metaDataAlbum());
            data.insert("artist", track.metaDataPerformer());
            data.insert("genre", track.metaDataGenre());
            data.insert("trackposition", track.metaDataTrackPosition());
            data.insert("disc", track.metaDataDisc());
            data.insert("duration", track.metaDataDuration());
            data.insert("samplerate", track.samplerate());
            data.insert("channelcount", track.channelCount());
            data.insert("picture", track.getByteAlbumArt());
            data.insert("format", track.metaDataFormat());
            data.insert("bitrate", track.bitrate());
        }
        else if (mime_type.startsWith("video/"))
        {
            DlnaVideoFile movie(log(), fileinfo.absoluteFilePath(), host, port);

            data.insert("duration", movie.metaDataDuration());
            data.insert("resolution", movie.resolution());
            data.insert("samplerate", movie.samplerate());
            data.insert("channelcount", movie.channelCount());
            data.insert("audiolanguages", movie.audioLanguages().join(","));
            data.insert("subtitlelanguages", movie.subtitleLanguages().join(","));
            data.insert("framerate", movie.framerate());
            data.insert("bitrate", movie.bitrate());
            data.insert("format", movie.metaDataFormat());

        }
        else
        {
            logDebug("resource not added to library: " + mime_type + ", " + fileinfo.absoluteFilePath());
            data.clear();
        }

        if (!data.isEmpty())
        {
            if (!library.add_media(data))
                logError(QString("unable to add or update resource %1 (%2)").arg(fileinfo.absoluteFilePath()).arg(mime_type));
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
        logError(QString("Unable to update library: %1").arg(filename));
    recentlyPlayedChild->needRefresh();
    resumeChild->needRefresh();
    favoritesChild->needRefresh();
}

void DlnaCachedRootFolder::incrementCounterPlayed(const QString &filename)
{
    if (!library.incrementCounterPlayed(filename))
        logError(QString("Unable to update counter played: %1").arg(filename));
    recentlyPlayedChild->needRefresh();
    resumeChild->needRefresh();
    favoritesChild->needRefresh();
}

bool DlnaCachedRootFolder::resetLibrarySlot()
{
    QString newDatabaseName = QString("%1.new").arg(library.getDatabase()->databaseName());
    return library.resetLibrary(newDatabaseName);
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
        logError("Unable to set NetWorkManager for Youtube.");
}
