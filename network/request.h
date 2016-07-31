#ifndef REQUEST_H
#define REQUEST_H

#include <QHostAddress>
#include <QElapsedTimer>

#include "logger.h"
#include "httprange.h"
#include "Models/listmodel.h"


class Request: public ListItem
{
    Q_OBJECT

public:
    explicit Request(QObject *parent = 0);
    explicit Request(Logger *log,
                     qintptr socket,
                     QString uuid,
                     QString servername, QString host, int port,
                     QObject *parent = 0);
    virtual ~Request();

    enum Roles {
        methodRole = Qt::UserRole+1,
        argumentRole,
        hostRole,
        peerAddressRole,
        statusRole,
        headerRole,
        contentRole,
        durationRole,
        dateRole,
        answerRole,
        networkStatusRole,
        transcodeLogRole
    };

    virtual QHash<int, QByteArray> roleNames() const { return m_roles; }
    virtual QVariant data(int role) const;
    virtual bool setData(const QVariant &value, const int &role);


    bool isHttp10() const { return http10; }

    qintptr socketDescriptor() const { return m_socket; }
    QString getServername() const { return servername; }
    int getPort() const { return port; }
    QString getUuid() const { return uuid; }

    HttpRange *getRange() const { return m_range; }
    qint64 getTimeSeekRangeStart() const { return timeSeekRangeStart; }
    qint64 getTimeSeekRangeEnd() const { return timeSeekRangeEnd; }

    QString getSoapaction() const { return getParamHeader("SOAPACTION"); }

    QString getTransferMode() const { return getParamHeader("TRANSFERMODE.DLNA.ORG"); }

    QString getContentFeatures() const { return getParamHeader("GETCONTENTFEATURES.DLNA.ORG"); }

    QString getMediaInfoSec() const { return getParamHeader("GETMEDIAINFO.SEC"); }

    QString getParamHeader(const QString &param) const;

private:
    bool isLogLevel(const LogLevel &level) const { return m_log ? m_log->isLevel(level) : false; }
    void logError(const QString &message)  const { if (m_log) m_log->Error(message); }
    void logDebug(const QString &message)  const { if (m_log) m_log->Debug(message); }
    void logInfo(const QString &message)   const { if (m_log) m_log->Info(message); }
    void logTrace(const QString &message)  const { if (m_log) m_log->Trace(message); }


signals:
    // emit signal when reply may be prepared and sent
    void readyToReply(const QString &method, const QString &argument, const QHash<QString, QString> &paramsHeader, const bool &http10, const QString &content, HttpRange *range, const int &timeSeekRangeStart, const int &timeSeekRangeEnd);

    // emit signal to add a new renderer
    void newRenderer(const QString &ip, const int &port, const QString &userAgent);

    void startServingRendererSignal(const QString &ip, const QString &mediaName);
    void stopServingRendererSignal(const QString &ip);

    void deleteRequest(Request *request);

    void clientDisconnected();
    void closeClient();
    void bytesSent(const qint64 &size, const qint64 &bytesToWrite);
    void sendTextLineToClientSignal(const QString &msg);
    void sendHeaderSignal(const QHash<QString, QString> &header);
    void sendDataToClientSignal(const QByteArray &data);
    void headerSent();
    void sendData(const QByteArray &data);

private slots:
    void setStatus(const QString &status)           { setData(status, statusRole);          }
    void setNetworkStatus(const QString &status)    { setData(status, networkStatusRole); if (m_networkStatus == "disconnected" && m_status == "OK") emit deleteRequest(this); }
    void setHttp10(const bool &http10)              { this->http10 = http10;                }
    void setMethod(const QString &method)           { setData(method, methodRole);          }
    void setArgument(const QString &argument)       { setData(argument, argumentRole);      }
    void setHost(const QString &host)               { setData(host, hostRole);              }
    void setPeerAddress(const QString &address)     { setData(address, peerAddressRole);    }
    void setDuration(const qint64 &duration)        { setData(duration, durationRole);      }
    void setDate(const QString &date)               { setData(date, dateRole);              }
    void setTextContent(const QString &content)     { setData(content, contentRole);        }

    void appendLog(const QString &msg) { requestLog.append(msg); emit itemChanged(QVector<int>(1, transcodeLogRole)); }
    void appendAnswer(const QString &string) { m_stringAnswer.append(string); emit itemChanged(QVector<int>(1, answerRole)); }

    void stateChanged(const QAbstractSocket::SocketState &state);

    void startServingRenderer(const QString &mediaName) { emit startServingRendererSignal(data(peerAddressRole).toString(), mediaName); }
    void stopServingRenderer()                          { emit stopServingRendererSignal(data(peerAddressRole).toString()); }

    void requestReceived(const QString &peerAddress, const QStringList &header, const bool &is_http10, const QString &method, const QString &argument, const QHash<QString, QString> &paramsHeader, const QString &content, HttpRange *range, const int &timeSeekRangeStart, const int &timeSeekRangeEnd);
    void replyFinished();

private:
    // Carriage return and line feed.
    static const QString CRLF;

    QHash<int, QByteArray> m_roles;

    Logger *m_log;
    qintptr m_socket;

    QString requestLog;  // internal log

    int replyNumber;
    bool replyInProgress;   // flag to know a reply is preparing to be sent
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
    HttpRange* m_range;
    qint64 timeSeekRangeStart;
    qint64 timeSeekRangeEnd;
    bool http10;

public:
    static qint64 objectCounter;
};

#endif // REQUEST_H
