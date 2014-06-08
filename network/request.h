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
#include "transcodeprocess.h"
#include "mediarenderermodel.h"

class Request: public QThread
{
    Q_OBJECT

public:
    Request(Logger* log,
            QTcpSocket* client, QString uuid,
            QString servername, QString host, int port,
            DlnaRootFolder *rootFolder, MediaRendererModel* renderersModel,
            QObject *parent = 0);
    virtual ~Request();

    bool isHttp10() const { return http10; }

    QString getMethod() const { return method; }

    // Retrieves the argument of the request.
    // It contains a command, a unique resource id and a resource name, all separated by slashes.
    // For example: "get/0$0$2$17/big_buck_bunny_1080p_h264.mov" or "get/0$0$2$13/thumbnail0000Sintel.2010.1080p.mkv"
    QString getArgument() const { return argument; }

    QString getHost() const { return host; }
    QString getpeerAddress() const { return peerAddress; }
    qintptr socketDescriptor() const { return socket; }

    QString getStatus() const { return status; }

    QString getNetworkStatus() const { return networkStatus; }

    QString getDuration() const { return duration; }

    QString getDate() const { return date; }

    QString getSoapaction() const { return soapaction; }

    QString getTextContent() const { return content; }

    QString getUserAgent() const { return userAgentString; }

    QString getTransferMode() const { return transferMode; }

    QString getContentFeatures() const { return contentFeatures; }

    int getReceivedContentLength() const { return receivedContentLength; }

    QString getTextHeader() const { return header.join(""); }

    QString getTextAnswer() const { return stringAnswer.join(""); }

    QString getTranscodeLog() const { if (transcodeProcess != 0) return transcodeProcess->getTranscodeLog(); else return QString(); }

    bool appendHeader(QString headerLine);

    // Construct a proper HTTP response to a received request
    // and provide answer to the client on the request
    // See "http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html" for HTTP header field definitions.
    void run();

signals:
    // emit signal when data changed
    void dataChanged();

    // emit signal to send answer to client
    void answerReady(QStringList headerAnswer, QByteArray contentAnswer = QByteArray(), long totalSize = -1);

    // emit signal to start transcoding
    void startTranscoding(DlnaItem* dlna);

    // emit signal to start streaming
    void startStreaming(DlnaItem* dlna);

    // emit signal to send data to client
    void DataToSend(char* data);

    // emit signal to add a new renderer
    void newRenderer(Logger* log, QString peerAddress, int port, QString userAgent);

    // emit signal to provide progress on serving media
    void serving(QString filename, int playedDurationInMs);

    // emit signal when serving is finished
    void servingFinished(QString filename);

private slots:
    // slots for incoming data
    void readSocket();
    void stateChanged(QAbstractSocket::SocketState state);
    void errorSocket(QAbstractSocket::SocketError error);
    void bytesSent(qint64 size);

    // slots for streaming
    void runStreaming(DlnaItem *dlna);

    // slots for transcoding
    void runTranscoding(DlnaItem *dlna);
    void receivedTranscodedData();
    void receivedTranscodingLogMessage();
    void finishedTranscodeData(int exitCode);

    // slot to send data to client
    void sendAnswer(QStringList headerAnswer, QByteArray contentAnswer = QByteArray(), long totalSize = -1);

    // slot to create new renderer
    void createRenderer(Logger* log, QString peerAddress, int port, QString userAgent);

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
    int networkBytesSent;
    bool keepSocketOpened;  // flag to not close automatically the client socket when answer is sent
    QElapsedTimer clock;  // clock to measure time taken to answer to the request
    QElapsedTimer clockSending; // clock to mesure time taken to send streamed or transcoded data.

    QIODevice* streamContent;
    TranscodeProcess* transcodeProcess;
    QMutex mutex;
    QWaitCondition servingStarted;

    QString status;  // status of the request
    QString networkStatus;  // status of network (interface client)
    QString duration;  // duration taken to answer to the request
    QString date;      // date when the request has been received
    DlnaRootFolder* rootFolder;
    MediaRendererModel* renderersModel;

    QString uuid;
    QString servername;
    QString host;
    int port;
    qintptr socket;
    QString peerAddress;

    QStringList header;  // header of the request received

    QStringList stringAnswer;  // answer sent
    QStringList headerAnswerToSend;  // answer header prepared but not yet sent

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
    long timeSeekRangeStart;
    long timeSeekRangeEnd;

    QString transferMode;
    QString contentFeatures;
    QString mediaInfoSec;
    QString httpConnection;
    double timeseek;
    double timeRangeEnd;

    bool http10;

    QString mediaFilename;

    void setHttp10(bool http10) { this->http10 = http10; }

    void setStatus(QString status) { this->status = status; emit dataChanged(); }

    void setNetworkStatus(QString status) { this->networkStatus = status; emit dataChanged(); }

    void setDuration(QString duration) { this->duration = duration; emit dataChanged(); }

    void setDate(QString date) { this->date = date; emit dataChanged(); }

    void setSoapaction(QString soapaction);

    void setTextContent(QString content) { this->content = content; }

    void sendLine(QTcpSocket *client, QString msg);

    // wait transcoding is finished
    void waitTranscodingFinished();

    // close the client
    void closeClient();

    // close the request
    void close();
};

#endif // REQUEST_H
