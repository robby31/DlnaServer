#include "medialibrary.h"

MediaLibrary::MediaLibrary(Logger* log, QSqlDatabase *database, QObject *parent) :
    QObject(parent),
    log(log != 0 ? log : new Logger(this)),
    db(database),
    foreignKeys(),
    libraryState(0),
    m_acoustId(this)
{
    open();
}

bool MediaLibrary::open()
{
    log->Debug(QString("Open MediaLibrary %1").arg(db->databaseName()));

    if (!db->open()) {
        log->Error("unable to open database " + db->databaseName());
        return false;
    } else {
        QSqlQuery query;

        if (!query.exec("pragma foreign_keys = on;")) {
            log->Error("unable to set FOREIGN KEYS in MediaLibrary " + query.lastError().text());
            return false;
        }

        if (!query.exec("create table if not exists media ("
                        "id integer primary key, "
                        "filename varchar unique, "
                        "is_reachable integer DEFAULT 1, "
                        "title varchar, album integer, artist integer, genre integer, trackposition varchar, "
                        "duration integer, samplerate integer, channelcount integer, bitrate integer, resolution varchar, framerate varchar, "
                        "picture integer, "
                        "audiolanguages varchar, subtitlelanguages varchar, "
                        "format varchar, "
                        "type integer, "
                        "mime_type integer, "
                        "rating integer, "
                        "last_modified DATETIME, "
                        "addedDate DATETIME, "
                        "last_played DATETIME, "
                        "progress_played integer, "
                        "counter_played integer DEFAULT 0, "
                        "acoustid varchar, "
                        "mbid varchar, "
                        "FOREIGN KEY(type) REFERENCES type(id), "
                        "FOREIGN KEY(mime_type) REFERENCES mime_type(id), "
                        "FOREIGN KEY(artist) REFERENCES artist(id), "
                        "FOREIGN KEY(album) REFERENCES album(id), "
                        "FOREIGN KEY(picture) REFERENCES picture(id), "
                        "FOREIGN KEY(genre) REFERENCES genre(id)"
                        ")")) {
            log->Error("unable to create table media in MediaLibrary " + query.lastError().text());
            return false;
        }

        if (!query.exec("create table if not exists type (id integer primary key, "
                                                         "name varchar unique)")) {
            log->Error("unable to create table type in MediaLibrary " + query.lastError().text());
            return false;
        }

        if (!query.exec("create table if not exists mime_type (id integer primary key, "
                                                              "name varchar unique)")) {
            log->Error("unable to create table mime_type in MediaLibrary " + query.lastError().text());
            return false;
        }

        if (!query.exec("create table if not exists artist (id integer primary key, "
                                                           "name varchar unique)")) {
            log->Error("unable to create table artist in MediaLibrary " + query.lastError().text());
            return false;
        }

        if (!query.exec("create table if not exists album (id integer primary key, "
                                                          "name varchar unique)")) {
            log->Error("unable to create table album in MediaLibrary " + query.lastError().text());
            return false;
        }

        if (!query.exec("create table if not exists genre (id integer primary key, "
                                                          "name varchar unique)")) {
            log->Error("unable to create table genre in MediaLibrary " + query.lastError().text());
            return false;
        }

        if (!query.exec("create table if not exists picture (id integer primary key, "
                                                            "name varchar unique)")) {
            log->Error("unable to create table picture in MediaLibrary " + query.lastError().text());
            return false;
        }

        if (!query.exec("CREATE INDEX IF NOT EXISTS idx_idmedia ON media(id)")) {
            log->Error("unable to create index in MediaLibrary " + query.lastError().text());
            return false;
        }

        if (!query.exec("CREATE INDEX IF NOT EXISTS idx_artistmedia ON media(artist)")) {
            log->Error("unable to create index in MediaLibrary " + query.lastError().text());
            return false;
        }

        if (!query.exec("CREATE INDEX IF NOT EXISTS idx_albummedia ON media(album)")) {
            log->Error("unable to create index in MediaLibrary " + query.lastError().text());
            return false;
        }

        if (!query.exec("CREATE INDEX IF NOT EXISTS idx_genremedia ON media(genre)")) {
            log->Error("unable to create index in MediaLibrary " + query.lastError().text());
            return false;
        }

        // update foreign keys
        foreach(QString tableName, db->tables()) {
            query.exec(QString("pragma foreign_key_list(%1);").arg(tableName));
            while (query.next()) {
                QHash<QString, QString> tmp;
                tmp["table"] = query.value("table").toString();
                tmp["to"] = query.value("to").toString();
                foreignKeys[tableName][query.value("from").toString()] = tmp;
            }
        }

        // check if all media are reachable
        if (query.exec("SELECT id, filename from media")) {
            while (query.next()) {
                QFile fd(query.value("filename").toString());
                if (!fd.exists()) {
                    log->Info(QString("unable to reach media %1 id=%2").arg(fd.fileName()).arg(query.value("id").toString()));
                }
            }
        }

        // check if all artists are used by at least one media
        if (query.exec("SELECT artist.id, artist.name, count(media.id) from artist LEFT OUTER JOIN media ON media.artist=artist.id GROUP BY artist.id, artist.name")) {
            while (query.next())
                if (query.value(2).toInt()==0)
                    log->Warning(QString("Artist with no media: %1(id=%2)").arg(query.value(1).toString()).arg(query.value(0).toInt()));
        }
    }

    log->Debug(QString("MediaLibrary %1 opened.").arg(db->databaseName()));

    return true;
}

MediaLibrary::~MediaLibrary() {
    if (libraryState)
        delete libraryState;
}

QVariant MediaLibrary::getmetaData(QString tagName, int idMedia) {

    QSqlQuery query;
    if (foreignKeys["media"].contains(tagName)) {
        QString foreignTable = foreignKeys["media"][tagName]["table"];
        QString foreignTo = foreignKeys["media"][tagName]["to"];
        query.exec(QString("SELECT %3.name FROM media LEFT OUTER JOIN %3 ON media.%2=%3.%4 WHERE media.id=%1").arg(idMedia).arg(tagName).arg(foreignTable).arg(foreignTo));
    } else {
        query.exec(QString("SELECT %1 FROM media WHERE id=%2").arg(tagName).arg(idMedia));
    }

    if (query.next()) {
        QVariant res = query.value(0);
        if (query.next()) {
            // at least two results have been found
            return QVariant();
        } else {
            // returns the unique result found
            return res;
        }
    } else {
        return QVariant();
    }
}

QSqlQuery MediaLibrary::getDistinctMetaData(int typeMedia, QString tagName) {

    QSqlQuery query;
    if (foreignKeys["media"].contains(tagName)) {
        QString foreignTable = foreignKeys["media"][tagName]["table"];
        QString foreignTo = foreignKeys["media"][tagName]["to"];
        query.exec(QString("SELECT DISTINCT %2.id, %2.name FROM media LEFT OUTER JOIN %2 ON media.%1=%2.%3 WHERE media.type=%4 and is_reachable=1 ORDER BY %2.name").arg(tagName).arg(foreignTable).arg(foreignTo).arg(typeMedia));
    } else {
        query.exec(QString("SELECT DISTINCT %1 FROM media WHERE type=%2 and is_reachable=1 ORDER by %1").arg(tagName).arg(typeMedia));
    }

    return query;
}

int MediaLibrary::countDistinctMetaData(int typeMedia, QString tagName) {
    QSqlQuery query;
    if (foreignKeys["media"].contains(tagName)) {
        QString foreignTable = foreignKeys["media"][tagName]["table"];
        QString foreignTo = foreignKeys["media"][tagName]["to"];
        query.exec(QString("SELECT count(DISTINCT %2.name) FROM media LEFT OUTER JOIN %2 ON media.%1=%2.%3 WHERE media.type=%4 and is_reachable=1").arg(tagName).arg(foreignTable).arg(foreignTo).arg(typeMedia));
    } else {
        query.exec(QString("SELECT count(DISTINCT %1) FROM media WHERE type=%2 and is_reachable=1").arg(tagName).arg(typeMedia));
    }

    if (query.next()) {
        return query.value(0).toInt();
    } else {
        return 0;
    }
}

int MediaLibrary::countMedia(QString where) {
    QSqlQuery query(QString("SELECT count(id) FROM media WHERE %1 and is_reachable=1").arg(where));
    if (query.next()) {
        return query.value(0).toInt();
    } else {
        return 0;
    }
}

bool MediaLibrary::insert(QHash<QString, QVariant> data) {
    QSqlQuery query;

    QStringList l_parameters;
    QStringList l_values;
    foreach(QString elt, data.keys()) {
        if (!data[elt].isNull() and !data[elt].toString().isEmpty()) {
            l_parameters << elt;
            l_values << QString(":%1").arg(elt);
        }
    }
    QString parameters = l_parameters.join(",");
    QString values = l_values.join(",");

    query.prepare(QString("INSERT INTO media (%1) VALUES (%2)").arg(parameters).arg(values));
    foreach(QString elt, l_parameters) {
        if (foreignKeys["media"].contains(elt)) {
            QString foreignTable = foreignKeys["media"][elt]["table"];

            // replace the value of the foreign key by its id
            int index = insertForeignKey(foreignTable, "name", data[elt]);
            query.bindValue(QString(":%1").arg(elt), index);
            if (index == -1) {
                log->Error("unable to bind " + elt);
                return false;
            }
        } else {
            query.bindValue(QString(":%1").arg(elt), data[elt]);
        }
    }

    int res = query.exec();

    if (!res) {
        log->Error("unable to update MediaLibrary " + query.lastError().text());
    }

    return res;
}

int MediaLibrary::insertForeignKey(QString table, QString parameter, QVariant value) {

    QSqlField field(parameter, value.type());
    field.setValue(value);

    QSqlQuery query;

    int index = -1;
    query.exec(QString("SELECT id FROM %1 WHERE %2 = %3").arg(table).arg(parameter).arg(db->driver()->formatValue(field)));
    if (query.next()) {
        index = query.value(0).toInt();
    } else {
        if (!query.exec(QString("INSERT INTO %1 (%2) VALUES (%3)").arg(table).arg(parameter).arg(db->driver()->formatValue(field)))) {
            log->Error("unable to update MediaLibrary " + query.lastError().text());
            return -1;
        } else {
            index = query.lastInsertId().toInt();
        }
    }
    return index;
}

bool MediaLibrary::update(int id, QHash<QString, QVariant> data)
{
    QSqlRecord record;
    QSqlQuery query;
    if (query.exec(QString("SELECT * from media where id=%1").arg(id)))
        if (query.next())
            record = query.record();

    if (record.isEmpty())
        return false;

    foreach(QString elt, data.keys()) {
        if (!data[elt].isNull() and !data[elt].toString().isEmpty()) {

            if (foreignKeys["media"].contains(elt)) {
                QString foreignTable = foreignKeys["media"][elt]["table"];

                // replace the value of the foreign key by its id
                int index = insertForeignKey(foreignTable, "name", data[elt]);

                if (record.value(elt) != index) {
                    log->Debug(QString("update %1, %2, index %3 --> %4").arg(elt).arg(record.value(elt).toString()).arg(index).arg(data[elt].toString()));
                    QSqlQuery queryUpdate;
                    if (!queryUpdate.exec(QString("UPDATE media SET %1=%2 WHERE id=%3").arg(elt).arg(index).arg(id)))
                        return false;
                }

            } else {
                if (record.value(elt) != data[elt]) {
                    log->Debug(QString("update %1, %2 --> %3").arg(elt).arg(record.value(elt).toString()).arg(data[elt].toString()));
                    QSqlQuery queryUpdate;
                    QSqlField field = record.field(record.indexOf(elt));
                    field.setValue(data[elt]);
                    if (!queryUpdate.exec(QString("UPDATE media SET %1=%2 WHERE id=%3").arg(elt).arg(db->driver()->formatValue(field)).arg(id)))
                        return false;
                }
            }
        }
    }

    return true;
}

bool MediaLibrary::updateFromFilename(QString filename, QHash<QString, QVariant> data)
{
    QSqlQuery query = getMedia(QString("filename=\"%1\"").arg(filename));
    if (query.next()) {
        return update(query.value("id").toInt(), data);
    }

    return false;
}

bool MediaLibrary::incrementCounterPlayed(const QString &filename)
{
    QSqlQuery query = getMedia(QString("filename=\"%1\"").arg(filename));
    QHash<QString, QVariant> data;
    data["progress_played"] = 0;
    if (query.next()) {
        data["counter_played"] = query.value("counter_played").toInt()+1;
        return update(query.value("id").toInt(), data);
    }

    return false;
}

bool MediaLibrary::add_media(QHash<QString, QVariant> data)
{
    QSqlQuery query = getMedia(QString("filename=\"%1\"").arg(data["filename"].toString()));

    if (query.next()) {
        // media already stored in library
        QDateTime lastModified = query.value("last_modified").toDateTime();

        if (lastModified >= data["last_modified"].toDateTime()) {
            // no need to update the media
            return true;
        } else {
            // update the media
            qWarning() << "update resource" << data["mime_type"].toString() << data["filename"].toString() << lastModified << data["last_modified"].toDateTime();
            return update(query.value("id").toInt(), data);
        }

    } else {
        // new media

        // set the DateTime when the media has been added
        data["addedDate"] = QDateTime::currentDateTime();

        // update data with state of the library to import in new media
        if (libraryState && libraryState->contains(data["filename"].toString())) {
            foreach(const QString &param, libraryState->operator [](data["filename"].toString()).keys()) {
                data[param] = libraryState->operator [](data["filename"].toString())[param];
            }
        }

        log->Debug("add resource " + data["mime_type"].toString() + " " + data["filename"].toString());
        return insert(data);
    }
}

bool MediaLibrary::contains(QFileInfo fileinfo)
{
    QSqlQuery query = getMedia(QString("filename=\"%1\"").arg(fileinfo.absoluteFilePath()));
    if (query.next()) {
        // media already stored in library but maybe need to be updated
        // function returns false if the media shall be updated
        QDateTime lastModified = query.value("last_modified").toDateTime();
        return lastModified >= fileinfo.lastModified();
    } else {
        return false;
    }
}

void MediaLibrary::checkMetaData(QFileInfo fileinfo)
{
    QSqlQuery query = getMedia(QString("filename=\"%1\"").arg(fileinfo.absoluteFilePath()));
    if (query.next()) {
        if (query.value("type_media") == "audio" && fileinfo.absoluteFilePath().contains("Daft")) {
            qWarning() << "CHECK" << fileinfo.absoluteFilePath();
            AcoustIdAnswer *answer = m_acoustId.requestId(fileinfo);
            if (answer) {
                if (answer->title() != getmetaData("title", query.value("id").toInt()))
                    qWarning() << "    title differs:" << answer->title() << getmetaData("title", query.value("id").toInt()).toString();
            } else {
                qWarning() << "No acoustid found";
            }
        }
    }
}

MediaLibrary::StateType *MediaLibrary::exportMediaState()
{
    StateType *res = 0;

    QStringList attributesToExport;
    attributesToExport << "rating" << "last_played" << "progress_played" << "counter_played" << "acoustid" << "mbid";
    attributesToExport << "addedDate";

    QSqlQuery query;
    if (query.exec(QString("SELECT filename, %1 from media").arg(attributesToExport.join(",")))) {
        res = new StateType();
        while (query.next()) {
            QHash<QString, QVariant> d_values;

            foreach (const QString &param, attributesToExport)
                if (!query.value(param).isNull())
                    d_values[param] = query.value(param);

            if (!d_values.isEmpty())
                res->operator [](query.value("filename").toString()) = d_values;
        }
    } else {
        log->Error(QString("Unable to execute request: %1").arg(query.lastError().text()));
    }

    return res;
}

bool MediaLibrary::resetLibrary(QString pathname)
{
    if (libraryState)
        delete libraryState;

    // save current state of the library
    libraryState = exportMediaState();

    if (libraryState) {
        // close current database
        db->close();

        // open new database
        db->setDatabaseName(pathname);
        return open();
    } else {
        return false;
    }
}
