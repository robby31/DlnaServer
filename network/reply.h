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
    explicit Reply(Logger *log, const bool &http10, const QString &method, const QString &argument, const QHash<QString, QString> &paramsHeader, const QString &content, HttpRange *range, const int &timeSeekRangeStart, const int &timeSeekRangeEnd, QString uuid, QString servername, QString host, int port, QObject *parent = 0);

    bool isHttp10() const { return m_http10; }

    QString requestMethod() const   { return m_requestMethod;   }
    QString requestArgument() const { return m_requestArgument; }
    QString requestTextContent() const { return m_requestContent; }
    HttpRange *requestRange() const { return m_requestRange; }
    int requestTimeSeekRangeStart() const { return m_requestTimeSeekRangeStart; }
    int requestTimeSeekRangeEnd() const { return m_requestTimeSeekRangeEnd; }
    QString getRequestParamHeader(const QString &param) const;
    QString getRequestSoapAction() const { return getRequestParamHeader("SOAPACTION"); }
    QString getRequestTransferMode() const { return getRequestParamHeader("TRANSFERMODE.DLNA.ORG"); }
    QString getRequestContentFeatures() const { return getRequestParamHeader("GETCONTENTFEATURES.DLNA.ORG"); }
    QString getRequestMediaInfoSec() const { return getRequestParamHeader("GETMEDIAINFO.SEC"); }

    QString userAgent() const { return getRequestParamHeader("USER-AGENT"); }

    QString uuid() const { return m_uuid; }
    QString servername() const { return m_servername; }
    QString host() const { return m_host; }
    int port() const { return m_port; }

    void run() { emit runSignal(); }


signals:
    void runSignal();

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

    void bytesSent(const qint64 &size, const qint64 &towrite);


private slots:
    void LogMessage(const QString &msg) { emit logTextSignal(msg); }

    // Construct a proper HTTP response to a received request
    // and provide answer to the client on the request
    // See "http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html" for HTTP header field definitions.
    virtual void _run();

    virtual void dlnaResources(QObject* requestor, QList<DlnaResource*> resources);

    virtual void bytesSentSlot(const qint64 &size, const qint64 &towrite);

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
    QString m_requestMethod;
    QString m_requestArgument;
    QHash<QString, QString> m_request_params;
    QString m_requestContent;
    HttpRange *m_requestRange;
    int m_requestTimeSeekRangeStart;
    int m_requestTimeSeekRangeEnd;

    bool m_http10;
    QString m_uuid;
    QString m_servername;
    QString m_host;
    int m_port;

    QHash<QString, QString> m_params;  // header for the reply
    bool headerSent;
    bool keepReplyOpened;

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
