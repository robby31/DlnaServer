#ifndef MEDIALIBRARY_H
#define MEDIALIBRARY_H

#include <QObject>
#include <QtSql>

#include "logger.h"

class MediaLibrary : public QObject
{
    Q_OBJECT

public:
    explicit MediaLibrary(Logger* log, QString pathname, QObject *parent = 0);
    virtual ~MediaLibrary();

    QSqlQuery getMediaType() { return QSqlQuery("SELECT DISTINCT id, name FROM type"); }

    QSqlQuery getMedia(QString where) { return QSqlQuery(QString("SELECT media.id, media.filename, type.name AS type_media from media LEFT OUTER JOIN type ON media.type=type.id WHERE %1").arg(where)); }
    int countMedia(QString where);

    QSqlQuery getAllMetaData(QString tagName) { return QSqlQuery(QString("SELECT DISTINCT id, name from %1 ORDER BY name").arg(tagName)); }
    int countAllMetaData(QString tagName);

    QVariant getmetaData(QString tagName, int idMedia);

    bool insert(QHash<QString, QVariant> data);

signals:

public slots:

private:
    Logger* log;
    QSqlDatabase db;

    QStringList foreignKeys;

    int insertForeignKey(QString table, QString parameter, QVariant value);

};

#endif // MEDIALIBRARY_H
