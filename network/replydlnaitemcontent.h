#ifndef REPLYDLNAITEMCONTENT_H
#define REPLYDLNAITEMCONTENT_H

#include "reply.h"

class ReplyDlnaItemContent : public Reply
{
    Q_OBJECT

public:
    explicit ReplyDlnaItemContent(Logger *log, Request *request, DlnaRootFolder *rootFolder, MediaRendererModel *renderersModel, QObject *parent = 0);
    virtual ~ReplyDlnaItemContent();

    // Construct a proper HTTP response to a received request
    // and provide answer to the client on the request
    // See "http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html" for HTTP header field definitions.
    virtual void run();


signals:
    // emit signal to provide progress on serving media
    void serving(QString filename, int playedDurationInMs);

    // emit signal when serving is finished
    //   status = 0 if serving finished successfully
    //   status = 1 if error occurs
    void servingFinished(QString filename, int status);


public slots:
    void bytesSent(const qint64 &size);

    // close the request
    void close();

    // slot to update status on streaming or transcoding
    void updateStatus();

    void receivedTranscodedData();
    void receivedTranscodingLogMessage(const QString &msg);
    void finishedTranscodeData(const int &exitCode);


private:
    // close the client
    virtual void closeClient();

    // streaming
    void runStreaming(DlnaItem *dlna);

    // transcoding
    void runTranscoding(DlnaItem *dlna);

    // wait transcoding is finished
    void waitTranscodingFinished();


private:
    static const int UPDATE_STATUS_PERIOD;

    QTimer timerStatus;    // timer used to update periodically the status on streaming or transcoding
    QStringList headerAnswerToSend;  // answer header prepared but not yet sent
    long networkBytesSent;
    long lastNetBytesSent;
    ElapsedTimer clockSending; // clock to mesure time taken to send streamed or transcoded data.
    QElapsedTimer clockUpdateStatus; // clock to check UpdateStatus period

    QString mediaFilename;
    StreamingFile *streamContent;
    TranscodeProcess *transcodeProcess;
    int maxBufferSize;
};

#endif // REPLYDLNAITEMCONTENT_H
