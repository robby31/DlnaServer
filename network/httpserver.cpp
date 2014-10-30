#include "httpserver.h"
#include "request.h"
#include <QRegExp>
#include <QStringList>
#include <QNetworkInterface>
#include <QHostInfo>

const QString HttpServer::UUID = "cdc79bcf-6985-4baf-b974-e83846efd903";

const QString HttpServer::SERVERNAME = "Mac_OS_X-x86_64-10.9.1, UPnP/1.0, QMS/1.0";

const int HttpServer::SERVERPORT = 5002;

HttpServer::HttpServer(Logger* log, QObject *parent):
    QTcpServer(parent),
    m_log(log),
    upnp(m_log, this),
    hostaddress(),
    serverport(SERVERPORT),
    database(QSqlDatabase::addDatabase("QSQLITE")),
    rootFolder(0),
    batch(0),
    batchThread(this)
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

    batch = new BatchedRootFolder(rootFolder);
    batch->moveToThread(&batchThread);
    connect(&batchThread, SIGNAL(finished()), batch, SLOT(deleteLater()));
    connect(this, SIGNAL(batched_addFolder(QString)), batch, SLOT(addFolder(QString)));
    connect(batch, SIGNAL(progress(int)), this, SIGNAL(progressUpdate(int)));
    batchThread.start();
}

HttpServer::~HttpServer()
{
    // stop batch processes
    batchThread.quit();
    if (batch != 0)
        batch->quit();
    if (!batchThread.wait(1000))
        logError("Unable to stop batch process in HttpServer.");

    logTrace("Close HTTP server.");
    close();
}

void HttpServer::_startServer()
{
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
            if (rootFolder)
                rootFolder->deleteLater();
            rootFolder = new DlnaCachedRootFolder(m_log, &database, hostaddress.toString(), serverport, this);

            batch->setRootFolder(rootFolder);

            upnp.setServerUrl(getURL());
            upnp.start();

            emit serverStarted();
        }
    }
}

void HttpServer::incomingConnection(qintptr socketDescriptor)
{
    logTrace("HTTP server: new connection");

    Request *request = new Request(m_log,
                                   socketDescriptor,
                                   UUID, QString("%1").arg(SERVERNAME),
                                   getHost().toString(), getPort());

    connect(request, SIGNAL(readyToReply()), this, SLOT(_readyToReply()));
    connect(request, SIGNAL(newRenderer(MediaRenderer*)), this, SIGNAL(newRenderer(MediaRenderer*)));

    emit newRequest(request);
}

void HttpServer::_newConnectionError(const QAbstractSocket::SocketError &error) {
    logError(QString("HTTP server: error at new connection (%1).").arg(error));
}

void HttpServer::_readyToReply()
{
    Request* request = (Request*) sender();
    Reply* reply;

    if ((request->getMethod() == "GET" || request->getMethod() == "HEAD") && request->getArgument().startsWith("get/"))
    {
        reply = new ReplyDlnaItemContent(m_log, request, rootFolder);
        connect(reply, SIGNAL(servingRenderer(QString,QString)), this, SIGNAL(servingRenderer(QString,QString)));
        connect(reply, SIGNAL(stopServingRenderer(QString)), this, SIGNAL(stopServingRenderer(QString)));
        connect(reply, SIGNAL(serving(QString,int)), this, SLOT(_servingProgress(QString,int)));
        connect(reply, SIGNAL(servingFinished(QString, int)), this, SLOT(_servingFinished(QString, int)));
    }
    else
    {
        reply = new Reply(m_log, request, rootFolder);
    }

    reply->moveToThread(request->thread());

    connect(reply, SIGNAL(appendAnswer(QString)), request, SLOT(appendAnswer(QString)));
    connect(reply, SIGNAL(replyStatus(QString)), request, SLOT(setStatus(QString)));
    connect(reply, SIGNAL(logText(QString)), request, SLOT(appendLog(QString)));
    connect(reply, SIGNAL(finished()), request, SLOT(replyFinished()));
    connect(reply, SIGNAL(finished()), reply, SLOT(deleteLater()));

    reply->run();
}

void HttpServer::_addFolder(const QString &folder)
{
    if (QFileInfo(folder).isDir()) {
        if (rootFolder) {
            DlnaRootFolder *obj = rootFolder->getRootFolder();
            if (obj and obj->addFolder(folder)) {
                emit batched_addFolder(folder);
                emit folderAdded(folder);
            } else {
                emit error_addFolder(folder);
            }
        } else {
            emit error_addFolder(folder);
        }
    } else {
        // error folder is not a directory
        emit error_addFolder(folder);
    }
}

bool HttpServer::addNetworkLink(const QString url)
{
    if (rootFolder && rootFolder->addNetworkLink(url)) {
        emit linkAdded(url);
        return true;
    } else {
        emit error_addNetworkLink(url);
        return false;
    }
}

void HttpServer::_checkNetworkLink()
{
    int nb = 0;
    logInfo("CHECK NETWORK LINK started");

    if (rootFolder)
    {
        QSqlQuery query = rootFolder->getAllNetworkLinks();
        while (query.next()) {
            ++nb;
            if (!rootFolder->networkLinkIsValid(query.value("filename").toString()))
                logError(QString("link %1 is broken, title: %2").arg(query.value("filename").toString()).arg(query.value("title").toString()));
        }
    }

    logInfo(QString("%1 links checked.").arg(nb));
}

void HttpServer::_servingProgress(const QString &filename, const int &playedDurationInMs)
{
    if (rootFolder)
    {
        QHash<QString, QVariant> data;
        data.insert("last_played", QDateTime::currentDateTime());
        if (playedDurationInMs>0)
            data.insert("progress_played", playedDurationInMs);
        if (!rootFolder->updateLibrary(filename, data))
            logError(QString("HTTP SERVER: unable to update library for media %1").arg(filename));
    }
}

void HttpServer::_servingFinished(const QString &filename, const int &status)
{
    if (status==0 && rootFolder)
    {
        if (!rootFolder->incrementCounterPlayed(filename))
            logError(QString("HTTP SERVER: unable to update library for media %1").arg(filename));
    }
}

