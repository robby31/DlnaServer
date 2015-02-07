#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QDateTime>
#include <QThread>

#include "logger.h"
#include "httprange.h"

class HttpClient : public QTcpSocket
{
    Q_OBJECT

public:
    explicit HttpClient(Logger *log, QObject *parent = 0);
    virtual ~HttpClient();

    bool isHttp10() const { return m_http10; }

    void clear();

private:
    bool isLogLevel(const LogLevel &level) const { return m_log ? m_log->isLevel(level) : false; }
    void logError(const QString &message)  const { if (m_log) m_log->Error(message); }
    void logDebug(const QString &message)  const { if (m_log) m_log->Debug(message); }
    void logInfo(const QString &message)   const { if (m_log) m_log->Info(message); }
    void logTrace(const QString &message)  const { if (m_log) m_log->Trace(message); }

    void setMethod(const QString &method) { m_method = method; }
    QString getMethod() const { return m_method; }

    void setArgument(const QString &argument);
    QString getArgument() const { return m_argument; }

    void setParamHeader(const QString &param, const QString &value);
    QString getParamHeader(const QString &param) const;
    bool appendHeader(const QString &headerLine);

    int getReceivedContentLength() const;

    QString getSoapaction() const { return getParamHeader("SOAPACTION"); }

    QString getHttpConnection() const { return getParamHeader("CONNECTION"); }

    void requestReceived();

signals:
    void appendLogSignal(const QString &msg);
    void appendAnswerSignal(const QString &msg);

    void bytesSent(const qint64 &size, const qint64 &bytesToWrite);

    void incomingRequest(const QString &peerAddress, const QStringList &header, const bool &is_http10, const QString &method, const QString &argument, const QHash<QString, QString> &paramsHeader, const QString &content, HttpRange *range, const int &timeSeekRangeStart, const int &timeSeekRangeEnd);

public slots:
    void closeClient();

    void setHttp10(const bool &http10) { m_http10 = http10; }

    void sendTextLine(const QString &msg);
    void sendHeader(const QHash<QString, QString> &header);
    void sendData(const QByteArray &data);

    void _bytesWritten(const qint64 &size) { emit bytesSent(size, bytesToWrite()); }
    void clientError(QAbstractSocket::SocketError error) { emit appendLogSignal(QString("%2: Network Error: %1, %3"+CRLF).arg(error).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(errorString())); }
    void readSocket();

private:
    // Carriage return and line feed.
    static const QString CRLF;

    static const QString HTTP_200_OK;
    static const QString HTTP_500;
    static const QString HTTP_206_OK;
    static const QString HTTP_200_OK_10;
    static const QString HTTP_206_OK_10;

    Logger *m_log;

    bool m_http10;

    bool flagHeaderReading;
    QStringList m_header;
    bool requestComplete;
    QHash<QString, QString> m_params;

    QString m_method;
    QString m_argument;
    QString m_content;

    HttpRange* range;
    qint64 timeSeekRangeStart;
    qint64 timeSeekRangeEnd;
};

#endif // HTTPCLIENT_H
