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


Reply::Reply(Logger *log, Request *request, DlnaRootFolder *rootFolder, QObject *parent):
    LogObject(log, parent),
    m_request(request),
    m_params(),
    headerSent(false),
    m_rootFolder(rootFolder)
{
    connect(this, SIGNAL(runSignal(QString,QString)), this, SLOT(_run(QString,QString)));

    connect(m_request, SIGNAL(destroyed()), this, SLOT(requestDestroyed()));
    connect(m_rootFolder, SIGNAL(destroyed()), this, SLOT(rootFolderDestroyed()));
}

QString Reply::getParamHeader(const QString &param) const
{
    if (m_params.contains(param))
        return m_params[param];
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
    if (m_request && m_request->getMethod() != "HEAD" && !contentAnswer.isNull())
        emit sendDataToClientSignal(contentAnswer);
}

void Reply::_run(const QString &method, const QString &argument) {
    if (!m_request)
        return;

    // prepare data and send answer
    QString soapaction = m_request->getSoapaction();

    logTrace("ANSWER: " + method + " " + argument);

    if ((method == "GET" || method == "HEAD") && (argument == "description/fetch" || argument.endsWith("1.0.xml"))) {

        setParamHeader("Content-Type",  "text/xml; charset=\"utf-8\"");
        setParamHeader("Cache-Control", "no-cache");
        setParamHeader("Expires", "0");
        setParamHeader("Accept-Ranges", "bytes");
        if (!m_request->isHttp10())
            setParamHeader("Connection", "keep-alive");
        setParamHeader("Server", m_request->getServername());

        QString answerContent;

        QFile inputStream(":/PMS.xml");
        if (inputStream.open(QFile::ReadOnly | QFile::Text)) {
            // read file PMS.xml
            answerContent = inputStream.readAll();
            inputStream.close();

            // replace parameter by its value
            answerContent.replace(QString("[servername]"), m_request->getServername());
            answerContent.replace(QString("[uuid]"), QString("uuid:%1").arg(m_request->getUuid()));
            answerContent.replace(QString("[host]"), m_request->getHost());
            answerContent.replace(QString("[port]"), QString("%1").arg(m_request->getPort()));
        }
        else {
            logError("Unable to read PMS.xml for description/fetch answer.");
        }

        sendAnswer(answerContent.toUtf8());

        emit replyStatusSignal("OK");
    }
    else if (method == "POST" && argument.endsWith("upnp/control/connection_manager")) {

        if (!soapaction.isEmpty() && soapaction.indexOf("ConnectionManager:1#GetProtocolInfo") > -1) {
            setParamHeader("Content-Type",  "text/xml; charset=\"utf-8\"");
            setParamHeader("Server", m_request->getServername());

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

            emit replyStatusSignal("OK");
        }

    }
    else if (method == "POST" && argument.endsWith("upnp/control/content_directory")) {
        setParamHeader("Content-Type",  "text/xml; charset=\"utf-8\"");
        setParamHeader("Server", m_request->getServername());

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

            emit replyStatusSignal("OK");

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

            emit replyStatusSignal("OK");

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

            emit replyStatusSignal("OK");

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

            emit replyStatusSignal("OK");

        } else if (m_rootFolder && !soapaction.isEmpty() && (soapaction.contains("ContentDirectory:1#Browse") || soapaction.contains("ContentDirectory:1#Search"))) {

            // read the content of the received request
            QDomDocument doc;
            doc.setContent(m_request->getTextContent());

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

            QStringList filter;
            list = doc.elementsByTagName("Filter");
            if (list.length() > 0) {
                filter = list.at(0).toElement().text().split(",");
            }

            // prepare the answer to send
            QDomDocument xml;

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

            QObject context_object;
            QList<DlnaResource*> files = m_rootFolder->getDLNAResources(objectID,
                                                                        !browseFlag.isNull() && browseFlag == "BrowseDirectChildren",
                                                                        startingIndex,
                                                                        requestCount,
                                                                        searchCriteria,
                                                                        &context_object);

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
            if (!files.isEmpty()) {
                foreach (DlnaResource* uf, files) {
                    //if (uf.isCompatible(mediaRenderer) && (uf.getPlayer() == null || uf.getPlayer().isPlayerCompatible(mediaRenderer))) {
                    didlLite.appendChild(uf->getXmlContentDirectory(&didlDoc, filter));
                    //} else {
                    //    minus++;
                    //}
                }
            }

            // add DIDL string
            result.appendChild(xml.createTextNode(didlDoc.toString(-1)));

            int filessize = files.size();

            // set number of returned results
            QDomElement nbReturned = xml.createElement("NumberReturned");
            nbReturned.appendChild(xml.createTextNode(QString("%1").arg(filessize - minus)));
            browse_search.appendChild(nbReturned);

            DlnaResource *parentFolder = 0;
            if (filessize > 0) {
                parentFolder = files.at(0)->getDlnaParent();
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

            emit replyStatusSignal("OK");
        } else {
            emit replyStatusSignal("KO");
        }
    } else if (method == "SUBSCRIBE" && !soapaction.isEmpty()) {
        setParamHeader("Content-Type",  "text/xml; charset=\"utf-8\"");
        setParamHeader("Content-Length", "0");
        setParamHeader("Connection",  "close");
        setParamHeader("SID", QString("uuid:%1").arg(m_request->getUuid()));
        setParamHeader("Server", m_request->getServername());
        setParamHeader("Timeout", "Second-1800");

        sendAnswer();

        QString cb = soapaction.replace("<", "").replace(">", "");

        QUrl soapActionUrl = QUrl(cb);
        QString addr = soapActionUrl.host();
        int port = soapActionUrl.port();

        QTcpSocket sock;
        sock.connectToHost(addr, port);
        if (sock.waitForConnected()) {
            sendLine(&sock, "NOTIFY /" + argument + " HTTP/1.1");
            sendLine(&sock, QString("SID: uuid:%1").arg(m_request->getUuid()));
            sendLine(&sock, "SEQ: 0");
            sendLine(&sock, "NT: upnp:event");
            sendLine(&sock, "NTS: upnp:propchange");
            sendLine(&sock, QString("HOST: %1:%2").arg(addr).arg(port));
            sendLine(&sock, "Content-Type: text/xml; charset=\"utf-8\"");
        }
        else {
            logError(QString("Cannot connect to %1").arg(cb));
            emit replyStatusSignal("ERROR");
            return;
        }

        sock.close();

        QString answerContent;
        m_params.clear();
        headerSent = false;

        if (argument.contains("connection_manager")) {
            setParamHeader("Server", m_request->getServername());

            answerContent.append(EVENT_Header.arg("urn:schemas-upnp-org:service:ConnectionManager:1"));
            answerContent.append(EVENT_Prop.arg("SinkProtocolInfo").arg(""));
            answerContent.append(EVENT_Prop.arg("SourceProtocolInfo").arg(""));
            answerContent.append(EVENT_Prop.arg("CurrentConnectionIDs").arg(""));
            answerContent.append(EVENT_FOOTER);

            sendAnswer(answerContent.toUtf8());

            emit replyStatusSignal("OK");

        } else if (argument.contains("content_directory")) {
            setParamHeader("Server", m_request->getServername());

            answerContent.append(EVENT_Header.arg("urn:schemas-upnp-org:service:ContentDirectory:1"));
            answerContent.append(EVENT_Prop.arg("TransferIDs").arg(""));
            answerContent.append(EVENT_Prop.arg("ContainerUpdateIDs").arg(""));
            answerContent.append(EVENT_Prop.arg("SystemUpdateID").arg(1));
            answerContent.append(EVENT_FOOTER);

            sendAnswer(answerContent.toUtf8());

            emit replyStatusSignal("OK");
        } else {
            emit replyStatusSignal("Unknown argument");
        }
    }
    else if ((method == "GET" || method == "HEAD") && (argument.toLower().endsWith(".png") || argument.toLower().endsWith(".jpg") || argument.toLower().endsWith(".jpeg"))) {

        if (argument.toLower().endsWith(".png")) {
            setParamHeader("Content-Type", "image/png");
        } else {
            setParamHeader("Content-Type", "image/jpeg");
        }

        setParamHeader("Accept-Ranges", "bytes");
        if (!m_request->isHttp10())
            setParamHeader("Connection", "keep-alive");
//        setParamHeader("Expires:", getFUTUREDATE() + " GMT");
        setParamHeader("Server", m_request->getServername());

        QByteArray answerContent;

        // read the image file
        QFile inputStream(":/" + argument);
        if (inputStream.open(QFile::ReadOnly)) {
            answerContent = inputStream.readAll();
            inputStream.close();

        }
        else {
            logError(QString("Unable to read %1 for %2 answer.").arg(argument).arg(method));
        }

        sendAnswer(answerContent);

        emit replyStatusSignal("OK");
    }
    else {
        logError("Unkown answer for: " + method + " " + argument);
        emit replyStatusSignal("KO");
    }

    emit closeClientSignal();
    emit finishedSignal();
}
