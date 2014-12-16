#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QTcpSocket>
#include <QDateTime>

#include "logger.h"

class HttpClient : public QTcpSocket
{
    Q_OBJECT

public:
    explicit HttpClient(Logger *log, QObject *parent = 0);
    virtual ~HttpClient();

    bool isHttp10() const { return m_http10; }

    QString connectionType() const { return m_connectionType; }

private:
    bool isLogLevel(const LogLevel &level) const { return m_log ? m_log->isLevel(level) : false; }
    void logError(const QString &message)  const { if (m_log) m_log->Error(message); }
    void logDebug(const QString &message)  const { if (m_log) m_log->Debug(message); }
    void logInfo(const QString &message)   const { if (m_log) m_log->Info(message); }
    void logTrace(const QString &message)  const { if (m_log) m_log->Trace(message); }

signals:
    void appendLogSignal(const QString &msg);
    void appendAnswerSignal(const QString &msg);

    void bytesSent(const qint64 &size, const qint64 &bytesToWrite);

public slots:
    void closeClient();

    void setHttp10(const bool &http10) { m_http10 = http10; }
    void setConnectionType(const QString &connection) { m_connectionType = connection.toLower(); }

    void sendTextLine(const QString &msg);
    void sendHeader(const QString &httpType, const QHash<QString, QString> &header);
    void sendData(const QByteArray &data);

    void _bytesWritten(const qint64 &size) { emit bytesSent(size, bytesToWrite()); }
    void clientError(QAbstractSocket::SocketError error) { emit appendLogSignal(QString("%2: Network Error: %1, %3"+CRLF).arg(error).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(errorString())); }

private:
    // Carriage return and line feed.
    static const QString CRLF;

    Logger *m_log;

    bool m_http10;
    QString m_connectionType;
};

#endif // HTTPCLIENT_H
