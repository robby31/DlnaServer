#ifndef REPLY_H
#define REPLY_H

#include <QObject>
#include "logger.h"
#include "request.h"
#include "dlnarootfolder.h"
#include "elapsedtimer.h"

class Reply : public QObject
{
    Q_OBJECT

public:
    explicit Reply(Logger *log, Request *request, DlnaRootFolder *rootFolder, QObject *parent = 0);

    // Construct a proper HTTP response to a received request
    // and provide answer to the client on the request
    // See "http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html" for HTTP header field definitions.
    virtual void run();


signals:
    // emit signal when reply is done
    void finished();

    void logText(const QString &text);

    void replyStatus(const QString &status);


public slots:
    void logDestroyed()        { m_log = new Logger(this); }
    void requestDestroyed()    { m_request = 0; }
    void rootFolderDestroyed() { m_rootFolder = 0; }
    void clientDestroyed()     { client = 0;
                                 emit logText(QString("%1: Client destroyed (reply)."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz"))); }

    void LogMessage(const QString &msg) { emit logText(msg); }


protected:
    void sendLine(QTcpSocket *client, const QString &msg);

    // send reply header to client
    void sendHeader();

    // send reply data to client
    void sendAnswer(const QByteArray &contentAnswer = QByteArray());

    void setParamHeader(const QString &param, const QString &value);
    QString getParamHeader(const QString &param) const;


private:
    // close the client
    virtual void closeClient();


protected:
    Logger* m_log;
    Request* m_request;

    QHash<QString, QString> m_params;  // header for the reply
    bool headerSent;

    QTcpSocket *client;
    bool keepSocketOpened;  // flag to not close automatically the client socket when answer is sent

    DlnaRootFolder *m_rootFolder;

    // Carriage return and line feed.
    static const QString CRLF;


private:
    static const QString HTTP_200_OK;
    static const QString HTTP_500;
    static const QString HTTP_206_OK;
    static const QString HTTP_200_OK_10;
    static const QString HTTP_206_OK_10;

    static const QString XML_HEADER;
    static const QString SOAP_ENCODING_HEADER;
    static const QString PROTOCOLINFO_RESPONSE;
    static const QString SOAP_ENCODING_FOOTER;
    static const QString SORTCAPS_RESPONSE;
    static const QString SEARCHCAPS_RESPONSE;
    static const QString SAMSUNG_ERROR_RESPONSE;
    static const QString GETSYSTEMUPDATEID_HEADER;
    static const QString GETSYSTEMUPDATEID_FOOTER;

    static const QString EVENT_Header;
    static const QString EVENT_Prop;
    static const QString EVENT_FOOTER;
};

#endif // REPLY_H
