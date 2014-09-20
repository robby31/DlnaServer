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
    Reply(Logger *log, Request *request, DlnaRootFolder *rootFolder, MediaRendererModel *renderersModel, QObject *parent = 0);
    virtual ~Reply();

    // Construct a proper HTTP response to a received request
    // and provide answer to the client on the request
    // See "http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html" for HTTP header field definitions.
    void run();

    void sendLine(QTcpSocket *client, const QString &msg);

    // send reply data to client
    void sendAnswer(const QStringList &headerAnswer, const QByteArray &contentAnswer = QByteArray(), const long &totalSize = -1);


signals:
    // emit signal when reply is done
    void finished();

    // emit signal to provide progress on serving media
    void serving(QString filename, int playedDurationInMs);

    // emit signal when serving is finished
    //   status = 0 if serving finished successfully
    //   status = 1 if error occurs
    void servingFinished(QString filename, int status);


public slots:
    void bytesSent(const qint64 &size);

    // slot to update status on streaming or transcoding
    void updateStatus();

    void receivedTranscodedData();
    void receivedTranscodingLogMessage(const QString &msg);
    void finishedTranscodeData(const int &exitCode);

    // close the request
    void close();


private:
    // streaming
    void runStreaming(DlnaItem *dlna);

    // transcoding
    void runTranscoding(DlnaItem *dlna);

    // close the client
    void closeClient();

    void appendTrancodeProcessLog(const QString &text) { m_request->appendLog(text); }

    // wait transcoding is finished
    void waitTranscodingFinished();

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

    Logger* m_log;
    Request* m_request;

    QTcpSocket *client;
    bool keepSocketOpened;  // flag to not close automatically the client socket when answer is sent

    QTimer timerStatus;    // timer used to update periodically the status on streaming or transcoding
    long networkBytesSent;
    long lastNetBytesSent;
    ElapsedTimer clockSending; // clock to mesure time taken to send streamed or transcoded data.
    QElapsedTimer clockUpdateStatus; // clock to check UpdateStatus period

    QStringList headerAnswerToSend;  // answer header prepared but not yet sent

    DlnaRootFolder *m_rootFolder;

    MediaRendererModel *m_renderersModel;

    QString mediaFilename;

    StreamingFile *streamContent;
    TranscodeProcess *transcodeProcess;
    int maxBufferSize;
};

#endif // REPLY_H
