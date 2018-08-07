#include "upnpclassimageprovider.h"

UpnpClassImageProvider::UpnpClassImageProvider():
    QQuickImageProvider(QQuickImageProvider::Image)
{

}


QImage UpnpClassImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QImage image;

    if (id.contains("object.item.audioItem"))
        image.load(":/images/class/music.png");
    else if (id.contains("object.item.videoItem"))
        image.load(":/images/class/video.png");
    else if (id.contains("object.container"))
        image.load(":/images/class/folder.png");
    else
        qCritical() << "unknown image requested" << id;

    if (size)
        *size = QSize(image.size().width(), image.size().height());

    if (requestedSize.width() > 0)
        return image.scaledToWidth(requestedSize.width(), Qt::SmoothTransformation);
    if (requestedSize.height() > 0)
        return image.scaledToHeight(requestedSize.height(), Qt::SmoothTransformation);
    return image;
}
