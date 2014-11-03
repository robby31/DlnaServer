#ifndef REPLYDLNAITEMCONTENT_H
#define REPLYDLNAITEMCONTENT_H

#include "reply.h"

class ReplyDlnaItemContent : public Reply
{
    Q_OBJECT

public:
    explicit ReplyDlnaItemContent(Logger *log, Request *request, QThread *streamThread, DlnaRootFolder *rootFolder, QObject *parent = 0);
    virtual ~ReplyDlnaItemContent();


signals:
    void servingRenderer(const QString &ip, const QString &mediaName);
    void stopServingRenderer(const QString &ip);

    // emit signal to provide progress on serving media
    void serving(QString filename, int playedDurationInMs);

    // emit signal when serving is finished
    //   status = 0 if serving finished successfully
    //   status = 1 if error occurs
    void servingFinished(QString filename, int status);


private:
    // close the client
    virtual void closeClient();

    void setMaxBufferSize(const qint64 &size) { if (size>0) m_maxBufferSize = size; }
    qint64 maxBufferSize() { return m_maxBufferSize; }

private slots:
    void bytesSent(const qint64 &size);

    // close the request
    void close();

    // slot to update status on streaming or transcoding
    void updateStatus();

    void nameChanged(QString name);
    void sendDataToClient();

    void streamContentDestroyed()    { streamContent = 0;
                                       emit logText(QString("%1: Stream removed."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
                                     }

    void streamingError(const QString &error) { Q_UNUSED(error) streamingWithErrors = true; }

    // Construct a proper HTTP response to a received request
    // and provide answer to the client on the request
    // See "http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html" for HTTP header field definitions.
    virtual void _run();


private:
    static const int UPDATE_STATUS_PERIOD;

    bool m_closed;

    QTimer timerStatus;                 // timer used to update periodically the status on streaming or transcoding
    long networkBytesSent;
    long lastNetBytesSent;
    ElapsedTimer clockSending;          // clock to mesure time taken to send streamed or transcoded data.
    QElapsedTimer clockUpdateStatus;    // clock to check UpdateStatus period

    QString mediaFilename;
    QThread *streamThread;
    QIODevice *streamContent;
    bool streamingWithErrors;
    qint64 m_maxBufferSize;
    int durationBuffer;                 // when bitrate is known, m_maxBufferSize is set to durationBuffer seconds of streaming

    long counter_bytesSent;
    qint64 timeElapsed_bytesSent;
    long counter_sendDataToClient;
    qint64 timeElapsed_sendDataToClient;
};

#endif // REPLYDLNAITEMCONTENT_H
