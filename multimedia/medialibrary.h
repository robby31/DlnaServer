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

    QSqlQuery getMediaType() const { return QSqlQuery("SELECT DISTINCT id, name FROM type"); }

    QSqlQuery getMedia(const QString &where, const QString &orderParam="media.id", const QString &sortOption="ASC") const { return QSqlQuery(QString("SELECT media.id, media.filename, type.name AS type_media, media.last_modified, media.counter_played from media LEFT OUTER JOIN type ON media.type=type.id WHERE %1 and is_reachable=1 ORDER BY %2 %3").arg(where).arg(orderParam).arg(sortOption)); }
    int countMedia(const QString &where) const;

    QSqlQuery getAllNetworkLinks() { return QSqlQuery("SELECT id, filename, title, artist from media WHERE filename like 'http%'"); }

    QSqlQuery getDistinctMetaData(const int &typeMedia, const QString &tagName, const QString &where = QString()) const;
    int countDistinctMetaData(const int &typeMedia, const QString &tagName) const;

    QVariant getmetaData(const QString &tagName, const int &idMedia) const;

    void checkMetaData(const QFileInfo &fileinfo) const;

    bool contains(const QFileInfo &fileinfo) const;
    bool add_media(QHash<QString, QVariant> data);
    bool updateFromFilename(const QString &filename, const QHash<QString, QVariant> &data);
    bool incrementCounterPlayed(const QString &filename);

    bool resetLibrary(const QString &pathname);

private:
    int insertForeignKey(const QString &table, const QString &parameter, const QVariant &value);

    bool insert(const QHash<QString, QVariant> &data);
    bool update(const int &id, const QHash<QString, QVariant> &data);

    // export attributes not stored in media file such as last_played, counter_played, progress_played.
    StateType *exportMediaState() const;

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
