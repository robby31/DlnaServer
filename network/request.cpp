#include "request.h"

qint64 Request::objectCounter = 0;

const QString Request::CRLF = "\r\n";

Request::Request(QObject *parent):
    ListItem(parent),
    m_roles(),
    m_log(0),
    m_client(0)
{
    ++objectCounter;

    m_roles[methodRole] = "method";
    m_roles[argumentRole] = "argument";
    m_roles[hostRole] = "host";
    m_roles[peerAddressRole] = "peerAddress";
    m_roles[statusRole] = "status";
    m_roles[headerRole] = "header";
    m_roles[contentRole] = "content";
    m_roles[durationRole] = "duration";
    m_roles[dateRole] = "date";
    m_roles[answerRole] = "answer";
    m_roles[networkStatusRole] = "network_status";
    m_roles[transcodeLogRole] = "transcode_log";
}

Request::Request(Logger* log, HttpClient *client, QString uuid, QString servername, QString host, int port, QObject *parent):
    ListItem(parent),
    m_roles(),
    m_log(log),
    m_client(0),
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
    m_content(""),
    m_range(0),
    timeSeekRangeStart(-1),
    timeSeekRangeEnd(-1),
    http10(true)
{
    ++objectCounter;

    m_roles[methodRole] = "method";
    m_roles[argumentRole] = "argument";
    m_roles[hostRole] = "host";
    m_roles[peerAddressRole] = "peerAddress";
    m_roles[statusRole] = "status";
    m_roles[headerRole] = "header";
    m_roles[contentRole] = "content";
    m_roles[durationRole] = "duration";
    m_roles[dateRole] = "date";
    m_roles[answerRole] = "answer";
    m_roles[networkStatusRole] = "network_status";
    m_roles[transcodeLogRole] = "transcode_log";

    setDate(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz"));
    setStatus("init");
    setHost(host);
    setNetworkStatus("connected");
    setClient(client);

    logTrace("Request: receiving a request from " + data(peerAddressRole).toString());
}

Request::~Request()
{
    --objectCounter;
}

void Request::setClient(HttpClient *client)
{
    m_client = client;

    qRegisterMetaType<QHash<QString, QString> >("QHash<QString, QString>>");
    qRegisterMetaType<QAbstractSocket::SocketState>("QAbstractSocket::SocketState");

    connect(m_client, SIGNAL(destroyed()), this, SLOT(clientDestroyed()));
    connect(m_client, SIGNAL(appendLogSignal(QString)), this, SLOT(appendLog(QString)));
    connect(m_client, SIGNAL(appendAnswerSignal(QString)), this, SLOT(appendAnswer(QString)));
    connect(m_client, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));
    connect(m_client, SIGNAL(incomingRequest(QString,QStringList,bool,QString,QString,QHash<QString,QString>,QString,HttpRange*,int,int)), this, SLOT(requestReceived(QString,QStringList,bool,QString,QString,QHash<QString,QString>,QString,HttpRange*,int,int)));

    connect(m_client, SIGNAL(disconnected()), this, SIGNAL(clientDisconnected()));
}

void Request::clientDestroyed()
{
    m_client = 0;
}

QVariant Request::data(int role) const
{
    switch (role) {
    case methodRole:
        return m_method;

    case argumentRole:
        return m_argument;

    case hostRole:
        if (m_client)
            return QString("%1 (%2)").arg(m_host).arg(m_client->socketDescriptor());
        else
            return QString("%1 (-)").arg(m_host);

    case peerAddressRole:
        return m_peerAddress;

    case statusRole:
        return m_status;

    case headerRole:
        return m_header.join("");

    case contentRole:
        return m_content;

    case durationRole:
        return QTime(0, 0).addMSecs(m_duration).toString("hh:mm:ss.zzz");

    case dateRole:
        return m_date;

    case answerRole:
        return m_stringAnswer.join("");

    case networkStatusRole:
        return m_networkStatus;

    case transcodeLogRole:
        return requestLog;

    default:
        return QVariant::Invalid;

    }

    return QVariant::Invalid;
}

bool Request::setData(const QVariant &value, const int &role)
{
//    logInfo(QString("SET DATA %1 = %2").arg(QVariant::fromValue(m_roles[role]).toString()).arg(value.toString()));

    QVector<int> roles;
    roles << role;

    switch(role)
    {
    case methodRole:
        if (value.toString() != m_method)
        {
            m_method = value.toString();
            emit itemChanged(roles);
        }
        return true;

    case argumentRole:
        if (value.toString() != m_argument)
        {
            m_argument = value.toString();
            emit itemChanged(roles);
        }
        return true;

    case hostRole:
        if (value.toString() != m_host)
        {
            m_host = value.toString();
            emit itemChanged(roles);
        }
        return true;

    case peerAddressRole:
        if (value.toString() != m_peerAddress)
        {
            m_peerAddress = value.toString();
            emit itemChanged(roles);
        }
        return true;

    case networkStatusRole:
        if (value.toString() != m_networkStatus)
        {
            m_networkStatus = value.toString();
            emit itemChanged(roles);
        }
        return true;

    case durationRole:
        if (value.toLongLong() != m_duration)
        {
            m_duration = value.toLongLong();
            emit itemChanged(roles);
        }
        return true;

    case dateRole:
        if (value.toString() != m_date)
        {
            m_date = value.toString();
            emit itemChanged(roles);
        }
        return true;

    case contentRole:
        if (value.toString() != m_content)
        {
            m_content = value.toString();
            emit itemChanged(roles);
        }
        return true;

    case statusRole:
        if (value.toString() != m_status)
        {
            m_status = value.toString();
            emit itemChanged(roles);
        }
        return true;

    default:
        qWarning() << "Unable to set data" << value << role;
        return false;
    }
}

QString Request::getParamHeader(const QString &param) const
{
    if (m_params.contains(param))
        return m_params[param];
    else
        return QString();
}

void Request::requestReceived(const QString &peerAddress, const QStringList &header, const bool &is_http10, const QString &method, const QString &argument, const QHash<QString, QString> &paramsHeader, const QString &content, HttpRange *range, const int &timeSeekRangeStart, const int &timeSeekRangeEnd)
{
    if (!replyInProgress)
    {
        replyInProgress = true;
        clock.start();

        setPeerAddress(peerAddress);
        setHttp10(is_http10);
        setMethod(method);
        setArgument(argument);
        setTextContent(content);
        m_params = paramsHeader;
        m_range = range;
        m_header = header;
        this->timeSeekRangeStart = timeSeekRangeStart;
        this->timeSeekRangeEnd = timeSeekRangeEnd;

        emit readyToReply(method, argument, paramsHeader, isHttp10(), data(contentRole).toString(), getRange(), getTimeSeekRangeStart(), getTimeSeekRangeEnd());

        if (data(argumentRole) == "description/fetch") {
            // new renderer is connecting to server
            emit newRenderer(data(peerAddressRole).toString(), getPort(), getParamHeader("USER-AGENT"));
        }
    }
    else
    {
        if (m_client)
            qWarning() << QString("unable to read request (socket %1), a reply is in progress.").arg(m_client->socketDescriptor()).toUtf8().constData();
        else
            qWarning() << QString("unable to read request, a reply is in progress.");

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
        m_header.append(QString("%1 **************************"+CRLF).arg(replyNumber)); emit itemChanged(QVector<int>(1, headerRole));
        m_params.clear();
        appendAnswer(QString("%1 **************************"+CRLF).arg(replyNumber));
    }
    else
    {
        appendLog(QString("%1: Reply finished but not yet started"+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
    }

    if (m_networkStatus == "disconnected" && m_status == "OK")
        emit deleteRequest(this);
}
