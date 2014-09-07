#include "request.h"

const QString Request::CRLF = "\r\n";


Request::Request(Logger* log, QTcpSocket* client, QString uuid, QString servername, QString host, int port, QObject *parent):
    QObject(parent),
    log(log),
    m_client(client),
    flagHeaderReading(true), // by default the header has not been received
    m_status(),
    m_networkStatus(),
    m_duration(0),
    m_date(),
    uuid(uuid),
    servername(servername),
    m_host(),
    port(port),
    socket(-1),
    soapaction(""),
    m_content(""),
    receivedContentLength(-1),
    range(0),
    timeSeekRangeStart(-1),
    timeSeekRangeEnd(-1),
    http10(false)
{
    // start clock to measure time taken to answer to the request
    clock.start();

    setDate(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz"));
    setStatus("init");
    setHost(host);
    setNetworkStatus("connected");

    if (client) {
        setPeerAddress(client->peerAddress().toString());
        socket = client->socketDescriptor();

        if (client->isOpen()) {
            setNetworkStatus("opened");
        } else {
            setNetworkStatus("not connected");
        }

        connect(client, SIGNAL(readyRead()), this, SLOT(readSocket()));
        connect(client, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));
    }

    log->Trace("Request: receiving a request from " + getpeerAddress());
}

Request::~Request() {
    if (range)
        delete range;
}

void Request::setSoapaction(const QString &soapaction) {
    if (!getSoapaction().isEmpty()) {
        log->Warning(QString("SOAPACTION in request is overwritten (old value was %1)").arg(soapaction));
    }
    this->soapaction = soapaction;
}

bool Request::appendHeader(const QString &headerLine)
{
    // add the line to header list
    m_header.append(headerLine);
    emit dataChanged("header");

    // read the header line and update request attributes
    QStringList fields = headerLine.split(QRegExp("\\s+"));

    QRegExp rxHost("Host:\\s*(.+)", Qt::CaseInsensitive);
    QRegExp rxLanguage("Accept-language:\\s*(.+)", Qt::CaseInsensitive);
    QRegExp rxEncoding("Accept-Encoding:\\s*(.+)", Qt::CaseInsensitive);
    QRegExp rxAccept("Accept: \\s*(.+)", Qt::CaseInsensitive);
    QRegExp rxConnection("Connection:\\s*(\\S+)", Qt::CaseInsensitive);
    QRegExp rxContentType("Content-Type:\\s*(.+)", Qt::CaseInsensitive);
    QRegExp rxTimeOut("Timeout:\\s*(.+)", Qt::CaseInsensitive);
    QRegExp rxNt("Nt:\\s*(.+)", Qt::CaseInsensitive);
    QRegExp rxIcyMetadata("Icy-Metadata:\\s*(\\w+)", Qt::CaseInsensitive);
    QRegExp rxRange("RANGE: BYTES=(\\d+)-(\\d*)", Qt::CaseInsensitive);
    QRegExp rxContentLength("CONTENT-LENGTH:\\s*(\\d+)", Qt::CaseInsensitive);
    QRegExp rxTransferMode("transferMode\\.dlna\\.org:\\s*(\\w+)", Qt::CaseInsensitive);
    QRegExp rxContentFeatures("getcontentFeatures\\.dlna\\.org:\\s*(\\w+)", Qt::CaseInsensitive);
    QRegExp rxMediaInfoSec("getMediaInfo\\.sec:\\s*(\\w+)", Qt::CaseInsensitive);
    QRegExp rxCaptionInfoSec("getCaptionInfo\\.sec:\\s*(\\w+)", Qt::CaseInsensitive);
    QRegExp rxTimeSeekRange("timeseekrange\\.dlna\\.org:\\s*npt\\s*=\\s*(\\d+)\\-?(\\d*)", Qt::CaseInsensitive);

    if (fields.length() > 0) {

        if (fields[0] == "SUBSCRIBE" || fields[0] == "GET" || fields[0] == "POST" || fields[0] == "HEAD") {
            setMethod(fields[0]);
            if (fields.length() > 1) {
                // Samsung 2012 TVs have a problematic preceding slash that needs to be removed.
                if (fields[1].startsWith("/"))
                {
                    log->Trace("Stripping preceding slash from: " + fields[1]);
                    setArgument(fields[1].remove(0, 1));
                }
                else
                {
                    setArgument(fields[1]);
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
                log->Error("A range has already been read in the request");
            } else {
                range = new HttpRange(headerLine);
                if (range->isNull()) {
                    // invalid range, ignore it
                    delete range;
                    range = 0;
                    log->Error("Invalid range in request: " + headerLine);
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
        else if (rxCaptionInfoSec.indexIn(headerLine) != -1) {

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
            httpConnection = rxConnection.cap(1).toLower();
        }
        else if (rxContentType.indexIn(headerLine) != -1) {

        }
        else if (rxTimeOut.indexIn(headerLine) != -1) {

        }
        else if (rxNt.indexIn(headerLine) != -1) {

        }
        else if (rxIcyMetadata.indexIn(headerLine) != -1) {

        }
        else if (rxTimeSeekRange.indexIn(headerLine) != -1) {
            if (!rxTimeSeekRange.cap(1).isEmpty()) {
                timeSeekRangeStart = rxTimeSeekRange.cap(1).toLong();
            }

            if (!rxTimeSeekRange.cap(2).isEmpty()) {
                timeSeekRangeEnd = rxTimeSeekRange.cap(2).toLong();
            }
        }
        else {
            log->Error("Unknown headerLine in request: <" + headerLine + ">");
            return false;
        }
    }
    else {
        log->Error("Unknown headerLine in request: <" + headerLine + ">");
        return false;
    }

    return true;
}



void Request::readSocket()
{
    QTcpSocket* client = (QTcpSocket*)sender();

    if (client == 0) {
        log->Error("data received but client is deleted.");

    } else {
        if (flagHeaderReading) {
            // read the header
            while (client->canReadLine()) {
                QString headerLine = client->readLine();
                if (flagHeaderReading) {
                    // header has not been completely received, so read the header
                    if (headerLine == CRLF) {
                        // header has been completely received
                        flagHeaderReading = false;
                        setStatus("header read");
                        emit headerReady();

                        if (getArgument() == "description/fetch") {
                            // new renderer is connecting to server
                            emit newRenderer(log, getpeerAddress(), getPort(), getUserAgent());
                        }
                    } else {
                        log->Trace("Request: Received on socket: " + headerLine);
                        appendHeader(headerLine);
                    }
                } else {
                    if (getReceivedContentLength() <= 0)
                        log->Error("Unable to detect end request (content length is invalid).");

                    // read the content
                    m_content.append(headerLine);
                    emit dataChanged("content");
                }
            }

            if (flagHeaderReading)
                log->Error("HEADER NOT READ");

            if ((getReceivedContentLength() <= 0) || (m_content.size() == getReceivedContentLength())) {
                // prepare and send answer
                emit sendReply();
            }

        } else {
            // read the content
            m_content.append(client->readAll());
            emit dataChanged("content");

            if (getReceivedContentLength() <= 0) {
                log->Error("Unable to detect end request (content length is invalid).");

            } else if (m_content.size() == getReceivedContentLength())
            {
                setStatus("content read");
                log->Trace("Bytes Read: " + QString("%1").arg(m_content.size()));
                log->Trace("Data Read: " + m_content);

                // prepare and send answer
                emit sendReply();
            }
        }
    }
}

void Request::stateChanged(const QAbstractSocket::SocketState &state)
{
    if (state == QAbstractSocket::UnconnectedState) {
        setNetworkStatus("disconnected");

    } else if (state == QAbstractSocket::ClosingState)
        setNetworkStatus("closed");
}

void Request::endReply()
{
    if (clock.isValid())
    {
        setDuration(clock.elapsed());
        clock.invalidate();
    }
}
