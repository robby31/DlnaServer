#ifndef MEDIALIBRARY_H
#define MEDIALIBRARY_H

#include <QObject>
#include <QtSql>

#include "logger.h"

class MediaLibrary : public QObject
{
    Q_OBJECT

public:
    explicit MediaLibrary(Logger *log, QSqlDatabase *database, QObject *parent = 0);
    virtual ~MediaLibrary();

    QSqlDatabase *getDatabase() const { return db; }

    QSqlQuery getMediaType() { return QSqlQuery("SELECT DISTINCT id, name FROM type"); }

    QSqlQuery getMedia(QString where) { return QSqlQuery(QString("SELECT media.id, media.filename, type.name AS type_media, media.last_modified from media LEFT OUTER JOIN type ON media.type=type.id WHERE %1").arg(where)); }
    int countMedia(QString where);

    QSqlQuery getDistinctMetaData(int typeMedia, QString tagName);
    int countDistinctMetaData(int typeMedia, QString tagName);

    QVariant getmetaData(QString tagName, int idMedia);

    bool insert(QHash<QString, QVariant> data);

signals:

public slots:

private:
    Logger* log;
    QSqlDatabase *db;

    QHash<QString, QHash<QString, QHash<QString, QString> > > foreignKeys;

    int insertForeignKey(QString table, QString parameter, QVariant value);

};

#endif // MEDIALIBRARY_H
