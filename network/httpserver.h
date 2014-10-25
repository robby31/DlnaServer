#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QTcpServer>

#include "logger.h"
#include "upnphelper.h"
#include "cached/dlnacachedrootfolder.h"
#include "cached/batchedrootfolder.h"
#include "reply.h"
#include "replydlnaitemcontent.h"

class HttpServer : public QTcpServer
{
    Q_OBJECT

public:
    HttpServer(Logger* log, QObject *parent = 0);
    virtual ~HttpServer();

    QHostAddress getHost()  const { return hostaddress; }
    int getPort()           const { return serverPort(); }
    QString getURL()        const { return "http://" + getHost().toString() + ":" + QString("%1").arg(getPort()); }

    DlnaRootFolder* getRootFolder() const { return rootFolder; }

    void start() { emit startSignal(); }
    bool resetLibrary() { return batch->resetLibrary(); }

    // identifier of the render (unique)
    static const QString UUID;

    // Server name
    static const QString SERVERNAME;

    // Server port
    static const int SERVERPORT;

protected:
    virtual void incomingConnection(qintptr socketDescriptor);

signals:
    void startSignal();
    void serverStarted();
    void batched_addFolder(const QString &folder);
    void progressUpdate(const int &value);

    void folderAdded(QString folder);
    void error_addFolder(QString folder);

    void linkAdded(QString url);
    void error_addNetworkLink(QString url);
    void brokenLink(QString url, QString title);

    void servingRenderer(const QString &ip, const QString &mediaName);
    void stopServingRenderer(const QString &ip);

    void newRequest(Request *request);
    void newRenderer(MediaRenderer *renderer);


public slots:
    bool addNetworkLink(const QString url);


private slots:
    void _checkNetworkLink();

    void _startServer();
    void _newConnectionError(const QAbstractSocket::SocketError &error);  // error during new connection
    void _readyToReply();                                                 // reply shall be sent

    void _servingProgress(const QString &filename, const int &playedDurationInMs);
    void _servingFinished(const QString &filename, const int &status);

    void _addFolder(const QString &folder);


private :
    Logger* m_log;

    UPNPHelper upnp;
    QHostAddress hostaddress;
    int serverport;

    // root folder containing DLNA nodes
    QSqlDatabase database;
    DlnaCachedRootFolder* rootFolder;
    BatchedRootFolder* batch;
    QThread batchThread;
};

#endif // HTTPSERVER_H
