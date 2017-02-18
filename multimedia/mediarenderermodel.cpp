#include "mediarenderermodel.h"

MediaRendererModel::MediaRendererModel(QObject *parent) :
    ListModel(new MediaRenderer, parent)
{

}

void MediaRendererModel::addMediaRenderer(const QHostAddress &ip, const int &port, const SsdpMessage &info)
{
    MediaRenderer *renderer = getFromIp(ip.toString());
    if (!renderer)
    {
        renderer = new MediaRenderer(ip.toString(), port, info.getHeader("SERVER"), this);
        appendRow(renderer);
    }
}

MediaRenderer* MediaRendererModel::getFromIp(const QString &ip) const
{
    foreach (ListItem* item, m_list)
    {
        MediaRenderer *renderer = qobject_cast<MediaRenderer*>(item);
        if (renderer && renderer->data(MediaRenderer::networkAddressRole) == ip)
            return renderer;
    }

    return 0;
}

void MediaRendererModel::serving(const QString &ip, const QString &mediaName)
{
    MediaRenderer* renderer = getFromIp(ip);
    if (renderer)
        renderer->setData(QString("Serving %1").arg(mediaName), MediaRenderer::statusRole);
}

void MediaRendererModel::stopServing(const QString &ip)
{
    MediaRenderer* renderer = getFromIp(ip);
    if (renderer)
        renderer->setData("standby", MediaRenderer::statusRole);
}
