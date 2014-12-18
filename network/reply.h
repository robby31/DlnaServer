#ifndef REPLY_H
#define REPLY_H

#include "logobject.h"
#include "request.h"
#include "dlnarootfolder.h"
#include "elapsedtimer.h"

class Reply : public LogObject
{
    Q_OBJECT

public:
    explicit Reply(Logger *log, Request *request, QObject *parent = 0);

    void run(const QString &method, const QString &argument) { emit runSignal(method, argument); }


signals:
    void runSignal(const QString &method, const QString &argument);

    // emit signal when reply is done
    void finishedSignal();

    void logTextSignal(const QString &text);

    void replyStatusSignal(const QString &status);
    void networkStatusSignal(const QString &status);

    void appendAnswerSignal(const QString &msg);

    void closeClientSignal();
    void sendTextLineToClientSignal(const QString &msg);
    void sendHeaderSignal(const QHash<QString, QString> &header);
    void sendDataToClientSignal(const QByteArray &data);

    void getDLNAResourcesSignal(QString objectId, bool returnChildren, int start, int count, QString searchStr);


private slots:
    void requestDestroyed()    { m_request = 0; }

    void LogMessage(const QString &msg) { emit logTextSignal(msg); }

    // Construct a proper HTTP response to a received request
    // and provide answer to the client on the request
    // See "http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html" for HTTP header field definitions.
    virtual void _run(const QString &method, const QString &argument);

    virtual void dlnaResources(QObject* requestor, QList<DlnaResource*> resources);


protected:
    void sendLine(QTcpSocket *client, const QString &msg);

    // send reply header to client
    void sendHeader();

    // send reply data to client
    void sendAnswer(const QByteArray &contentAnswer = QByteArray());

    void setParamHeader(const QString &param, const QString &value);
    QString getParamHeader(const QString &param) const;

    void replyDone(const QString &status);


protected:
    Request* m_request;

    QHash<QString, QString> m_params;  // header for the reply
    bool headerSent;

    QDomDocument doc;
    QDomDocument xml;

    // Carriage return and line feed.
    static const QString CRLF;


private:
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
