#include "httpclient.h"

const QString HttpClient::CRLF = "\r\n";

HttpClient::HttpClient(Logger *log, QObject *parent) :
    QTcpSocket(parent),
    m_log(log),
    m_http10(true),
    m_connectionType()
{
    if (!m_log)
        qWarning() << "log is not available for" << this;

    connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(clientError(QAbstractSocket::SocketError)));
    connect(this, SIGNAL(bytesWritten(qint64)), this, SLOT(_bytesWritten(qint64)));
}

HttpClient::~HttpClient()
{
}

void HttpClient::closeClient()
{
    if (isHttp10() or connectionType() == "close")
    {
        if (socketDescriptor() == -1)
        {
            appendLogSignal(QString("%1: client already disconnected."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
        }
        else
        {
            disconnectFromHost();
            appendLogSignal(QString("%2: Close client (%1)"+CRLF).arg(socketDescriptor()).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
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

void HttpClient::sendHeader(const QString &httpType, const QHash<QString, QString> &header)
{
    logDebug("Send header.");
    appendLogSignal(QString("%1: Send header."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));

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
        logError("HTTP request: Unable to send content.");
    else if (isLogLevel(DEBG))
        appendLogSignal(QString("%1: bytes written?%2"+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(data.size()));

//    flush();
}
