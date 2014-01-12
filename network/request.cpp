#include "request.h"

const QString Request::CRLF = "\r\n";

const QString Request::CONTENT_TYPE_UTF8 = "CONTENT-TYPE: text/xml; charset=\"utf-8\"";
const QString Request::CONTENT_TYPE = "Content-Type: text/xml; charset=\"utf-8\"";

const QString Request::HTTP_200_OK = "HTTP/1.1 200 OK";
const QString Request::HTTP_500 = "HTTP/1.1 500 Internal Server Error";
const QString Request::HTTP_206_OK = "HTTP/1.1 206 Partial Content";
const QString Request::HTTP_200_OK_10 = "HTTP/1.0 200 OK";
const QString Request::HTTP_206_OK_10 = "HTTP/1.0 206 Partial Content";

const QString Request::XML_HEADER = "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
const QString Request::SOAP_ENCODING_HEADER = "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">" + CRLF + "<s:Body>";
const QString Request::PROTOCOLINFO_RESPONSE =  "<u:GetProtocolInfoResponse xmlns:u=\"urn:schemas-upnp-org:service:ConnectionManager:1\"><Source>http-get:*:image/jpeg:DLNA.ORG_PN=JPEG_SM,http-get:*:image/jpeg:DLNA.ORG_PN=JPEG_MED,http-get:*:image/jpeg:DLNA.ORG_PN=JPEG_LRG,http-get:*:audio/mpeg:DLNA.ORG_PN=MP3,http-get:*:audio/L16:DLNA.ORG_PN=LPCM,http-get:*:video/mpeg:DLNA.ORG_PN=AVC_TS_HD_24_AC3_ISO;SONY.COM_PN=AVC_TS_HD_24_AC3_ISO,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_HD_24_AC3;SONY.COM_PN=AVC_TS_HD_24_AC3,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_HD_24_AC3_T;SONY.COM_PN=AVC_TS_HD_24_AC3_T,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_PS_PAL,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_PS_NTSC,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_50_L2_T,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_60_L2_T,http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_TS_SD_EU_ISO,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_EU,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_EU_T,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_50_AC3_T,http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_TS_HD_50_L2_ISO;SONY.COM_PN=HD2_50_ISO,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_60_AC3_T,http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_TS_HD_60_L2_ISO;SONY.COM_PN=HD2_60_ISO,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_50_L2_T;SONY.COM_PN=HD2_50_T,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_60_L2_T;SONY.COM_PN=HD2_60_T,http-get:*:video/mpeg:DLNA.ORG_PN=AVC_TS_HD_50_AC3_ISO;SONY.COM_PN=AVC_TS_HD_50_AC3_ISO,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_HD_50_AC3;SONY.COM_PN=AVC_TS_HD_50_AC3,http-get:*:video/mpeg:DLNA.ORG_PN=AVC_TS_HD_60_AC3_ISO;SONY.COM_PN=AVC_TS_HD_60_AC3_ISO,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_HD_60_AC3;SONY.COM_PN=AVC_TS_HD_60_AC3,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_HD_50_AC3_T;SONY.COM_PN=AVC_TS_HD_50_AC3_T,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_HD_60_AC3_T;SONY.COM_PN=AVC_TS_HD_60_AC3_T,http-get:*:video/x-mp2t-mphl-188:*,http-get:*:*:*,http-get:*:video/*:*,http-get:*:audio/*:*,http-get:*:image/*:*</Source><Sink></Sink></u:GetProtocolInfoResponse>";
const QString Request::SOAP_ENCODING_FOOTER = "</s:Body>" + CRLF + "</s:Envelope>";
const QString Request::SORTCAPS_RESPONSE = "<u:GetSortCapabilitiesResponse xmlns:u=\"urn:schemas-upnp-org:service:ContentDirectory:1\"><SortCaps></SortCaps></u:GetSortCapabilitiesResponse>";
const QString Request::SEARCHCAPS_RESPONSE = "<u:GetSearchCapabilitiesResponse xmlns:u=\"urn:schemas-upnp-org:service:ContentDirectory:1\"><SearchCaps></SearchCaps></u:GetSearchCapabilitiesResponse>";
const QString Request::SAMSUNG_ERROR_RESPONSE = "<s:Fault><faultCode>s:Client</faultCode><faultString>UPnPError</faultString><detail><UPnPError xmlns=\"urn:schemas-upnp-org:control-1-0\"><errorCode>401</errorCode><errorDescription>Invalid Action</errorDescription></UPnPError></detail></s:Fault>";
const QString Request::GETSYSTEMUPDATEID_HEADER = "<u:GetSystemUpdateIDResponse xmlns:u=\"urn:schemas-upnp-org:service:ContentDirectory:1\">";
const QString Request::GETSYSTEMUPDATEID_FOOTER = "</u:GetSystemUpdateIDResponse>";

const QString Request::EVENT_Header = "<e:propertyset xmlns:e=\"urn:schemas-upnp-org:event-1-0\" xmlns:s=\"%1\">";
const QString Request::EVENT_Prop = "<e:property><%1>%2</%1></e:property>";
const QString Request::EVENT_FOOTER = "</e:propertyset>";

Request::Request(Logger* log, QTcpSocket* client, QString uuid, QString servername, QString host, int port, DlnaRootFolder *rootFolder):
    log(log),
    client(client),
    keepSocketOpened(false),
    streamContent(0),
    transcodeProcess(0),
    status("init"),
    networkStatus("connected"),
    rootFolder(rootFolder),
    uuid(uuid),
    servername(servername),
    host(host),
    port(port),
    peerAddress(client->peerAddress().toString()),
    soapaction(""),
    content(""),
    receivedContentLength(-1),
    range(0),
    http10(false)
{
    // start clock to measure time taken to answer to the request
    clock.start();

    setDate(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz"));

    connect(client, SIGNAL(readyRead()), this, SLOT(readSocket()));
    connect(client, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));
    connect(client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorSocket(QAbstractSocket::SocketError)));
    connect(client, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesSent(qint64)));

    connect(this, SIGNAL(answerReady(QStringList,QByteArray,long)), this, SLOT(sendAnswer(QStringList,QByteArray,long)));

    connect(this, SIGNAL(startTranscoding(DlnaResource*, QStringList)), this, SLOT(runTranscoding(DlnaResource*, QStringList)));

    log->TRACE("Request: receiving a request from " + peerAddress);

    if (client->isOpen()) {
        setNetworkStatus("opened");
    } else {
        setNetworkStatus("not connected");
    }
}

Request::~Request() {
    if (range != 0) {
        //delete range;
        delete range;
    }
}

void Request::setSoapaction(QString soapaction) {
    if (!getSoapaction().isEmpty()) {
        log->WARNING(QString("SOAPACTION in request is overwritten (old value was %1)").arg(soapaction));
    }
    this->soapaction = soapaction;
}

long Request::getLowRange() const {
    if (range == 0) {
        return -1;
    } else {
        return range->getLowRange();
    }
}

long Request::getHighRange() const {
    if (range == 0) {
        return -1;
    } else {
        return range->getHighRange();
    }
}

bool Request::appendHeader(QString headerLine)
{
    // add the line to header list
    header.append(headerLine);

    // read the header line and update request attributes
    QStringList fields = headerLine.split(QRegExp("\\s+"));

    QRegExp rxHost("Host:\\s*(.+)", Qt::CaseInsensitive);
    QRegExp rxLanguage("Accept-language:\\s*(.+)", Qt::CaseInsensitive);
    QRegExp rxEncoding("Accept-Encoding:\\s*(.+)", Qt::CaseInsensitive);
    QRegExp rxAccept("Accept: \\s*(.+)", Qt::CaseInsensitive);
    QRegExp rxConnection("Connection:\\s*(.+)", Qt::CaseInsensitive);
    QRegExp rxContentType("Content-Type:\\s*(.+)", Qt::CaseInsensitive);
    QRegExp rxTimeOut("Timeout:\\s*(.+)", Qt::CaseInsensitive);
    QRegExp rxNt("Nt:\\s*(.+)", Qt::CaseInsensitive);
    QRegExp rxIcyMetadata("Icy-Metadata:\\s*(\\w+)", Qt::CaseInsensitive);
    QRegExp rxRange("RANGE: BYTES=(\\d+)-(\\d*)", Qt::CaseInsensitive);
    QRegExp rxContentLength("CONTENT-LENGTH:\\s*(\\d+)", Qt::CaseInsensitive);
    QRegExp rxTransferMode("transferMode.dlna.org:\\s*(\\w+)", Qt::CaseInsensitive);
    QRegExp rxContentFeatures("getcontentFeatures.dlna.org:\\s*(\\w+)", Qt::CaseInsensitive);
    QRegExp rxMediaInfoSec("getMediaInfo.sec:\\s*(\\w+)", Qt::CaseInsensitive);

    if (fields.length() > 0) {

        if (fields[0] == "SUBSCRIBE" || fields[0] == "GET" || fields[0] == "POST" || fields[0] == "HEAD") {
            method = fields[0];
            if (fields.length() > 1) {
                // Samsung 2012 TVs have a problematic preceding slash that needs to be removed.
                if (fields[1].startsWith("/"))
                {
                    log->TRACE("Stripping preceding slash from: " + fields[1]);
                    this->argument = fields[1].remove(0, 1);
                }
                else
                {
                    this->argument = fields[1];
                }
            }
            if (fields.length() > 2 && fields[2] == "HTTP/1.0")
            {
                setHttp10(true);
            }
        }
        else if (fields[0].toUpper() == "CALLBACK:") {
            setSoapaction(fields[1]);
        }
        else if (fields[0].toUpper() == "SOAPACTION:") {
            setSoapaction(fields[1]);
        }
        else if (rxRange.indexIn(headerLine) != -1) {
            if (range != 0) {
                // range already read, ignore it
                log->ERROR("A range has already been read in the request");
            } else {
                range = new HttpRange(headerLine);
                if (range->isNull()) {
                    // invalid range, ignore it
                    delete range;
                    range = 0;
                    log->ERROR("Invalid range in request: " + headerLine);
                }
            }
        }
        else if (fields[0].toUpper() == "USER-AGENT:") {
            userAgentString = fields[1];
        }
        else if (rxContentLength.indexIn(headerLine) != -1) {
            receivedContentLength = rxContentLength.cap(1).toInt();
        }
        else if (rxTransferMode.indexIn(headerLine) != -1) {
            transferMode = rxTransferMode.cap(1);
        }
        else if (rxContentFeatures.indexIn(headerLine) != -1) {
            contentFeatures = rxContentFeatures.cap(1);
        }
        else if (rxMediaInfoSec.indexIn(headerLine) != -1) {
            mediaInfoSec = rxMediaInfoSec.cap(1);
        }
        else if (rxHost.indexIn(headerLine) != -1) {

        }
        else if (rxLanguage.indexIn(headerLine) != -1) {

        }
        else if (rxEncoding.indexIn(headerLine) != -1) {

        }
        else if (rxAccept.indexIn(headerLine) != -1) {

        }
        else if (rxConnection.indexIn(headerLine) != -1) {

        }
        else if (rxContentType.indexIn(headerLine) != -1) {

        }
        else if (rxTimeOut.indexIn(headerLine) != -1) {

        }
        else if (rxNt.indexIn(headerLine) != -1) {

        }
        else if (rxIcyMetadata.indexIn(headerLine) != -1) {

        }
        else {
            log->ERROR("Unknown headerLine in request: <" + headerLine + ">");
            return false;
        }

  /*else if (headerLine.toUpperCase().indexOf("TIMESEEKRANGE.DLNA.ORG: NPT=") > -1) { // firmware 2.50+
        String timeseek = headerLine.substring(headerLine.toUpperCase().indexOf("TIMESEEKRANGE.DLNA.ORG: NPT=") + 28);
        if (timeseek.endsWith("-")) {
            timeseek = timeseek.substring(0, timeseek.length() - 1);
        } else if (timeseek.indexOf("-") > -1) {
            timeseek = timeseek.substring(0, timeseek.indexOf("-"));
        }
        request.setTimeseek(convertStringToTime(timeseek));
    } else if (headerLine.toUpperCase().indexOf("TIMESEEKRANGE.DLNA.ORG : NPT=") > -1) { // firmware 2.40
        String timeseek = headerLine.substring(headerLine.toUpperCase().indexOf("TIMESEEKRANGE.DLNA.ORG : NPT=") + 29);
        if (timeseek.endsWith("-")) {
            timeseek = timeseek.substring(0, timeseek.length() - 1);
        } else if (timeseek.indexOf("-") > -1) {
            timeseek = timeseek.substring(0, timeseek.indexOf("-"));
        }
        request.setTimeseek(convertStringToTime(timeseek));*/

    }
    else {
        log->ERROR("Unknown headerLine in request: <" + headerLine + ">");
        return false;
    }

    return true;
}

void Request::sendLine(QTcpSocket *client, QString msg)
{
    if (client == 0) {
        log->ERROR("Unable to send line (client deleted).");
    } else {
        QByteArray tmp;

        tmp.append(msg.toUtf8()).append(CRLF.toUtf8());

        if (client->write(tmp) == -1) {
            log->ERROR("HTTP Request: Unable to send line: " + msg);
        }

        stringAnswer.append(msg+CRLF);
        log->DEBUG("Wrote on socket: " + msg);
    }
}

void Request::sendAnswer(QStringList headerAnswer, QByteArray contentAnswer, long totalSize)
{
    if (client == 0) {
        log->ERROR("Unable to send answer (client deleted).");
    } else {
        if (range != 0)
        {
            // partial content requested (range is present in the header)
            sendLine(client, http10 ? HTTP_206_OK_10 : HTTP_206_OK);

            if (totalSize != -1) {
                sendLine(client, QString("Content-Range: bytes %1-%2/%3").arg(range->getStartByte()).arg(range->getEndByte()).arg(totalSize));
            }

        } else {
            if (soapaction.contains("X_GetFeatureList")) {
                //  If we don't return a 500 error, Samsung 2012 TVs time out.
                sendLine(client, HTTP_500);
            } else {
                sendLine(client, http10 ? HTTP_200_OK_10 : HTTP_200_OK);
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
        if (method != "HEAD" && !contentAnswer.isNull()) {
            // send the content
            stringAnswer.append(QString("content size: %1%2").arg(contentAnswer.size()).arg(CRLF));
            stringAnswer.append(contentAnswer);
            if (client->write(contentAnswer) == -1) {

                log->ERROR("HTTP request: Unable to send content.");

            } else {
                log->DEBUG(QString("Send content (%1 bytes).").arg(contentAnswer.size()));
                log->TRACE("Wrote on socket content: " + contentAnswer);

            }
        }

        closeClient();
    }
}

void Request::run() {
    // prepare data and send answer

    log->TRACE("ANSWER: " + method + " " + argument);

    if ((method == "GET" || method == "HEAD") && (argument == "description/fetch" || argument.endsWith("1.0.xml"))) {
        QStringList answerHeader;

        answerHeader << CONTENT_TYPE;
        answerHeader << "Cache-Control: no-cache";
        answerHeader << "Expires: 0";
        answerHeader << "Accept-Ranges: bytes";
        answerHeader << "Connection: keep-alive";
        answerHeader << "Server: " + servername;

        QString answerContent;

        QFile inputStream(":/PMS.xml");
        if (inputStream.open(QFile::ReadOnly | QFile::Text)) {
            // read file PMS.xml
            answerContent = inputStream.readAll();
            inputStream.close();

            // replace parameter by its value
            answerContent.replace(QString("[servername]"), servername);
            answerContent.replace(QString("[uuid]"), QString("uuid:%1").arg(uuid));
            answerContent.replace(QString("[host]"), host);
            answerContent.replace(QString("[port]"), QString("%1").arg(port));
        }
        else {
            log->ERROR("Unable to read PMS.xml for description/fetch answer.");
        }

        emit answerReady(answerHeader, answerContent.toUtf8());

        setStatus("OK");
    }
    else if (method == "POST" && argument.endsWith("upnp/control/connection_manager")) {

        if (!soapaction.isEmpty() && soapaction.indexOf("ConnectionManager:1#GetProtocolInfo") > -1) {
            QStringList answerHeader;
            answerHeader << CONTENT_TYPE_UTF8;
            answerHeader << "Server: " + servername;

            QString answerContent;
            answerContent.append(XML_HEADER);
            answerContent.append(CRLF);
            answerContent.append(SOAP_ENCODING_HEADER);
            answerContent.append(CRLF);
            answerContent.append(PROTOCOLINFO_RESPONSE);
            answerContent.append(CRLF);
            answerContent.append(SOAP_ENCODING_FOOTER);
            answerContent.append(CRLF);

            emit answerReady(answerHeader, answerContent.toUtf8());

            setStatus("OK");
        }

    }
    else if (method == "POST" && argument.endsWith("upnp/control/content_directory")) {
        QStringList answerHeader;
        answerHeader << CONTENT_TYPE_UTF8;
        answerHeader << "Server: " + servername;

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

            emit answerReady(answerHeader, answerContent.toUtf8());

            setStatus("OK");

        } else if (!soapaction.isEmpty() && soapaction.indexOf("ContentDirectory:1#GetSortCapabilities") > -1) {
            answerContent.append(XML_HEADER);
            answerContent.append(CRLF);
            answerContent.append(SOAP_ENCODING_HEADER);
            answerContent.append(CRLF);
            answerContent.append(SORTCAPS_RESPONSE);
            answerContent.append(CRLF);
            answerContent.append(SOAP_ENCODING_FOOTER);
            answerContent.append(CRLF);

            emit answerReady(answerHeader, answerContent.toUtf8());

            setStatus("OK");

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

            emit answerReady(answerHeader, answerContent.toUtf8());

            setStatus("OK");

        } else if (!soapaction.isEmpty()  && soapaction.indexOf("ContentDirectory:1#GetSearchCapabilities") > -1) {
            answerContent.append(XML_HEADER);
            answerContent.append(CRLF);
            answerContent.append(SOAP_ENCODING_HEADER);
            answerContent.append(CRLF);
            answerContent.append(SEARCHCAPS_RESPONSE);
            answerContent.append(CRLF);
            answerContent.append(SOAP_ENCODING_FOOTER);
            answerContent.append(CRLF);

            emit answerReady(answerHeader, answerContent.toUtf8());

            setStatus("OK");

        } else if (!soapaction.isEmpty() && (soapaction.contains("ContentDirectory:1#Browse") || soapaction.contains("ContentDirectory:1#Search"))) {

            // read the content of the received request
            QDomDocument doc;
            doc.setContent(content);

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

            QList<DlnaResource*> files = rootFolder->getDLNAResources(objectID,
                                                                      !browseFlag.isNull() && browseFlag == "BrowseDirectChildren",
                                                                      startingIndex,
                                                                      requestCount,
                                                                      searchCriteria);

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
                parentFolder = files.at(0)->getParent();
            }

            // set number of total results
            QDomElement totalMatches = xml.createElement("TotalMatches");
            browse_search.appendChild(totalMatches);
            if (!browseFlag.isNull() && browseFlag == "BrowseDirectChildren") {
                if (parentFolder != 0) {
                    totalMatches.appendChild(xml.createTextNode(QString("%1").arg(parentFolder->getChildren().size() - minus)));
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
            emit answerReady(answerHeader, xml.toString(-1).toUtf8());

            setStatus("OK");
        } else {
            setStatus("KO");
        }
    }
    else if ((method == "GET" || method == "HEAD") && argument.startsWith("get/")) {
        // Request to retrieve a file
        QStringList answerHeader;

        /**
        * Skip the leading "get/" and extract the resource ID from the first path element
        * e.g. "get/0$1$5$3$4/Foo.mp4" -> "0$1$5$3$4"
        */
        QRegExp rxId("get/([\\d$]+)/(.+)");
        QString id;
        QString fileName;
        if (rxId.indexIn(argument) != -1) {
            id = rxId.capturedTexts().at(1);
            fileName = rxId.capturedTexts().at(2);
        }

        // Some clients escape the separators in their request: unescape them.
        id = id.replace("%24", "$");

        // Retrieve the DLNAresource itself.
        QList<DlnaResource*> files = rootFolder->getDLNAResources(id, false, 0, 0, "");

        if (!transferMode.isNull()) {
            answerHeader << "TransferMode.DLNA.ORG: " + transferMode;
        }

        if (files.size() == 1) {
            // DLNAresource was found.
            DlnaResource* dlna = files.at(0);

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
                answerHeader << "Server: " + servername;

                QByteArray answerContent = dlna->getByteAlbumArt();
                if (answerContent.isNull()) {
                    log->ERROR("Unable to get thumbnail: " + dlna->getDisplayName());
                    setStatus("KO");
                } else {
                    emit answerReady(answerHeader, answerContent);
                    setStatus("OK");
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

                if (!contentFeatures.isNull()) {
                    answerHeader << "ContentFeatures.DLNA.ORG: " + dlna->getDlnaContentFeatures();
                }

                if (!mediaInfoSec.isNull()) {
//                    answerHeader << QString("MediaInfo.sec: SEC_Duration=%1").arg(dlna->getLengthInSeconds());
                }

                answerHeader << "Accept-Ranges: bytes";

                answerHeader << "Connection: keep-alive";

                answerHeader << "Server: " + servername;

                if (method == "HEAD") {
                    emit answerReady(answerHeader, QByteArray(""), dlna->size());
                    setStatus("OK");

                } else {

                    if (!dlna->toTranscode()) {
                        // get stream file
                        streamContent = dlna->getStream();

                        if (streamContent == 0) {
                            // No inputStream indicates that transcoding / remuxing probably crashed.
                            log->ERROR("There is no inputstream to return for " + dlna->getDisplayName());
                            setStatus("KO");
                        } else {
                            emit answerReady(answerHeader, QByteArray(), dlna->size());
                            log->INFO("Serving " + dlna->getDisplayName());
                        }

                    } else {
                        // transcode file
                        emit startTranscoding(dlna, answerHeader);
                    }
                }
            }
        }
    } else if (method == "SUBSCRIBE" && !soapaction.isEmpty()) {
        QStringList answerHeader;
        answerHeader << CONTENT_TYPE_UTF8;
        answerHeader << "Content-Length: 0";
        answerHeader << "Connection: close";
        answerHeader << QString("SID: uuid:%1").arg(uuid);
        answerHeader << "Server: " + servername;
        answerHeader << "Timeout: Second-1800";
        answerHeader << "";

        keepSocketOpened = true;
        emit answerReady(answerHeader);
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
            sendLine(&sock, "NOTIFY /" + argument + " HTTP/1.1");
            sendLine(&sock, QString("SID: uuid:%1").arg(uuid));
            sendLine(&sock, "SEQ: 0");
            sendLine(&sock, "NT: upnp:event");
            sendLine(&sock, "NTS: upnp:propchange");
            sendLine(&sock, QString("HOST: %1:%2").arg(addr).arg(port));
            sendLine(&sock, CONTENT_TYPE_UTF8);
        }
        else {
            log->ERROR(QString("Cannot connect to %1").arg(cb));
            setStatus("ERROR");
            return;
        }

        sock.close();

        QString answerContent;
        keepSocketOpened = false;

        if (argument.contains("connection_manager")) {
            QStringList answerHeader;
            answerHeader << "Server: " + servername;

            answerContent.append(EVENT_Header.arg("urn:schemas-upnp-org:service:ConnectionManager:1"));
            answerContent.append(EVENT_Prop.arg("SinkProtocolInfo").arg(""));
            answerContent.append(EVENT_Prop.arg("SourceProtocolInfo").arg(""));
            answerContent.append(EVENT_Prop.arg("CurrentConnectionIDs").arg(""));
            answerContent.append(EVENT_FOOTER);

            emit answerReady(answerHeader, answerContent.toUtf8());

            setStatus("OK");

        } else if (argument.contains("content_directory")) {
            QStringList answerHeader;
            answerHeader << "Server: " + servername;

            answerContent.append(EVENT_Header.arg("urn:schemas-upnp-org:service:ContentDirectory:1"));
            answerContent.append(EVENT_Prop.arg("TransferIDs").arg(""));
            answerContent.append(EVENT_Prop.arg("ContainerUpdateIDs").arg(""));
            answerContent.append(EVENT_Prop.arg("SystemUpdateID").arg(1));
            answerContent.append(EVENT_FOOTER);

            emit answerReady(answerHeader, answerContent.toUtf8());

            setStatus("OK");
        } else {
            closeClient();
            setStatus("Unknown argument");
        }
    }
    else if ((method == "GET" || method == "HEAD") && (argument.toLower().endsWith(".png") || argument.toLower().endsWith(".jpg") || argument.toLower().endsWith(".jpeg"))) {
        QStringList answerHeader;

        if (argument.toLower().endsWith(".png")) {
            answerHeader << "Content-Type: image/png";
        } else {
            answerHeader << "Content-Type: image/jpeg";
        }

        answerHeader << "Accept-Ranges: bytes";
        answerHeader << "Connection: keep-alive";
//        answerHeader << "Expires: " + getFUTUREDATE() + " GMT";
        answerHeader << "Server: " + servername;

        QByteArray answerContent;

        // read the image file
        QFile inputStream(":/" + argument);
        if (inputStream.open(QFile::ReadOnly)) {
            answerContent = inputStream.readAll();
            inputStream.close();

        }
        else {
            log->ERROR(QString("Unable to read %1 for %2 answer.").arg(argument).arg(method));
        }

        emit answerReady(answerHeader, answerContent);

        setStatus("OK");
    }
    else {
        log->ERROR("Unkown answer for: " + method + " " + argument);
        setStatus("KO");
    }
}

void Request::runTranscoding(DlnaResource* dlna, QStringList answerHeader) {
    if (transcodeProcess == 0) {

        transcodeProcess = dlna->getTranscodeProcess(range);

        if (transcodeProcess == 0) {
            log->ERROR(QString("Cannot create transcoding process"));
            setStatus("KO");

        } else {
            // send header answer
            emit answerReady(answerHeader, QByteArray(), dlna->size());

            log->INFO(QString("Start transcoding (%1)").arg(dlna->getDisplayName()));

            connect(transcodeProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(receivedTranscodedData()));
            connect(transcodeProcess, SIGNAL(readyReadStandardError()), this, SLOT(receivedTranscodingLogMessage()));
            connect(transcodeProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(errorTrancodedData(QProcess::ProcessError)));
            connect(transcodeProcess, SIGNAL(finished(int)), this, SLOT(finishedTranscodeData(int)));

            transcodeClock.start();

            transcodeProcess->start();
            if (!transcodeProcess->waitForStarted()) {
                log->ERROR(transcodeProcess->errorString());
                setStatus("KO");
            } else {
                // transcoding process started
                log->INFO("Serving " + dlna->getDisplayName());
                setStatus("Transcoding started");
            }
        }
    } else {
        log->ERROR(QString("Transcoding already in progress"));
        setStatus("KO");
    }
}

void Request::waitTranscodingFinished() {
    if (transcodeProcess != 0) {
        // wait until process is finished
        transcodeProcess->waitForFinished(-1);
    }
}

void Request::receivedTranscodingLogMessage() {
    if (transcodeProcess != 0) {
        // error output occurs
        QString msg = transcodeProcess->readAllStandardError();
        log->INFO(QString("Transcoding output: %1").arg(msg));
    }
}

void Request::errorTrancodedData(QProcess::ProcessError error) {
    Q_UNUSED(error);

    // trancoding failed
    if (transcodeProcess != 0) {
        // an error occured
        log->ERROR(QString("Transcoding failed (%1).").arg(transcodeProcess->errorString()));
    }
}

void Request::receivedTranscodedData() {
    if (transcodeProcess != 0) {
        // read transcoded data
        QByteArray bytes = transcodeProcess->readAllStandardOutput();
        if (client != 0) {
            // send data to client
            if (client->write(bytes) == -1) {
                log->ERROR("Unable to send transcoded data to client.");
            }

            client->flush();
        } else {
            log->ERROR("Unable to send transcoded data to client (client deleted).");
        }
    }
}

void Request::finishedTranscodeData(int exitCode) {
    if (transcodeProcess != 0) {
        log->DEBUG(QString("TRANSCODE FINISHED with exitCode %2 (%1)").arg(transcodeProcess->arguments().join(",")).arg(exitCode));
        log->INFO(QString("TRANCODING done in %1 ms").arg(transcodeClock.elapsed()));

        transcodeProcess->deleteLater();
        transcodeProcess = 0;
    }

    if (exitCode != 0) {
        // trancoding failed
        setStatus("Transcoding failed.");
    } else {
        setStatus("OK");
    }

    closeClient();
}

void Request::readSocket() {

    if (client == 0) {
        log->ERROR("data received but client is deleted.");

    } else {
        bool flagHeaderReading = true;

        if (header.isEmpty()) {
            // read the header
            foreach (QString headerLine, client->readAll().split('\n')) {
                if (flagHeaderReading && !headerLine.isEmpty() && headerLine != "\r") {
                    log->TRACE("Request: Received on socket: " + headerLine);
                    appendHeader(headerLine);
                } else {
                    if (flagHeaderReading) {
                        flagHeaderReading = false;
                    } else if (!headerLine.isEmpty()) {
                        content.append(headerLine).append('\n');
                    }
                }
            }

            setStatus("header read");

            if (!getUserAgent().isEmpty())
            {
                log->DEBUG("HTTP User-Agent: " + getUserAgent());
            }

            log->DEBUG("HTTP: " + getMethod() + " " + getArgument() + " / " + QString("%1").arg(getLowRange()) + "-" + QString("%1").arg(getHighRange()));

            if ((getReceivedContentLength() <= 0) || (content.size() == getReceivedContentLength())) {
                // no content expected to received

                // prepare and send answer
                start();
            }

        } else {
            // read the content
            content.append(client->readAll());

            if (content.size() == getReceivedContentLength()) {
                setStatus("content read");
                log->TRACE("Bytes Read: " + QString("%1").arg(content.size()));
                log->TRACE("Data Read: " + content);

                // prepare and send answer
                start();
            }
        }
    }
}

void Request::bytesSent(qint64 size) {
    Q_UNUSED(size)

    if (streamContent != 0) {

        if (range != 0 && streamContent->pos() == 0) {
            if (range->getStartByte() > 0) {
                streamContent->seek(range->getStartByte());
            }
        }

        if (streamContent->size()-streamContent->pos() > 0) {

            setStatus(QString("Streaming (%1%)").arg(int(100.0*double(streamContent->pos())/double(streamContent->size()))));

            if (client == 0) {
                log->ERROR("HTTP Request: Unable to send content (client deleted).");

                delete streamContent;
                streamContent = 0;

                setStatus("KO");
            } else {
                if (client->bytesToWrite() == 0) {

                    int maxBytesToRead = 1024;
                    int bytesToRead = maxBytesToRead;
                    if (range != 0 && range->getEndByte() > 0) {
                        if (range->getEndByte() > streamContent->pos()) {
                            bytesToRead = range->getEndByte() - streamContent->pos();
                            if (bytesToRead > maxBytesToRead) {
                                bytesToRead = maxBytesToRead;
                            }
                        } else {
                            bytesToRead = 0;
                        }
                    }

                    // read the stream
                    char bytesToSend[bytesToRead];
                    qint64 bytes = streamContent->read(bytesToSend, sizeof(bytesToSend));

                    if (bytes == -1) {
                        log->ERROR("HTTP Request: Unable to send content.");

                        delete streamContent;
                        streamContent = 0;

                        setStatus("KO");

                        closeClient();

                    } else if (bytes == 0) {
                        // stream ended

                        delete streamContent;
                        streamContent = 0;

                        setStatus("OK");

                        closeClient();

                    } else {
                        if (client->write(bytesToSend, sizeof(bytesToSend))== -1) {
                            log->ERROR("HTTP Request: Unable to send content.");

                            delete streamContent;
                            streamContent = 0;

                            setStatus("KO");

                            closeClient();
                        }
                    }
                }
            }
        } else {
            // stream ended

            delete streamContent;
            streamContent = 0;

            setStatus("OK");

            closeClient();
        }
    }
}

void Request::stateChanged(QAbstractSocket::SocketState state) {
    if (state == QAbstractSocket::UnconnectedState) {
        setNetworkStatus("disconnected");

        if (streamContent != 0) {
            setStatus("Streaming aborted.");
            delete streamContent;
            streamContent = 0;
        }

        client->deleteLater();
        client = 0;

    } else if (state == QAbstractSocket::ClosingState) {
        setNetworkStatus("closed");
    }
}

void Request::errorSocket(QAbstractSocket::SocketError error) {
    log->ERROR("error occurs with network interface: " + client->errorString());

    if (error == QAbstractSocket::RemoteHostClosedError) {
        if (transcodeProcess != 0) {
            log->INFO("Kill transcoding process.");
            transcodeProcess->kill();
        }
    }
}

void Request::closeClient() {
    if (!keepSocketOpened && (transcodeProcess == 0) && (streamContent == 0)) {
        // No transcoding in progress
        log->TRACE("Close connection");
        if (client != 0) {
            client->close();
        } else {
            log->ERROR("Unable to close client (client deleted).");
        }

        setDuration(QString("%1 ms").arg(clock.elapsed()));
    }
}
