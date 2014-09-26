#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QTcpServer>

#include "logger.h"
#include "requestlistmodel.h"
#include "mediarenderermodel.h"
#include "cached/dlnacachedrootfolder.h"
#include "cached/batchedrootfolder.h"
#include "reply.h"
#include "replydlnaitemcontent.h"

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

    bool resetLibrary() { return batch->resetLibrary(); }

    // identifier of the render (unique)
    static const QString UUID;

    // Server name
    static const QString SERVERNAME;

    // Server port
    static const int SERVERPORT;

signals:
    void batched_addFolder(const QString &folder);
    void progressUpdate(const int &value);

    void folderAdded(QString folder);
    void error_addFolder(QString folder);

    void linkAdded(QString url);
    void error_addNetworkLink(QString url);
    void brokenLink(QString url, QString title);

public slots:
    bool addNetworkLink(const QString url);
    void checkNetworkLink();

private slots :
    void acceptConnection();                                             // new connection detected
    void newConnectionError(const QAbstractSocket::SocketError &error);  // error during new connection
    void newRequest(Request *request);                                   // new request has been created successfully
    void sendReply();                                                    // reply shall be sent

    void servingProgress(const QString &filename, const int &playedDurationInMs);
    void servingFinished(const QString &filename, const int &status);

    void addFolder(const QString &folder);

    // slot to create new renderer
    void createRenderer(Logger* log, const QString &peerAddress, const int &port, const QString &userAgent);

private :
    RequestListModel* requestsModel;
    MediaRendererModel* renderersModel;

    Logger* m_log;

    QHostAddress hostaddress;
    int serverport;

    // root folder containing DLNA nodes
    QSqlDatabase database;
    DlnaCachedRootFolder* rootFolder;
    BatchedRootFolder* batch;
    QThread batchThread;
};

#endif // HTTPSERVER_H
