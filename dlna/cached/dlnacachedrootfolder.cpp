#include "dlnacachedrootfolder.h"

DlnaCachedRootFolder::DlnaCachedRootFolder(Logger* log, QSqlDatabase *database, QString host, int port, QObject *parent):
    DlnaRootFolder(log, host, port, parent),
    library(log, database, this),
    mimeDb(),
    rootFolder(log, host, port, this)
{
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
                                                           typeMedia, host, port, this);
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
    QString mime_type = mimeDb.mimeTypeForFile(fileinfo).name();

    QSqlQuery query = library.getMedia(QString("filename=\"%1\"").arg(fileinfo.absoluteFilePath()));

    if (query.next()) {
        // media already stored in library
        int fieldDate = query.record().indexOf("last_modified");
        QDateTime lastModified = query.value(fieldDate).toDateTime();

        if (lastModified < fileinfo.lastModified()) {
            // update the media
            qWarning() << "update resource" << mime_type << fileinfo.absoluteFilePath() << lastModified << fileinfo.lastModified();

            // TODO: update database
        }

    } else {
        // new media

        QHash<QString, QVariant> data;

        data.insert("filename", fileinfo.absoluteFilePath());
        data.insert("type", mime_type.split("/").at(0));
        data.insert("mime_type", mime_type);
        data.insert("last_modified", fileinfo.lastModified());

        if (mime_type.startsWith("audio/")) {

            log->Debug("add resource " + mime_type + " " + fileinfo.absoluteFilePath());

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

            if (!library.insert(data)) {
                log->Error(QString("unable to add resource %1 (%2)").arg(fileinfo.absoluteFilePath()).arg(mime_type));
            }

        } else if (mime_type.startsWith("video/")) {

            log->Debug("add resource " + mime_type + " " + fileinfo.absoluteFilePath());

            DlnaVideoFile movie(log, fileinfo.absoluteFilePath(), host, port);

            data.insert("duration", movie.metaDataDuration());
            data.insert("resolution", movie.resolution());
            data.insert("samplerate", movie.samplerate());
            data.insert("channelcount", movie.channelCount());
            data.insert("audiolanguages", movie.audioLanguages());
            data.insert("subtitlelanguages", movie.subtitleLanguages());
            data.insert("framerate", movie.framerate());
            data.insert("bitrate", movie.bitrate());
            data.insert("format", movie.metaDataFormat());

            if (!library.insert(data)) {
                log->Error(QString("unable to add resource %1 (%2)").arg(fileinfo.absoluteFilePath()).arg(mime_type));
            }
        } else {
            log->Debug("resource not added to library: " + mime_type + ", " + fileinfo.absoluteFilePath());
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
}
