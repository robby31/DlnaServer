#include "httpserver.h"
#include "request.h"
#include <QRegExp>
#include <QStringList>
#include <QNetworkInterface>
#include <QHostInfo>

const QString HttpServer::UUID = "cdc79bcf-6985-4baf-b974-e83846efd903";

const QString HttpServer::SERVERNAME = "Mac_OS_X-x86_64-10.9.1, UPnP/1.0, QMS/1.0";

const int HttpServer::SERVERPORT = 5002;

HttpServer::HttpServer(Logger* log, RequestListModel *requestsModel, MediaRendererModel* renderersModel, QObject *parent):
    QTcpServer(parent),
    requestsModel(requestsModel),
    renderersModel(renderersModel),
    m_log(log != 0 ? log : new Logger(this)),
    hostaddress(),
    serverport(SERVERPORT),
    database(QSqlDatabase::addDatabase("QSQLITE")),
    rootFolder(0),
    batch(0),
    batchThread(this)
{
    // read the LocalIpAddress
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            hostaddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (hostaddress.isNull())
        hostaddress = QHostAddress(QHostAddress::LocalHost).toString();

    connect(requestsModel, SIGNAL(newRequest(Request*)), this, SLOT(newRequest(Request*)));

    connect(this, SIGNAL(newConnection()), this, SLOT(acceptConnection()));

    connect(this, SIGNAL(acceptError(QAbstractSocket::SocketError)),
            this, SLOT(newConnectionError(QAbstractSocket::SocketError)));

    if (hostaddress.isNull()) {
        m_log->Error("HTTP server: unable to define host ip address.");
    }
    else {
        if (!listen(hostaddress, serverport))
            m_log->Error("HTTP server: " + errorString());
        else
            m_log->Trace("HTTP server: listen " + getHost().toString() + ":" + QString("%1").arg(getPort()));
    }

    // initialize the root folder
    database.setDatabaseName("/Users/doudou/workspaceQT/DLNA_server/MEDIA.database");
    rootFolder = new DlnaCachedRootFolder(log, &database, hostaddress.toString(), serverport, this);

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
    batchThread.wait();

    m_log->Trace("Close HTTP server.");
    close();
}

QString HttpServer::getURL() const {
    return "http://" + getHost().toString() + ":" + QString("%1").arg(getPort());
}

void HttpServer::acceptConnection()
{
    QTcpSocket *newConnection = nextPendingConnection();
    if (newConnection) {
        m_log->Trace("HTTP server: new connection");
        connect(newConnection, SIGNAL(disconnected()), newConnection, SLOT(deleteLater()));

        if (requestsModel) {
            requestsModel->createRequest(m_log,
                                         newConnection,
                                         UUID, QString("%1").arg(SERVERNAME),
                                         getHost().toString(), getPort());
        }
        else
        {
            m_log->Error("Unable to add new request (requestsModel is null).");
        }
    } else {
        m_log->Error("No new connection available.");
    }
}

void HttpServer::newRequest(Request *request)
{
    connect(request, SIGNAL(readyToReply()), this, SLOT(readyToReply()));
    connect(request, SIGNAL(newRenderer(Logger*,QString,int,QString)), this, SLOT(createRenderer(Logger*,QString,int,QString)));
}

void HttpServer::newConnectionError(const QAbstractSocket::SocketError &error) {
    m_log->Error(QString("HTTP server: error at new connection (%1).").arg(error));
}

void HttpServer::readyToReply()
{
    Request* request = (Request*) sender();
    Reply* reply;

    if ((request->getMethod() == "GET" || request->getMethod() == "HEAD") && request->getArgument().startsWith("get/"))
    {
        reply = new ReplyDlnaItemContent(m_log, request, rootFolder, request);
        connect(reply, SIGNAL(servingRenderer(QString,QString)), renderersModel, SLOT(serving(QString,QString)));
        connect(reply, SIGNAL(stopServingRenderer(QString)), renderersModel, SLOT(stopServing(QString)));
        connect(reply, SIGNAL(serving(QString,int)), this, SLOT(servingProgress(QString,int)));
        connect(reply, SIGNAL(servingFinished(QString, int)), this, SLOT(servingFinished(QString, int)));
    }
    else
    {
        reply = new Reply(m_log, request, rootFolder, request);
    }

    connect(reply, SIGNAL(finished()), request, SLOT(replyFinished()));
    connect(reply, SIGNAL(finished()), reply, SLOT(deleteLater()));
    reply->run();
}

void HttpServer::addFolder(const QString &folder) {
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
    if (rootFolder->addNetworkLink(url)) {
        emit linkAdded(url);
        return true;
    } else {
        emit error_addNetworkLink(url);
        return false;
    }
}

void HttpServer::checkNetworkLink()
{
    int nb = 0;
    m_log->Info("CHECK NETWORK LINK started");

    QSqlQuery query = rootFolder->getAllNetworkLinks();
    while (query.next()) {
        ++nb;
        if (!rootFolder->networkLinkIsValid(query.value("filename").toString()))
            m_log->Error(QString("link %1 is broken, title: %2").arg(query.value("filename").toString()).arg(query.value("title").toString()));
    }

    m_log->Info(QString("%1 links checked.").arg(nb));
}

void HttpServer::servingProgress(const QString &filename, const int &playedDurationInMs)
{
    QHash<QString, QVariant> data;
    data.insert("last_played", QDateTime::currentDateTime());
    if (playedDurationInMs>0)
        data.insert("progress_played", playedDurationInMs);
    if (!rootFolder->updateLibrary(filename, data))
        m_log->Error(QString("HTTP SERVER: unable to update library for media %1").arg(filename));
}

void HttpServer::servingFinished(const QString &filename, const int &status)
{
    if (status==0) {
        if (!rootFolder->incrementCounterPlayed(filename))
            m_log->Error(QString("HTTP SERVER: unable to update library for media %1").arg(filename));
    }
}

void HttpServer::createRenderer(Logger *log, const QString &peerAddress, const int &port, const QString &userAgent)
{
    renderersModel->createRenderer(log, peerAddress, port, userAgent);
}
