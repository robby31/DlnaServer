#include "httpserver.h"
#include "request.h"
#include <QRegExp>
#include <QStringList>
#include <QNetworkInterface>
#include <QHostInfo>

const QString HttpServer::UUID = "cdc79bcf-6985-4baf-b974-e83846efd903";

const QString HttpServer::SERVERNAME = "Mac_OS_X-x86_64-10.9.1, UPnP/1.0, QMS/1.0";

HttpServer::HttpServer(Logger* log, RequestListModel *requestsModel, MediaRendererModel* renderersModel, QObject *parent):
    QTcpServer(parent),
    requestsModel(requestsModel),
    renderersModel(renderersModel),
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

    connect(this, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    connect(this, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(newConnectionError(QAbstractSocket::SocketError)));

    if (hostaddress.isNull()) {
        log->ERROR("HTTP server: unable to define host ip address.");
    }
    else {
        if (!listen(hostaddress, serverport)) {
            log->ERROR("HTTP server: " + errorString());
        }
        else {
            log->TRACE("HTTP server: listen " + getHost().toString() + ":" + QString("%1").arg(getPort()));
        }
    }

    // initialize the root folder
    rootFolder = new DlnaCachedRootFolder(log, hostaddress.toString(), serverport, this);
}

HttpServer::~HttpServer()
{
    log->TRACE("Close HTTP server.");
    close();
    delete rootFolder;
}

QString HttpServer::getURL() const {
    return "http://" + getHost().toString() + ":" + QString("%1").arg(getPort());
}

void HttpServer::incomingConnection(qintptr socketDescriptor) {

    if (!l_socket.contains(socketDescriptor)) {
        // create new socket
        l_socket[socketDescriptor] = new QTcpSocket(this);
    }

    // put the socket in connected state
    if (!l_socket[socketDescriptor]->setSocketDescriptor(socketDescriptor)) {
        log->ERROR(QString("Unable to create TcpSocket %1").arg(socketDescriptor));
    } else {
        addPendingConnection(l_socket[socketDescriptor]);
    }
}

void HttpServer::acceptConnection()
{
    while (hasPendingConnections()) {
        log->TRACE("HTTP server: new connection");

        requestsModel->addRequest(log,
                                  nextPendingConnection(),
                                  UUID, QString("%1").arg(SERVERNAME),
                                  getHost().toString(), getPort(),
                                  rootFolder, renderersModel);
    }
}

void HttpServer::newConnectionError(QAbstractSocket::SocketError error) {
    log->ERROR("HTTP server: error at new connection.");
}

bool HttpServer::addFolder(QString folder) {
    return rootFolder->addFolder(folder);
}
