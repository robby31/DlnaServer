#include "httpclient.h"

const QString HttpClient::CRLF = "\r\n";

const QString HttpClient::HTTP_200_OK = "HTTP/1.1 200 OK";
const QString HttpClient::HTTP_500 = "HTTP/1.1 500 Internal Server Error";
const QString HttpClient::HTTP_206_OK = "HTTP/1.1 206 Partial Content";
const QString HttpClient::HTTP_200_OK_10 = "HTTP/1.0 200 OK";
const QString HttpClient::HTTP_206_OK_10 = "HTTP/1.0 206 Partial Content";

HttpClient::HttpClient(Logger *log, QObject *parent) :
    QTcpSocket(parent),
    m_log(log),
    m_http10(true),
    flagHeaderReading(true),
    m_header(),
    requestComplete(false),
    m_params(),
    m_method(),
    m_argument(),
    m_content(),
    range(0),
    timeSeekRangeStart(-1),
    timeSeekRangeEnd(-1),
    sizeWritten(0),
    timerDataSent()
{
    if (!m_log)
        qWarning() << "log is not available for" << this;

    timerDataSent.start();

    connect(this, SIGNAL(readyRead()), this, SLOT(readSocket()));
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(clientError(QAbstractSocket::SocketError)));
    connect(this, SIGNAL(bytesWritten(qint64)), this, SLOT(_bytesWritten(qint64)));
}

HttpClient::~HttpClient()
{
    if (range)
    {
        delete range;
        range = 0;
    }

    appendLogSignal(QString("%1: destroy client."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
}

void HttpClient::clear()
{
    m_http10 = true;
    flagHeaderReading = true;
    requestComplete = false;
    m_header.clear();
    m_params.clear();
    m_method.clear();
    m_argument.clear();
    m_content.clear();
    range = 0;
    timeSeekRangeStart = -1;
    timeSeekRangeEnd = -1;
}

void HttpClient::closeClient()
{
    if (isHttp10() or getHttpConnection().toLower() == "close")
    {
        if (socketDescriptor() == -1)
        {
            appendLogSignal(QString("%1: client already disconnected."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
        }
        else
        {
            appendLogSignal(QString("%2: Close client (%1)"+CRLF).arg(socketDescriptor()).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
            disconnectFromHost();
        }
    }
}

void HttpClient::sendTextLine(const QString &msg)
{
    QByteArray tmp;

    tmp.append(msg.toUtf8()).append(CRLF.toUtf8());

    if (write(tmp) == -1)
        logError("HTTP Request: Unable to send line: " + msg);

    appendAnswerSignal(msg+CRLF);

    logDebug("Wrote on socket: " + msg);
}

void HttpClient::sendHeader(const QHash<QString, QString> &header)
{
    logDebug("Send header.");
    appendLogSignal(QString("%1: Send header."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));

    QString httpType;
    if (range)
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

    sendTextLine(httpType.toUtf8());

    foreach (const QString &param, header.keys())
        sendTextLine(QString("%1: %2").arg(param).arg(header[param]).toUtf8());

    sendTextLine("");

//    flush();
}

void HttpClient::sendData(const QByteArray &data)
{
    // send the content
    if (write(data) == -1)
    {
        QString msg = QString("HTTP request: Unable to send content, %1").arg(errorString());
        logError(msg);
        appendLogSignal(QString("%1: %2"+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(msg));
    }
    else if (isLogLevel(DEBG))
    {
        QString msg = QString("HttpClient::sendData - bytes written=%1, bytes to write=%2"+CRLF).arg(data.size()).arg(bytesToWrite());
        logInfo(msg);
    }

//    flush();
}

void HttpClient::setArgument(const QString &argument)
{
    m_argument = argument;

    if (argument.startsWith("/"))
    {
        logTrace("Stripping preceding slash from: " + argument);
        m_argument = m_argument.remove(0, 1);
    }
}

QString HttpClient::getParamHeader(const QString &param) const
{
    if (m_params.contains(param))
        return m_params[param];
    return QString();
}

void HttpClient::setParamHeader(const QString &param, const QString &value)
{
    if (m_params.contains(param))
        logError(QString("Param %1 is defined several times in header.").arg(param));
    m_params[param] = value;
}

int HttpClient::getReceivedContentLength() const
{
    QString value = getParamHeader("CONTENT-LENGTH");
    if (!value.isEmpty())
        return value.toInt();
    return -1;
}

bool HttpClient::appendHeader(const QString &headerLine)
{
    m_header.append(headerLine);

    QRegExp rxDefinition("(GET|HEAD|POST|SUBSCRIBE)\\s*(\\S+)\\s*HTTP/(\\S+)");
    QRegExp rxParam("(\\S+)\\s*:\\s*(.+)");

    if (rxDefinition.indexIn(headerLine.trimmed()) != -1)
    {
        setMethod(rxDefinition.cap(1));
        setArgument(rxDefinition.cap(2));
        if (rxDefinition.cap(3)=="1.0")
            setHttp10(true);
        else
            setHttp10(false);
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

void HttpClient::readSocket()
{
    if (requestComplete)
        clear();   // remove old data from previous request, new request is coming

    if (flagHeaderReading)
    {
        // read the header
        while (canReadLine())
        {
            QString headerLine = readLine();
            if (flagHeaderReading) {
                // header has not been completely received, so read the header
                if (headerLine == CRLF)
                {
                    // header has been completely received
                    flagHeaderReading = false;
                } else
                {
                    logTrace("Request: Received on socket: " + headerLine);
                    appendHeader(headerLine);
                }
            } else
            {
                if (getReceivedContentLength() <= 0)
                    logError("Unable to detect end request (content length is invalid).");

                // read the content
                m_content.append(headerLine);
            }
        }

        if (flagHeaderReading)
            logError("HEADER NOT READ");

        if ((getReceivedContentLength() <= 0) || (m_content.size() == getReceivedContentLength()))
            requestReceived();

    } else {
        // read the content
        m_content.append(readAll());

        if (getReceivedContentLength() <= 0) {
            logError("Unable to detect end request (content length is invalid).");

        } else if (m_content.size() == getReceivedContentLength())
        {
            logTrace("Bytes Read: " + QString("%1").arg(m_content.size()));
            logTrace("Data Read: " + m_content);

            requestReceived();
        }
    }
}

void HttpClient::requestReceived()
{
    requestComplete = true;

    emit incomingRequest(peerAddress().toString(), m_header, m_http10, m_method, m_argument, m_params, m_content, range, timeSeekRangeStart, timeSeekRangeEnd);
}

void HttpClient::clientError(QAbstractSocket::SocketError error)
{
    emit appendLogSignal(QString("%2: Network Error: %1, %3"+CRLF).arg(error).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(errorString()));

    if (error == RemoteHostClosedError)
        deleteLater();
}

void HttpClient::_bytesWritten(const qint64 &size)
{
    sizeWritten += size;

//        qWarning() << QString("%1: %2 bytes sent, %4 total bytes sent, %3 bytes to write.").arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(size).arg(bytesToWrite()).arg(sizeWritten);

    if (timerDataSent.elapsed() > 500 or sizeWritten > bytesToWrite())
    {
        emit bytesSent(sizeWritten, bytesToWrite());
        sizeWritten = 0;
        timerDataSent.restart();
    }
}
