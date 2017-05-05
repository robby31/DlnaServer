#include "device.h"

qint64 Device::objectCounter = 0;

const QString Device::CRLF = "\r\n";

Device::Device(QObject *parent) :
    QObject(parent),
    m_range(Q_NULLPTR),
    timeseek_start(-1),
    timeseek_end(-1),
    m_bitrate(-1),
    m_maxBufferSize(1024*1024*10),   // 10 MBytes by default when bitrate is unknown
    m_durationBuffer(20),
    requestDataStarted(false)
{
    ++objectCounter;

    connect(this, SIGNAL(openedSignal()), this, SLOT(deviceOpened()));
}

Device::~Device()
{
    --objectCounter;
}

void Device::appendLog(const QString &msg)
{
    emit LogMessage(msg);
}

HttpRange *Device::range() const
{
    return m_range;
}

void Device::setRange(HttpRange *range)
{
    if (range)
        m_range = range;
    else
        qCritical() << "invalid range" << range;
}

qint64 Device::progress()
{
    if (size() == 0)
        return 0;
    else
        return qint64(100.0*double(pos())/double(size()));
}

qint64 Device::timeSeekStart() const
{
    return timeseek_start;
}

qint64 Device::timeSeekEnd() const
{
    return timeseek_end;
}

void Device::setTimeSeek(qint64 start, qint64 end)
{
    timeseek_start = start;
    timeseek_end = end;
}

qint64 Device::maxBufferSize() const
{
    return m_maxBufferSize;
}

void Device::setMaxBufferSize(const qint64 &size)
{
    if (size > 0)
        m_maxBufferSize = size;
    else
        qCritical() << "setMaxBufferSize : invalid size" << size;
}

qint64 Device::bitrate() const
{
    return m_bitrate;
}

void Device::setBitrate(const qint64 &bitrate)
{
    if (bitrate > 0)
    {
        m_bitrate = bitrate;
        setMaxBufferSize(m_bitrate/8*durationBuffer());
    }
    else
    {
        qCritical() << "invalid bitrate" << bitrate;
    }
}

int Device::durationBuffer() const
{
    return m_durationBuffer;
}

void Device::setDurationBuffer(int duration)
{
    if (duration > 0)
    {
        m_durationBuffer = duration;

        setBitrate(m_bitrate);
    }
    else
    {
        qCritical() << "invalid duration" << duration;
    }
}

void Device::deviceOpened()
{
    appendLog(QString("%1: device opened, %3 bytes to send, %2 bytes available."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(bytesAvailable()).arg(size()));
}

void Device::startRequestData()
{
    appendLog(QString("%1: START REQUEST DATA : %2 bytes available"+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(bytesAvailable()));

    requestDataStarted = true;
    emit status("Streaming");

    requestData(m_maxBufferSize);
}

void Device::requestData(qint64 maxlen)
{
    if (requestDataStarted)
    {
        if (!atEnd() && bytesAvailable() > 0)
        {
            // read the stream
            QByteArray bytesToSend = read(maxlen);
            if (!bytesToSend.isEmpty())
                emit sendDataToClientSignal(bytesToSend);

            if (atEnd())
                emit endReached();
        }
        else
        {
            qDebug() << QString("%1: sendDataToClient no data available").arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz"));
        }
    }
}
