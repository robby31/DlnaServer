#ifndef REQUEST_H
#define REQUEST_H

#include <QString>
#include <QStringList>
#include <QTcpSocket>

#include <QFile>
#include <QUrl>
#include <QtXml>
#include <QHostAddress>

#include "logger.h"
#include "dlnarootfolder.h"
#include "dlnaresource.h"
#include "httprange.h"

class Request: public QObject
{
    Q_OBJECT

public:
    Request(Logger* log,
            QTcpSocket* client, QString uuid,
            QString servername, QString host, int port,
            DlnaRootFolder *rootFolder);
     ~Request();

    bool isHttp10() const { return http10; }

    QString getMethod() const { return method; }

    // Retrieves the argument of the request.
    // It contains a command, a unique resource id and a resource name, all separated by slashes.
    // For example: "get/0$0$2$17/big_buck_bunny_1080p_h264.mov" or "get/0$0$2$13/thumbnail0000Sintel.2010.1080p.mkv"
    QString getArgument() const { return argument; }

    QString getHost() const { return host; }
    QString getpeerAddress() const { return peerAddress; }

    QString getStatus() const { return status; }

    QString getDuration() const { return duration; }

    QString getDate() const { return date; }

    // When sending an input stream, the highRange indicates which byte to stop at.
    long getHighRange() const;

    // When sending an input stream, the lowRange indicates which byte to start from.
    long getLowRange() const;

    QString getSoapaction() const { return soapaction; }

    QString getTextContent() const { return content; }

    QString getUserAgent() const { return userAgentString; }

    QString getTransferMode() const { return transferMode; }

    QString getContentFeatures() const { return contentFeatures; }

    int getReceivedContentLength() const { return receivedContentLength; }

    QString getTextHeader() const { return header.join(""); }

    QString getTextAnswer() const { return stringAnswer.join(""); }

    bool appendHeader(QString headerLine);

    // Construct a proper HTTP response to a received request
    // and provide answer to the client on the request
    // See "http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html" for HTTP header field definitions.
    void answer(QTcpSocket *client);

    // read the http request received (header+content)
    void start_read();

private slots:
    // slots for transcoding
    void receivedTranscodedData();
    void errorTrancodedData(QProcess::ProcessError error);
    void finishedTranscodeData(int exitCode);

private:
    static const QString CONTENT_TYPE_UTF8;
    static const QString CONTENT_TYPE;

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

    // Carriage return and line feed.
    static const QString CRLF;

    Logger* log;

    QTcpSocket* client;

    QProcess* transcodeProcess;
    QElapsedTimer transcodeClock;
    QByteArray transcodedBytes;

    QString status;
    QString duration;  // duration taken to answer to the request
    QString date;      // date when the request has been received
    DlnaRootFolder *rootFolder;

    QString uuid;
    QString servername;
    QString host;
    int port;
    QString peerAddress;

    QStringList header;  // header of the request received

    QStringList stringAnswer;  // answer sent

    QString method;

    // argument of the request:
    // It contains a command, a unique resource id and a resource name, all
    // separated by slashes. For example: "get/0$0$2$17/big_buck_bunny_1080p_h264.mov" or
    // "get/0$0$2$13/thumbnail0000Sintel.2010.1080p.mkv"
    QString argument;

    QString soapaction;
    QString content;
    QString userAgentString;
    int receivedContentLength;

    // range requested
    HttpRange* range;

    QString transferMode;
    QString contentFeatures;
    double timeseek;
    double timeRangeEnd;

    bool http10;

    void setHttp10(bool http10) { this->http10 = http10; }

    void setStatus(QString status) { this->status = status; }

    void setDuration(QString duration) { this->duration = duration; }

    void setDate(QString date) { this->date = date; }

    void setSoapaction(QString soapaction);

    void setTextContent(QString content) { this->content = content; }

    void sendLine(QTcpSocket *client, QString msg);
    void sendAnswer(QTcpSocket *client, QString method, QStringList headerAnswer, QByteArray contentAnswer = QByteArray(), int totalSize = -1);

    // wait transcoding is finished
    void waitTranscodingFinished();
};

#endif // REQUEST_H
