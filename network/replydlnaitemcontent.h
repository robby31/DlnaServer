#ifndef REPLYDLNAITEMCONTENT_H
#define REPLYDLNAITEMCONTENT_H

#include "reply.h"

class ReplyDlnaItemContent : public Reply
{
    Q_OBJECT

public:
    explicit ReplyDlnaItemContent(Logger *log, const bool &http10, const QString &method, const QString &argument, const QHash<QString, QString> &paramsHeader, const QString &content, HttpRange *range, const int &timeSeekRangeStart, const int &timeSeekRangeEnd, QString uuid, QString servername, QString host, int port, QObject *parent = 0);

signals:
    void startServingRendererSignal(const QString &mediaName);
    void stopServingRendererSignal();

    // emit signal to provide progress on serving media
    void servingSignal(QString filename, int playedDurationInMs);

    // emit signal when serving is finished
    //   status = 0 if serving finished successfully
    //   status = 1 if error occurs
    void servingFinishedSignal(QString filename, int status);

    void networkPaused();


private slots:
    // close the request
    void close();

    // slot to update status on streaming or transcoding
    void updateStatus();

    void streamOpened();

    void streamContentDestroyed()    {
                                       emit logTextSignal(QString("%1: Stream removed."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
                                     }

    void streamingError(const QString &error) { Q_UNUSED(error) streamingWithErrors = true; }

    virtual void bytesSentSlot(const qint64 &size, const qint64 &towrite);

    // Construct a proper HTTP response to a received request
    // and provide answer to the client on the request
    // See "http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html" for HTTP header field definitions.
    virtual void _run();

    virtual void dlnaResources(QObject* requestor, QList<DlnaResource*> resources);

    void streamingCompleted() { m_streamingCompleted = true; }


private:
    static const int UPDATE_STATUS_PERIOD;

    bool m_closed;

    QTimer timerStatus;                 // timer used to update periodically the status on streaming or transcoding
    qint64 bytesToWrite;
    long networkBytesSent;
    long lastNetBytesSent;
    ElapsedTimer clockSending;          // clock to mesure time taken to send streamed or transcoded data.
    QElapsedTimer clockUpdateStatus;    // clock to check UpdateStatus period

    bool m_requestResource;
    QString requestFilename;
    QString mediaFilename;
    bool m_streamingCompleted;
    bool streamingWithErrors;
    qint64 m_maxBufferSize;
};

#endif // REPLYDLNAITEMCONTENT_H
