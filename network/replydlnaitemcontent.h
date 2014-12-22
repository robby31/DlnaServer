#ifndef REPLYDLNAITEMCONTENT_H
#define REPLYDLNAITEMCONTENT_H

#include "reply.h"

class ReplyDlnaItemContent : public Reply
{
    Q_OBJECT

public:
    explicit ReplyDlnaItemContent(Logger *log, Request *request, QThread *streamThread, QObject *parent = 0);
    virtual ~ReplyDlnaItemContent();


signals:
    void startServingRendererSignal(const QString &mediaName);
    void stopServingRendererSignal();

    // emit signal to provide progress on serving media
    void servingSignal(QString filename, int playedDurationInMs);

    // emit signal when serving is finished
    //   status = 0 if serving finished successfully
    //   status = 1 if error occurs
    void servingFinishedSignal(QString filename, int status);

    void requestData(const int &bytesToRead);


private:
    void setMaxBufferSize(const qint64 &size) { if (size>0) m_maxBufferSize = size; }
    qint64 maxBufferSize() { return m_maxBufferSize; }

private slots:
    // close the request
    void close();

    // slot to update status on streaming or transcoding
    void updateStatus();

    void streamOpened();

    void streamContentDestroyed()    { streamContent = 0;
                                       emit logTextSignal(QString("%1: Stream removed."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
                                     }

    void streamingError(const QString &error) { Q_UNUSED(error) streamingWithErrors = true; }

    void bytesSent(const qint64 &size, const qint64 &towrite) { networkBytesSent += size; bytesToWrite = towrite;
                                                                if (towrite==0)
                                                                {
                                                                    emit logTextSignal(QString("%1: low buffer."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
                                                                    emit requestData(maxBufferSize()-bytesToWrite);
                                                                }
                                                              }

    // Construct a proper HTTP response to a received request
    // and provide answer to the client on the request
    // See "http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html" for HTTP header field definitions.
    virtual void _run(const QString &method, const QString &argument);

    virtual void dlnaResources(QObject* requestor, QList<DlnaResource*> resources);


private:
    static const int UPDATE_STATUS_PERIOD;

    bool m_closed;

    QTimer timerStatus;                 // timer used to update periodically the status on streaming or transcoding
    qint64 bytesToWrite;
    long networkBytesSent;
    long lastNetBytesSent;
    ElapsedTimer clockSending;          // clock to mesure time taken to send streamed or transcoded data.
    QElapsedTimer clockUpdateStatus;    // clock to check UpdateStatus period

    QString requestFilename;
    QString mediaFilename;
    QThread *streamThread;
    Device *streamContent;
    bool streamingWithErrors;
    qint64 m_maxBufferSize;
    int durationBuffer;                 // when bitrate is known, m_maxBufferSize is set to durationBuffer seconds of streaming
};

#endif // REPLYDLNAITEMCONTENT_H
