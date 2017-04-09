#include "streamingfile.h"

StreamingFile::StreamingFile(QString filename, Logger *log, QObject *parent) :
    Device(log, parent),
    m_file(filename, parent)
{
    emit readyToOpen();
}

qint64 StreamingFile::size() const
{
    if (range() && range()->getLength()!=-1)
        return range()->getLength();

    return m_file.size();
}

qint64 StreamingFile::bytesAvailable() const
{
    if (range() && range()->getEndByte()!=-1)
        return range()->getEndByte() - pos() + 1;

    return m_file.bytesAvailable();
}

bool StreamingFile::atEnd() const
{
    if (range() && range()->getEndByte()!=-1)
        return pos() > range()->getEndByte();

    return m_file.atEnd();
}

QByteArray StreamingFile::read(qint64 maxlen)
{
    QByteArray res;

    if (range() && range()->getEndByte()>0)
    {
        int bytesToRead = range()->getEndByte() - pos() + 1;
        if (bytesToRead>=0 && bytesToRead<maxlen)
            res = m_file.read(bytesToRead);
    }

    if (res.isNull())
        res = m_file.read(maxlen);

    emit status(QString("Streaming (%1%)").arg(progress()));

    return res;
}

bool StreamingFile::open()
{
    bool res = m_file.open(QIODevice::ReadOnly);

    if (range() && pos() < range()->getStartByte())
        if (!m_file.seek(range()->getStartByte()))
            logError("Cannot seek file");

    if (res)
        emit openedSignal();

    return res;
}

bool StreamingFile::waitForFinished(int msecs)
{
    QElapsedTimer timer;
    timer.start();

    while (!atEnd())
    {
        emit readyRead();

        if (msecs != -1 && timer.elapsed() > msecs)
            return false;

        QThread::msleep(200);
        QThread::yieldCurrentThread();
    }

    return true;
}

bool StreamingFile::isReadyToOpen() const
{
    return true;
}
