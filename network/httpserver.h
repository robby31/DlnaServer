#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QTcpServer>

#include "logger.h"
#include "requestlistmodel.h"
#include "mediarenderermodel.h"
#include "cached/dlnacachedrootfolder.h"
#include "cached/batchedrootfolder.h"

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
    bool addFolder(QString folder);

    // identifier of the render (unique)
    static const QString UUID;

    // Server name
    static const QString SERVERNAME;

signals:
    void batched_addFolder(QString folder);
    void progressUpdate(int value);

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
    QSqlDatabase database;
    DlnaCachedRootFolder* rootFolder;
    BatchedRootFolder* batch;
    QThread* batchThread;

    QHash<int, QTcpSocket*> l_socket;
};

#endif // HTTPSERVER_H
