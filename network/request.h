#ifndef REQUEST_H
#define REQUEST_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QElapsedTimer>

#include "logger.h"
#include "httprange.h"
#include "mediarenderer.h"


class Request: public QObject
{
    Q_OBJECT

public:
    Request(Logger *log,
            QTcpSocket *client, QString uuid,
            QString servername, QString host, int port,
            QObject *parent = 0);
    virtual ~Request();

    bool isHttp10() const { return http10; }

    QString getMethod() const { return m_method; }

    // Retrieves the argument of the request.
    // It contains a command, a unique resource id and a resource name, all separated by slashes.
    // For example: "get/0$0$2$17/big_buck_bunny_1080p_h264.mov" or "get/0$0$2$13/thumbnail0000Sintel.2010.1080p.mkv"
    QString getArgument() const { return m_argument; }

    QString getHost() const { return m_host; }
    QString getpeerAddress() const { return m_peerAddress; }
    qintptr socketDescriptor() const { return socket; }
    QTcpSocket *getClient() const { return m_client; }
    QString getServername() const { return servername; }
    int getPort() const { return port; }
    QString getUuid() const { return uuid; }

    HttpRange *getRange() const { return range; }
    long getTimeSeekRangeStart() const { return timeSeekRangeStart; }
    long getTimeSeekRangeEnd() const { return timeSeekRangeEnd; }

    QString getStatus() const { return m_status; }

    QString getNetworkStatus() const { return m_networkStatus; }

    QString getDuration() const { return QTime(0, 0).addMSecs(m_duration).toString("hh:mm:ss.zzz"); }

    QString getDate() const { return m_date; }

    QString getSoapaction() const { return getParamHeader("SOAPACTION"); }

    QString getTextContent() const { return m_content; }

    QString getTransferMode() const { return getParamHeader("TRANSFERMODE.DLNA.ORG"); }

    QString getContentFeatures() const { return getParamHeader("GETCONTENTFEATURES.DLNA.ORG"); }

    int getReceivedContentLength() const;

    QString getTextHeader() const { return m_header.join(""); }

    QString getMediaInfoSec() const { return getParamHeader("GETMEDIAINFO.SEC"); }

    QString getTextAnswer() const { return m_stringAnswer.join(""); }

    QString getLog() const { return requestLog; }

    QString getHttpConnection() const { return getParamHeader("CONNECTION"); }

    QString getParamHeader(const QString &param) const;

    bool appendHeader(const QString &headerLine);

    void setNetworkStatus(const QString &status) { m_networkStatus = status; emit dataChanged("network_status"); }

    void appendAnswer(const QString &string) { m_stringAnswer.append(string); emit dataChanged("answer"); }


signals:
    // emit signal when data changed
    void dataChanged(const QString &roleChanged);

    // emit signal when header has been completely received
    void headerReady();

    // emit signal when reply may be prepared and sent
    void readyToReply();

    // emit signal to add a new renderer
    void newRenderer(MediaRenderer *renderer);


public slots:
    void replyFinished();

    void clientError(QAbstractSocket::SocketError error) { appendLog(QString("%2: Network Error: %1"+CRLF).arg(error).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz"))); }
    void clientDestroyed() { m_client = 0;
                             appendLog(QString("%1: Client destroyed (request)."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz"))); }

    void appendLog(const QString &msg) { requestLog.append(msg); emit dataChanged("transcode_log"); }


private slots:
    void setStatus(const QString &status) { m_status = status; emit dataChanged("status"); }


private slots:
    // slots for incoming data
    void readSocket();
    void stateChanged(const QAbstractSocket::SocketState &state);


private:
    // Carriage return and line feed.
    static const QString CRLF;

    Logger* log;
    QString requestLog;  // internal log

    QTcpSocket* m_client;
    int replyNumber;
    bool replyInProgress;   // flag to know a reply is preparing to be sent
    bool flagHeaderReading; // flag to know if the header has been received or not
    QStringList m_stringAnswer;  // answer sent

    QElapsedTimer clock;  // clock to measure time taken to answer to the request

    QString m_status;  // status of the request
    QString m_networkStatus;  // status of network (interface client)
    qint64 m_duration;  // duration taken to answer to the request
    QString m_date;      // date when the request has been received

    QString uuid;
    QString servername;
    QString m_host;
    int port;
    qintptr socket;
    QString m_peerAddress;

    QStringList m_header;  // header of the request received
    QHash<QString, QString> m_params;

    QString m_method;

    // argument of the request:
    // It contains a command, a unique resource id and a resource name, all
    // separated by slashes. For example: "get/0$0$2$17/big_buck_bunny_1080p_h264.mov" or
    // "get/0$0$2$13/thumbnail0000Sintel.2010.1080p.mkv"
    QString m_argument;

    QString m_content;

    // range requested
    HttpRange* range;
    long timeSeekRangeStart;
    long timeSeekRangeEnd;
    bool http10;

    void setHttp10(const bool &http10) { this->http10 = http10; }

    void setMethod(const QString &method) { m_method = method; emit dataChanged("method"); }

    void setArgument(const QString &argument);

    void setHost(const QString &host) { m_host = host; emit dataChanged("host"); }

    void setPeerAddress(const QString &address) { m_peerAddress = address; emit dataChanged("peerAddress"); }

    void setDuration(const qint64 &duration) { m_duration = duration; emit dataChanged("duration"); }

    void setDate(const QString &date) { m_date = date; emit dataChanged("date"); }

    void setTextContent(const QString &content) { m_content = content; emit dataChanged("content"); }

    void setParamHeader(const QString &param, const QString &value);
};

#endif // REQUEST_H
