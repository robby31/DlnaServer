#ifndef DEVICE_H
#define DEVICE_H

#include "logobject.h"
#include <httprange.h>

class Device : public LogObject
{
    Q_OBJECT

public:
    explicit Device(Logger *log, QObject *parent = 0);
    virtual ~Device();

    void setRange(HttpRange *range)             { m_range = range; }
    HttpRange * range() const                   { return m_range; }

    void setTimeSeek(qint64 start, qint64 end) { timeseek_start = start; timeseek_end = end; }
    qint64 timeSeekStart() const { return timeseek_start; }
    qint64 timeSeekEnd() const   { return timeseek_end; }

    virtual qint64 size() const = 0;
    virtual bool atEnd() const = 0;
    virtual qint64 bytesAvailable() const = 0;
    virtual qint64 pos() const = 0;

    virtual bool open() = 0;
    virtual bool isOpen() const = 0;

    qint64 maxBufferSize() const { return m_maxBufferSize;  }
    int durationBuffer()   const { return m_durationBuffer; }
    void setDurationBuffer(int duration) { m_durationBuffer = duration;
                                           if (m_bitrate != -1)
                                               setBitrate(m_bitrate); }

    void setBitrate(const qint64 &bitrate)                  { m_bitrate = bitrate;
                                                              setMaxBufferSize(m_bitrate/8*durationBuffer());
                                                             }
    qint64 bitrate() const { return m_bitrate; }


protected:
    virtual void updateArguments() = 0;

    void appendLog(const QString &msg) { emit LogMessage(msg); }

    qint64 progress();

    void setMaxBufferSize(const qint64 &size)       { if (size>0) m_maxBufferSize = size; }


private:
    virtual QByteArray read(qint64 maxlen) = 0;


signals:
    void LogMessage(const QString &msg);
    void status(const QString &status);
    void errorRaised(const QString &errorString);
    void openedSignal();
    void sendDataToClientSignal(const QByteArray &data);
    void endReached();
    void readyRead();

public slots:

private slots:
    void deviceOpened();
    void startRequestData();
    void requestData();
    void bytesSent(const qint64 &size, const qint64 &towrite);

private:
    // Carriage return and line feed.
    static const QString CRLF;

    HttpRange *m_range;
    qint64 timeseek_start;
    qint64 timeseek_end;

    qint64 m_bitrate;
    qint64 m_maxBufferSize;
    int m_durationBuffer;       // when bitrate is known, m_maxBufferSize is set to m_durationBuffer seconds of streaming
    qint64 bytesToWrite;
    bool requestDataStarted;

public:
    qint64 static objectCounter;
};

#endif // DEVICE_H
