#include "request.h"

const QString Request::CRLF = "\r\n";


Request::Request(Logger* log, QTcpSocket* client, QString uuid, QString servername, QString host, int port, QObject *parent):
    QObject(parent),
    log(log),
    m_client(client),
    replyNumber(0),
    replyInProgress(false),  // by default no reply is in progress, we wait a request
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
    m_content(""),
    range(0),
    timeSeekRangeStart(-1),
    timeSeekRangeEnd(-1),
    http10(false)
{
    clock.invalidate();

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
        connect(client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(clientError(QAbstractSocket::SocketError)));
        connect(client, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));
        connect(client, SIGNAL(destroyed()), this, SLOT(clientDestroyed()));
    }

    log->Trace("Request: receiving a request from " + getpeerAddress());
}

Request::~Request() {
    if (range)
        delete range;
}

void Request::setArgument(const QString &argument)
{
    m_argument = argument;

    if (argument.startsWith("/"))
    {
        log->Trace("Stripping preceding slash from: " + argument);
        m_argument = m_argument.remove(0, 1);
    }

    emit dataChanged("argument");
}

QString Request::getParamHeader(const QString &param) const
{
    if (m_params.contains(param))
        return m_params[param];
    return QString();
}

void Request::setParamHeader(const QString &param, const QString &value)
{
    if (m_params.contains(param))
        log->Error(QString("Param %1 is defined several times in header.").arg(param));
    else
        m_params[param] = value;
}

int Request::getReceivedContentLength() const
{
    QString value = getParamHeader("CONTENT-LENGTH");
    if (!value.isEmpty())
        return value.toInt();
    return -1;
}

bool Request::appendHeader(const QString &headerLine)
{
    // add the line to header list
    m_header.append(headerLine);
    emit dataChanged("header");

    QRegExp rxDefinition("(GET|HEAD|POST|SUBSCRIBE)\\s*(\\S+)\\s*HTTP/(\\S+)");
    QRegExp rxParam("(\\S+)\\s*:\\s*(.+)");

    if (rxDefinition.indexIn(headerLine.trimmed()) != -1)
    {
        setMethod(rxDefinition.cap(1));
        setArgument(rxDefinition.cap(2));
        if (rxDefinition.cap(3)=="1.0")
            setHttp10(true);
    }
    else if (rxParam.indexIn(headerLine.trimmed()) != -1)
    {
        QString param = rxParam.cap(1).trimmed().toUpper();
        if (param == "RANGE")
        {
            if (range)
            {
                // range already read, ignore it
                log->Error("A range has already been read in the request");
                return false;
            }
            else
            {
                range = new HttpRange(headerLine);
                if (range->isNull()) {
                    // invalid range, ignore it
                    delete range;
                    range = 0;
                    log->Error("Invalid range in request: " + headerLine);
                    return false;
                }
            }
        }
        else if (param == "TIMESEEKRANGE.DLNA.ORG")
        {
            QRegExp rxTimeSeekRange("timeseekrange\\.dlna\\.org:\\s*npt\\s*=\\s*(\\d+)\\-?(\\d*)", Qt::CaseInsensitive);
            if (rxTimeSeekRange.indexIn(headerLine) != -1)
            {
                if (!rxTimeSeekRange.cap(1).isEmpty())
                    timeSeekRangeStart = rxTimeSeekRange.cap(1).toLong();

                if (!rxTimeSeekRange.cap(2).isEmpty())
                    timeSeekRangeEnd = rxTimeSeekRange.cap(2).toLong();
            }
            else
            {
                log->Error(QString("Invalid param TimeSeekRange %1").arg(headerLine.trimmed()));
                return false;
            }
        }
        else
        {
            QString value = rxParam.cap(2).trimmed();
            setParamHeader(param, value);

            if (param=="CALLBACK")
                setParamHeader("SOAPACTION", value);
        }
    }
    else
    {
        qWarning() << "ERROR" << headerLine;
        return false;
    }

    return true;
}

void Request::readSocket()
{
    if (!clock.isValid())
    {
        clock.start();      // start clock to measure time taken to answer to the request

        // initialize data
        setMethod(QString());
        setArgument(QString());
        setTextContent(QString());
        if (range)
        {
            delete range;
            range = 0;
        }
        timeSeekRangeStart = -1;
        timeSeekRangeEnd = -1;
        setHttp10(false);
        setDate(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz"));
    }

    QTcpSocket* client = (QTcpSocket*)sender();

    if (client == 0) {
        log->Error("data received but client is deleted.");

    } else if (!replyInProgress) {
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
                            emit newRenderer(log, getpeerAddress(), getPort(), getParamHeader("USER-AGENT"));
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
                replyInProgress = true;
                emit readyToReply();
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
                replyInProgress = true;
                emit readyToReply();
            }
        }
    } else {
        qWarning() << "unable to read request, a reply is in progress.";
    }
}

void Request::stateChanged(const QAbstractSocket::SocketState &state)
{
    if (state == QAbstractSocket::UnconnectedState)
        setNetworkStatus("disconnected");
    else if (state == QAbstractSocket::ClosingState)
        setNetworkStatus("closed");
    else
        setNetworkStatus(QString("State %1").arg(state));
}

void Request::replyFinished()
{
    if (clock.isValid())
    {
        setDuration(clock.elapsed());
        clock.invalidate();

        // initialize to get new request
        replyNumber++;
        appendLog(QString("%1: REPLY %2 FINISHED"+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(replyNumber));
        replyInProgress = false;
        flagHeaderReading = true;
        m_header.append(QString("%1 **************************"+CRLF).arg(replyNumber)); emit dataChanged("header");
        m_params.clear();
        appendAnswer(QString("%1 **************************"+CRLF).arg(replyNumber));
    }
    else
    {
        appendLog(QString("%1: Reply finished but not yet started"+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
    }
}
