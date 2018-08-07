#include "mediaimageprovider.h"

MediaImageProvider::MediaImageProvider():
    QQuickImageProvider(QQuickImageProvider::Image)
{

}

QImage MediaImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QImage image;

    QSqlDatabase db = GET_DATABASE("MEDIA_DATABASE");
    QSqlQuery query(db);
    query.prepare("SELECT * from picture WHERE id=:id");
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
    }

    if (requestedSize.width() > 0)
        return image.scaledToWidth(requestedSize.width(), Qt::SmoothTransformation);
    if (requestedSize.height() > 0)
        return image.scaledToHeight(requestedSize.height(), Qt::SmoothTransformation);
    return image;
}
