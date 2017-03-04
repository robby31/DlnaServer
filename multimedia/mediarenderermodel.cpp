#include "mediarenderermodel.h"

MediaRendererModel::MediaRendererModel(QObject *parent) :
    ListModel(new MediaRenderer, parent)
{

}

void MediaRendererModel::addMediaRenderer(UpnpRootDevice *device)
{
    MediaRenderer *renderer = qobject_cast<MediaRenderer*>(find(device->uuid()));
    if (!renderer)
    {
        renderer = new MediaRenderer(device, this);
        connect(renderer, SIGNAL(destroyed(QObject*)), this, SLOT(rendererDestroyed(QObject*)));
        connect(renderer, SIGNAL(removeRenderer()), this, SLOT(removeRenderer()));
        appendRow(renderer);
    }
}

void MediaRendererModel::rendererDestroyed(QObject *object)
{
    MediaRenderer *renderer = qobject_cast<MediaRenderer*>(object);

    QModelIndex index = indexFromItem(renderer);

    if (index.isValid())
        removeRow(index.row());
}

void MediaRendererModel::removeRenderer()
{
    MediaRenderer *renderer = qobject_cast<MediaRenderer*>(sender());

    QModelIndex index = indexFromItem(renderer);

    if (index.isValid())
        removeRow(index.row());
}

void MediaRendererModel::serving(const QString &ip, const QString &mediaName)
{
//    MediaRenderer* renderer = getFromIp(ip);
//    if (renderer)
//        renderer->setData(QString("Serving %1").arg(mediaName), MediaRenderer::statusRole);
}

void MediaRendererModel::stopServing(const QString &ip)
{
//    MediaRenderer* renderer = getFromIp(ip);
//    if (renderer)
//        renderer->setData("standby", MediaRenderer::statusRole);
}
