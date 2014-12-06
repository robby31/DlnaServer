#include "request.h"

const QString Request::CRLF = "\r\n";

const QString Request::HTTP_200_OK = "HTTP/1.1 200 OK";
const QString Request::HTTP_500 = "HTTP/1.1 500 Internal Server Error";
const QString Request::HTTP_206_OK = "HTTP/1.1 206 Partial Content";
const QString Request::HTTP_200_OK_10 = "HTTP/1.0 200 OK";
const QString Request::HTTP_206_OK_10 = "HTTP/1.0 206 Partial Content";

Request::Request(Logger* log, QThread *worker, qintptr socketDescriptor, QString uuid, QString servername, QString host, int port, QObject *parent):
    LogObject(log, parent),
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
    socket(socketDescriptor),
    m_content(""),
    range(0),
    timeSeekRangeStart(-1),
    timeSeekRangeEnd(-1),
    http10(true)
{
    this->moveToThread(worker);

    clock.invalidate();

    setDate(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz"));
    setStatus("init");
    setHost(host);
    setNetworkStatus("connected");

    connect(this, SIGNAL(newSocketDescriptor()), this, SLOT(createTcpSocket()));
    emit newSocketDescriptor();

    logTrace("Request: receiving a request from " + getpeerAddress());
}

Request::~Request() {
    if (range)
        delete range;
}

void Request::createTcpSocket()
{
    HttpClient *client = new HttpClient(log(), this);

    if (!client->setSocketDescriptor(socket))
    {
        logError(QString("unable to create TCPSOCKET (%1): %2").arg(socket).arg(client->errorString()));
    }
    else
    {
        setPeerAddress(client->peerAddress().toString());

        if (client->isOpen()) {
            setNetworkStatus("opened");
        } else {
            setNetworkStatus("not connected");
        }

        connect(this, SIGNAL(closeClientSignal()), client, SLOT(closeClient()));
        connect(this, SIGNAL(http10Signal(bool)), client, SLOT(setHttp10(bool)));
        connect(this, SIGNAL(connectionTypeSignal(QString)), client, SLOT(setConnectionType(QString)));
        connect(client, SIGNAL(appendLogSignal(QString)), this, SLOT(appendLog(QString)));
        connect(client, SIGNAL(appendAnswerSignal(QString)), this, SLOT(appendAnswer(QString)));
        connect(client, SIGNAL(readyRead()), this, SLOT(readSocket()));
        connect(this, SIGNAL(sendDataSignal(QByteArray)), client, SLOT(sendData(QByteArray)));
        connect(this, SIGNAL(sendHeaderSignal(QString, QHash<QString,QString>)), client, SLOT(sendHeader(QString, QHash<QString,QString>)));
        connect(this, SIGNAL(sendTextLineSignal(QString)), client, SLOT(sendTextLine(QString)));
        connect(client, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));
        connect(client, SIGNAL(disconnected()), this, SIGNAL(clientDisconnected()));
        connect(client, SIGNAL(bytesSent(qint64,qint64)), this, SIGNAL(bytesSent(qint64,qint64)));
        connect(this, SIGNAL(deleteClient()), client, SLOT(deleteLater()));
    }
}

void Request::setArgument(const QString &argument)
{
    m_argument = argument;

    if (argument.startsWith("/"))
    {
        logTrace("Stripping preceding slash from: " + argument);
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
        logError(QString("Param %1 is defined several times in header.").arg(param));
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
                logError("A range has already been read in the request");
                return false;
            }
            else
            {
                range = new HttpRange(headerLine);
                if (range->isNull()) {
                    // invalid range, ignore it
                    delete range;
                    range = 0;
                    logError("Invalid range in request: " + headerLine);
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
                logError(QString("Invalid param TimeSeekRange %1").arg(headerLine.trimmed()));
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

    HttpClient* client = qobject_cast<HttpClient*>(sender());

    if (client == 0) {
        logError("data received but client is deleted.");

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
                            MediaRenderer *renderer = new MediaRenderer(getpeerAddress(), getPort(), getParamHeader("USER-AGENT"));
                            emit newRenderer(renderer);
                        }
                    } else {
                        logTrace("Request: Received on socket: " + headerLine);
                        appendHeader(headerLine);
                    }
                } else {
                    if (getReceivedContentLength() <= 0)
                        logError("Unable to detect end request (content length is invalid).");

                    // read the content
                    m_content.append(headerLine);
                    emit dataChanged("content");
                }
            }

            if (flagHeaderReading)
                logError("HEADER NOT READ");

            if ((getReceivedContentLength() <= 0) || (m_content.size() == getReceivedContentLength()))
                requestReceived();

        } else {
            // read the content
            m_content.append(client->readAll());
            emit dataChanged("content");

            if (getReceivedContentLength() <= 0) {
                logError("Unable to detect end request (content length is invalid).");

            } else if (m_content.size() == getReceivedContentLength())
            {
                setStatus("content read");
                logTrace("Bytes Read: " + QString("%1").arg(m_content.size()));
                logTrace("Data Read: " + m_content);

                requestReceived();
            }
        }
    } else {
        qWarning() << QString("unable to read request (socket %1), a reply is in progress.").arg(socket).toUtf8().constData();
    }
}

void Request::requestReceived()
{
    // prepare and send answer
    emit http10Signal(isHttp10());
    emit connectionTypeSignal(getHttpConnection());

    replyInProgress = true;

    emit readyToReply();
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

void Request::sendHeader(const QHash<QString, QString> &header)
{
    QString httpType;
    if (getRange())
    {
        // partial content requested (range is present in the header)
        httpType = isHttp10() ? HTTP_206_OK_10 : HTTP_206_OK;

    } else {
        if (getSoapaction().contains("X_GetFeatureList")) {
            //  If we don't return a 500 error, Samsung 2012 TVs time out.
            httpType = HTTP_500;
        } else {
            httpType = isHttp10() ? HTTP_200_OK_10 : HTTP_200_OK;
        }
    }

    emit sendHeaderSignal(httpType, header);
}
