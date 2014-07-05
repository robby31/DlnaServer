#include "dlnacachedrootfolder.h"

DlnaCachedRootFolder::DlnaCachedRootFolder(Logger* log, QSqlDatabase *database, QString host, int port, QObject *parent):
    DlnaRootFolder(log, host, port, parent),
    library(log, database, this),
    mimeDb(),
    rootFolder(log, host, port, this),
    recentlyPlayedChild(0),
    resumeChild(0),
    favoritesChild(0),
    lastAddedChild(0)
{
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

bool DlnaCachedRootFolder::addFolder(QString path) {
    if (QFileInfo(path).isDir()) {
        readDirectory(QDir(path));

        rootFolder.addFolder(path);

        clearChildren();

        QSqlQuery query = library.getMediaType();

        while (query.next()) {
            int id_type = query.value(0).toInt();
            QString typeMedia = query.value(1).toString();

            if (typeMedia == "audio") {
                DlnaCachedMusicFolder* child = new DlnaCachedMusicFolder(log, &library, host, port, id_type, this);
                addChild(child);
            } else {
                DlnaCachedFolder* child = new DlnaCachedFolder(log, &library,
                                                               QString("type='%1'").arg(id_type),
                                                               QString("title"),
                                                               QString("ASC"),
                                                               typeMedia, host, port, this);
                addChild(child);
            }
        }

        addChild(&rootFolder);

        return true;
    }

    return false;
}

void DlnaCachedRootFolder::addResource(QFileInfo fileinfo) {
//    // check meta data
//    library.checkMetaData(fileinfo);

    if (!library.contains(fileinfo)) {
        QString mime_type = mimeDb.mimeTypeForFile(fileinfo).name();

        QHash<QString, QVariant> data;

        data.insert("filename", fileinfo.absoluteFilePath());
        data.insert("type", mime_type.split("/").at(0));
        data.insert("mime_type", mime_type);
        data.insert("last_modified", fileinfo.lastModified());

        if (mime_type.startsWith("audio/")) {

            DlnaMusicTrackFile track(log, fileinfo.absoluteFilePath(), host, port);

            data.insert("title", track.metaDataTitle());
            data.insert("album", track.metaDataAlbum());
            data.insert("artist", track.metaDataPerformer());
            data.insert("genre", track.metaDataGenre());
            data.insert("trackposition", track.metaDataTrackPosition());
            data.insert("duration", track.metaDataDuration());
            data.insert("samplerate", track.samplerate());
            data.insert("channelcount", track.channelCount());
            data.insert("picture", track.getByteAlbumArt());
            data.insert("format", track.metaDataFormat());
            data.insert("bitrate", track.bitrate());

        } else if (mime_type.startsWith("video/")) {

            DlnaVideoFile movie(log, fileinfo.absoluteFilePath(), host, port);

            data.insert("duration", movie.metaDataDuration());
            data.insert("resolution", movie.resolution());
            data.insert("samplerate", movie.samplerate());
            data.insert("channelcount", movie.channelCount());
            data.insert("audiolanguages", movie.audioLanguages().join(","));
            data.insert("subtitlelanguages", movie.subtitleLanguages().join(","));
            data.insert("framerate", movie.framerate());
            data.insert("bitrate", movie.bitrate());
            data.insert("format", movie.metaDataFormat());

        } else {
            log->Debug("resource not added to library: " + mime_type + ", " + fileinfo.absoluteFilePath());
            data.clear();
        }

        if (!data.isEmpty()) {
            if (!library.add_media(data)) {
                log->Error(QString("unable to add or update resource %1 (%2)").arg(fileinfo.absoluteFilePath()).arg(mime_type));
            }
        }
    }
}

void DlnaCachedRootFolder::readDirectory(QDir folder) {

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

bool DlnaCachedRootFolder::updateLibrary(const QString &filename, const QHash<QString, QVariant> &data)
{
    bool ret = library.updateFromFilename(filename, data);
    recentlyPlayedChild->needRefresh();
    resumeChild->needRefresh();
    favoritesChild->needRefresh();
    return ret;
}

bool DlnaCachedRootFolder::incrementCounterPlayed(const QString &filename)
{
    bool ret = library.incrementCounterPlayed(filename);
    recentlyPlayedChild->needRefresh();
    resumeChild->needRefresh();
    favoritesChild->needRefresh();
    return ret;
}

bool DlnaCachedRootFolder::resetLibrary()
{
    QString newDatabaseName = QString("%1.new").arg(library.getDatabase()->databaseName());
    return library.resetLibrary(newDatabaseName);
}
