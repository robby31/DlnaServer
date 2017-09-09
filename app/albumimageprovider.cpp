#include "albumimageprovider.h"

AlbumImageProvider::AlbumImageProvider():
    QQuickImageProvider(QQuickImageProvider::Image)
{

}

QImage AlbumImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QImage image;

    QSqlDatabase db = GET_DATABASE("MEDIA_DATABASE");
    QSqlQuery query(db);
    query.prepare("SELECT DISTINCT album.name AS albumName, picture.id, picture.name from media LEFT OUTER JOIN picture ON media.picture=picture.id LEFT OUTER JOIN album ON media.album=album.id WHERE album=:id and picture is not null");
    query.bindValue(":id", id);

    bool hasError = false;
    if (!query.exec())
    {
        hasError = true;
        qCritical() << "invalid query" << query.lastError().text();
    }
    else
    {
        hasError = !query.next();
    }

    if (hasError)
    {
        int width = 50;
        int height = 50;

        if (size)
            *size = QSize(width, height);

        image.load(":/images/noalbum.png");
    }
    else
    {
        QByteArray data = QByteArray::fromHex(query.value("name").toByteArray());
        if (image.loadFromData(data))
        {
            if (size)
                *size = QSize(image.size().width(), image.size().height());
        }

        query.last();
        if (query.at() > 0)
            qWarning() << "ALBUM" << query.value("albumName") << "has" << query.at()+1 << "different pictures.";
    }

    if (requestedSize.width() > 0)
        return image.scaledToWidth(requestedSize.width());
    else if (requestedSize.height() > 0)
        return image.scaledToHeight(requestedSize.height());
    else
        return image;
}
