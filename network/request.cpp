#include "request.h"

const QString Request::CRLF = "\r\n";

Request::Request(Logger* log, QThread *worker, qintptr socketDescriptor, QString uuid, QString servername, QString host, int port, QObject *parent):
    LogObject(log, parent),
    replyNumber(0),
    replyInProgress(false),  // by default no reply is in progress, we wait a request
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
    m_range(0),
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
        connect(client, SIGNAL(appendLogSignal(QString)), this, SLOT(appendLog(QString)));
        connect(client, SIGNAL(appendAnswerSignal(QString)), this, SLOT(appendAnswer(QString)));
        connect(this, SIGNAL(sendDataSignal(QByteArray)), client, SLOT(sendData(QByteArray)));
        connect(this, SIGNAL(sendHeaderSignal(QHash<QString,QString>)), client, SLOT(sendHeader(QHash<QString,QString>)));
        connect(this, SIGNAL(sendTextLineSignal(QString)), client, SLOT(sendTextLine(QString)));
        connect(client, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));
        connect(client, SIGNAL(newRequest(bool,QString,QString,QHash<QString,QString>,QString,HttpRange*,int,int)), this, SLOT(requestReceived(bool,QString,QString,QHash<QString,QString>,QString,HttpRange*,int,int)));
        connect(client, SIGNAL(disconnected()), this, SIGNAL(clientDisconnected()));
        connect(client, SIGNAL(bytesSent(qint64,qint64)), this, SIGNAL(bytesSent(qint64,qint64)));
        connect(this, SIGNAL(deleteClient()), client, SLOT(deleteLater()));
    }
}

QString Request::getParamHeader(const QString &param) const
{
    if (m_params.contains(param))
        return m_params[param];
    else
        return QString();
}

void Request::requestReceived(const bool &is_http10, const QString &method, const QString &argument, const QHash<QString, QString> &paramsHeader, const QString &content, HttpRange *range, const int &timeSeekRangeStart, const int &timeSeekRangeEnd)
{
    if (!replyInProgress)
    {
        replyInProgress = true;

        setHttp10(is_http10);
        setMethod(method);
        setArgument(argument);
        setTextContent(content);
        m_params = paramsHeader;
        m_range = range;
        this->timeSeekRangeStart = timeSeekRangeStart;
        this->timeSeekRangeEnd = timeSeekRangeEnd;

        emit readyToReply();

        if (getArgument() == "description/fetch") {
            // new renderer is connecting to server
            emit newRenderer(getpeerAddress(), getPort(), getParamHeader("USER-AGENT"));
        }
    }
    else
    {
        qWarning() << QString("unable to read request (socket %1), a reply is in progress.").arg(socketDescriptor()).toUtf8().constData();
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
        m_header.append(QString("%1 **************************"+CRLF).arg(replyNumber)); emit dataChanged("header");
        m_params.clear();
        appendAnswer(QString("%1 **************************"+CRLF).arg(replyNumber));
    }
    else
    {
        appendLog(QString("%1: Reply finished but not yet started"+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
    }

//    if (getStatus() == "OK")
//        emit deleteRequest(this);
}
