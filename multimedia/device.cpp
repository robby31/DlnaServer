#include "device.h"

const QString Device::CRLF = "\r\n";

Device::Device(Logger *log, QObject *parent) :
    LogObject(log, parent),
    m_range(0),
    timeseek_start(-1),
    timeseek_end(-1)
{
    connect(this, SIGNAL(openedSignal()), this, SLOT(deviceOpened()));
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
