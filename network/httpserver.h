#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QTcpServer>
#include <QThread>

#include "logger.h"
#include "upnphelper.h"
#include "cached/dlnacachedrootfolder.h"
#include "reply.h"
#include "replydlnaitemcontent.h"

class HttpServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit HttpServer(Logger* log, QObject *parent = 0);
    virtual ~HttpServer();

    QHostAddress getHost()  const { return hostaddress; }
    int getPort()           const { return serverPort(); }
    QString getURL()        const { return "http://" + getHost().toString() + ":" + QString("%1").arg(getPort()); }

    void start() { emit startSignal(); }
    void stop()  { emit stopSignal();  }
//    bool resetLibrary() { return batch->resetLibrary(); }

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
    void stopSignal();
    void serverStarted();
    void progressUpdate(const int &value);

    void addFolderSignal(QString folder);
    void folderAdded(QString folder);
    void error_addFolder(QString folder);

    void checkNetworkLinkSignal();
    void addNetworkLinkSignal(const QString url);
    void linkAdded(QString url);
    void error_addNetworkLink(QString url);
    void brokenLink(QString url, QString title);

    void servingRenderer(const QString &ip, const QString &mediaName);
    void stopServingRenderer(const QString &ip);

    void createRequest(qintptr socket, QString uuid, QString servername, QString host, int port);
    void deleteRequest(Request *request);
    void newRenderer(const QString &ip, const int &port, const QString &userAgent);

    void updateMediaData(const QString &filename, const QHash<QString, QVariant> &data);
    void incrementCounterPlayedSignal(const QString &filename);

    void getDLNAResourcesSignal(QObject *sender, QString objectId, bool returnChildren, int start, int count, QString searchStr);
    void dlnaResources(QObject* sender, QList<DlnaResource*>);

private slots:
    void _logDestroyed() { m_log = 0; }

    void _startServer();
    void _newConnectionError(const QAbstractSocket::SocketError &error);  // error during new connection
    void _readyToReply(const QString &method, const QString &argument, const QHash<QString, QString> &paramsHeader, const bool &http10, const QString &content, HttpRange *range, const int &timeSeekRangeStart, const int &timeSeekRangeEnd);                                                 // reply shall be sent

    void _servingProgress(const QString &filename, const int &playedDurationInMs);
    void _servingFinished(const QString &filename, const int &status);

    void _addFolder(const QString &folder);

    void requestDLNAResourcesSignal(QString objectId, bool returnChildren, int start, int count, QString searchStr) { emit getDLNAResourcesSignal(sender(), objectId, returnChildren, start, count, searchStr); }

    void newRequest(Request *request);


private:
    bool isLogLevel(const LogLevel &level) const { return m_log ? m_log->isLevel(level) : false; }
    void logError(const QString &message)  const { if (m_log) m_log->Error(message); }
    void logDebug(const QString &message)  const { if (m_log) m_log->Debug(message); }
    void logInfo(const QString &message)   const { if (m_log) m_log->Info(message); }
    void logTrace(const QString &message)  const { if (m_log) m_log->Trace(message); }

    void createTcpSocket(Request *request);

private :
    Logger* m_log;

    UPNPHelper upnp;
    QHostAddress hostaddress;
    int serverport;

    QThread workerRoot;
    QThread workerNetwork;
    QThread workerTranscoding;

    // root folder containing DLNA nodes
    QSqlDatabase database;

    QNetworkAccessManager netManager;
};

#endif // HTTPSERVER_H
