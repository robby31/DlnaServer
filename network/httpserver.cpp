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
    log(log != 0 ? log : new Logger(this)),
    hostaddress(),
    serverport(SERVERPORT),
    database(QSqlDatabase::addDatabase("QSQLITE")),
    rootFolder(0),
    batch(0),
    batchThread(this)
{
    // read the LocalIpAddress
    foreach(QNetworkInterface net, QNetworkInterface::allInterfaces()) {
        if (!(net.flags() & QNetworkInterface::IsLoopBack)) {
            foreach(QNetworkAddressEntry address, net.addressEntries()) {
                if (address.ip().toString().count('.') == 3) {
                    // ip address is on format x.x.x.x
                    hostaddress = address.ip();
                    break;
                }
            }
        }
    }

    connect(this, SIGNAL(newConnection()),
            this, SLOT(acceptConnection()));

    connect(this, SIGNAL(acceptError(QAbstractSocket::SocketError)),
            this, SLOT(newConnectionError(QAbstractSocket::SocketError)));

    if (hostaddress.isNull()) {
        log->Error("HTTP server: unable to define host ip address.");
    }
    else {
        if (!listen(hostaddress, serverport))
            log->Error("HTTP server: " + errorString());
        else
            log->Trace("HTTP server: listen " + getHost().toString() + ":" + QString("%1").arg(getPort()));
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

    log->Trace("Close HTTP server.");
    close();
}

QString HttpServer::getURL() const {
    return "http://" + getHost().toString() + ":" + QString("%1").arg(getPort());
}

void HttpServer::acceptConnection()
{
    while (hasPendingConnections()) {
        log->Trace("HTTP server: new connection");

        if (requestsModel != 0) {
            Request *request = requestsModel->addRequest(log,
                                                         nextPendingConnection(),
                                                         UUID, QString("%1").arg(SERVERNAME),
                                                         getHost().toString(), getPort(),
                                                         rootFolder, renderersModel);
            connect(request, SIGNAL(serving(QString,int)), this, SLOT(servingProgress(QString,int)));
            connect(request, SIGNAL(servingFinished(QString, int)), this, SLOT(servingFinished(QString, int)));
        }
        else
            log->Error("Unable to add new request (requestsModel is null).");
    }
}

void HttpServer::newConnectionError(QAbstractSocket::SocketError error) {
    log->Error(QString("HTTP server: error at new connection (%1).").arg(error));
}

bool HttpServer::addFolder(QString folder) {
    if (QFileInfo(folder).isDir()) {
        emit batched_addFolder(folder);
        if (rootFolder != 0) {
            DlnaRootFolder *obj = rootFolder->getRootFolder();
            if (obj != 0) {
                return obj->addFolder(folder);
            }
        }
    }
    return false;
}

void HttpServer::servingProgress(QString filename, int playedDurationInMs)
{
    QHash<QString, QVariant> data;
    data.insert("last_played", QDateTime::currentDateTime());
    data.insert("progress_played", playedDurationInMs);
    if (!rootFolder->updateLibrary(filename, data))
        log->Error(QString("HTTP SERVER: unable to update library for media %1").arg(filename));
}

void HttpServer::servingFinished(QString filename, int status)
{
    if (status==0) {
        if (!rootFolder->incrementCounterPlayed(filename))
            log->Error(QString("HTTP SERVER: unable to update library for media %1").arg(filename));
    }
}
