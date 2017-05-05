#include "medialibrary.h"

MediaLibrary::MediaLibrary(QObject *parent) :
    QObject(parent),
    foreignKeys(),
    libraryState(0)
//    m_acoustId(this)
{
    initialize();
}

bool MediaLibrary::isValid()
{
    return GET_DATABASE("MEDIA_DATABASE").isValid();
}

bool MediaLibrary::initialize()
{
    qDebug() << QString("Initialize MediaLibrary %1").arg(databaseName());

    QSqlDatabase db = GET_DATABASE("MEDIA_DATABASE");

    if (db.isValid())
    {
        QSqlQuery query(db);

        if (!query.exec("pragma journal_mode = WAL;"))
        {
            qCritical() << "unable to set JOURNAL MODE = WAL in MediaLibrary " + query.lastError().text();
            return false;
        }

        if (!query.exec("pragma foreign_keys = on;"))
        {
            qCritical() << "unable to set FOREIGN KEYS in MediaLibrary " + query.lastError().text();
            return false;
        }

        if (!query.exec("create table if not exists media ("
                        "id INTEGER PRIMARY KEY, "
                        "filename VARCHAR UNIQUE NOT NULL, "
                        "is_reachable INTEGER DEFAULT 1, "
                        "title VARCHAR, album INTEGER, artist INTEGER, genre INTEGER, trackposition INTEGER, disc INTEGER, "
                        "duration INTEGER, samplerate INTEGER, channelcount INTEGER, bitrate INTEGER, resolution VARCHAR, framerate VARCHAR, "
                        "picture INTEGER, "
                        "audiolanguages VARCHAR, subtitlelanguages VARCHAR, "
                        "format VARCHAR, "
                        "type INTEGER, "
                        "mime_type INTEGER, "
                        "rating INTEGER, "
                        "last_modified DATETIME, "
                        "addedDate DATETIME, "
                        "last_played DATETIME, "
                        "progress_played INTEGER, "
                        "counter_played INTEGER DEFAULT 0, "
                        "acoustid VARCHAR, "
                        "mbid VARCHAR, "
                        "FOREIGN KEY(type) REFERENCES type(id), "
                        "FOREIGN KEY(mime_type) REFERENCES mime_type(id), "
                        "FOREIGN KEY(artist) REFERENCES artist(id), "
                        "FOREIGN KEY(album) REFERENCES album(id), "
                        "FOREIGN KEY(picture) REFERENCES picture(id), "
                        "FOREIGN KEY(genre) REFERENCES genre(id)"
                        ")")) {
            qCritical() << "unable to create table media in MediaLibrary " + query.lastError().text();
            return false;
        }

        if (!query.exec("create table if not exists type (id INTEGER PRIMARY KEY, "
                        "name VARCHAR UNIQUE NOT NULL)")) {
            qCritical() << "unable to create table type in MediaLibrary " + query.lastError().text();
            return false;
        }

        if (!query.exec("create table if not exists mime_type (id INTEGER PRIMARY KEY, "
                        "name VARCHAR UNIQUE NOT NULL)")) {
            qCritical() << "unable to create table mime_type in MediaLibrary " + query.lastError().text();
            return false;
        }

        if (!query.exec("create table if not exists artist (id INTEGER PRIMARY KEY, "
                        "name VARCHAR UNIQUE NOT NULL, "
                        "sortname VARCHAR UNIQUE "
                        ")")) {
            qCritical() << "unable to create table artist in MediaLibrary " + query.lastError().text();
            return false;
        }

        if (!query.exec("create table if not exists album (id INTEGER PRIMARY KEY, "
                        "name VARCHAR NOT NULL, "
                        "artist INTEGER, "
                        "year INTEGER, "
                        "FOREIGN KEY(artist) REFERENCES artist(id), "
                        "UNIQUE(name, artist)"
                        ")"))
        {
            qCritical() << "unable to create table album in MediaLibrary " + query.lastError().text();
            return false;
        }

        if (!query.exec("create table if not exists genre (id INTEGER PRIMARY KEY, "
                        "name VARCHAR UNIQUE NOT NULL)")) {
            qCritical() << "unable to create table genre in MediaLibrary " + query.lastError().text();
            return false;
        }

        if (!query.exec("create table if not exists picture (id INTEGER PRIMARY KEY, "
                        "name VARCHAR UNIQUE NOT NULL)")) {
            qCritical() << "unable to create table picture in MediaLibrary " + query.lastError().text();
            return false;
        }

        //        qWarning() << query.exec("DROP TABLE param_value");
        //        qWarning() << query.exec("DROP TABLE param_name");

        if (!query.exec("create table if not exists param_name (id INTEGER PRIMARY KEY, "
                        "name VARCHAR UNIQUE NOT NULL)"))
        {
            qCritical() << "unable to create table parameter names in MediaLibrary " + query.lastError().text();
            return false;
        }

        if (!query.exec("create table if not exists param_value (id INTEGER PRIMARY KEY, "
                        "name INTEGER NOT NULL, "
                        "media INTEGER NOT NULL, "
                        "value VARCHAR NOT NULL, "
                        "FOREIGN KEY(name) REFERENCES param_name(id), "
                        "FOREIGN KEY(media) REFERENCES media(id), "
                        "UNIQUE(name, media)"
                        ")"))
        {
            qCritical() << "unable to create table parameter values in MediaLibrary " + query.lastError().text();
            return false;
        }

        //        if (!query.exec("CREATE INDEX IF NOT EXISTS idx_idmedia ON media(id)")) {
        //            logError("unable to create index in MediaLibrary " + query.lastError().text());
        //            return false;
        //        }

        if (!query.exec("CREATE INDEX IF NOT EXISTS idx_artistmedia ON media(artist)")) {
            qCritical() << "unable to create index in MediaLibrary " + query.lastError().text();
            return false;
        }

        if (!query.exec("CREATE INDEX IF NOT EXISTS idx_albummedia ON media(album)")) {
            qCritical() << "unable to create index in MediaLibrary " + query.lastError().text();
            return false;
        }

        if (!query.exec("CREATE INDEX IF NOT EXISTS idx_genremedia ON media(genre)")) {
            qCritical() << "unable to create index in MediaLibrary " + query.lastError().text();
            return false;
        }


        // update foreign keys
        foreach(QString tableName, db.tables()) {
            query.exec(QString("pragma foreign_key_list(%1);").arg(tableName));
            while (query.next()) {
                QHash<QString, QString> tmp;
                tmp["table"] = query.value("table").toString();
                tmp["to"] = query.value("to").toString();
                foreignKeys[tableName][query.value("from").toString()] = tmp;
            }
        }

        // check if unreachable media are still unreachable
        if (query.exec("SELECT id, filename from media WHERE is_reachable=0"))
        {
            QHash<QString, QVariant> data;
            data["is_reachable"] = QVariant(1);
            while (query.next())
            {
                QString url(query.value("filename").toString());
                if (!url.startsWith("http"))
                {
                    QFile fd(url);
                    if (fd.exists())
                    {
                        qInfo() << QString("media %1 id=%2 is now reachable").arg(fd.fileName()).arg(query.value("id").toString());
                        update("media", query.value("id").toInt(), data);
                    }
                }
            }
        }

        // check if all media are reachable
        if (query.exec("SELECT id, filename from media WHERE is_reachable=1"))
        {
            QHash<QString, QVariant> data;
            data["is_reachable"] = QVariant(0);
            while (query.next()) {
                QString url(query.value("filename").toString());
                if (!url.startsWith("http")) {
                    QFile fd(url);
                    if (!fd.exists()) {
                        qInfo() << QString("unable to reach media %1 id=%2").arg(fd.fileName()).arg(query.value("id").toString());
                        update("media", query.value("id").toInt(), data);
                    }
                }
            }
        }

        // check if all artists are used by at least one media
        if (query.exec("SELECT artist_id, artist_name, count(id) FROM "
                       "(SELECT artist.id AS artist_id, artist.name AS artist_name, media.id AS id from artist "
                       "LEFT OUTER JOIN media ON media.artist=artist.id "
                       "UNION "
                       "SELECT artist.id AS artist_id, artist.name AS artist_name, album.id AS id from artist "
                       "LEFT OUTER JOIN album ON album.artist=artist.id)"
                       "GROUP BY artist_id, artist_name")) {
            while (query.next())
                if (query.value(2).toInt()==0)
                    qWarning() << QString("Artist with no media: %1(id=%2)").arg(query.value(1).toString()).arg(query.value(0).toInt());
        }
        else
        {
            qDebug() << "ERROR in check artists" << query.lastError().text();
        }

        // check if duration is valid
        if (query.exec("SELECT filename, title, duration from media WHERE duration<1000")) {
            while (query.next())
                qWarning() << QString("invalid duration: %1 (filename=%2, title=%3)").arg(query.value(2).toString()).arg(query.value(0).toString()).arg(query.value(1).toString());
        }

        // check sortname for artists
        query.exec("SELECT count(id) from artist");
        if (query.next())
        {
            int nbArtists = query.value(0).toInt();
            query.exec("SELECT count(id) from artist WHERE artist.sortname is null");
            if (query.next())
            {
                qWarning() << query.value(0).toInt() << "artists with no sortname, total number of artists is" << nbArtists;
            }
        }

        // check artist for albums
        query.exec("SELECT count(id) from album");
        if (query.next())
        {
            int nbAlbums = query.value(0).toInt();
            query.exec("SELECT count(id) from album WHERE album.artist is null");
            if (query.next())
            {
                qWarning() << query.value(0).toInt() << "album with no artist, total number of albums is" << nbAlbums;
            }
        }

        // correct duration
        //        if (!query.exec(QString("UPDATE media SET duration=339776 WHERE filename='http://www.youtube.com/watch?v=a5uQMwRMHcs'")))
        //            qWarning() << query.lastError();

        // update youtube link
        //        if (!query.exec(QString("UPDATE media SET filename='https://www.youtube.com/watch?v=THGgyPfiNHs' WHERE id=14960")))
        //            qWarning() << query.lastError();

        if (query.exec("SELECT count(id) from media") && query.next())
            qInfo() << QString("%1 medias in database.").arg(query.value(0).toInt());


        /////////////////////////////////////////////////////////
        // update youtube link broken

        //        if (query.exec("SELECT id, filename, title from media WHERE filename like '%youtube%' and title like '%prayer%'"))
        //            while (query.next())
        //                qWarning() << query.value("id") << query.value("filename") << query.value("title");


        //        if (!query.exec(QString("UPDATE media SET filename='https://www.youtube.com/watch?v=JrlfFTS9kGU' where id=60")))
        //            qWarning() << query.lastError();

        /////////////////////////////////////////////////////////


        //        if (query.exec("SELECT filename, counter_played from media ORDER BY counter_played"))
        //            while (query.next())
        //                qWarning() << query.value("counter_played") << query.value("filename");

        //        QString artist("adele");
        //        int idArtist = -1;
        //        if (query.exec("SELECT id, name from artist where name like '%"+artist+"%'"))
        //            while (query.next()) {
        //                qWarning() << "ARTIST found" << query.value("name");
        //                idArtist = query.value("id").toInt();
        //                break;
        //            }

        //        if (idArtist != -1) {
        //            qWarning() << "UPDATE" << artist << "=" << idArtist;

        ////            if (!query.exec(QString("UPDATE media SET artist=%1 where type=1 and artist is null and title like '%"+artist+"%'").arg(idArtist)))
        ////                qWarning() << query.lastError();

        //            if (query.exec("SELECT filename, title, artist from media where type=1 and title like '%"+artist+"%'"))
        //                while (query.next())
        //                    qWarning() << query.value("filename") << query.value("title") << query.value("artist");
        //        }


        qDebug() << QString("MediaLibrary %1 initialized.").arg(db.databaseName());

        return true;
    }
    else
    {
        qCritical() << "Unable to initialize Medialibrary (connection is not valid).";
        return false;
    }
}

MediaLibrary::~MediaLibrary() {
    if (libraryState)
        delete libraryState;
}

QSqlQuery MediaLibrary::getMedia(const QString &where, const QString &orderParam, const QString &sortOption) const
{
    return QSqlQuery(QString("SELECT media.id, media.filename, type.name AS type_media, media.last_modified, media.counter_played "
                             "from media "
                             "LEFT OUTER JOIN type ON media.type=type.id "
                             "WHERE %1 and is_reachable=1 "
                             "ORDER BY %2 %3").arg(where).arg(orderParam).arg(sortOption), GET_DATABASE("MEDIA_DATABASE"));

}

QVariant MediaLibrary::getmetaDataAlbum(const QString &tagName, const int &idMedia) const
{
    QSqlQuery query(GET_DATABASE("MEDIA_DATABASE"));
    query.prepare("SELECT album.name AS album_name, artist.name AS artist_name, album.year AS year FROM media LEFT OUTER JOIN album ON media.album=album.id LEFT OUTER JOIN artist ON album.artist=artist.id WHERE media.id=:idMedia");
    query.bindValue(":idMedia", idMedia);
    query.exec();

    if (query.next()) {
        QVariant res = query.value(tagName);
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

QVariant MediaLibrary::getmetaDataArtist(const QString &tagName, const int &idMedia) const
{
    QSqlQuery query(GET_DATABASE("MEDIA_DATABASE"));
    query.prepare("SELECT artist.name AS artist_name, artist.sortname AS artist_sort FROM media LEFT OUTER JOIN artist ON media.artist=artist.id WHERE media.id=:idMedia");
    query.bindValue(":idMedia", idMedia);
    query.exec();

    if (query.next()) {
        QVariant res = query.value(tagName);
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

QVariant MediaLibrary::getmetaData(const QString &tagName, const int &idMedia) const {
    QSqlQuery query(GET_DATABASE("MEDIA_DATABASE"));
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

QHash<QString, double> MediaLibrary::volumeInfo(const int &idMedia)
{
    QHash<QString, double> result;

    QSqlQuery query(GET_DATABASE("MEDIA_DATABASE"));

    query.prepare("SELECT param_name.name, param_value.value from param_value LEFT OUTER JOIN param_name ON param_value.name=param_name.id WHERE param_value.media=:idmedia and param_name.name LIKE '%_volume'");
    query.bindValue(":idmedia", idMedia);
    if (query.exec())
    {
        while (query.next())
        {
            result[query.record().value("name").toString()] = query.record().value("value").toDouble();
        }
    }
    else
    {
        qWarning() << query.lastQuery();
        qWarning() << "ERROR in volumeInfo" << query.lastError().text();
    }

    return result;
}

bool MediaLibrary::setVolumeInfo(const int idMedia, const QHash<QString, double> info)
{
    QSqlDatabase db = GET_DATABASE("MEDIA_DATABASE");

    QSqlQuery query(db);

    bool ret = false;

    if (!db.transaction())
    {
        qDebug() << "unable to begin transaction" << db.lastError().text();
    }
    else
    {
        foreach(const QString &param, info.keys())
        {
            if (param.endsWith("_volume"))
            {
                int paramId = -1;
                query.prepare("SELECT id from param_name WHERE name=:name");
                query.bindValue(":name", param);
                ret = query.exec();
                if (ret)
                {
                    if (query.next())
                    {
                        paramId = query.record().value("id").toInt();
                    }
                    else
                    {
                        qWarning() << "param not found" << param;
                        query.prepare("INSERT INTO param_name (name) VALUES (:name)");
                        query.bindValue(":name", param);
                        ret = query.exec();
                        if (!ret)
                        {
                            qWarning() << "unable to add param" << param;
                            break;
                        }
                        else
                        {
                            paramId = query.lastInsertId().toInt();
                        }
                    }
                }
                else
                {
                    qWarning() << "ERROR unable to find param name" << param << query.lastError().text();
                    break;
                }

                query.prepare("INSERT INTO param_value (name, media, value) VALUES (:name, :media, :value)");
                query.bindValue(":name", paramId);
                query.bindValue(":media", idMedia);
                query.bindValue(":value", info[param]);

                ret = query.exec();
                if (!ret)
                {
                    qWarning() << "ERROR" << query.lastError().text();
                    break;
                }
            }
        }

        if (ret)
        {
            if (!db.commit())
                qCritical() << "unable to commit" << db.lastError().text();
        }
        else
        {
            if (!db.rollback())
                qCritical() << "unable to rollback" << db.lastError().text();
        }
    }

    return ret;
}

QSqlQuery MediaLibrary::getDistinctMetaData(const int &typeMedia, const QString &tagName, const QString &where) const
{
    QString whereQuery;
    if (!where.isEmpty())
        whereQuery = "and " + where;

    QSqlQuery query(GET_DATABASE("MEDIA_DATABASE"));
    if (foreignKeys["media"].contains(tagName)) {
        QString foreignTable = foreignKeys["media"][tagName]["table"];
        QString foreignTo = foreignKeys["media"][tagName]["to"];
        query.exec(QString("SELECT DISTINCT %2.id, %2.name FROM media LEFT OUTER JOIN %2 ON media.%1=%2.%3 WHERE media.type=%4 and is_reachable=1 %5 ORDER BY %2.name").arg(tagName).arg(foreignTable).arg(foreignTo).arg(typeMedia).arg(whereQuery));
    } else {
        query.exec(QString("SELECT DISTINCT %1 FROM media WHERE type=%2 and is_reachable=1 %3 ORDER by %1").arg(tagName).arg(typeMedia).arg(whereQuery));
    }

    return query;
}

int MediaLibrary::countDistinctMetaData(const int &typeMedia, const QString &tagName) const {
    QSqlQuery query(GET_DATABASE("MEDIA_DATABASE"));
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

int MediaLibrary::countMedia(const QString &where) const {
    QSqlQuery query(QString("SELECT count(id) FROM media WHERE %1 and is_reachable=1").arg(where), GET_DATABASE("MEDIA_DATABASE"));
    if (query.next()) {
        return query.value(0).toInt();
    } else {
        return 0;
    }
}

bool MediaLibrary::insert(const QString &table, const QHash<QString, QVariant> &data) {
    QSqlDatabase db = GET_DATABASE("MEDIA_DATABASE");

    QSqlQuery query(db);

    bool ret = false;

    if (!db.transaction())
    {
        qDebug() << "unable to begin transaction" << db.lastError().text();
    }
    else
    {
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

        ret = query.prepare(QString("INSERT INTO %3 (%1) VALUES (%2)").arg(parameters).arg(values).arg(table));
        if (ret)
        {
            foreach(QString elt, l_parameters) {
                if (data[elt].type() != QVariant::Int && foreignKeys[table].contains(elt)) {
                    QString foreignTable = foreignKeys[table][elt]["table"];

                    // replace the value of the foreign key by its id
                    int index = insertForeignKey(foreignTable, "name", data[elt]);
                    query.bindValue(QString(":%1").arg(elt), index);
                    if (index == -1) {
                        qCritical() << "unable to bind " + elt;
                        if (!db.rollback())
                            qCritical() << "unable to rollback" << db.lastError().text();
                        return false;
                    }
                } else {
                    query.bindValue(QString(":%1").arg(elt), data[elt]);
                }
            }

            ret = query.exec();
            if (!ret)
                qCritical() << "unable to insert data in MediaLibrary " + query.lastError().text();
        }

        if (ret)
        {
            if (!db.commit())
                qCritical() << "unable to commit" << db.lastError().text();
        }
        else
        {
            if (!db.rollback())
                qCritical() << "unable to rollback" << db.lastError().text();
        }
    }

    return ret;
}

int MediaLibrary::insertForeignKey(const QString &table, const QString &parameter, const QVariant &value)
{
    QSqlDatabase db = GET_DATABASE("MEDIA_DATABASE");
    QSqlField field(parameter, value.type());
    field.setValue(value);

    QSqlQuery query(db);

    int index = -1;
    query.exec(QString("SELECT id FROM %1 WHERE %2 = %3").arg(table).arg(parameter).arg(db.driver()->formatValue(field)));
    if (query.next())
    {
        index = query.value(0).toInt();
    }
    else
    {
        if (!query.exec(QString("INSERT INTO %1 (%2) VALUES (%3)").arg(table).arg(parameter).arg(db.driver()->formatValue(field))))
        {
            qCritical() << "unable to update MediaLibrary " + query.lastError().text();
            return -1;
        }
        else
        {
            index = query.lastInsertId().toInt();
        }
    }
    return index;
}

bool MediaLibrary::update(const QString &table, const int &id, const QHash<QString, QVariant> &data)
{
    QSqlDatabase db = GET_DATABASE("MEDIA_DATABASE");
    QSqlRecord record;

    {
        QSqlQuery query(db);
        if (query.exec(QString("SELECT * from %2 where id=%1").arg(id).arg(table)))
            if (query.next())
                record = query.record();
    }

    if (record.isEmpty())
    {
        qDebug() << "media" << id << "not found";
        return false;
    }

    if (!db.transaction())
    {
        qDebug() << "unable to begin transaction" << db.lastError().text();
        return false;
    }
    else
    {
        foreach(QString elt, data.keys())
        {
            if (!data[elt].isNull() and !data[elt].toString().isEmpty())
            {

                if (data[elt].type() != QVariant::Int && foreignKeys[table].contains(elt))
                {
                    QString foreignTable = foreignKeys[table][elt]["table"];

                    // replace the value of the foreign key by its id
                    int index = insertForeignKey(foreignTable, "name", data[elt]);

                    if (index == -1)
                    {
                        qCritical() << "unable to bind " << elt;
                        if (!db.rollback())
                            qCritical() << "unable to rollback" << db.lastError().text();
                        return false;
                    }
                    else if (record.value(elt) != index)
                    {
                        qDebug() << QString("update %1, %2, index %3 --> %4").arg(elt).arg(record.value(elt).toString()).arg(index).arg(data[elt].toString());
                        QSqlQuery queryUpdate(db);
                        if (!queryUpdate.exec(QString("UPDATE %4 SET %1=%2 WHERE id=%3").arg(elt).arg(index).arg(id).arg(table)))
                        {
                            qCritical() << queryUpdate.lastError().text();
                            if (!db.rollback())
                                qCritical() << "unable to rollback" << db.lastError().text();
                            return false;
                        }
                    }
                }
                else
                {
                    if (record.value(elt) != data[elt])
                    {
                        qDebug() << QString("update %1, %2 --> %3").arg(elt).arg(record.value(elt).toString()).arg(data[elt].toString());
                        QSqlQuery queryUpdate(db);
                        QSqlField field = record.field(record.indexOf(elt));
                        field.setValue(data[elt]);
                        if (!queryUpdate.exec(QString("UPDATE %4 SET %1=%2 WHERE id=%3").arg(elt).arg(db.driver()->formatValue(field)).arg(id).arg(table)))
                        {
                            qDebug() << queryUpdate.lastError().text();
                            if (!db.rollback())
                                qCritical() << "unable to rollback" << db.lastError().text();
                            return false;
                        }
                    }
                }
            }
        }

        if (!db.commit())
        {
            qDebug() << "unable to commit" << db.lastError().text();
            return false;
        }
    }

    return true;
}

bool MediaLibrary::updateFromFilename(const QString &filename, const QHash<QString, QVariant> &data)
{
    int id = -1;

    {
        QSqlQuery query = QSqlQuery(QString("SELECT id from media WHERE filename=\"%1\"").arg(filename), GET_DATABASE("MEDIA_DATABASE"));
        if (query.next())
            id = query.value("id").toInt();
    }

    if (id != -1)
    {
        return update("media", id, data);
    }
    else
    {
        qCritical() << "updateFromFilename" << filename << "not found";
        return false;
    }
}

bool MediaLibrary::updateFromId(const int &id, const QHash<QString, QVariant> &data)
{
    return update("media", id, data);
}

bool MediaLibrary::incrementCounterPlayed(const QString &filename)
{
    QSqlQuery query = getMedia(QString("filename=\"%1\"").arg(filename));
    if (query.next())
    {
        QHash<QString, QVariant> data;
        data["progress_played"] = 0;
        data["counter_played"] = query.value("counter_played").toInt()+1;

        return update("media", query.value("id").toInt(), data);
    }

    return false;
}

int MediaLibrary::add_album(QHash<QString, QVariant> data_album)
{
    int id_album = -1;
    if (data_album.contains("name") && !data_album["name"].toString().isEmpty() && data_album.contains("artist"))
    {
        QSqlQuery queryAlbum(GET_DATABASE("MEDIA_DATABASE"));
        if (!data_album["artist"].toString().isEmpty())
        {
            queryAlbum.prepare("SELECT album.id, artist.name from album LEFT OUTER JOIN artist ON artist.id=album.artist WHERE album.name=:name and artist.name=:artist");
            queryAlbum.bindValue(":name", data_album["name"]);
            queryAlbum.bindValue(":artist", data_album["artist"]);
        }
        else
        {
            queryAlbum.prepare("SELECT album.id, artist.name from album LEFT OUTER JOIN artist ON artist.id=album.artist WHERE album.name=:name and artist.name is null");
            queryAlbum.bindValue(":name", data_album["name"]);
        }

        if (!queryAlbum.exec())
        {
            qCritical() << QString("unable to update album %1").arg(queryAlbum.lastError().text());
        }
        else if (queryAlbum.next())
        {
            id_album = queryAlbum.value(0).toInt();

            update("album", id_album, data_album);
        }
        else
        {
            if (!insert("album", data_album))
            {
                qCritical() << QString("unable to add album: %1 with artist %2").arg(data_album["name"].toString()).arg(data_album["artist"].toString());
            }

            if (queryAlbum.exec() && queryAlbum.next())
                id_album = queryAlbum.value(0).toInt();
            else
                qCritical() << QString("unable to add album: %1 with artist %2 (%3)").arg(data_album["name"].toString()).arg(data_album["artist"].toString()).arg(queryAlbum.lastError().text());
        }
    }

    return id_album;
}

int MediaLibrary::add_artist(QHash<QString, QVariant> data_artist)
{
    int id_artist = -1;
    if (data_artist.contains("name") && !data_artist["name"].toString().isEmpty())
    {
        QSqlQuery queryArtist(GET_DATABASE("MEDIA_DATABASE"));
        queryArtist.prepare("SELECT artist.id from artist WHERE artist.name=:name");
        queryArtist.bindValue(":name", data_artist["name"]);

        if (!queryArtist.exec())
        {
            qCritical() << QString("unable to update artist %1").arg(queryArtist.lastError().text());
        }
        else if (queryArtist.next())
        {
            id_artist = queryArtist.value(0).toInt();

            update("artist", id_artist, data_artist);
        }
        else
        {
            if (!insert("artist", data_artist))
            {
                qCritical() << QString("unable to add artist: %1").arg(data_artist["name"].toString());
            }

            if (queryArtist.exec() && queryArtist.next())
                id_artist = queryArtist.value(0).toInt();
            else
                qCritical() << QString("unable to add artist: %1 (%2)").arg(data_artist["name"].toString()).arg(queryArtist.lastError().text());
        }
    }

    return id_artist;
}

bool MediaLibrary::add_media(QHash<QString, QVariant> data, QHash<QString, QVariant> data_album, QHash<QString, QVariant> data_artist)
{
    int id_album = add_album(data_album);
    if (id_album != -1)
        data["album"] = id_album;

    int id_artist = add_artist(data_artist);
    if (id_artist != -1)
        data["artist"] = id_artist;

    QSqlQuery query(GET_DATABASE("MEDIA_DATABASE"));
    query.prepare("SELECT id, last_modified FROM media WHERE filename=:filename");
    query.bindValue(":filename", data["filename"]);

    if (query.exec() && query.next()) {
        // media already stored in library
        QDateTime lastModified = query.value("last_modified").toDateTime();

        if (lastModified.isValid() && lastModified >= data["last_modified"].toDateTime()) {
            // no need to update the media
            return true;
        } else {
            // update the media
            qWarning() << "update resource" << data["mime_type"].toString() << data["filename"].toString() << lastModified << data["last_modified"].toDateTime();
            return update("media", query.value("id").toInt(), data);
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

        qDebug() << "add resource " + data["mime_type"].toString() + " " + data["filename"].toString();
        return insert("media", data);
    }
}

bool MediaLibrary::contains(const QFileInfo &fileinfo) const
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

//void MediaLibrary::checkMetaData(const QFileInfo &fileinfo) const
//{
//    QSqlQuery query = getMedia(QString("filename=\"%1\"").arg(fileinfo.absoluteFilePath()), db);
//    if (query.next()) {
//        if (query.value("type_media") == "audio" && fileinfo.absoluteFilePath().contains("Daft")) {
//            qWarning() << "CHECK" << fileinfo.absoluteFilePath();
//            AcoustIdAnswer *answer = m_acoustId.requestId(fileinfo);
//            if (answer) {
//                if (answer->title() != getmetaData("title", query.value("id").toInt()))
//                    qWarning() << "    title differs:" << answer->title() << getmetaData("title", query.value("id").toInt()).toString();
//            } else {
//                qWarning() << "No acoustid found";
//            }
//        }
//    }
//}

MediaLibrary::StateType *MediaLibrary::exportMediaState() const
{
    StateType *res = 0;

    QStringList attributesToExport;
    attributesToExport << "rating" << "last_played" << "progress_played" << "counter_played" << "acoustid" << "mbid";
    attributesToExport << "addedDate";
    attributesToExport << "artist";

    QSqlQuery query(GET_DATABASE("MEDIA_DATABASE"));
    if (query.exec(QString("SELECT id, filename from media")))
    {
        res = new StateType();
        while (query.next())
        {
            QHash<QString, QVariant> d_values;

            foreach (const QString &param, attributesToExport)
            {
                QVariant value = getmetaData(param, query.value("id").toInt());
                if (!value.isNull())
                    d_values[param] = value;
            }

            if (!d_values.isEmpty())
                res->operator [](query.value("filename").toString()) = d_values;
        }
    } else {
        qCritical() << QString("Unable to execute request: %1").arg(query.lastError().text());
    }

    return res;
}

bool MediaLibrary::resetLibrary(const QString &pathname)
{
    if (libraryState)
        delete libraryState;

    // save current state of the library
    libraryState = exportMediaState();

    if (libraryState) {
        // close current database
        QSqlDatabase db = GET_DATABASE("MEDIA_DATABASE");
        db.close();

        // open new database
        db.setDatabaseName(pathname);
        return db.open() && initialize();
    } else {
        return false;
    }
}
