#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QTcpServer>

#include "logger.h"
#include "requestlistmodel.h"
#include "dlnarootfolder.h"

class HttpServer : public QObject
{
    Q_OBJECT

public:
    HttpServer(Logger* log, RequestListModel* requestsModel);
    ~HttpServer();

    QHostAddress getHost() const { return hostaddress; }
    int getPort() const { return server.serverPort(); }
    QString getURL() const;

    DlnaRootFolder* getRootFolder() const { return rootFolder; }

    // identifier of the render (unique)
    static const QString UUID;

    // Server name
    static const QString SERVERNAME;

private slots :
    // new connection detected
    void acceptConnection();

private :
    QTcpServer server;

    RequestListModel* requestsModel;

    Logger* log;

    QHostAddress hostaddress;
    int serverport;

    // root folder containing DLNA nodes
    DlnaRootFolder* rootFolder;
};

#endif // HTTPSERVER_H
