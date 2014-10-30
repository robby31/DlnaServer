#include "mediarenderer.h"

MediaRenderer::MediaRenderer(const QString &ip, const int &port, const QString &userAgent, QObject *parent) :
    QObject(parent),
    ip(ip),
    port(port),
    userAgent(userAgent),
    status("standby"),
    name("DefaultRenderer")
{
}
