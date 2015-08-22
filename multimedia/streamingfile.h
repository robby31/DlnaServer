#ifndef STREAMINGFILE_H
#define STREAMINGFILE_H

#include <QFile>
#include <QDebug>
#include "device.h"

class StreamingFile : public Device
{
    Q_OBJECT

public:
    explicit StreamingFile(QString filename, Logger *log, QObject *parent = 0);
    virtual ~StreamingFile() { }

    bool waitForFinished(int msecs = 30000) { Q_UNUSED(msecs) while (!atEnd())  emit readyRead(); return true; }

    virtual qint64 size() const;
    virtual bool atEnd() const;
    virtual qint64 bytesAvailable() const;
    virtual qint64 pos() const { return m_file.pos(); }

    virtual bool open();
    virtual bool isOpen() const { return m_file.isOpen(); }

private:
    virtual QByteArray read(qint64 maxlen);

protected:
    virtual void updateArguments() { }

signals:

public slots:

private:
    QFile m_file;
};

#endif // STREAMINGFILE_H
