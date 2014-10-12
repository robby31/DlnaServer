#ifndef REPLYDLNAITEMCONTENT_H
#define REPLYDLNAITEMCONTENT_H

#include "reply.h"

class ReplyDlnaItemContent : public Reply
{
    Q_OBJECT

public:
    explicit ReplyDlnaItemContent(Logger *log, Request *request, DlnaRootFolder *rootFolder, QObject *parent = 0);
    virtual ~ReplyDlnaItemContent();

    // Construct a proper HTTP response to a received request
    // and provide answer to the client on the request
    // See "http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html" for HTTP header field definitions.
    virtual void run();


signals:
    void servingRenderer(const QString &ip, const QString &mediaName);
    void stopServingRenderer(const QString &ip);

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

    void sendDataToClient();

    void streamContentDestroyed()    { streamContent = 0; }


private:
    // close the client
    virtual void closeClient();

private slots:
    void streamingError(const QString &error) { Q_UNUSED(error) streamingWithErrors = true; }

private:
    static const int UPDATE_STATUS_PERIOD;

    QTimer timerStatus;    // timer used to update periodically the status on streaming or transcoding
    long networkBytesSent;
    long lastNetBytesSent;
    ElapsedTimer clockSending; // clock to mesure time taken to send streamed or transcoded data.
    QElapsedTimer clockUpdateStatus; // clock to check UpdateStatus period

    QString mediaFilename;
    QIODevice *streamContent;
    bool streamingWithErrors;
    int maxBufferSize;

    long counter_bytesSent;
    long counter_sendDataToClient;
};

#endif // REPLYDLNAITEMCONTENT_H
