#ifndef REQUEST_H
#define REQUEST_H

#include <QString>
#include <QStringList>
#include <QTcpSocket>
#include <QTimer>

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
#include "elapsedtimer.h"

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

    QString getMethod() const { return m_method; }

    // Retrieves the argument of the request.
    // It contains a command, a unique resource id and a resource name, all separated by slashes.
    // For example: "get/0$0$2$17/big_buck_bunny_1080p_h264.mov" or "get/0$0$2$13/thumbnail0000Sintel.2010.1080p.mkv"
    QString getArgument() const { return m_argument; }

    QString getHost() const { return m_host; }
    QString getpeerAddress() const { return m_peerAddress; }
    qintptr socketDescriptor() const { return socket; }

    QString getStatus() const { return m_status; }

    QString getNetworkStatus() const { return m_networkStatus; }

    QString getDuration() const { return QTime(0, 0).addMSecs(m_duration).toString("hh:mm:ss.zzz"); }

    QString getDate() const { return m_date; }

    QString getSoapaction() const { return soapaction; }

    QString getTextContent() const { return m_content; }

    QString getUserAgent() const { return userAgentString; }

    QString getTransferMode() const { return transferMode; }

    QString getContentFeatures() const { return contentFeatures; }

    int getReceivedContentLength() const { return receivedContentLength; }

    QString getTextHeader() const { return m_header.join(""); }

    QString getTextAnswer() const { return m_stringAnswer.join(""); }

    QString getTranscodeLog() const { if (transcodeProcess != 0) return transcodeProcess->getTranscodeLog(); else return QString(); }

    bool appendHeader(const QString &headerLine);

    // Construct a proper HTTP response to a received request
    // and provide answer to the client on the request
    // See "http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html" for HTTP header field definitions.
    void run();

signals:
    // emit signal when data changed
    void dataChanged(const QString &roleChanged);

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
    //   status = 0 if serving finished successfully
    //   status = 1 if error occurs
    void servingFinished(QString filename, int status);

private slots:
    // slots for incoming data
    void readSocket();
    void stateChanged(const QAbstractSocket::SocketState &state);
    void errorSocket(const QAbstractSocket::SocketError &error);
    void bytesSent(const qint64 &size);

    // slots for streaming
    void runStreaming(DlnaItem *dlna);

    // slots for transcoding
    void runTranscoding(DlnaItem *dlna);
    void receivedTranscodedData();
    void receivedTranscodingLogMessage();
    void finishedTranscodeData(const int &exitCode);

    // slot to update status on streaming or transcoding
    void updateStatus();

    // slot to send data to client
    void sendAnswer(const QStringList &headerAnswer, const QByteArray &contentAnswer = QByteArray(), const long &totalSize = -1);

    // slot to create new renderer
    void createRenderer(Logger* log, const QString &peerAddress, const int &port, const QString &userAgent);

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

    static const int UPDATE_STATUS_PERIOD;

    Logger* log;

    QTcpSocket* client;
    long networkBytesSent;
    long lastNetBytesSent;
    bool keepSocketOpened;  // flag to not close automatically the client socket when answer is sent

    QTimer timerStatus;    // timer used to update periodically the status on streaming or transcoding

    QElapsedTimer clock;  // clock to measure time taken to answer to the request
    ElapsedTimer clockSending; // clock to mesure time taken to send streamed or transcoded data.
    QElapsedTimer clockUpdateStatus; // clock to check UpdateStatus period

    StreamingFile* streamContent;
    TranscodeProcess* transcodeProcess;
    int maxBufferSize;
    QMutex mutex;
    QWaitCondition servingStarted;

    QString m_status;  // status of the request
    QString m_networkStatus;  // status of network (interface client)
    qint64 m_duration;  // duration taken to answer to the request
    QString m_date;      // date when the request has been received
    DlnaRootFolder* rootFolder;
    MediaRendererModel* renderersModel;

    QString uuid;
    QString servername;
    QString m_host;
    int port;
    qintptr socket;
    QString m_peerAddress;

    QStringList m_header;  // header of the request received

    QStringList m_stringAnswer;  // answer sent
    QStringList headerAnswerToSend;  // answer header prepared but not yet sent

    QString m_method;

    // argument of the request:
    // It contains a command, a unique resource id and a resource name, all
    // separated by slashes. For example: "get/0$0$2$17/big_buck_bunny_1080p_h264.mov" or
    // "get/0$0$2$13/thumbnail0000Sintel.2010.1080p.mkv"
    QString m_argument;

    QString soapaction;
    QString m_content;
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

    bool http10;

    QString mediaFilename;

    void setHttp10(const bool &http10) { this->http10 = http10; }

    void setMethod(const QString &method) { m_method = method; emit dataChanged("method"); }

    void setArgument(const QString &argument) { m_argument = argument; emit dataChanged("argument"); }

    void setHost(const QString &host) { m_host = host; emit dataChanged("host"); }

    void setPeerAddress(const QString &address) { m_peerAddress = address; emit dataChanged("peerAddress"); }

    void setStatus(const QString &status) { m_status = status; emit dataChanged("status"); }

    void setNetworkStatus(const QString &status) { m_networkStatus = status; emit dataChanged("network_status"); }

    void setDuration(const qint64 &duration) { m_duration = duration; emit dataChanged("duration"); }

    void setDate(const QString &date) { m_date = date; emit dataChanged("date"); }

    void setSoapaction(const QString &soapaction);

    void setTextContent(const QString &content) { m_content = content; emit dataChanged("content"); }

    void sendLine(QTcpSocket *client, const QString &msg);

    void appendTrancodeProcessLog(const QString &text) { if (transcodeProcess) { transcodeProcess->appendLog(text); emit dataChanged("transcode_log"); } }

    // wait transcoding is finished
    void waitTranscodingFinished();

    // close the client
    void closeClient();

    // close the request
    void close();
};

#endif // REQUEST_H
