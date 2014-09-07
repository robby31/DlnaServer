#include "reply.h"

const QString Reply::CRLF = "\r\n";

const QString Reply::CONTENT_TYPE_UTF8 = "CONTENT-TYPE: text/xml; charset=\"utf-8\"";
const QString Reply::CONTENT_TYPE = "Content-Type: text/xml; charset=\"utf-8\"";

const QString Reply::HTTP_200_OK = "HTTP/1.1 200 OK";
const QString Reply::HTTP_500 = "HTTP/1.1 500 Internal Server Error";
const QString Reply::HTTP_206_OK = "HTTP/1.1 206 Partial Content";
const QString Reply::HTTP_200_OK_10 = "HTTP/1.0 200 OK";
const QString Reply::HTTP_206_OK_10 = "HTTP/1.0 206 Partial Content";

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

// call updateStatus function every second
const int Reply::UPDATE_STATUS_PERIOD = 1000;


Reply::Reply(Logger* log, Request* request, DlnaRootFolder *rootFolder, MediaRendererModel *renderersModel, QObject *parent):
    QObject(parent),
    m_log(log),
    m_request(request),
    client(request->getClient()),
    keepSocketOpened(false),
    timerStatus(this),
    networkBytesSent(0),
    lastNetBytesSent(-1),
    m_rootFolder(rootFolder),
    m_renderersModel(renderersModel),
    streamContent(0),
    transcodeProcess(0),
    maxBufferSize(1024*1024*100)  // 100 MBytes
{
    connect(request->getClient(), SIGNAL(bytesWritten(qint64)), this, SLOT(bytesSent(qint64)));
    connect(request->getClient(), SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));

    connect(&timerStatus, SIGNAL(timeout()), this, SLOT(updateStatus()));

    clockUpdateStatus.invalidate();
}

Reply::~Reply() {
    if (transcodeProcess != 0)
        delete transcodeProcess;

    if (streamContent != 0)
        delete streamContent;

    close();
}

void Reply::sendLine(QTcpSocket *client, const QString &msg)
{
    if (client == 0) {
        m_log->Error("Unable to send line (client deleted).");
    } else {
        QByteArray tmp;

        tmp.append(msg.toUtf8()).append(CRLF.toUtf8());

        if (client->write(tmp) == -1) {
            m_log->Error("HTTP Request: Unable to send line: " + msg);
        }

        m_request->appendAnswer(msg+CRLF);
        m_log->Debug("Wrote on socket: " + msg);
    }
}

void Reply::sendAnswer(const QStringList &headerAnswer, const QByteArray &contentAnswer, const long &totalSize)
{
    if (client == 0) {
        m_log->Error("Unable to send answer (client deleted).");
    } else {
        HttpRange *range = m_request->getRange();
        if (range != 0)
        {
            // partial content requested (range is present in the header)
            sendLine(client, m_request->isHttp10() ? HTTP_206_OK_10 : HTTP_206_OK);

            if (totalSize != -1) {
                sendLine(client, QString("Content-Range: bytes %1-%2/%3").arg(range->getStartByte()).arg(range->getEndByte()).arg(totalSize));
            }

        } else {
            if (m_request->getSoapaction().contains("X_GetFeatureList")) {
                //  If we don't return a 500 error, Samsung 2012 TVs time out.
                sendLine(client, HTTP_500);
            } else {
                sendLine(client, m_request->isHttp10() ? HTTP_200_OK_10 : HTTP_200_OK);
            }

            if (totalSize != -1) {
                sendLine(client, "Content-Length: " + QString("%1").arg(totalSize));
            } else if (!contentAnswer.isNull()) {
                sendLine(client, "Content-Length: " + QString("%1").arg(contentAnswer.size()));
            }
        }

        // send the header
        foreach (QString line, headerAnswer) {
            sendLine(client, line.toUtf8());
        }

        sendLine(client, "");

        // HEAD requests only require headers to be set, no need to set contents.
        if (m_request->getMethod() != "HEAD" && !contentAnswer.isNull()) {
            // send the content
            m_request->appendAnswer(QString("content size: %1%2").arg(contentAnswer.size()).arg(CRLF));
            m_request->appendAnswer(contentAnswer);

            if (client->write(contentAnswer) == -1) {

                m_log->Error("HTTP request: Unable to send content.");

            } else {
                m_log->Debug(QString("Send content (%1 bytes).").arg(contentAnswer.size()));
                m_log->Trace("Wrote on socket content: " + contentAnswer);
            }
        }

        closeClient();
    }
}


void Reply::run() {
    // prepare data and send answer
    QString soapaction = m_request->getSoapaction();
    HttpRange *range = m_request->getRange();

    m_log->Trace("ANSWER: " + m_request->getMethod() + " " + m_request->getArgument());

    if ((m_request->getMethod() == "GET" || m_request->getMethod() == "HEAD") && (m_request->getArgument() == "description/fetch" || m_request->getArgument().endsWith("1.0.xml"))) {

        QStringList answerHeader;

        answerHeader << CONTENT_TYPE;
        answerHeader << "Cache-Control: no-cache";
        answerHeader << "Expires: 0";
        answerHeader << "Accept-Ranges: bytes";
        answerHeader << "Connection: keep-alive";
        answerHeader << "Server: " + m_request->getServername();

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
            m_log->Error("Unable to read PMS.xml for description/fetch answer.");
        }

        sendAnswer(answerHeader, answerContent.toUtf8());

        m_request->setStatus("OK");
    }
    else if (m_request->getMethod() == "POST" && m_request->getArgument().endsWith("upnp/control/connection_manager")) {

        if (!soapaction.isEmpty() && soapaction.indexOf("ConnectionManager:1#GetProtocolInfo") > -1) {
            QStringList answerHeader;
            answerHeader << CONTENT_TYPE_UTF8;
            answerHeader << "Server: " + m_request->getServername();

            QString answerContent;
            answerContent.append(XML_HEADER);
            answerContent.append(CRLF);
            answerContent.append(SOAP_ENCODING_HEADER);
            answerContent.append(CRLF);
            answerContent.append(PROTOCOLINFO_RESPONSE);
            answerContent.append(CRLF);
            answerContent.append(SOAP_ENCODING_FOOTER);
            answerContent.append(CRLF);

            sendAnswer(answerHeader, answerContent.toUtf8());

            m_request->setStatus("OK");
        }

    }
    else if (m_request->getMethod() == "POST" && m_request->getArgument().endsWith("upnp/control/content_directory")) {
        QStringList answerHeader;
        answerHeader << CONTENT_TYPE_UTF8;
        answerHeader << "Server: " + m_request->getServername();

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

            sendAnswer(answerHeader, answerContent.toUtf8());

            m_request->setStatus("OK");

        } else if (!soapaction.isEmpty() && soapaction.indexOf("ContentDirectory:1#GetSortCapabilities") > -1) {
            answerContent.append(XML_HEADER);
            answerContent.append(CRLF);
            answerContent.append(SOAP_ENCODING_HEADER);
            answerContent.append(CRLF);
            answerContent.append(SORTCAPS_RESPONSE);
            answerContent.append(CRLF);
            answerContent.append(SOAP_ENCODING_FOOTER);
            answerContent.append(CRLF);

            sendAnswer(answerHeader, answerContent.toUtf8());

            m_request->setStatus("OK");

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

            sendAnswer(answerHeader, answerContent.toUtf8());

            m_request->setStatus("OK");

        } else if (!soapaction.isEmpty()  && soapaction.indexOf("ContentDirectory:1#GetSearchCapabilities") > -1) {
            answerContent.append(XML_HEADER);
            answerContent.append(CRLF);
            answerContent.append(SOAP_ENCODING_HEADER);
            answerContent.append(CRLF);
            answerContent.append(SEARCHCAPS_RESPONSE);
            answerContent.append(CRLF);
            answerContent.append(SOAP_ENCODING_FOOTER);
            answerContent.append(CRLF);

            sendAnswer(answerHeader, answerContent.toUtf8());

            m_request->setStatus("OK");

        } else if (!soapaction.isEmpty() && (soapaction.contains("ContentDirectory:1#Browse") || soapaction.contains("ContentDirectory:1#Search"))) {

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
            sendAnswer(answerHeader, xml.toString(-1).toUtf8());

            m_request->setStatus("OK");
        } else {
            m_request->setStatus("KO");
        }
    }
    else if ((m_request->getMethod() == "GET" || m_request->getMethod() == "HEAD") && m_request->getArgument().startsWith("get/")) {
        // Request to retrieve a file
        QStringList answerHeader;

        /**
        * Skip the leading "get/" and extract the resource ID from the first path element
        * e.g. "get/0$1$5$3$4/Foo.mp4" -> "0$1$5$3$4"
        */
        QRegExp rxId("get/([\\d$]+)/(.+)");
        QString id;
        QString fileName;
        if (rxId.indexIn(m_request->getArgument()) != -1) {
            id = rxId.capturedTexts().at(1);
            fileName = rxId.capturedTexts().at(2);
        }

        // Some clients escape the separators in their request: unescape them.
        id = id.replace("%24", "$");

        // Retrieve the DLNAresource itself.
        QObject context_object;
        QList<DlnaResource*> files = m_rootFolder->getDLNAResources(id, false, 0, 0, "", &context_object);

        if (!m_request->getTransferMode().isNull()) {
            answerHeader << "TransferMode.DLNA.ORG: " + m_request->getTransferMode();
            headerAnswerToSend << "TransferMode.DLNA.ORG: " + m_request->getTransferMode();
        }

        if (files.size() == 1) {
            // DLNAresource was found.
            DlnaItem *dlna = static_cast<DlnaItem*>(files.at(0));

            if (range != 0) {
                // update range with size of dlna object
                range->setSize(dlna->size());
            }

            if (fileName.startsWith("thumbnail0000")) {
                // This is a request for a thumbnail file.
                answerHeader << "Content-Type: image/jpeg";
                answerHeader << "Accept-Ranges: bytes";
//                answerHeader << "Expires: " + getFUTUREDATE() + " GMT";
                answerHeader << "Connection: keep-alive";
                answerHeader << "Server: " + m_request->getServername();

                QByteArray answerContent = dlna->getByteAlbumArt();
                if (answerContent.isNull()) {
                    m_log->Error("Unable to get thumbnail: " + dlna->getDisplayName());
                    m_request->setStatus("KO");
                } else {
                    sendAnswer(answerHeader, answerContent);
                    m_request->setStatus("OK");
                }

            } else if (fileName.indexOf("subtitle0000") > -1) {
                // This is a request for a subtitle file
                /*output(output, "Content-Type: text/plain");
                output(output, "Expires: " + getFUTUREDATE() + " GMT");
                List<DLNAMediaSubtitle> subs = dlna.getMedia().getSubtitleTracksList();

                if (subs != null && !subs.isEmpty()) {
                    // TODO: maybe loop subs to get the requested subtitle type instead of using the first one
                    DLNAMediaSubtitle sub = subs.get(0);

                    try {
                        // XXX external file is null if the first subtitle track is embedded:
                        // http://www.ps3mediaserver.org/forum/viewtopic.php?f=3&t=15805&p=75534#p75534
                        if (sub.isExternal()) {
                            inputStream = new java.io.FileInputStream(sub.getExternalFile());
                        }
                    } catch (NullPointerException npe) {
                        LOGGER.trace("Could not find external subtitles: " + sub);
                    }
                }*/
            } else {
                // This is a request for a regular file.
                answerHeader << "Content-Type: " + dlna->mimeType();
                headerAnswerToSend << "Content-Type: " + dlna->mimeType();

                if (!m_request->getContentFeatures().isNull()) {
                    answerHeader << "ContentFeatures.DLNA.ORG: " + dlna->getDlnaContentFeatures();
                    headerAnswerToSend << "ContentFeatures.DLNA.ORG: " + dlna->getDlnaContentFeatures();
                }

                if (!m_request->getMediaInfoSec().isNull()) {
                    answerHeader << QString("MediaInfo.sec: SEC_Duration=%1").arg(dlna->getLengthInMilliSeconds());
                    headerAnswerToSend << QString("MediaInfo.sec: SEC_Duration=%1").arg(dlna->getLengthInMilliSeconds());
                }

                if (dlna->getdlnaOrgOpFlags().at(1) == '1') {
                    answerHeader << "Accept-Ranges: bytes";
                    headerAnswerToSend << "Accept-Ranges: bytes";
                }

                answerHeader << "Connection: keep-alive";
                headerAnswerToSend << "Connection: keep-alive";

                answerHeader << "Server: " + m_request->getServername();
                headerAnswerToSend << "Server: " + m_request->getServername();

                if (m_request->getTimeSeekRangeStart() >= 0 && dlna->getLengthInMilliSeconds() > 0) {
                    QTime start_time(0, 0, 0);
                    start_time = start_time.addSecs(m_request->getTimeSeekRangeStart());

                    QTime end_time(0, 0, 0);
                    if (m_request->getTimeSeekRangeEnd() != -1) {
                        end_time = end_time.addSecs(m_request->getTimeSeekRangeEnd());
                    } else {
                        end_time = end_time.addMSecs(dlna->getLengthInMilliSeconds());
                    }

                    QTime length_time(0, 0, 0);
                    length_time = length_time.addMSecs(dlna->getLengthInMilliSeconds());

                    headerAnswerToSend << QString("TimeSeekRange.dlna.org: npt=%1-%2/%3").arg(start_time.toString("hh:mm:ss,z")).arg(end_time.toString("hh:mm:ss,z")).arg(length_time.toString("hh:mm:ss,z"));
                    headerAnswerToSend << QString("X-Seek-Range: npt=%1-%2/%3").arg(start_time.toString("hh:mm:ss,z")).arg(end_time.toString("hh:mm:ss,z")).arg(length_time.toString("hh:mm:ss,z"));
                    headerAnswerToSend << QString("X-AvailableSeekRange: 1 npt=%1-%2").arg(0).arg(dlna->getLengthInSeconds());
                }

                if (m_request->getMethod() == "HEAD") {
                    sendAnswer(answerHeader, QByteArray(), dlna->size());
                    m_request->setStatus("OK");

                } else {
                    m_log->Debug(QString("%1 bytes to send, %2 to send.").arg(dlna->size()).arg(QTime(0, 0).addMSecs(dlna->getLengthInMilliSeconds()).toString("hh:mm:ss.zzz")));

                    mediaFilename = dlna->getSystemName();
                    emit serving(mediaFilename, 0);

                    if (!dlna->toTranscode()) {

                        // stream file
                        runStreaming(dlna);

                        if (streamContent != 0) {
                            // TODO: header shall be sent with the first part of the content in the same time
                            sendAnswer(answerHeader, QByteArray(), dlna->size());
                        } else
                            m_log->Error(QString("Streaming not started : %1").arg(dlna->getSystemName()));

                    } else {
                        if (dlna->size() != -1) {
                            headerAnswerToSend << "Content-Length: " + QString("%1").arg(dlna->size());
                        }

                        // transcode file
                        runTranscoding(dlna);
                    }
                }
            }
        }
    } else if (m_request->getMethod() == "SUBSCRIBE" && !soapaction.isEmpty()) {
        QStringList answerHeader;
        answerHeader << CONTENT_TYPE_UTF8;
        answerHeader << "Content-Length: 0";
        answerHeader << "Connection: close";
        answerHeader << QString("SID: uuid:%1").arg(m_request->getUuid());
        answerHeader << "Server: " + m_request->getServername();
        answerHeader << "Timeout: Second-1800";
        answerHeader << "";

        keepSocketOpened = true;
        sendAnswer(answerHeader);
        if (client != 0) {
            client->flush();
        }

        QString cb = soapaction.replace("<", "").replace(">", "");

        QUrl soapActionUrl = QUrl(cb);
        QString addr = soapActionUrl.host();
        int port = soapActionUrl.port();

        QTcpSocket sock;
        sock.connectToHost(addr, port);
        if (sock.waitForConnected()) {
            sendLine(&sock, "NOTIFY /" + m_request->getArgument() + " HTTP/1.1");
            sendLine(&sock, QString("SID: uuid:%1").arg(m_request->getUuid()));
            sendLine(&sock, "SEQ: 0");
            sendLine(&sock, "NT: upnp:event");
            sendLine(&sock, "NTS: upnp:propchange");
            sendLine(&sock, QString("HOST: %1:%2").arg(addr).arg(port));
            sendLine(&sock, CONTENT_TYPE_UTF8);
        }
        else {
            m_log->Error(QString("Cannot connect to %1").arg(cb));
            m_request->setStatus("ERROR");
            return;
        }

        sock.close();

        QString answerContent;
        keepSocketOpened = false;

        if (m_request->getArgument().contains("connection_manager")) {
            QStringList answerHeader;
            answerHeader << "Server: " + m_request->getServername();

            answerContent.append(EVENT_Header.arg("urn:schemas-upnp-org:service:ConnectionManager:1"));
            answerContent.append(EVENT_Prop.arg("SinkProtocolInfo").arg(""));
            answerContent.append(EVENT_Prop.arg("SourceProtocolInfo").arg(""));
            answerContent.append(EVENT_Prop.arg("CurrentConnectionIDs").arg(""));
            answerContent.append(EVENT_FOOTER);

            sendAnswer(answerHeader, answerContent.toUtf8());

            m_request->setStatus("OK");

        } else if (m_request->getArgument().contains("content_directory")) {
            QStringList answerHeader;
            answerHeader << "Server: " + m_request->getServername();

            answerContent.append(EVENT_Header.arg("urn:schemas-upnp-org:service:ContentDirectory:1"));
            answerContent.append(EVENT_Prop.arg("TransferIDs").arg(""));
            answerContent.append(EVENT_Prop.arg("ContainerUpdateIDs").arg(""));
            answerContent.append(EVENT_Prop.arg("SystemUpdateID").arg(1));
            answerContent.append(EVENT_FOOTER);

            sendAnswer(answerHeader, answerContent.toUtf8());

            m_request->setStatus("OK");
        } else {
            closeClient();
            m_request->setStatus("Unknown argument");
        }
    }
    else if ((m_request->getMethod() == "GET" || m_request->getMethod() == "HEAD") && (m_request->getArgument().toLower().endsWith(".png") || m_request->getArgument().toLower().endsWith(".jpg") || m_request->getArgument().toLower().endsWith(".jpeg"))) {
        QStringList answerHeader;

        if (m_request->getArgument().toLower().endsWith(".png")) {
            answerHeader << "Content-Type: image/png";
        } else {
            answerHeader << "Content-Type: image/jpeg";
        }

        answerHeader << "Accept-Ranges: bytes";
        answerHeader << "Connection: keep-alive";
//        answerHeader << "Expires: " + getFUTUREDATE() + " GMT";
        answerHeader << "Server: " + m_request->getServername();

        QByteArray answerContent;

        // read the image file
        QFile inputStream(":/" + m_request->getArgument());
        if (inputStream.open(QFile::ReadOnly)) {
            answerContent = inputStream.readAll();
            inputStream.close();

        }
        else {
            m_log->Error(QString("Unable to read %1 for %2 answer.").arg(m_request->getArgument()).arg(m_request->getMethod()));
        }

        sendAnswer(answerHeader, answerContent);

        m_request->setStatus("OK");
    }
    else {
        m_log->Error("Unkown answer for: " + m_request->getMethod() + " " + m_request->getArgument());
        m_request->setStatus("KO");
    }

}


void Reply::runStreaming(DlnaItem* dlna) {
    if (streamContent == 0) {
        //set buffer size
        if (dlna->bitrate()>0) {
            // set buffer size to 10 seconds transcoding
            maxBufferSize = dlna->bitrate()/8*10;
        }

        // recover resume time
        long timeSeekRangeStart = m_request->getTimeSeekRangeStart();
        long timeSeekRangeEnd = m_request->getTimeSeekRangeEnd();
        qint64 resume = dlna->getResumeTime();
        if (resume>0) {
            timeSeekRangeStart = resume/1000 - 10;
            clockSending.addMSec(timeSeekRangeStart*1000);
        }

        // get stream file
        streamContent = dlna->getStream(m_request->getRange(), timeSeekRangeStart, timeSeekRangeEnd, this);

        if (streamContent == 0) {
            // No inputStream indicates that transcoding / remuxing probably crashed.
            m_log->Error("There is no inputstream to return for " + dlna->getDisplayName());
            m_request->setStatus("KO");
        } else {
            m_renderersModel->serving(m_request->getpeerAddress(), dlna->getDisplayName());

            // Start timer to update periodically the status on streaming or transcoding
            timerStatus.start(UPDATE_STATUS_PERIOD);
        }
    } else {
        m_log->Error(QString("Streaming already in progress"));
        m_request->setStatus("KO");
    }
}

void Reply::runTranscoding(DlnaItem* dlna) {
    if (transcodeProcess == 0) {
        //set buffer size
        if (dlna->bitrate()>0) {
            // set buffer size to 10 seconds transcoding
            maxBufferSize = dlna->bitrate()/8*10;
        }

        // recover resume time
        long timeSeekRangeStart = m_request->getTimeSeekRangeStart();
        long timeSeekRangeEnd = m_request->getTimeSeekRangeEnd();
        qint64 resume = dlna->getResumeTime();
        if (resume>0) {
            timeSeekRangeStart = resume/1000 - 10;
            clockSending.addMSec(timeSeekRangeStart*1000);
        }

        // get transcoding process
        transcodeProcess = dlna->getTranscodeProcess(m_request->getRange(), timeSeekRangeStart, timeSeekRangeEnd, this);

        if (transcodeProcess == 0) {
            m_log->Error(QString("Cannot create transcoding process"));
            m_request->setStatus("KO");

        } else {
            connect(transcodeProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(receivedTranscodedData()));
            connect(transcodeProcess, SIGNAL(receivedTranscodingLogMessage(QString)), this, SLOT(receivedTranscodingLogMessage(QString)));
            connect(transcodeProcess, SIGNAL(finished(int)), this, SLOT(finishedTranscodeData(int)));

            transcodeProcess->launch();
            if (!transcodeProcess->waitForStarted()) {
                m_log->Error(transcodeProcess->errorString());
                m_request->setStatus("KO");
            } else {
                // transcoding process started
                m_renderersModel->serving(m_request->getpeerAddress(), dlna->getDisplayName());
                m_request->setStatus("Transcoding");

                // Start timer to update periodically the status on streaming or transcoding
                timerStatus.start(UPDATE_STATUS_PERIOD);
            }
        }
    } else {
        m_log->Error(QString("Transcoding already in progress"));
        m_request->setStatus("KO");
    }
}

void Reply::waitTranscodingFinished() {
    if (transcodeProcess != 0) {
        // wait until process is finished
        transcodeProcess->waitForFinished(-1);
    }
}

void Reply::receivedTranscodingLogMessage(const QString &msg) {
    m_request->appendLog(msg);
}

void Reply::receivedTranscodedData() {
    if (transcodeProcess != 0) {
        if (transcodeProcess->bytesAvailable() > 0) {
            if (client != 0) {
                if (!headerAnswerToSend.isEmpty()) {
                    // send the header
                    sendAnswer(headerAnswerToSend);

                    // header sent
                    headerAnswerToSend.clear();
                }

                // send data to client
                if (client->write(transcodeProcess->readAllStandardOutput()) == -1) {
                    m_log->Error("Unable to send transcoded data to client.");
                } else if (transcodeProcess->state()==QProcess::Running) {
                    // data sent and transcoding is in progress
                    if (client->bytesToWrite() > maxBufferSize) {
                        // pause transcoding process
                        if (transcodeProcess->pause() == false)
                            m_log->Error(QString("Unable to pause transcoding: pid=%1").arg(transcodeProcess->pid()));
                    }
                }
            } else {
                m_log->Error("Unable to send transcoded data to client (client deleted).");
            }
        }
    }
}

void Reply::finishedTranscodeData(const int &exitCode) {
    if (transcodeProcess != 0) {
        if (transcodeProcess->bytesAvailable() > 0) {
            // send last transcoded data to client
            if (client && client->write(transcodeProcess->readAllStandardOutput()) == -1)
                m_log->Error("Unable to send last transcoded data to client.");
        }


        if (transcodeProcess->isKilled() == false) {
            if (exitCode != 0) {
                // trancoding failed
                m_request->setStatus("Transcoding failed.");
            } else {
                m_request->setStatus("Transcoding finished.");
            }
        } else {
            m_request->setStatus("Transcoding aborted.");
        }
    }
}

void Reply::closeClient() {
    if (m_log->isLevel(DEBG) and transcodeProcess)
        appendTrancodeProcessLog(QString("%3: closeclient, state transcodeprocess %1, client valid? %2").arg(transcodeProcess->state()).arg(client != 0).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));

    if (!keepSocketOpened and (transcodeProcess == 0 or transcodeProcess->state() != QProcess::Running) and (streamContent == 0 or streamContent->atEnd())) {
        // No streaming or transcoding in progress
        m_log->Trace("Close client connection in request");
        if (client != 0) {
            if (m_log->isLevel(DEBG) and transcodeProcess)
                appendTrancodeProcessLog(QString("%2: CLOSE CLIENT (%1)").arg(client->socketDescriptor()).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));

            client->disconnectFromHost();
        } else
            m_log->Debug("Unable to close client (client deleted).");

        m_request->endReply();
    }
}

void Reply::updateStatus()
{
    if (clockUpdateStatus.isValid()) {
        int delta = clockUpdateStatus.restart() - UPDATE_STATUS_PERIOD;

        if (qAbs(delta) > UPDATE_STATUS_PERIOD/10) {
            if (streamContent)
                m_log->Info(QString("UPDATE STATUS delta %2 streaming <%1>").arg(mediaFilename).arg(delta));
            else if (transcodeProcess)
                m_log->Info(QString("UPDATE STATUS delta %2 transcoding <%1>").arg(mediaFilename).arg(delta));
            else
                m_log->Info(QString("UPDATE STATUS delta %2 <%1>").arg(mediaFilename).arg(delta));
        }
    } else {
        clockUpdateStatus.start();
    }

    if (streamContent)
        m_request->setStatus(QString("Streaming (%1%)").arg(int(100.0*double(streamContent->pos())/double(streamContent->size()))));

    if (!mediaFilename.isNull()) {
        if (clockSending.isValid())
            emit serving(mediaFilename, clockSending.elapsedFromBeginning());
        else
            emit serving(mediaFilename, 0);

        if (client && clockSending.isValid() && maxBufferSize != 0) {
            // display the network buffer and network speed
            int networkSpeed = int((double(networkBytesSent)/1024.0)/(double(clockSending.elapsed())/1000.0));
            double bufferTime = double(maxBufferSize)/double(networkSpeed*1024);
            m_request->setNetworkStatus(QString("Time: %5 Buffer: %4 bytes %1% %3 seconds, Speed: %2 Ko/s").arg(int(100.0*double(client->bytesToWrite())/double(maxBufferSize))).arg(networkSpeed).arg(bufferTime).arg(maxBufferSize).arg(QTime(0, 0).addMSecs(clockSending.elapsedFromBeginning()).toString("hh:mm:ss")));
        }

        if (lastNetBytesSent!=-1 && lastNetBytesSent==networkBytesSent)
            clockSending.pause();
        else if (networkBytesSent > 0)
            clockSending.start();

        lastNetBytesSent = networkBytesSent;
    }
}

void Reply::bytesSent(const qint64 &size)
{
    networkBytesSent += size;

    if (!clockSending.isValid()) {
        // start clock to measure time taken to stream or transcode
        clockSending.start();
    }

    if (m_log->isLevel(TRA) and transcodeProcess != 0) {
        if (client != 0)
            appendTrancodeProcessLog(QString("%2: bytes sent %3 (%1)").arg(client->socketDescriptor()).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(size));
        else
            appendTrancodeProcessLog(QString("%1: bytes sent %2 (client deleted)").arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(size));
    }

    if (streamContent) {
        if (client == 0) {
            m_log->Error("HTTP Request: Unable to send content (client deleted).");

            close();

            m_request->setStatus("KO");
        } else {
            int bytesToRead = maxBufferSize - client->bytesToWrite();
            if (bytesToRead > 0) {
                // read the stream
                char bytesToSend[bytesToRead];
                qint64 bytes = streamContent->read(bytesToSend, sizeof(bytesToSend));

                if (bytes == -1) {
                    m_log->Error("HTTP Request: Unable to send content.");

                    close();

                    m_request->setStatus("KO");

                } else if (bytes > 0) {
                    if (client->write(bytesToSend, bytes)== -1) {
                        m_log->Error("HTTP Request: Unable to send content.");

                        close();

                        m_request->setStatus("KO");
                    }
                }
            }
        }
    }

    if (transcodeProcess && client && client->bytesToWrite() < (maxBufferSize*0.5)) {
        // restart transcoding process
        if (transcodeProcess->resume() == false)
            m_log->Error(QString("Unable to restart transcoding: pid=%1").arg(transcodeProcess->pid()));
    }
}

void Reply::stateChanged(const QAbstractSocket::SocketState &state)
{
    if (m_log->isLevel(DEBG) and transcodeProcess) {
        if (client != 0)
            appendTrancodeProcessLog(QString("%2: Socket state changed %3 (%1)").arg(client->socketDescriptor()).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(state));
        else
            appendTrancodeProcessLog(QString("%1: Socket state changed %2 (client deleted)").arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(state));
    }

    if (state == QAbstractSocket::UnconnectedState) {
        close();
    }
}

void Reply::errorSocket(const QAbstractSocket::SocketError &error)
{
    Q_UNUSED(error)

    if (transcodeProcess) {
        if (client) {
            appendTrancodeProcessLog(QString("%3: Socket ERROR (%1): %2").arg(client->socketDescriptor()).arg(client->errorString()).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
            appendTrancodeProcessLog(QString("Available socket data: %1 bytes, Bytes sent: %2 bytes").arg(client->bytesAvailable()).arg(networkBytesSent));
        } else
            appendTrancodeProcessLog(QString("%1: Socket ERROR (client deleted)").arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
    }
}

void Reply::close() {
    timerStatus.stop();

    if (clockSending.isValid())
        m_log->Debug(QString("REQUEST CLOSED: %1 bytes sent, %2 taken to send data.").arg(networkBytesSent).arg(QTime(0, 0).addMSecs(clockSending.elapsed()).toString("hh:mm:ss.zzz")));

    if (streamContent) {
        if (streamContent->atEnd() && client && client->bytesToWrite() == 0) {
            m_request->setStatus("Streaming finished.");
            emit servingFinished(mediaFilename, 0);
        } else {
            m_request->setStatus("Streaming aborted.");
            emit servingFinished(mediaFilename, 1);
        }

        delete streamContent;
        streamContent = 0;

        m_renderersModel->stopServing(m_request->getpeerAddress());
    }

    if (transcodeProcess) {
        if (transcodeProcess->state() != QProcess::NotRunning) {
            m_request->setStatus("Transcoding aborted.");
            transcodeProcess->killProcess();
        }

        if (!transcodeProcess->isKilled() && transcodeProcess->transcodeExitCode()==0 && client && client->bytesToWrite() == 0)
            emit servingFinished(mediaFilename, 0);
        else
            emit servingFinished(mediaFilename, 1);

        if (!transcodeProcess->isKilled()) {
            delete transcodeProcess;
            transcodeProcess = 0;
        } else {
            transcodeProcess->disconnect(this);
        }

        m_renderersModel->stopServing(m_request->getpeerAddress());
    }

    // invalidate clock
    clockSending.invalidate();
    clockUpdateStatus.invalidate();

    if (client) {
        if (clockSending.isValid())
            m_log->Debug(QString("remaining data to send: %1").arg(client->bytesToWrite()));
    }

    m_request->endReply();
}
