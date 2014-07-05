#ifndef MEDIALIBRARY_H
#define MEDIALIBRARY_H

#include <QObject>
#include <QtSql>

#include "logger.h"
#include "acoustid.h"

class MediaLibrary : public QObject
{
    Q_OBJECT

    typedef QHash<QString, QHash<QString, QVariant> > StateType;

public:
    explicit MediaLibrary(Logger *log, QSqlDatabase *database, QObject *parent = 0);
    virtual ~MediaLibrary();

    bool open();

    QSqlDatabase *getDatabase() const { return db; }

    QSqlQuery getMediaType() { return QSqlQuery("SELECT DISTINCT id, name FROM type"); }

    QSqlQuery getMedia(QString where, QString orderParam="media.id", QString sortOption="ASC") { return QSqlQuery(QString("SELECT media.id, media.filename, type.name AS type_media, media.last_modified, media.counter_played from media LEFT OUTER JOIN type ON media.type=type.id WHERE %1 and is_reachable=1 ORDER BY %2 %3").arg(where).arg(orderParam).arg(sortOption)); }
    int countMedia(QString where);

    QSqlQuery getDistinctMetaData(int typeMedia, QString tagName);
    int countDistinctMetaData(int typeMedia, QString tagName);

    QVariant getmetaData(QString tagName, int idMedia);

    void checkMetaData(QFileInfo fileinfo);

    bool contains(QFileInfo fileinfo);
    bool add_media(QHash<QString, QVariant> data);
    bool updateFromFilename(QString filename, QHash<QString, QVariant> data);
    bool incrementCounterPlayed(const QString &filename);

    bool resetLibrary(QString pathname);

private:
    int insertForeignKey(QString table, QString parameter, QVariant value);

    bool insert(QHash<QString, QVariant> data);
    bool update(int id, QHash<QString, QVariant> data);

    // export attributes not stored in media file such as last_played, counter_played, progress_played.
    StateType *exportMediaState();

signals:

public slots:

private:
    Logger* log;
    QSqlDatabase *db;

    QHash<QString, QHash<QString, QHash<QString, QString> > > foreignKeys;
    StateType *libraryState;

    Acoustid m_acoustId;
};

#endif // MEDIALIBRARY_H
