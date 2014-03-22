#include "medialibrary.h"

MediaLibrary::MediaLibrary(Logger* log, QSqlDatabase *database, QObject *parent) :
    QObject(parent),
    log(log),
    db(database)
{
    if (!db->open()) {
        log->Error("unable to open database " + database->databaseName());
    } else {
        QSqlQuery query;

        if (!query.exec("pragma foreign_keys = on;")) {
            log->Error("unable to set FOREIGN KEYS in MediaLibrary " + query.lastError().text());
        }

        if (!query.exec("create table if not exists media ("
                   "id integer primary key, "
                   "filename varchar unique, "
                   "title varchar, album integer, artist integer, genre integer, trackposition varchar, "
                   "duration integer, samplerate integer, channelcount integer, bitrate integer, resolution varchar, framerate varchar, "
                   "picture integer, "
                   "audiolanguages varchar, subtitlelanguages varchar, "
                   "format varchar, "
                   "type integer, "
                   "mime_type integer, "
                   "last_modified DATETIME, "
                   "last_played DATETIME, "
                   "FOREIGN KEY(type) REFERENCES type(id), "
                   "FOREIGN KEY(mime_type) REFERENCES mime_type(id), "
                   "FOREIGN KEY(artist) REFERENCES artist(id), "
                   "FOREIGN KEY(album) REFERENCES album(id), "
                   "FOREIGN KEY(picture) REFERENCES picture(id), "
                   "FOREIGN KEY(genre) REFERENCES genre(id)"
                   ")")) {
            log->Error("unable to create table media in MediaLibrary " + query.lastError().text());
        }

        if (!query.exec("create table if not exists type (id integer primary key, "
                                                         "name varchar unique)")) {
            log->Error("unable to create table type in MediaLibrary " + query.lastError().text());
        }

        if (!query.exec("create table if not exists mime_type (id integer primary key, "
                                                              "name varchar unique)")) {
            log->Error("unable to create table mime_type in MediaLibrary " + query.lastError().text());
        }

        if (!query.exec("create table if not exists artist (id integer primary key, "
                                                           "name varchar unique)")) {
            log->Error("unable to create table artist in MediaLibrary " + query.lastError().text());
        }

        if (!query.exec("create table if not exists album (id integer primary key, "
                                                          "name varchar unique)")) {
            log->Error("unable to create table album in MediaLibrary " + query.lastError().text());
        }

        if (!query.exec("create table if not exists genre (id integer primary key, "
                                                          "name varchar unique)")) {
            log->Error("unable to create table genre in MediaLibrary " + query.lastError().text());
        }

        if (!query.exec("create table if not exists picture (id integer primary key, "
                                                            "name varchar unique)")) {
            log->Error("unable to create table picture in MediaLibrary " + query.lastError().text());
        }

        if (!query.exec("CREATE INDEX IF NOT EXISTS idx_idmedia ON media(id)")) {
            log->Error("unable to create index in MediaLibrary " + query.lastError().text());
        }

        if (!query.exec("CREATE INDEX IF NOT EXISTS idx_artistmedia ON media(artist)")) {
            log->Error("unable to create index in MediaLibrary " + query.lastError().text());
        }

        if (!query.exec("CREATE INDEX IF NOT EXISTS idx_albummedia ON media(album)")) {
            log->Error("unable to create index in MediaLibrary " + query.lastError().text());
        }

        if (!query.exec("CREATE INDEX IF NOT EXISTS idx_genremedia ON media(genre)")) {
            log->Error("unable to create index in MediaLibrary " + query.lastError().text());
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
    }
}

MediaLibrary::~MediaLibrary() {

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
        query.exec(QString("SELECT DISTINCT %2.id, %2.name FROM media LEFT OUTER JOIN %2 ON media.%1=%2.%3 WHERE media.type=%4 ORDER BY %2.name").arg(tagName).arg(foreignTable).arg(foreignTo).arg(typeMedia));
    } else {
        query.exec(QString("SELECT DISTINCT %1 FROM media WHERE type=%2 ORDER by %1").arg(tagName).arg(typeMedia));
    }

    return query;
}

int MediaLibrary::countDistinctMetaData(int typeMedia, QString tagName) {
    QSqlQuery query;
    if (foreignKeys["media"].contains(tagName)) {
        QString foreignTable = foreignKeys["media"][tagName]["table"];
        QString foreignTo = foreignKeys["media"][tagName]["to"];
        query.exec(QString("SELECT count(DISTINCT %2.name) FROM media LEFT OUTER JOIN %2 ON media.%1=%2.%3 WHERE media.type=%4").arg(tagName).arg(foreignTable).arg(foreignTo).arg(typeMedia));
    } else {
        query.exec(QString("SELECT count(DISTINCT %1) FROM media WHERE type=%2").arg(tagName).arg(typeMedia));
    }

    if (query.next()) {
        return query.value(0).toInt();
    } else {
        return 0;
    }
}

int MediaLibrary::countMedia(QString where) {
    QSqlQuery query(QString("SELECT count(id) FROM media WHERE %1").arg(where));
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
        if (!data[elt].isNull()) {
            l_parameters << elt;
            l_values << QString(":%1").arg(elt);
        }
    }
    QString parameters = l_parameters.join(",");
    QString values = l_values.join(",");

    query.prepare(QString("INSERT INTO media (%1) VALUES (%2)").arg(parameters).arg(values));
    foreach(QString elt, data.keys()) {
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
