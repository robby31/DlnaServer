#include "applicationworker.h"

ApplicationWorker::ApplicationWorker(QObject *parent):
    Worker(parent)
{
    connect(this, &ApplicationWorker::initializeSignal, this, &ApplicationWorker::initialisation);
}

void ApplicationWorker::scanFolder(const QString &path)
{
//    emit processStarted();

    #if !defined(QT_NO_DEBUG_OUTPUT)
    qDebug() << this << "scan folder" << path;
    #endif

    QSqlDatabase db = GET_DATABASE("MEDIA_DATABASE");
    if (db.isValid() && db.isOpen())
    {
        DlnaCachedRootFolder root;
        root.readDirectory(QDir(path));

//        emit processOver();
    }
    else
    {
        qCritical() << "database is not valid, unable to scan folder" << path;
//        emit errorDuringProcess(QString("database is not valid, unable to scan folder %1").arg(path.absolutePath()));
    }

    #if !defined(QT_NO_DEBUG_OUTPUT)
    qDebug() << "scan folder finished" << path;
    #endif
}

void ApplicationWorker::checkNetworkLink()
{
    emit processStarted();

    #if !defined(QT_NO_DEBUG_OUTPUT)
    qDebug() << "start check network links.";
    #endif

    // initialize database in current Thread
    QSqlDatabase database = GET_DATABASE("MEDIA_DATABASE");

    if (database.isValid() && database.isOpen())
    {
        emit progress(0);

        MediaLibrary library;

        int nb = 0;

        QSqlQuery query = library.getAllNetworkLinks();

        query.last();
        int total = query.at();

        #if !defined(QT_NO_DEBUG_OUTPUT)
        qDebug() << total << "links to check.";
        #endif

        query.exec();
        while (query.next())
        {
            emit progress(nb*100/total);
            if (processWasAborted())
                break;  // Abort process

            QString name = QString("%1 %2").arg(query.value("artist").toString(), query.value("title").toString());

            QString url(query.value("filename").toString());
            bool isReachable = query.value("is_reachable").toBool();

            DlnaNetworkVideo movie;
            movie.setAnalyzeStream(false);

            movie.setUrl(url);
            bool res = movie.waitUrl(30000);

            if (!(res && movie.isValid()))
            {
                if (isReachable)
                {
                    if (res && !movie.isValid())
                    {
                        emit addMessage(name, QString("put OFFLINE - %1").arg(movie.error()));

                        QHash<QString, QVariant> data;
                        data["is_reachable"] = QVariant(0);

                        library.updateFromFilename(url, data);
                    }
                }
                else
                {
                    emit addMessage(name, QString("link is still unreachable - %1").arg(movie.error()));
                }
            }
            else
            {
                if (!isReachable)
                {
                    emit addMessage(name, "put ONLINE");

                    QHash<QString, QVariant> data;
                    data["is_reachable"] = QVariant(1);

                    library.updateFromFilename(url, data);

                    // refresh data
                    emit refresh(QUrl(url).toString());
                }
            }

            ++nb;
        }

        if (!COMMIT_DATABASE("MEDIA_DATABASE"))
            qCritical() << "unable to COMMIT DATABASE";

        emit processOver(QString("%1 network links checked.").arg(total));
    }
    else
    {
        qCritical() << "database is not valid, unable to check network links.";
        emit errorDuringProcess( "database is not valid, unable to check network links.");
    }
}

void ApplicationWorker::scanVolumeInfo()
{
    emit processStarted();

    #if !defined(QT_NO_DEBUG_OUTPUT)
    qDebug() << "start volume information scan";
    #endif

    // initialize database in current Thread
    QSqlDatabase database = GET_DATABASE("MEDIA_DATABASE");

    if (database.isValid() && database.isOpen())
    {
        MediaLibrary library;

        if (library.isValid())
        {
            QSqlQuery query(database);
            query.prepare("SELECT media.id, media.filename, mime_type.name from media LEFT OUTER JOIN mime_type ON media.mime_type=mime_type.id WHERE media.is_reachable=1");

            if (query.exec())
            {
                while (query.next())
                {
                    if (processWasAborted())
                        break;  // Abort process

                    int idMedia = query.record().value("id").toInt();
                    QString filename = query.record().value("filename").toString();
                    QString mime_type = query.record().value("name").toString();
                    QHash<QString, double> volumeInfo = library.volumeInfo(idMedia);

                    if (volumeInfo.isEmpty())
                    {
                        if (mime_type.startsWith("audio/"))
                        {
                            #if !defined(QT_NO_DEBUG_OUTPUT)
                            qDebug() << "Analyze audio" << filename;
                            #endif

                            DlnaMusicTrackFile track(filename);
                            if (!library.setVolumeInfo(idMedia, track.volumeInfo()))
                                qCritical() << "Unable to set volume information for" << filename;
                        }
                        else if (mime_type.startsWith("video/")  && library.isLocalUrl(filename))
                        {
                            #if !defined(QT_NO_DEBUG_OUTPUT)
                            qDebug() << "Analyze local video" << filename;
                            #endif

                            DlnaVideoFile movie(filename);
                            if (!library.setVolumeInfo(idMedia, movie.volumeInfo(-1)))
                                qCritical() << "Unable to set volume information for" << filename;
                        }
                        else if (mime_type.startsWith("video/") && !library.isLocalUrl(filename))
                        {
                            #if !defined(QT_NO_DEBUG_OUTPUT)
                            qDebug() << "Analyze internet video" << filename;
                            #endif

                            DlnaNetworkVideo video;
                            video.setUrl(filename);
                            bool res = video.waitUrl(30000);

                            if (res && video.isValid())
                            {
                                if (!library.setVolumeInfo(idMedia, video.volumeInfo(-1)))
                                    qCritical() << "Unable to set volume information for" << filename;
                            }
                            else
                            {
                                qCritical() << "NO VOLUME INFO (TIMEOUT)" << filename;
                            }
                        }
                        else
                        {
                            qWarning() << "NO VOLUME INFO" << filename;
                        }
                    }
                }
            }
            else
            {
                qCritical() << "ERROR in request" << query.lastError().text();
            }
        }

        emit processOver();
    }
    else
    {
        qCritical() << "database is not valid, unable to scan volume.";
        emit errorDuringProcess( "database is not valid, unable to scan volume.");
    }
}

void ApplicationWorker::initialize()
{
    emit initializeSignal();
}

void ApplicationWorker::initialisation()
{
    MyNetwork::manager();
}

void ApplicationWorker::export_playlist(const QUrl &url)
{
    emit processStarted();
    emit progress(-1);

    #if !defined(QT_NO_DEBUG_OUTPUT)
    qDebug() << "EXPORT PLAYLIST" << url;
    #endif

    if (playlist)
    {
        qCritical() << "previous export not finished!";
        emit errorDuringProcess("export playlist already in progress");
        return;
    }

    playlist = new DlnaNetworkPlaylist(url, this);
    connect(playlist, &DlnaNetworkPlaylist::destroyed, this, &ApplicationWorker::playlistDestroyed);
    playlist->setProperty("index_media", 0);

    export_media_playlist();
}

void ApplicationWorker::export_media_playlist()
{
    QSettings settings("HOME", "QMS");
    QUrl exportFolder = settings.value("exportFolder").toUrl();

    if (playlist && playlist->property("index_media").isValid())
    {        
        int index_media = playlist->property("index_media").toInt();
        if (index_media < playlist->getChildrenSize())
        {
            const AbstractPlaylist::T_URL *info = playlist->getMediaInfo(index_media);

            auto media = qobject_cast<DlnaNetworkVideo*>(playlist->getChild(index_media));
            if (media && media->isValid())
            {
                connect(media, &DlnaNetworkVideo::destroyed, this, &ApplicationWorker::mediaDestroyed);

                #if !defined(QT_NO_DEBUG_OUTPUT)
                qDebug() << media->getDisplayName() << info->title << media->getSystemName() << media->mediaUrl() << media->size();
                #endif

                QUrl folder = QUrl(QString("%1/%2").arg(exportFolder.toString(), playlist->getName()));
                if (!QFileInfo::exists(folder.toLocalFile()))
                    QDir().mkdir(folder.toLocalFile());

                QString mediaName = media->getDisplayName();
                if (info->title.size() > mediaName.size())
                    mediaName = info->title;

                QString extension = "mp4";
                if (media->sourceAudioFormat() == "opus")
                    extension = "mkv";

                QString filename = QString("%1/%2.%3").arg(folder.toLocalFile(), mediaName, extension);
                if (QFileInfo::exists(filename))
                {
                    qWarning() << "file exists, export aborted for" << filename;
                    media->deleteLater();
                }
                else
                {
                    _exportMediaTo(media, filename);
                }
            }
            else
            {
                qCritical() << "invalid media in playlist" << index_media << info->url << info->title;

                if (media)
                {
                    media->deleteLater();
                }
                else
                {
                    playlist->setProperty("index_media", index_media+1);
                    export_media_playlist();
                }
            }
        }
        else
        {
            playlist->deleteLater();
        }
    }
    else
    {
        qCritical() << "invalid playlist initialisation.";
        if (playlist)
            playlist->deleteLater();
    }
}

void ApplicationWorker::export_media(const QUrl &url)
{
    emit processStarted();
    emit progress(-1);

    if (media)
    {
        qWarning() << "previous media export not finished.";
        emit errorDuringProcess("export media already in progress");
        return;
    }

    media = new DlnaNetworkVideo(this);
    connect(media, &DlnaNetworkVideo::destroyed, this, &ApplicationWorker::mediaDestroyed);

    if (!media->setUrl(url))
    {
        emit errorDuringProcess(QString("invalid url %1").arg(url.url()));
        media->deleteLater();
        return;
    }

    media->setMaxVideoHeight(720);

    if (!media->waitUrl(5000))
    {
        qCritical() << "network media not ready" << media;
        emit errorDuringProcess("network media not ready");
        media->deleteLater();
        return;
    }

    QSettings settings("HOME", "QMS");
    QUrl exportFolder = settings.value("exportFolder").toUrl();

    QString mediaName = media->getDisplayName();

    QString extension = "mp4";
    if (media->sourceAudioFormat() == "opus")
        extension = "mkv";

    QString filename = QString("%1/%2.%3").arg(exportFolder.toLocalFile(), mediaName, extension);
    if (QFileInfo::exists(filename))
    {
        qDebug() << "file exists, export aborted for" << filename;
        media->deleteLater();
        return;
    }

    _exportMediaTo(media, filename);
}

void ApplicationWorker::_exportMediaTo(DlnaNetworkVideo *media, const QString &filename)
{
    auto process = new FfmpegTranscoding(media);

    connect(process, &FfmpegTranscoding::readyToOpen, this, &ApplicationWorker::streamToOpen);
    connect(process, &FfmpegTranscoding::openedSignal, process, &FfmpegTranscoding::startRequestData);
    connect(process, &FfmpegTranscoding::endReached, media, &DlnaNetworkVideo::deleteLater);

    #if !defined(QT_NO_DEBUG_OUTPUT)
    connect(process, &FfmpegTranscoding::LogMessage, this, &ApplicationWorker::logMessage);
    #endif

    process->setOutput(filename);
    process->setFormat(COPY);
    process->setOriginalLengthInMSeconds(media->metaDataDuration());
    process->setBitrate(media->bitrate());
    process->setUrls(media->mediaUrl());

}

void ApplicationWorker::streamToOpen()
{
    auto stream = qobject_cast<FfmpegTranscoding*>(sender());

    #if !defined(QT_NO_DEBUG_OUTPUT)
    qDebug() << "open" << stream;
    #endif

    if (stream)
        stream->open();
}

void ApplicationWorker::playlistDestroyed(QObject *object)
{
    if (object == playlist)
    {
        playlist = Q_NULLPTR;
        emit processOver();
    }
}

void ApplicationWorker::mediaDestroyed(QObject *object)
{
    if (object == media)
    {
        media = Q_NULLPTR;
        emit processOver();
    }
    else if (playlist)
    {
        playlist->setProperty("index_media", playlist->property("index_media").toInt()+1);
        export_media_playlist();
    }
}

void ApplicationWorker::logMessage(const QString &message)
{
    Q_UNUSED(message)

    #if !defined(QT_NO_DEBUG_OUTPUT)
    qDebug() << message;
    #endif
}
