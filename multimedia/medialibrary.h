#ifndef MEDIALIBRARY_H
#define MEDIALIBRARY_H

#include <QObject>
#include <QtSql>

#include "logobject.h"
//#include "acoustid.h"

class MediaLibrary : public LogObject
{
    Q_OBJECT

    typedef QHash<QString, QHash<QString, QVariant> > StateType;

public:
    explicit MediaLibrary(Logger *log, QObject *parent = 0);
    virtual ~MediaLibrary();

    bool initialize();

    QSqlDatabase getDatabase() const { return QSqlDatabase::database("MEDIA_DATABASE"); }

    QSqlQuery getMediaType() const { return QSqlQuery("SELECT DISTINCT id, name FROM type", db); }

    QSqlQuery getMedia(const QString &where, const QString &orderParam="media.id", const QString &sortOption="ASC") const;
    int countMedia(const QString &where) const;

    QSqlQuery getAllNetworkLinks() { return QSqlQuery("SELECT id, filename, title, artist, is_reachable from media WHERE filename like 'http%'", db); }

    QSqlQuery getDistinctMetaData(const int &typeMedia, const QString &tagName, const QString &where = QString()) const;
    int countDistinctMetaData(const int &typeMedia, const QString &tagName) const;

    QVariant getmetaData(const QString &tagName, const int &idMedia) const;
    QVariant getmetaDataAlbum(const QString &tagName, const int &idMedia) const;
    QVariant getmetaDataArtist(const QString &tagName, const int &idMedia) const;

    QHash<QString, double> volumeInfo(const int &idMedia);
    bool setVolumeInfo(const int idMedia, const QHash<QString, double> info);

//    void checkMetaData(const QFileInfo &fileinfo) const;

    bool contains(const QFileInfo &fileinfo) const;
    bool add_media(QHash<QString, QVariant> data_media, QHash<QString, QVariant> data_album, QHash<QString, QVariant> data_artist);
    int add_album(QHash<QString, QVariant> data_album);
    int add_artist(QHash<QString, QVariant> data_artist);
    bool updateFromFilename(const QString &filename, const QHash<QString, QVariant> &data);
    bool incrementCounterPlayed(const QString &filename);

    bool resetLibrary(const QString &pathname);

private:
    int insertForeignKey(const QString &table, const QString &parameter, const QVariant &value);

    bool insert(const QString &table, const QHash<QString, QVariant> &data);
    bool update(const QString &table, const int &id, const QHash<QString, QVariant> &data);

    // export attributes not stored in media file such as last_played, counter_played, progress_played.
    StateType *exportMediaState() const;

signals:

public slots:

private:
    QSqlDatabase db;

    QHash<QString, QHash<QString, QHash<QString, QString> > > foreignKeys;
    StateType *libraryState;

//    Acoustid m_acoustId;
};

#endif // MEDIALIBRARY_H
