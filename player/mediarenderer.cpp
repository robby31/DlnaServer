#include "mediarenderer.h"

MediaRenderer::MediaRenderer(const QString& macAddress, const QString& host, int port, QObject *parent):
    UpnpRootDevice (macAddress, QString(), parent)
{
    QUrl tmp(QString("http://%1:%2").arg(host).arg(port));
    setUrl(tmp.resolved(QString("/description/fetch")));

    initDescription();

    initConnectionManager();
}

void MediaRenderer::initDescription()
{
    auto deviceDescription = new UpnpRootDeviceDescription(this);
    deviceDescription->setDeviceAttribute("deviceType", "urn:schemas-upnp-org:device:MediaRenderer:1");
    deviceDescription->setDeviceAttribute("friendlyName", "Media Player");
    deviceDescription->setDeviceAttribute("manufacturer", "G HIMBERT");
    deviceDescription->setDeviceAttribute("modelDescription", "UPnP/AV 1.0 Compliant Media Player");
    deviceDescription->setDeviceAttribute("modelName", "QMP");
    deviceDescription->setDeviceAttribute("modelNumber", "01");
    deviceDescription->setDeviceAttribute("presentationURL", QString("http://%1:%2/console/index.html").arg(url().host()).arg(url().port()));

    deviceDescription->setDeviceAttribute("UDN", QString("uuid:%1").arg(id()));
    setDescription(deviceDescription);

    setServerName(QString("%1/%2 UPnP/%3 QM/1.0").arg(QSysInfo::productType(), QSysInfo::productVersion(), version()));

}

void MediaRenderer::initConnectionManager()
{
    auto connection_manager = new ServiceConnectionManager(this, this);

    QStringList format;

    format << "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3";
    format << "http-get:*:audio/L16:DLNA.ORG_PN=LPCM";
    format << "http-get:*:audio/mp4:DLNA.ORG_PN=AAC_ISO";
    format << "http-get:*:audio/mp4:DLNA.ORG_PN=AAC_ISO_320";

    format << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_MP_HD_AC3";
    format << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_MP_HD_AAC_MULT5";

    connection_manager->updateStateVariable("SinkProtocolInfo", format.join(","));

    connection_manager->setDirection(ServiceConnectionManager::Output);

    addService(connection_manager);
}
