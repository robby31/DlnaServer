#ifndef DEVICE_H
#define DEVICE_H

#include "logobject.h"
#include <Http/httprange.h>

class Device : public LogObject
{
    Q_OBJECT

public:
    explicit Device(Logger *log, QObject *parent = 0);
    virtual ~Device() Q_DECL_OVERRIDE;

    void setRange(HttpRange *range);
    HttpRange * range() const;

    void setTimeSeek(qint64 start, qint64 end);
    qint64 timeSeekStart() const;
    qint64 timeSeekEnd() const;

    virtual qint64 size() const = 0;
    virtual bool atEnd() const = 0;
    virtual qint64 bytesAvailable() const = 0;
    virtual qint64 pos() const = 0;
    qint64 progress();

    virtual bool open() = 0;
    virtual bool isOpen() const = 0;

    qint64 maxBufferSize() const;
    void setMaxBufferSize(const qint64 &size);

    int durationBuffer()   const;
    void setDurationBuffer(int duration);

    void setBitrate(const qint64 &bitrate);
    qint64 bitrate() const;

    void appendLog(const QString &msg);

    virtual bool isReadyToOpen() const = 0;

protected:
    virtual void updateArguments() = 0;

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
    void readyToOpen();
    void closed();

public slots:
    void startRequestData();
    void requestData(qint64 maxlen);
    virtual void close() = 0;

private slots:
    void deviceOpened();

private:
    // Carriage return and line feed.
    static const QString CRLF;

    HttpRange *m_range;
    qint64 timeseek_start;
    qint64 timeseek_end;

    qint64 m_bitrate;
    qint64 m_maxBufferSize;
    int m_durationBuffer;       // when bitrate is known, m_maxBufferSize is set to m_durationBuffer seconds of streaming

    bool requestDataStarted;

public:
    qint64 static objectCounter;
};

#endif // DEVICE_H
