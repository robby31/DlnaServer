#include "reply.h"

const QString Reply::CRLF = "\r\n";

const QString Reply::XML_HEADER = "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
const QString Reply::SOAP_ENCODING_HEADER = "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">" + CRLF + "<s:Body>";
const QString Reply::PROTOCOLINFO_RESPONSE =  "<u:GetProtocolInfoResponse xmlns:u=\"urn:schemas-upnp-org:service:ConnectionManager:1\"><Source>http-get:*:image/jpeg:DLNA.ORG_PN=JPEG_SM,http-get:*:image/jpeg:DLNA.ORG_PN=JPEG_MED,http-get:*:image/jpeg:DLNA.ORG_PN=JPEG_LRG,http-get:*:audio/mpeg:DLNA.ORG_PN=MP3,http-get:*:audio/L16:DLNA.ORG_PN=LPCM,http-get:*:video/mpeg:DLNA.ORG_PN=AVC_TS_HD_24_AC3_ISO;SONY.COM_PN=AVC_TS_HD_24_AC3_ISO,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_HD_24_AC3;SONY.COM_PN=AVC_TS_HD_24_AC3,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_HD_24_AC3_T;SONY.COM_PN=AVC_TS_HD_24_AC3_T,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_PS_PAL,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_PS_NTSC,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_50_L2_T,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_60_L2_T,http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_TS_SD_EU_ISO,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_EU,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_EU_T,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_50_AC3_T,http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_TS_HD_50_L2_ISO;SONY.COM_PN=HD2_50_ISO,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_60_AC3_T,http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_TS_HD_60_L2_ISO;SONY.COM_PN=HD2_60_ISO,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_50_L2_T;SONY.COM_PN=HD2_50_T,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_60_L2_T;SONY.COM_PN=HD2_60_T,http-get:*:video/mpeg:DLNA.ORG_PN=AVC_TS_HD_50_AC3_ISO;SONY.COM_PN=AVC_TS_HD_50_AC3_ISO,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_HD_50_AC3;SONY.COM_PN=AVC_TS_HD_50_AC3,http-get:*:video/mpeg:DLNA.ORG_PN=AVC_TS_HD_60_AC3_ISO;SONY.COM_PN=AVC_TS_HD_60_AC3_ISO,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_HD_60_AC3;SONY.COM_PN=AVC_TS_HD_60_AC3,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_HD_50_AC3_T;SONY.COM_PN=AVC_TS_HD_50_AC3_T,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_HD_60_AC3_T;SONY.COM_PN=AVC_TS_HD_60_AC3_T,http-get:*:video/x-mp2t-mphl-188:*,http-get:*:*:*,http-get:*:video/*:*,http-get:*:audio/*:*,http-get:*:image/*:*</Source><Sink></Sink></u:GetProtocolInfoResponse>";
const QString Reply::SOAP_ENCODING_FOOTER = "</s:Body>" + CRLF + "</s:Envelope>";
const QString Reply::SORTCAPS_RESPONSE = "<u:GetSortCapabilitiesResponse xmlns:u=\"urn:schemas-upnp-org:service:ContentDirectory:1\"><SortCaps></SortCaps></u:GetSortCapabilitiesResponse>";
const QString Reply::SEARCHCAPS_RESPONSE = "<u:GetSearchCapabilitiesResponse xmlns:u=\"urn:schemas-upnp-org:service:ContentDirectory:1\"><SearchCaps></SearchCaps></u:GetSearchCapabilitiesResponse>";
const QString Reply::SAMSUNG_ERROR_RESPONSE = "<s:Fault><faultCode>s:Client</faultCode><faultString>UPnPError</faultString><detail><UPnPError xmlns=\"urn:schemas-upnp-org:control-1-0\"><errorCode>401</errorCode><errorDescription>Invalid Action</errorDescription></UPnPError></detail></s:Fault>";
const QString Reply::GETSYSTEMUPDATEID_HEADER = "<u:GetSystemUpdateIDResponse xmlns:u=\"urn:schemas-upnp-org:service:ContentDirectory:1\">";
const QString Reply::GETSYSTEMUPDATEID_FOOTER = "</u:GetSystemUpdateIDResponse>";

const QString Reply::EVENT_Header = "<e:propertyset xmlns:e=\"urn:schemas-upnp-org:event-1-0\" xmlns:s=\"%1\">";
const QString Reply::EVENT_Prop = "<e:property><%1>%2</%1></e:property>";
const QString Reply::EVENT_FOOTER = "</e:propertyset>";


Reply::Reply(Logger *log, const bool &http10, const QString &method, const QString &argument, const QHash<QString, QString> &paramsHeader, const QString &content, HttpRange *range, const int &timeSeekRangeStart, const int &timeSeekRangeEnd, QString uuid, QString servername, QString host, int port, QObject *parent):
    LogObject(log, parent),
    m_requestMethod(method),
    m_requestArgument(argument),
    m_request_params(paramsHeader),
    m_requestContent(content),
    m_requestRange(range),
    m_requestTimeSeekRangeStart(timeSeekRangeStart),
    m_requestTimeSeekRangeEnd(timeSeekRangeEnd),
    m_http10(http10),
    m_uuid(uuid),
    m_servername(servername),
    m_host(host),
    m_port(port),
    m_params(),
    headerSent(false),
    keepReplyOpened(false),
    doc(),
    xml()
{
    connect(this, SIGNAL(runSignal()), this, SLOT(_run()));
    connect(this, SIGNAL(bytesSent(qint64,qint64)), this, SLOT(bytesSentSlot(qint64,qint64)));
}

QString Reply::getParamHeader(const QString &param) const
{
    if (m_params.contains(param))
        return m_params[param];
    return QString();
}

QString Reply::getRequestParamHeader(const QString &param) const
{
    if (m_request_params.contains(param))
        return m_request_params[param];
    return QString();
}

void Reply::setParamHeader(const QString &param, const QString &value)
{
    if (m_params.contains(param))
        logError(QString("Param %1 is defined several times in header: %2 - %3.").arg(param).arg(m_params[param]).arg(value));
    else
        m_params[param] = value;
}

void Reply::sendLine(QTcpSocket *client, const QString &msg)
{
    if (client == 0) {
        logError("Unable to send line (client deleted).");
    } else {
        QByteArray tmp;

        tmp.append(msg.toUtf8()).append(CRLF.toUtf8());

        if (client->write(tmp) == -1) {
            logError("HTTP Request: Unable to send line: " + msg);
        }

        emit appendAnswerSignal(msg+CRLF);

        logDebug("Wrote on socket: " + msg);
    }
}

void Reply::sendHeader()
{
    if (!headerSent)
    {
        headerSent = true;
        emit sendHeaderSignal(m_params);
    }
}

void Reply::sendAnswer(const QByteArray &contentAnswer)
{
    if (!contentAnswer.isNull())
        setParamHeader("Content-Length", QString("%1").arg(contentAnswer.size()));

    // send the header
    sendHeader();

    // HEAD requests only require headers to be set, no need to set contents.
    if (requestMethod() != "HEAD" && !contentAnswer.isNull())
        emit sendDataToClientSignal(contentAnswer);
}

void Reply::_run()
{
    // prepare data and send answer
    QString soapaction = getRequestSoapAction();

    logTrace("ANSWER: " + requestMethod() + " " + requestArgument());

    if ((requestMethod() == "GET" || requestMethod() == "HEAD") && (requestArgument() == "description/fetch" || requestArgument().endsWith("1.0.xml"))) {

        setParamHeader("Content-Type",  "text/xml; charset=\"utf-8\"");
        setParamHeader("Cache-Control", "no-cache");
        setParamHeader("Expires", "0");
        setParamHeader("Accept-Ranges", "bytes");
        if (!isHttp10())
            setParamHeader("Connection", "keep-alive");
        setParamHeader("Server", servername());

        QString answerContent;

        QFile inputStream(":/PMS.xml");
        if (inputStream.open(QFile::ReadOnly | QFile::Text)) {
            // read file PMS.xml
            answerContent = inputStream.readAll();
            inputStream.close();

            // replace parameter by its value
            answerContent.replace(QString("[servername]"), servername());
            answerContent.replace(QString("[uuid]"), QString("uuid:%1").arg(uuid()));
            answerContent.replace(QString("[host]"), host());
            answerContent.replace(QString("[port]"), QString("%1").arg(port()));
        }
        else {
            logError("Unable to read PMS.xml for description/fetch answer.");
        }

        sendAnswer(answerContent.toUtf8());
    }
    else if (requestMethod() == "POST" && requestArgument().endsWith("upnp/control/connection_manager")) {

        if (!soapaction.isEmpty() && soapaction.indexOf("ConnectionManager:1#GetProtocolInfo") > -1) {
            setParamHeader("Content-Type",  "text/xml; charset=\"utf-8\"");
            setParamHeader("Server", servername());

            QString answerContent;
            answerContent.append(XML_HEADER);
            answerContent.append(CRLF);
            answerContent.append(SOAP_ENCODING_HEADER);
            answerContent.append(CRLF);
            answerContent.append(PROTOCOLINFO_RESPONSE);
            answerContent.append(CRLF);
            answerContent.append(SOAP_ENCODING_FOOTER);
            answerContent.append(CRLF);

            sendAnswer(answerContent.toUtf8());
        }

    }
    else if (requestMethod() == "POST" && requestArgument().endsWith("upnp/control/content_directory")) {
        setParamHeader("Content-Type",  "text/xml; charset=\"utf-8\"");
        setParamHeader("Server", servername());

        QString answerContent;
        if (!soapaction.isEmpty() && soapaction.indexOf("ContentDirectory:1#GetSystemUpdateID") > -1) {
            answerContent.append(XML_HEADER);
            answerContent.append(CRLF);
            answerContent.append(SOAP_ENCODING_HEADER);
            answerContent.append(CRLF);
            answerContent.append(GETSYSTEMUPDATEID_HEADER);
            answerContent.append(CRLF);
            answerContent.append("<Id>1</Id>");
            answerContent.append(CRLF);
            answerContent.append(GETSYSTEMUPDATEID_FOOTER);
            answerContent.append(CRLF);
            answerContent.append(SOAP_ENCODING_FOOTER);
            answerContent.append(CRLF);

            sendAnswer(answerContent.toUtf8());

        } else if (!soapaction.isEmpty() && soapaction.indexOf("ContentDirectory:1#GetSortCapabilities") > -1) {
            answerContent.append(XML_HEADER);
            answerContent.append(CRLF);
            answerContent.append(SOAP_ENCODING_HEADER);
            answerContent.append(CRLF);
            answerContent.append(SORTCAPS_RESPONSE);
            answerContent.append(CRLF);
            answerContent.append(SOAP_ENCODING_FOOTER);
            answerContent.append(CRLF);

            sendAnswer(answerContent.toUtf8());

        } else if (!soapaction.isEmpty() && soapaction.indexOf("ContentDirectory:1#X_GetFeatureList") > -1) {
            // Added for Samsung 2012 TVs
            answerContent.append(XML_HEADER);
            answerContent.append(CRLF);
            answerContent.append(SOAP_ENCODING_HEADER);
            answerContent.append(CRLF);
            answerContent.append(SAMSUNG_ERROR_RESPONSE);
            answerContent.append(CRLF);
            answerContent.append(SOAP_ENCODING_FOOTER);
            answerContent.append(CRLF);

            sendAnswer(answerContent.toUtf8());

        } else if (!soapaction.isEmpty()  && soapaction.indexOf("ContentDirectory:1#GetSearchCapabilities") > -1) {
            answerContent.append(XML_HEADER);
            answerContent.append(CRLF);
            answerContent.append(SOAP_ENCODING_HEADER);
            answerContent.append(CRLF);
            answerContent.append(SEARCHCAPS_RESPONSE);
            answerContent.append(CRLF);
            answerContent.append(SOAP_ENCODING_FOOTER);
            answerContent.append(CRLF);

            sendAnswer(answerContent.toUtf8());

        } else if (!soapaction.isEmpty() && (soapaction.contains("ContentDirectory:1#Browse") || soapaction.contains("ContentDirectory:1#Search"))) {

            // read the content of the received request
            doc.setContent(requestTextContent());

            QString objectID;
            QDomNodeList list = doc.elementsByTagName("ObjectID");
            if (list.length() > 0) {
                objectID = list.at(0).toElement().text();
            }
            else {
                objectID = QString();
            }

            QString containerID;
            if (objectID.isNull() || objectID.isEmpty()) {
                QDomNodeList list = doc.elementsByTagName("ContainerID");
                if (list.length() > 0) {
                    containerID = list.at(0).toElement().text();
                }
                else {
                    containerID = QString();
                }

                if (containerID.isNull() || !containerID.contains("$")) {
                    objectID = "0";
                } else {
                    objectID = containerID;
                    containerID = QString();
                }
            }

            QString sI;
            int startingIndex = 0;
            list = doc.elementsByTagName("StartingIndex");
            if (list.length() > 0) {
                sI = list.at(0).toElement().text();
            }
            if (!sI.isEmpty()) {
                startingIndex = sI.toInt();
            }

            QString rC;
            int requestCount = 0;
            list = doc.elementsByTagName("RequestedCount");
            if (list.length() > 0) {
                rC = list.at(0).toElement().text();
            }
            if (!rC.isEmpty()) {
                requestCount = rC.toInt();
            }

            QString browseFlag;
            list = doc.elementsByTagName("BrowseFlag");
            if (list.length() > 0) {
                browseFlag = list.at(0).toElement().text();
            }

            if (!soapaction.isNull() && soapaction.contains("ContentDirectory:1#Search")) {
                browseFlag = "BrowseDirectChildren";
            }

            QString searchCriteria;
            if (soapaction.contains("ContentDirectory:1#Search")) {
                list = doc.elementsByTagName("SearchCriteria");
                if (list.length() > 0) {
                    searchCriteria = list.at(0).toElement().text();
                }
            }

            emit getDLNAResourcesSignal(objectID,
                                        !browseFlag.isNull() && browseFlag == "BrowseDirectChildren",
                                        startingIndex,
                                        requestCount,
                                        searchCriteria);
        } else {
            replyDone("KO");
        }
    } else if (requestMethod() == "SUBSCRIBE") {
        setParamHeader("Content-Type",  "text/xml; charset=\"utf-8\"");
        setParamHeader("Connection",  "close");
        setParamHeader("SID", QString("uuid:%1").arg(uuid()));
        setParamHeader("Server", servername());
        setParamHeader("Timeout", "Second-1800");

        if (!soapaction.isEmpty())
        {
            QString cb = soapaction.replace("<", "").replace(">", "");

            QUrl soapActionUrl = QUrl(cb);
            QString addr = soapActionUrl.host();
            int port = soapActionUrl.port();

            QTcpSocket sock;
            sock.connectToHost(addr, port);
            if (sock.waitForConnected()) {
                sendLine(&sock, "NOTIFY /" + requestArgument() + " HTTP/1.1");
                sendLine(&sock, QString("SID: uuid:%1").arg(uuid()));
                sendLine(&sock, "SEQ: 0");
                sendLine(&sock, "NT: upnp:event");
                sendLine(&sock, "NTS: upnp:propchange");
                sendLine(&sock, QString("HOST: %1:%2").arg(addr).arg(port));
                sendLine(&sock, "Content-Type: text/xml; charset=\"utf-8\"");
            }
            else {
                logError(QString("Cannot connect to %1").arg(cb));
                replyDone("ERROR");
                return;
            }

            sock.flush();
            sock.close();
        }

        QString answerContent;

        if (requestArgument().contains("connection_manager"))
        {
            answerContent.append(EVENT_Header.arg("urn:schemas-upnp-org:service:ConnectionManager:1"));
            answerContent.append(EVENT_Prop.arg("SinkProtocolInfo").arg(""));
            answerContent.append(EVENT_Prop.arg("SourceProtocolInfo").arg(""));
            answerContent.append(EVENT_Prop.arg("CurrentConnectionIDs").arg(""));
            answerContent.append(EVENT_FOOTER);

            sendAnswer(answerContent.toUtf8());
        }
        else if (requestArgument().contains("content_directory"))
        {
            answerContent.append(EVENT_Header.arg("urn:schemas-upnp-org:service:ContentDirectory:1"));
            answerContent.append(EVENT_Prop.arg("TransferIDs").arg(""));
            answerContent.append(EVENT_Prop.arg("ContainerUpdateIDs").arg(""));
            answerContent.append(EVENT_Prop.arg("SystemUpdateID").arg(1));
            answerContent.append(EVENT_FOOTER);

            sendAnswer(answerContent.toUtf8());
        }
        else
        {
            replyDone("Unknown argument");
        }

    }
    else if ((requestMethod() == "GET" || requestMethod() == "HEAD") && (requestArgument().toLower().endsWith(".png") || requestArgument().toLower().endsWith(".jpg") || requestArgument().toLower().endsWith(".jpeg")))
    {
        if (requestArgument().toLower().endsWith(".png")) {
            setParamHeader("Content-Type", "image/png");
        } else {
            setParamHeader("Content-Type", "image/jpeg");
        }

        setParamHeader("Accept-Ranges", "bytes");
        if (!isHttp10())
            setParamHeader("Connection", "keep-alive");
//        setParamHeader("Expires:", getFUTUREDATE() + " GMT");
        setParamHeader("Server", servername());

        QByteArray answerContent;

        // read the image file
        QFile inputStream(":/" + requestArgument());
        if (inputStream.open(QFile::ReadOnly)) {
            answerContent = inputStream.readAll();
            inputStream.close();

        }
        else {
            logError(QString("Unable to read %1 for %2 answer.").arg(requestArgument()).arg(requestMethod()));
        }

        sendAnswer(answerContent);

    }
    else {
        logError("Unkown answer for: " + requestMethod() + " " + requestArgument());
        replyDone("KO");
    }
}

void Reply::replyDone(const QString &status)
{
    emit replyStatusSignal(status);
    emit closeClientSignal();
    emit finishedSignal();
}

void Reply::dlnaResources(QObject *requestor, QList<DlnaResource *> resources)
{
    if (requestor != this)
        return;  // ignore resources

    QString soapaction = getRequestSoapAction();

    // prepare the answer to send
    QStringList filter;
    QDomNodeList list = doc.elementsByTagName("Filter");
    if (list.length() > 0) {
        filter = list.at(0).toElement().text().split(",");
    }

    QString browseFlag;
    list = doc.elementsByTagName("BrowseFlag");
    if (list.length() > 0) {
        browseFlag = list.at(0).toElement().text();
    }

    if (!soapaction.isNull() && soapaction.contains("ContentDirectory:1#Search")) {
        browseFlag = "BrowseDirectChildren";
    }

    // set the header
    xml.appendChild(xml.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\""));

    // set envelope and body
    QDomElement envelope = xml.createElementNS("http://schemas.xmlsoap.org/soap/envelope/", "s:Envelope");
    envelope.setAttribute("s:encodingStyle", "http://schemas.xmlsoap.org/soap/encoding/");
    QDomElement body = xml.createElement("s:Body");
    envelope.appendChild(body);
    xml.appendChild(envelope);

    QDomElement browse_search;
    if (!soapaction.isNull() && soapaction.contains("ContentDirectory:1#Search")) {
        browse_search = xml.createElementNS("urn:schemas-upnp-org:service:ContentDirectory:1", "u:SearchResponse");
    } else {
        browse_search = xml.createElementNS("urn:schemas-upnp-org:service:ContentDirectory:1", "u:BrowseResponse");
        body.appendChild(browse_search);
    }

    // set results
    QDomElement result = xml.createElement("Result");
    browse_search.appendChild(result);

    // set DIDL header
    QDomDocument didlDoc;
    QDomElement didlLite = didlDoc.createElementNS("urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/", "DIDL-Lite");
    didlLite.setAttribute("xmlns:dc", "http://purl.org/dc/elements/1.1/");
    didlLite.setAttribute("xmlns:upnp", "urn:schemas-upnp-org:metadata-1-0/upnp/");
    didlDoc.appendChild(didlLite);

    /*if (searchCriteria != null && files != null) {
        searchCriteria = searchCriteria.toLowerCase();

        for (int i = files.size() - 1; i >= 0; i--) {
            DLNAResource res = files.get(i);

            if (res.isSearched()) {
                continue;
            }

            boolean keep = res.getName().toLowerCase().indexOf(searchCriteria) != -1;
            final DLNAMediaInfo media = res.getMedia();

            if (media!=null) {
                for (int j = 0;j < media.getAudioTracksList().size(); j++) {
                    DLNAMediaAudio audio = media.getAudioTracksList().get(j);
                    keep |= audio.getAlbum().toLowerCase().indexOf(searchCriteria) != -1;
                    keep |= audio.getArtist().toLowerCase().indexOf(searchCriteria) != -1;
                    keep |= audio.getSongname().toLowerCase().indexOf(searchCriteria) != -1;
                }
            }

            if (!keep) { // dump it
                files.remove(i);
            }
        }
    }*/

    int minus = 0;
    if (!resources.isEmpty()) {
        foreach (DlnaResource* uf, resources) {
            //if (uf.isCompatible(mediaRenderer) && (uf.getPlayer() == null || uf.getPlayer().isPlayerCompatible(mediaRenderer))) {
            didlLite.appendChild(uf->getXmlContentDirectory(&didlDoc, filter));
            //} else {
            //    minus++;
            //}
        }
    }

    // add DIDL string
    result.appendChild(xml.createTextNode(didlDoc.toString(-1)));

    int filessize = resources.size();

    // set number of returned results
    QDomElement nbReturned = xml.createElement("NumberReturned");
    nbReturned.appendChild(xml.createTextNode(QString("%1").arg(filessize - minus)));
    browse_search.appendChild(nbReturned);

    DlnaResource *parentFolder = 0;
    if (filessize > 0) {
        parentFolder = resources.at(0)->getDlnaParent();
    }

    // set number of total results
    QDomElement totalMatches = xml.createElement("TotalMatches");
    browse_search.appendChild(totalMatches);
    if (!browseFlag.isNull() && browseFlag == "BrowseDirectChildren") {
        if (parentFolder != 0) {
            totalMatches.appendChild(xml.createTextNode(QString("%1").arg(parentFolder->getChildrenSize() - minus)));
        }
        else {
            totalMatches.appendChild(xml.createTextNode(QString("%1").arg(filessize - minus)));
        }
    } else {
        // From upnp spec: If BrowseMetadata is specified in the BrowseFlags then TotalMatches = 1
        totalMatches.appendChild(xml.createTextNode("1"));
    }

    // set update ID
    QDomElement updateID = xml.createElement("UpdateID");
    browse_search.appendChild(updateID);
    if (parentFolder != 0) {
        updateID.appendChild(xml.createTextNode(QString("%1").arg(parentFolder->getUpdateId())));
    } else {
        updateID.appendChild(xml.createTextNode("1"));
    }

    //send the answer to client
    sendAnswer(xml.toString(-1).toUtf8());
}

void Reply::bytesSentSlot(const qint64 &size, const qint64 &towrite)
{
    Q_UNUSED(size)

    if (!keepReplyOpened && towrite == 0)
    {
        replyDone("OK");
    }
}
