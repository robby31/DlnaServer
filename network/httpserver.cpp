#include "httpserver.h"
#include "request.h"
#include <QRegExp>
#include <QStringList>
#include <QNetworkInterface>
#include <QHostInfo>

const QString HttpServer::UUID = "cdc79bcf-6985-4baf-b974-e83846efd903";

const QString HttpServer::SERVERNAME = "QT Media Server";

HttpServer::HttpServer(Logger* log, RequestListModel *requestsModel, QObject *parent):
    QObject(parent),
    requestsModel(requestsModel),
    log(log),
    serverport(5002),
    rootFolder(0)
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

    connect(&server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    connect(&server, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(newConnectionError(QAbstractSocket::SocketError)));

    if (hostaddress.isNull()) {
        log->ERROR("HTTP server: unable to define host ip address.");
    }
    else {
        if (!server.listen(hostaddress, serverport)) {
            log->ERROR("HTTP server: " + server.errorString());
        }
        else {
            log->TRACE("HTTP server: listen " + getHost().toString() + ":" + QString("%1").arg(getPort()));
        }
    }

    // initialize the root folder
    rootFolder = new DlnaRootFolder(log, hostaddress.toString(), serverport);
    rootFolder->addFolder("/Users/doudou/workspace/DLNA_server/tests/AUDIO");
    rootFolder->addFolder("/Users/doudou/Music/iTunes/iTunes Media/Music");
    rootFolder->addFolder("/Users/doudou/Movies");
}

HttpServer::~HttpServer()
{
    log->TRACE("Close HTTP server.");
    server.close();
    delete rootFolder;
}

QString HttpServer::getURL() const {
    return "http://" + getHost().toString() + ":" + QString("%1").arg(getPort());
}


void HttpServer::acceptConnection()
{
    while (server.hasPendingConnections()) {
        log->TRACE("HTTP server: new connection");

        requestsModel->addRequest(log,
                                  server.nextPendingConnection(),
                                  UUID, QString("%1 [%2]").arg(SERVERNAME).arg(QHostInfo::localHostName()),
                                  getHost().toString(), getPort(),
                                  rootFolder);
    }
}

void HttpServer::newConnectionError(QAbstractSocket::SocketError error) {
    log->ERROR("HTTP server: error at new connection.");
}
