#include "device.h"

const QString Device::CRLF = "\r\n";

Device::Device(Logger *log, QObject *parent) :
    LogObject(log, parent),
    m_range(0),
    timeseek_start(-1),
    timeseek_end(-1),
    m_bitrate(-1),
    m_maxBufferSize(1024*1024*10),   // 10 MBytes by default when bitrate is unknown
    m_durationBuffer(10),
    bytesToWrite(0)
{
    connect(this, SIGNAL(openedSignal()), this, SLOT(deviceOpened()));
    connect(this, SIGNAL(readyRead()), this, SLOT(requestData()));
}

Device::~Device()
{

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

void Device::requestData()
{
    if (!atEnd() && bytesAvailable() > 0)
    {
        int bytesToRead = maxBufferSize() - bytesToWrite;
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

void Device::bytesSent(const qint64 &size, const qint64 &towrite)
{
    Q_UNUSED(size)

    bytesToWrite = towrite;

    if (!atEnd() && towrite == 0)
    {
        emit LogMessage(QString("%1: low buffer, %2 bytes sent."+CRLF).arg(QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm:ss,zzz")).arg(size));
        requestData();
    }
}
