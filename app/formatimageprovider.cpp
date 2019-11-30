#include "formatimageprovider.h"

FormatImageProvider::FormatImageProvider():
    QQuickImageProvider(QQuickImageProvider::Image)
{

}

QImage FormatImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
#if !defined(QT_NO_DEBUG_OUTPUT)
    qDebug() << "IMAGE" << id << size << requestedSize;
#endif
    QImage image;

    if (id.contains("/"))
    {
        QStringList tmp = id.split("/");

        if (tmp.size() == 2)
        {
            QString type = tmp[0];
            QString format = tmp[1];

            if (format == "mp3")
                image.load(":/images/media/mp3.png");
            else if (format == "aac")
                image.load(":/images/media/aac.png");
            else if (format == "alac")
                image.load(":/images/media/alac.png");
            else if (type.startsWith("audio"))
                image.load(":/images/media/music.png");
            else
                image.load(":/images/media/movie.png");

            if (size)
                *size = QSize(image.size().width(), image.size().height());
        }
        else
        {
            qCritical() << "invalid format" << id;
        }

    }
    else
    {
        qCritical() << "invalid format" << id;
    }

    if (requestedSize.width() > 0)
        return image.scaledToWidth(requestedSize.width(), Qt::SmoothTransformation);
    if (requestedSize.height() > 0)
        return image.scaledToHeight(requestedSize.height(), Qt::SmoothTransformation);
    return image;
}
