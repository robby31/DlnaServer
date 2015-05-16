#include "httpserver.h"
#include "request.h"
#include <QRegExp>
#include <QStringList>
#include <QNetworkInterface>
#include <QHostInfo>

const QString HttpServer::UUID = "cdc79bcf-6985-4baf-b974-e83846efd903";

const QString HttpServer::SERVERNAME = "QMS/1.0";

const int HttpServer::SERVERPORT = 5002;

HttpServer::HttpServer(Logger* log, QObject *parent):
    QTcpServer(parent),
    m_log(log),
    upnp(m_log, this),
    hostaddress(),
    serverport(SERVERPORT),
    workerNetwork(this),
    workerTranscoding(this),
    database(QSqlDatabase::addDatabase("QSQLITE")),
    netManager()
{
    if (!m_log)
        qWarning() << "log is not available for" << this;

    connect(m_log, SIGNAL(destroyed()), this, SLOT(_logDestroyed()));

    upnp.setUuid(UUID);
    upnp.setServerName(SERVERNAME);

    connect(this, SIGNAL(startSignal()), this, SLOT(_startServer()));

    connect(this, SIGNAL(acceptError(QAbstractSocket::SocketError)),
            this, SLOT(_newConnectionError(QAbstractSocket::SocketError)));

    database.setDatabaseName("/Users/doudou/workspaceQT/DLNA_server/MEDIA.database");

    workerNetwork.setObjectName("Network, request, reply Thread");
    workerNetwork.start();

    workerTranscoding.setObjectName("Transcoding");
    workerTranscoding.start();

    netManager.moveToThread(&workerNetwork);
}

HttpServer::~HttpServer()
{
    // stop network thread
    workerNetwork.quit();
    if (!workerNetwork.wait(1000))
        logError("Unable to stop network, request and reply thread in HttpServer.");

    // stop transcoding thread
    workerTranscoding.quit();
    if (!workerTranscoding.wait(1000))
        logError("Unable to stop transcoding thread in HttpServer.");

    logTrace("Close HTTP server.");
    close();
}

void HttpServer::_startServer()
{
    if (isListening())
        stop();

    // read the LocalIpAddress
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address())
        {
            hostaddress = ipAddressesList.at(i).toString();
            break;
        }
    }

    // if we did not find one, use IPv4 localhost
    if (hostaddress.isNull())
        hostaddress = QHostAddress(QHostAddress::LocalHost).toString();

    if (hostaddress.isNull())
    {
        logError("HTTP server: unable to define host ip address.");
    }
    else
    {
        if (!listen(hostaddress, serverport))
        {
            logError("HTTP server: " + errorString());
        }
        else
        {
            logTrace("HTTP server: listen " + getHost().toString() + ":" + QString("%1").arg(getPort()));

            // initialize the root folder
            DlnaCachedRootFolder *rootFolder = new DlnaCachedRootFolder(m_log, &database, hostaddress.toString(), serverport, this);
            rootFolder->setNetworkAccessManager(&netManager);

            connect(this, SIGNAL(destroyed()), rootFolder, SLOT(deleteLater()));
            connect(this, SIGNAL(stopSignal()), rootFolder, SLOT(deleteLater()));

            connect(this, SIGNAL(addFolderSignal(QString)), rootFolder, SLOT(addFolderSlot(QString)));
            connect(rootFolder, SIGNAL(folderAddedSignal(QString)), this, SIGNAL(folderAdded(QString)));
            connect(rootFolder, SIGNAL(error_addFolder(QString)), this, SIGNAL(error_addFolder(QString)));

            connect(this, SIGNAL(addNetworkLinkSignal(QString)), rootFolder, SLOT(addNetworkLink(QString)));
            connect(rootFolder, SIGNAL(linkAdded(QString)), this, SIGNAL(linkAdded(QString)));
            connect(rootFolder, SIGNAL(error_addNetworkLink(QString)), this, SIGNAL(error_addNetworkLink(QString)));

            connect(this, SIGNAL(checkNetworkLinkSignal()), rootFolder, SLOT(checkNetworkLink()));

            connect(this, SIGNAL(incrementCounterPlayedSignal(QString)), rootFolder, SLOT(incrementCounterPlayed(QString)));
            connect(this, SIGNAL(updateMediaData(QString,QHash<QString,QVariant>)), rootFolder, SLOT(updateLibrary(QString,QHash<QString,QVariant>)));

            connect(this, SIGNAL(getDLNAResourcesSignal(QObject*, QString,bool,int,int,QString)), rootFolder, SLOT(requestDlnaResources(QObject*, QString,bool,int,int,QString)));
            connect(rootFolder, SIGNAL(dlnaResources(QObject*,QList<DlnaResource*>)), this, SIGNAL(dlnaResources(QObject*,QList<DlnaResource*>)));

            upnp.setServerUrl(getURL());
            upnp.start();

            emit serverStarted();
        }
    }
}

void HttpServer::incomingConnection(qintptr socketDescriptor)
{
    logTrace("HTTP server: new connection");
    emit createRequest(socketDescriptor, UUID, QString("%1").arg(SERVERNAME), getHost().toString(), getPort());
}

void HttpServer::createTcpSocket(Request *request)
{
    if (request)
    {
        HttpClient *client = new HttpClient(m_log);

        if (!client->setSocketDescriptor(request->socketDescriptor()))
        {
            logError(QString("unable to create TCPSOCKET (%1): %2").arg(request->socketDescriptor()).arg(client->errorString()));
            client->deleteLater();
        }
        else
        {
            qRegisterMetaType<QHash<QString, QString> >("QHash<QString, QString>>");
            qRegisterMetaType<QAbstractSocket::SocketState>("QAbstractSocket::SocketState");

            // connection between request and client
            connect(request, SIGNAL(closeClientSignal()), client, SLOT(closeClient()));
            connect(client, SIGNAL(appendLogSignal(QString)), request, SLOT(appendLog(QString)));
            connect(client, SIGNAL(appendAnswerSignal(QString)), request, SLOT(appendAnswer(QString)));
            connect(request, SIGNAL(sendDataSignal(QByteArray)), client, SLOT(sendData(QByteArray)));
            connect(request, SIGNAL(sendHeaderSignal(QHash<QString,QString>)), client, SLOT(sendHeader(QHash<QString,QString>)));
            connect(request, SIGNAL(sendTextLineSignal(QString)), client, SLOT(sendTextLine(QString)));
            connect(client, SIGNAL(stateChanged(QAbstractSocket::SocketState)), request, SLOT(stateChanged(QAbstractSocket::SocketState)));
            connect(client, SIGNAL(incomingRequest(QString,QStringList,bool,QString,QString,QHash<QString,QString>,QString,HttpRange*,int,int)), request, SLOT(requestReceived(QString,QStringList,bool,QString,QString,QHash<QString,QString>,QString,HttpRange*,int,int)));
            connect(client, SIGNAL(disconnected()), request, SIGNAL(clientDisconnected()));
            connect(client, SIGNAL(bytesSent(qint64,qint64)), request, SIGNAL(bytesSent(qint64,qint64)));
            connect(request, SIGNAL(deleteClient()), client, SLOT(deleteLater()));
        }

        client->moveToThread(&workerNetwork);
    }
}

void HttpServer::newRequest(Request *request)
{
    if (request)
    {
        // connection between request and httpserver
        connect(request, SIGNAL(readyToReply(QString,QString,QHash<QString,QString>,bool,QString,HttpRange*,int,int)), this, SLOT(_readyToReply(QString,QString,QHash<QString,QString>,bool,QString,HttpRange*,int,int)));
        connect(request, SIGNAL(newRenderer(QString,int,QString)), this, SIGNAL(newRenderer(QString,int,QString)));
        connect(request, SIGNAL(startServingRendererSignal(QString,QString)), this, SIGNAL(servingRenderer(QString,QString)));
        connect(request, SIGNAL(stopServingRendererSignal(QString)), this, SIGNAL(stopServingRenderer(QString)));
        connect(request, SIGNAL(deleteRequest(Request*)), this, SIGNAL(deleteRequest(Request*)));

        createTcpSocket(request);
    }
}

void HttpServer::_newConnectionError(const QAbstractSocket::SocketError &error) {
    logError(QString("HTTP server: error at new connection (%1).").arg(error));
}

void HttpServer::_readyToReply(const QString &method, const QString &argument, const QHash<QString, QString> &paramsHeader, const bool &http10, const QString &content, HttpRange *range, const int &timeSeekRangeStart, const int &timeSeekRangeEnd)
{
    Request* request = (Request*) sender();
    Reply* reply = 0;

    if ((method == "GET" || method == "HEAD") && argument.startsWith("get/"))
    {
        reply = new ReplyDlnaItemContent(m_log, &workerTranscoding, http10, method, argument, paramsHeader, content, range, timeSeekRangeStart, timeSeekRangeEnd, UUID, QString("%1").arg(SERVERNAME), getHost().toString(), getPort(), this);
        connect(reply, SIGNAL(startServingRendererSignal(QString)), request, SLOT(startServingRenderer(QString)));
        connect(reply, SIGNAL(stopServingRendererSignal()), request, SLOT(stopServingRenderer()));
        connect(reply, SIGNAL(servingSignal(QString,int)), this, SLOT(_servingProgress(QString,int)));
        connect(reply, SIGNAL(servingFinishedSignal(QString, int)), this, SLOT(_servingFinished(QString, int)));
        connect(request, SIGNAL(clientDisconnected()), reply, SLOT(close()));
    }
    else
    {
        reply = new Reply(m_log, http10, method, argument, paramsHeader, content, range, timeSeekRangeStart, timeSeekRangeEnd, UUID, QString("%1").arg(SERVERNAME), getHost().toString(), getPort(), this);
    }

    connect(request, SIGNAL(bytesSent(qint64,qint64)), reply, SIGNAL(bytesSent(qint64,qint64)));

    connect(reply, SIGNAL(getDLNAResourcesSignal(QString,bool,int,int,QString)), this, SLOT(requestDLNAResourcesSignal(QString,bool,int,int,QString)));
    connect(this, SIGNAL(dlnaResources(QObject*,QList<DlnaResource*>)), reply, SLOT(dlnaResources(QObject*,QList<DlnaResource*>)));

    connect(reply, SIGNAL(closeClientSignal()), request, SIGNAL(closeClientSignal()));
    connect(reply, SIGNAL(destroyed()), request, SIGNAL(deleteClient()));

    connect(reply, SIGNAL(sendTextLineToClientSignal(QString)), request, SIGNAL(sendTextLineSignal(QString)));
    connect(reply, SIGNAL(sendHeaderSignal(QHash<QString,QString>)), request, SIGNAL(sendHeaderSignal(QHash<QString,QString>)));
    connect(reply, SIGNAL(sendDataToClientSignal(QByteArray)), request, SIGNAL(sendDataSignal(QByteArray)));

    connect(reply, SIGNAL(appendAnswerSignal(QString)), request, SLOT(appendAnswer(QString)));
    connect(reply, SIGNAL(logTextSignal(QString)), request, SLOT(appendLog(QString)));

    connect(reply, SIGNAL(replyStatusSignal(QString)), request, SLOT(setStatus(QString)));
    connect(reply, SIGNAL(networkStatusSignal(QString)), request, SLOT(setNetworkStatus(QString)));

    connect(reply, SIGNAL(finishedSignal()), request, SLOT(replyFinished()));
    connect(reply, SIGNAL(finishedSignal()), reply, SLOT(deleteLater()));

    reply->run();
}

void HttpServer::_addFolder(const QString &folder)
{
    if (QFileInfo(folder).isDir())
        emit addFolderSignal(folder);
    else
        emit error_addFolder(folder);   // error folder is not a directory
}

void HttpServer::_servingProgress(const QString &filename, const int &playedDurationInMs)
{
    QHash<QString, QVariant> data;
    data.insert("last_played", QDateTime::currentDateTime());
    if (playedDurationInMs>0)
        data.insert("progress_played", playedDurationInMs);
    emit updateMediaData(filename, data);
}

void HttpServer::_servingFinished(const QString &filename, const int &status)
{
    if (status == 0)
        emit incrementCounterPlayedSignal(filename);
}

