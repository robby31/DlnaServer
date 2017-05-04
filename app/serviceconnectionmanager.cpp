#include "serviceconnectionmanager.h"

ServiceConnectionManager::ServiceConnectionManager(QObject *parent):
    QObject(parent)
{

}

void ServiceConnectionManager::reply(HttpRequest *request)
{
    if (request->operation() == QNetworkAccessManager::PostOperation && request->url().toString() == "/upnp/control/connection_manager")
    {
        SoapAction action(request->requestData());

        QString soapaction = request->header("SOAPACTION");

        if (!action.isValid() or soapaction != action.soapaction())
        {
            qCritical() << "invalid action" << soapaction << action.soapaction();
            UpnpError error(UpnpError::INVALID_ACTION);
            request->replyError(error);
        }
        else
        {
            if (action.actionName() == "GetProtocolInfo")
            {
                SoapActionResponse response(action.serviceType(), action.actionName());

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

                response.addArgument("Source", format.join(","));
                response.addArgument("Sink", "");
                request->replyAction(response);
            }
            else if (action.actionName() == "GetCurrentConnectionIDs")
            {
                SoapActionResponse response(action.serviceType(), action.actionName());
                response.addArgument("ConnectionIDs", "");
                request->replyAction(response);
            }
//            else if (action.actionName() == "GetCurrentConnectionInfo")
//            {

//            }
            else
            {
                qCritical() << "unknwon action" << action.actionName();
                UpnpError error(UpnpError::INVALID_ACTION);
                request->replyError(error);
            }
        }
    }
    else
    {
        request->setError(QString("unknown request to reply : %1 %2").arg(request->operationString()).arg(request->url().toString()));
        request->close();
    }
}
