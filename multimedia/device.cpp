#include "device.h"

qint64 Device::objectCounter = 0;

const QString Device::CRLF = "\r\n";

Device::Device(Logger *log, QObject *parent) :
    LogObject(log, parent),
    m_range(0),
    timeseek_start(-1),
    timeseek_end(-1),
    m_bitrate(-1),
    m_maxBufferSize(1024*1024*10),   // 10 MBytes by default when bitrate is unknown
    m_durationBuffer(20),
    bytesToWrite(0),
    requestDataStarted(false)
{
    ++objectCounter;

    connect(this, SIGNAL(openedSignal()), this, SLOT(deviceOpened()));
    connect(this, SIGNAL(readyRead()), this, SLOT(requestData()));
}

Device::~Device()
{
    --objectCounter;
}

qint64 Device::progress()
{
    if (size()==0)
        return 0;
    else
        return qint64(100.0*double(pos())/double(size()));
}

void Device::deviceOpened()
{
    emit LogMessage(QString("%1: device opened, %2 bytes available."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(bytesAvailable()));
    emit status("Streaming");
}

void Device::startRequestData()
{
    appendLog(QString("%1: START REQUEST DATA"+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
    requestDataStarted = true;
}

void Device::requestData()
{
    if (requestDataStarted)
    {
        if (!atEnd() && bytesAvailable() > 0)
        {
            qint64 bytesToRead = maxBufferSize() - bytesToWrite;
            if (bytesToRead > 500000)
                bytesToRead = 500000;
            if (bytesToRead > 0) {
                // read the stream
                QByteArray bytesToSend = read(bytesToRead);
                bytesToWrite += bytesToSend.size();
                if (!bytesToSend.isEmpty())
                    emit sendDataToClientSignal(bytesToSend);
            }

            if (atEnd())
                emit endReached();
        }
        else
        {
            if (isLogLevel(DEBG))
                appendLog(QString("%1: sendDataToClient no data available"+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")));
        }
    }
}

void Device::bytesSent(const qint64 &size, const qint64 &towrite)
{
    Q_UNUSED(size)

    bytesToWrite = towrite;

//    logInfo(QString("bytesSent atEnd?%1 towrite:%2 bufferLimit:%3 size:%4").arg(atEnd()).arg(towrite).arg(maxBufferSize()).arg(size));
    if (!atEnd() && towrite < maxBufferSize()*3/4)
        requestData();
}
