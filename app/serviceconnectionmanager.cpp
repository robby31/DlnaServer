#include "serviceconnectionmanager.h"

ServiceConnectionManager::ServiceConnectionManager(UpnpControlPoint *upnp, QNetworkAccessManager *nam, QObject *parent):
    QObject(parent),
    netManager(nam),
    m_upnp(upnp)
{

}

QString ServiceConnectionManager::getConnectionIDs()
{
    return QString();
}

QStringList ServiceConnectionManager::getSinkProtocolInfo()
{
    return QStringList();
}

QStringList ServiceConnectionManager::getSourceProtocolInfo()
{
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

    return format;
}

void ServiceConnectionManager::reply(HttpRequest *request)
{
    if (request->operation() == QNetworkAccessManager::PostOperation && request->url().toString() == "/upnp/control/connection_manager")
    {
        SoapAction action(request->requestData());

        QString soapaction = request->header("SOAPACTION");

        if (!action.isValid() || soapaction != action.soapaction())
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

                response.addArgument("Source", getSourceProtocolInfo().join(","));
                response.addArgument("Sink", getSinkProtocolInfo().join(","));
                request->replyAction(response);
            }
            else if (action.actionName() == "GetCurrentConnectionIDs")
            {
                SoapActionResponse response(action.serviceType(), action.actionName());
                response.addArgument("ConnectionIDs", getConnectionIDs());
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
    else if (request->operationString() == "SUBSCRIBE" && request->url().toString() == "/upnp/event/connection_manager" && request->header("NT") == "upnp:event")
    {
        QStringList l_url;
        QRegularExpression callback("<(.+)>");
        QRegularExpressionMatchIterator i = callback.globalMatch(request->header("CALLBACK"));
        while (i.hasNext())
        {
            QRegularExpressionMatch match = i.next();
            if (match.hasMatch())
                l_url << match.captured(1);
        }

        QStringList header;

        QDateTime sdf;
        header << QString("DATE: %1").arg(sdf.currentDateTime().toString("ddd, dd MMM yyyy hh:mm:ss") + " GMT");
        if (!request->serverName().isEmpty())
            header << QString("SERVER: %1").arg(request->serverName());

        QString uuid = m_upnp->generateUuid();
        header << QString("SID: uuid:%1").arg(uuid);
        header << QString("Content-Length: 0");
        header << QString("TIMEOUT: Second-1800");

        if (!request->sendHeader(header))
            request->setError(QString("unable to send header to client"));
        else
            m_subscription[uuid] = l_url;

        sendEvent(uuid);
    }
    else if (request->operationString() == "SUBSCRIBE" && request->url().toString() == "/upnp/event/connection_manager" && !request->header("SID").isEmpty())
    {
        QRegularExpression pattern("uuid:\\s*(.+)");
        QRegularExpressionMatch match = pattern.match(request->header("SID"));
        if (match.hasMatch())
        {
            QString sid = match.captured(1);
            if (m_subscription.contains(sid))
            {
                qDebug() << "RENEW SUBSCRIPTION" << sid << m_subscription[sid];

                QStringList header;

                QDateTime sdf;
                header << QString("DATE: %1").arg(sdf.currentDateTime().toString("ddd, dd MMM yyyy hh:mm:ss") + " GMT");
                if (!request->serverName().isEmpty())
                    header << QString("SERVER: %1").arg(request->serverName());

                header << QString("SID: uuid:%1").arg(sid);
                header << QString("Content-Length: 0");
                header << QString("TIMEOUT: Second-1800");

                if (!request->sendHeader(header))
                    request->setError(QString("RENEW: unable to send header to client"));
            }
            else
            {
                qCritical() << "Invalid subscription renewal" << sid;
            }
        }
        else
        {
            qCritical() << "invalid SID" << request->header("SID") << "in SUBSCRIPTION renewal";
        }
    }
    else
    {
        request->setError(QString("unknown request to reply : %1 %2").arg(request->operationString()).arg(request->url().toString()));
        qCritical() << request->data(HttpRequest::headerRole);
        request->close();
    }
}

void ServiceConnectionManager::sendEvent(const QString &uuid)
{
    if (m_subscription[uuid].size() > 0)
    {
        QNetworkRequest request(QUrl(m_subscription[uuid].at(0)));
        request.setRawHeader("HOST", QString("%1:%2").arg(request.url().host()).arg(request.url().port(80)).toUtf8());
        request.setRawHeader("CONTENT-TYPE", "text/xml; charset=\"utf-8\"");
        request.setRawHeader("NT", "upnp:event");
        request.setRawHeader("NTS", "upnp:propchange");
        request.setRawHeader("SID", QString("uuid:%1").arg(uuid).toUtf8());
        request.setRawHeader("SEQ", "event key");

        QByteArray data;
        data.append("<?xml version=\"1.0\"?>");
        data.append("<e:propertyset xmlns:e=\"urn:schemas-upnp-org:event-1-0\">");
        data.append(QString("<e:property> <%1>%2</%1> </e:property>").arg("SourceProtocolInfo").arg(getSourceProtocolInfo().join(",")));
        data.append(QString("<e:property> <%1>%2</%1> </e:property>").arg("SinkProtocolInfo").arg(getSinkProtocolInfo().join(",")));
        data.append(QString("<e:property> <%1>%2</%1> </e:property>").arg("CurrentConnectionIDs").arg(getConnectionIDs()));
        data.append("</e:propertyset>");

        QNetworkReply *reply = netManager->sendCustomRequest(request, "NOTIFY", data);
        connect(reply, SIGNAL(finished()), this, SLOT(sendEventReply()));
    }


}

void ServiceConnectionManager::sendEventReply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (reply)
    {
        qWarning() << "send event reply" << reply << "from" << reply->request().url();
        qWarning() << "answer data" << reply->rawHeaderList() << reply->readAll();
        reply->deleteLater();
    }

}
