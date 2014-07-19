#include "mediarenderer.h"

MediaRenderer::MediaRenderer(Logger *log, const QString &ip, const int &port, const QString &userAgent, QObject *parent) :
    QObject(parent),
    log(log != 0 ? log : new Logger(this)),
    ip(ip),
    port(port),
    userAgent(userAgent),
    status("standby"),
    name("DefaultRenderer")
{
}
