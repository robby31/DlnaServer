#include "mediaserver.h"

MediaServer::MediaServer(QNetworkAccessManager *nam, QString macAddress, QString host, int port, MediaRendererModel *renderersModel, QObject *parent):
    UpnpRootDevice(nam, macAddress, QString(), parent),
    m_renderersModel(renderersModel)
{
    QUrl tmp(QString("http://%1:%2").arg(host).arg(port));
    setUrl(tmp.resolved(QString("/description/fetch")));

    initDescription();

    initConnectionManager();

    initContentDirectory();
}

void MediaServer::initDescription()
{
    UpnpRootDeviceDescription *deviceDescription = new UpnpRootDeviceDescription();
    deviceDescription->setDeviceAttribute("deviceType", "urn:schemas-upnp-org:device:MediaServer:1");
    deviceDescription->setDeviceAttribute("friendlyName", "QT Media Server");
    deviceDescription->setDeviceAttribute("manufacturer", "G HIMBERT");
    deviceDescription->setDeviceAttribute("modelDescription", "UPnP/AV 1.0 Compliant Media Server");
    deviceDescription->setDeviceAttribute("modelName", "QMS");
    deviceDescription->setDeviceAttribute("modelNumber", "01");
    deviceDescription->setDeviceAttribute("presentationURL", QString("http://%1:%2/console/index.html").arg(url().host()).arg(url().port()));
    deviceDescription->addIcon("image/png", 512, 512, 24, "/images/icon-512.png");
    deviceDescription->addIcon("image/png", 256, 256, 24, "/images/icon-256.png");
    deviceDescription->addIcon("image/png", 128, 128, 24, "/images/icon-128.png");
    deviceDescription->addIcon("image/jpeg", 128, 128, 24, "/images/icon-128.jpg");
    deviceDescription->addIcon("image/png", 120, 120, 24, "/images/icon-120.png");
    deviceDescription->addIcon("image/jpeg", 120, 120, 24, "/images/icon-120.jpg");
    deviceDescription->addIcon("image/png", 48, 48, 24, "/images/icon-48.png");
    deviceDescription->addIcon("image/jpeg", 48, 48, 24, "/images/icon-48.jpg");
    deviceDescription->addIcon("image/png", 32, 32, 24, "/images/icon-32.png");
    deviceDescription->addIcon("image/png", 16, 16, 24, "/images/icon-16.png");

    deviceDescription->setDeviceAttribute("UDN", QString("uuid:%1").arg(id()));
    setDescription(deviceDescription);

    setServerName(QString("%1/%2 UPnP/%3 QMS/1.0").arg(QSysInfo::productType()).arg(QSysInfo::productVersion()).arg(version()));
}

void MediaServer::initConnectionManager()
{
    ServiceConnectionManager *connection_manager = new ServiceConnectionManager(this);

    QStringList format;
    format << "http-get:*:image/jpeg:DLNA.ORG_PN=JPEG_SM";
    format << "http-get:*:image/jpeg:DLNA.ORG_PN=JPEG_MED";
    format << "http-get:*:image/jpeg:DLNA.ORG_PN=JPEG_LRG";

    format << "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3";
    format << "http-get:*:audio/L16:DLNA.ORG_PN=LPCM";

    format << "http-get:*:video/mpeg:DLNA.ORG_PN=AVC_TS_HD_24_AC3_ISO;SONY.COM_PN=AVC_TS_HD_24_AC3_ISO";
    format << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_HD_24_AC3;SONY.COM_PN=AVC_TS_HD_24_AC3";
    format << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_HD_24_AC3_T;SONY.COM_PN=AVC_TS_HD_24_AC3_T";
    format << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_PS_PAL";
    format << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_PS_NTSC";
    format << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_50_L2_T";
    format << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_60_L2_T";
    format << "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_TS_SD_EU_ISO";
    format << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_EU";
    format << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_EU_T";
    format << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_50_AC3_T";
    format << "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_TS_HD_50_L2_ISO;SONY.COM_PN=HD2_50_ISO";
    format << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_60_AC3_T";
    format << "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_TS_HD_60_L2_ISO;SONY.COM_PN=HD2_60_ISO";
    format << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_50_L2_T;SONY.COM_PN=HD2_50_T";
    format << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_60_L2_T;SONY.COM_PN=HD2_60_T";
    format << "http-get:*:video/mpeg:DLNA.ORG_PN=AVC_TS_HD_50_AC3_ISO;SONY.COM_PN=AVC_TS_HD_50_AC3_ISO";
    format << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_HD_50_AC3;SONY.COM_PN=AVC_TS_HD_50_AC3";
    format << "http-get:*:video/mpeg:DLNA.ORG_PN=AVC_TS_HD_60_AC3_ISO;SONY.COM_PN=AVC_TS_HD_60_AC3_ISO";
    format << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_HD_60_AC3;SONY.COM_PN=AVC_TS_HD_60_AC3";
    format << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_HD_50_AC3_T;SONY.COM_PN=AVC_TS_HD_50_AC3_T";
    format << "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_HD_60_AC3_T;SONY.COM_PN=AVC_TS_HD_60_AC3_T";
    format << "http-get:*:video/x-mp2t-mphl-188:*";
    format << "http-get:*:*:*,http-get:*:video/*:*";
    format << "http-get:*:audio/*:*";
    format << "http-get:*:image/*:*";
    connection_manager->updateStateVariable("SourceProtocolInfo", format.join(","));

    connection_manager->setDirection(ServiceConnectionManager::Output);

    addService(connection_manager);
}

void MediaServer::initContentDirectory()
{
    MediaServerContent *content_directory = new MediaServerContent(m_renderersModel, this, url().host(), url().port());

    connect(m_renderersModel, SIGNAL(mediaRendererDestroyed(QString)), content_directory, SLOT(mediaRendererDestroyed(QString)));

    connect(content_directory, SIGNAL(scanFolder(QString)), this, SIGNAL(scanFolder(QString)));
    connect(this, SIGNAL(addFolder(QString)), content_directory, SLOT(_addFolder(QString)));
    connect(content_directory, SIGNAL(folderAdded(QString)), this, SIGNAL(folderAdded(QString)));
    connect(content_directory, SIGNAL(error_addFolder(QString)), this, SIGNAL(folderNotAdded(QString)));

    connect(this, SIGNAL(addLink(QString)), content_directory, SIGNAL(addNetworkLinkSignal(QString)));
    connect(content_directory, SIGNAL(linkAdded(QString)), this, SIGNAL(linkAdded(QString)));
    connect(content_directory, SIGNAL(error_addNetworkLink(QString)), this, SIGNAL(linkNotAdded(QString)));

    connect(this, SIGNAL(reloadLibrarySignal()), content_directory, SLOT(reloadLibrary()));
    connect(this, SIGNAL(updateMediaFromId(int,QHash<QString,QVariant>)), content_directory, SIGNAL(updateMediaFromId(int,QHash<QString,QVariant>)));

    connect(content_directory, SIGNAL(servingRendererSignal(QString,QString)), m_renderersModel, SLOT(serving(QString,QString)));
    connect(content_directory, SIGNAL(servingFinishedSignal(QString,QString,int)), this, SIGNAL(servingFinishedSignal(QString,QString,int)));

    connect(this, SIGNAL(refresh(QString)), content_directory, SIGNAL(addNetworkLinkSignal(QString)));

    addService(content_directory);
}

void MediaServer::replyGetIcon(HttpRequest *request)
{
    request->replyFile(":" + request->url().toString());
}
