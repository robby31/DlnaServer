#ifndef MEDIARENDERER_H
#define MEDIARENDERER_H

#include "upnprootdevice.h"
#include "Services/serviceconnectionmanager.h"
#include "mynetwork.h"

class MediaRenderer : public UpnpRootDevice
{
    Q_OBJECT

public:
    explicit MediaRenderer(const QString &macAddress, const QString& host, int port, QObject *parent = Q_NULLPTR);

private:
    void initDescription();
    void initConnectionManager();
};

#endif // MEDIARENDERER_H
