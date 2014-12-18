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

    void setTimeSeek(qint64 start, qint64 end) { timeseek_start = start; timeseek_end = end;
                                                 if (timeseek_start != -1 or timeseek_end != -1)
                                                     qWarning() << "TimeSeek Option not taken into account for media" << this; }
    qint64 timeSeekStart() const { return timeseek_start; }
    qint64 timeSeekEnd() const   { return timeseek_end; }

    virtual qint64 size() const = 0;
    virtual bool atEnd() const = 0;
    virtual qint64 bytesAvailable() const = 0;
    virtual qint64 pos() const = 0;

    virtual bool open() = 0;
    virtual bool isOpen() const = 0;

    virtual QByteArray read(qint64 maxlen) = 0;

protected:
    virtual void updateArguments() = 0;

    void appendLog(const QString &msg) { emit LogMessage(msg); }

    qint64 progress();

signals:
    void LogMessage(const QString &msg);
    void status(const QString &status);
    void errorRaised(const QString &errorString);
    void openedSignal();

public slots:

private slots:
    void deviceOpened();

private:
    // Carriage return and line feed.
    static const QString CRLF;

    HttpRange *m_range;
    qint64 timeseek_start;
    qint64 timeseek_end;
};

#endif // DEVICE_H
