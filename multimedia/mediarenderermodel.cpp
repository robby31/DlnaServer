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
    {
        removeRow(index.row());
        emit mediaRendererDestroyed(renderer->netWorkAddress());
    }
}

void MediaRendererModel::removeRenderer()
{
    MediaRenderer *renderer = qobject_cast<MediaRenderer*>(sender());

    QModelIndex index = indexFromItem(renderer);

    if (index.isValid())
    {
        removeRow(index.row());
        emit mediaRendererDestroyed(renderer->netWorkAddress());
    }
}

void MediaRendererModel::serving(const QString &ip, const QString &mediaName)
{
    MediaRenderer* renderer = rendererFromIp(ip);
    if (renderer)
    {
        if (mediaName.isEmpty())
            renderer->setData("standby", MediaRenderer::statusRole);
        else
            renderer->setData(QString("Serving %1").arg(mediaName), MediaRenderer::statusRole);
    }
    else
    {
        qCritical() << this << "unable to find renderer" << ip;
    }
}

MediaRenderer *MediaRendererModel::rendererFromIp(const QString &ip)
{
    for (int i=0;i<rowCount();++i)
    {
        MediaRenderer *renderer = qobject_cast<MediaRenderer*>(at(i));
        if (renderer->netWorkAddress() == ip)
            return renderer;
    }

    return 0;
}
