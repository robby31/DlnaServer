#include "mediarenderer.h"

MediaRenderer::MediaRenderer(Logger *log, QString ip, int port, QString userAgent, QObject *parent) :
    QObject(parent),
    log(log),
    ip(ip),
    port(port),
    userAgent(userAgent),
    status("standby"),
    name("DefaultRenderer")
{
}
