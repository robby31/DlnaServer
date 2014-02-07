#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QTcpServer>

#include "logger.h"
#include "requestlistmodel.h"
#include "mediarenderermodel.h"
#include "dlnarootfolder.h"

class HttpServer : public QTcpServer
{
    Q_OBJECT

public:
    HttpServer(Logger* log, RequestListModel* requestsModel, MediaRendererModel* renderersModel, QObject *parent = 0);
    virtual ~HttpServer();

    QHostAddress getHost() const { return hostaddress; }
    int getPort() const { return serverPort(); }
    QString getURL() const;

    DlnaRootFolder* getRootFolder() const { return rootFolder; }

    // identifier of the render (unique)
    static const QString UUID;

    // Server name
    static const QString SERVERNAME;

protected:
    virtual void incomingConnection(qintptr socketDescriptor);

private slots :
    void acceptConnection();                                      // new connection detected
    void newConnectionError(QAbstractSocket::SocketError error);  // error during new connection


private :
    RequestListModel* requestsModel;
    MediaRendererModel* renderersModel;

    Logger* log;

    QHostAddress hostaddress;
    int serverport;

    // root folder containing DLNA nodes
    DlnaRootFolder* rootFolder;

    QHash<int, QTcpSocket*> l_socket;
};

#endif // HTTPSERVER_H
