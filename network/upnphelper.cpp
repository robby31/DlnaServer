#include "upnphelper.h"

const QString UPNPHelper::CRLF = "\r\n";

const QString UPNPHelper::ALIVE = "ssdp:alive";

const QHostAddress UPNPHelper::IPV4_UPNP_HOST = QHostAddress("239.255.255.250");

const int UPNPHelper::UPNP_PORT = 1900;

const QString UPNPHelper::BYEBYE = "ssdp:byebye";


UPNPHelper::UPNPHelper(Logger* log, QObject *parent):
    LogObject(log, parent),
    m_uuid(),
    m_servername(),
    m_serverurl(),
    udpSocketBroadcast(this),
    udpSocketReceiver(this),
    timerAlive(this),
    counterAlive(0),
    m_bootid(0),
    m_configid(0)
{
    udpSocketBroadcast.setSocketOption(QAbstractSocket::MulticastTtlOption, 2);

    udpSocketReceiver.setSocketOption(QAbstractSocket::MulticastTtlOption, 4);
    udpSocketReceiver.bind(QHostAddress::AnyIPv4, UPNP_PORT, QUdpSocket::ShareAddress);
    udpSocketReceiver.joinMulticastGroup(IPV4_UPNP_HOST);

    connect(&udpSocketReceiver, SIGNAL(readyRead()), this, SLOT(_processPendingDatagrams()));
    connect(&timerAlive, SIGNAL(timeout()), this, SLOT(_sendAlive()));
    connect(this, SIGNAL(startSignal()), this, SLOT(_start()));
}

UPNPHelper::~UPNPHelper()
{
    logTrace("Close UPNPHelper.");
    close();
}

void UPNPHelper::_start()
{
    logTrace("Starting UPNPHelper");

    // Start timer to broadcast UPnP ALIVE messages every 0.5 seconds 3 times
    timerAlive.start(500);
}

void UPNPHelper::close()
{
    if (timerAlive.isActive())
    {
        timerAlive.stop();
        _sendByeBye();
    }
}

// Function called when a request is received
void UPNPHelper::_processPendingDatagrams()
{
    while (udpSocketReceiver.hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocketReceiver.pendingDatagramSize());

        QHostAddress remoteAddr;
        quint16 remotePort;
        udpSocketReceiver.readDatagram(datagram.data(), datagram.size(), &remoteAddr, &remotePort);
        QString message = datagram.data();

        if (message.startsWith("M-SEARCH"))
        {
            logTrace("Receiving a M-SEARCH from [" + remoteAddr.toString() + ":" + QString("%1").arg(remotePort) + "] ");
            logTrace("M-SEARCH message: " + message);

            QRegularExpression stValue("\\bST: (.+?)\\r\\n", QRegularExpression::CaseInsensitiveOption);
            QRegularExpressionMatch match = stValue.match(message);
            if (match.hasMatch())
            {
                QString stValue = match.captured(1);

                if (stValue == "urn:schemas-upnp-org:service:ContentDirectory:1")
                {
                    _sendDiscover(remoteAddr, remotePort, "urn:schemas-upnp-org:service:ContentDirectory:1");
                }
                else if (stValue == "urn:schemas-upnp-org:service:ConnectionManager:1")
                {
                    _sendDiscover(remoteAddr, remotePort, "urn:schemas-upnp-org:service:ConnectionManager:1");
                }
                else if (stValue == "upnp:rootdevice")
                {
                    _sendDiscover(remoteAddr, remotePort, "upnp:rootdevice");
                }
                else if (stValue == "urn:schemas-upnp-org:device:MediaServer:1")
                {
                    _sendDiscover(remoteAddr, remotePort, "urn:schemas-upnp-org:device:MediaServer:1");
                }
                else if (stValue == "ssdp:all")
                {
                    _sendDiscover(remoteAddr, remotePort, "upnp:rootdevice");
                    _sendDiscover(remoteAddr, remotePort, QString("uuid:%1").arg(m_uuid));
                    _sendDiscover(remoteAddr, remotePort, "urn:schemas-upnp-org:device:MediaServer:1");
                    _sendDiscover(remoteAddr, remotePort, "urn:schemas-upnp-org:service:ContentDirectory:1");
                    _sendDiscover(remoteAddr, remotePort, "urn:schemas-upnp-org:service:ConnectionManager:1");
                }
                else if (stValue == QString("uuid:%1").arg(m_uuid))
                {
                    _sendDiscover(remoteAddr, remotePort, QString("uuid:%1").arg(m_uuid));
                }
                else
                {
                    logError(QString("Invalid ST value: %1").arg(stValue));
                }
            }
            else
            {
                logError(QString("Unknown M-SEARCH request: %1").arg(message));
            }
        }
        else if (message.startsWith("NOTIFY"))
        {
            logTrace("Receiving a NOTIFY from [" + remoteAddr.toString() + ":" + QString("%1").arg(remotePort) + "]");
        }
        else
        {
            logError("Receiving an unknwon request from [" + remoteAddr.toString() + ":" + QString("%1").arg(remotePort) + "] " + message);
        }
    }
}

void UPNPHelper::_sendDiscover(const QHostAddress &host, const int &port, const QString &st)
{
    QDateTime sdf;
    QString discovery;

    discovery.append(QString("HTTP/1.1 200 OK")).append(CRLF);
    discovery.append(QString("CACHE-CONTROL: max-age=1800")).append(CRLF);
    discovery.append(QString("DATE: ")).append(sdf.currentDateTime().toString("ddd, dd MMM yyyy hh:mm:ss")).append(QString(" GMT")).append(CRLF);
    discovery.append(QString("LOCATION: %1/description/fetch").arg(m_serverurl)).append(CRLF);
    discovery.append(QString("SERVER: %1").arg(m_servername)).append(CRLF);
    discovery.append(QString("ST: %1").arg(st)).append(CRLF);
    discovery.append(QString("EXT:")).append(CRLF);

    if (st == QString("uuid:%1").arg(m_uuid))
        discovery.append(QString("USN: uuid:%1").arg(m_uuid)).append(CRLF);
    else
        discovery.append(QString("USN: uuid:%1::%2").arg(m_uuid).arg(st)).append(CRLF);

    discovery.append(QString("BOOTID.UPNP.ORG: %1").arg(m_bootid)).append(CRLF);
    discovery.append(QString("CONFIGID.UPNP.ORG: %1").arg(m_configid)).append(CRLF);

    discovery.append(CRLF);

    _sendReply(host, port, discovery.toUtf8());
}

void UPNPHelper::_sendReply(const QHostAddress &host, const int &port, const QByteArray &msg)
{
    QUdpSocket socket;

    logTrace(QString("Sending this reply [%1:%2]: %3").arg(host.toString()).arg(port).arg(QString(msg).replace(CRLF, "<CRLF>")));

    if (socket.writeDatagram(msg, host, port) == -1)
        logError("UPNPHELPER: Unable to send reply.");
}

QByteArray UPNPHelper::_buildMsg(const QString &nt, const QString &nts)
{
    QString sb;

    sb.append(QString("NOTIFY * HTTP/1.1")).append(CRLF);
    sb.append(QString("HOST: %1:%2").arg(IPV4_UPNP_HOST.toString()).arg(UPNP_PORT)).append(CRLF);
    sb.append(QString("NT: %1").arg(nt)).append(CRLF);
    sb.append(QString("NTS: %1").arg(nts)).append(CRLF);

    if (nt == QString("uuid:%1").arg(m_uuid))
        sb.append(QString("USN: uuid:%1").arg(m_uuid)).append(CRLF);
    else
        sb.append(QString("USN: uuid:%1::%2").arg(m_uuid).arg(nt)).append(CRLF);

    if (nts == ALIVE)
    {
        sb.append(QString("LOCATION: ")).append(m_serverurl).append(QString("/description/fetch")).append(CRLF);
        sb.append(QString("CACHE-CONTROL: max-age=1800")).append(CRLF);
        sb.append(QString("SERVER: ")).append(m_servername).append(CRLF);
    }

    sb.append(QString("BOOTID.UPNP.ORG: %1").arg(m_bootid)).append(CRLF);
    sb.append(QString("CONFIGID.UPNP.ORG: %1").arg(m_configid)).append(CRLF);

    sb.append(CRLF);

    return sb.toUtf8();
}

void UPNPHelper::_sendMessage(const QString &nt, const QString &nts)
{
    QByteArray message = _buildMsg(nt, nts);

    if (message.isNull() or udpSocketBroadcast.writeDatagram(message, IPV4_UPNP_HOST, UPNP_PORT) == -1)
        logError("UPNPHELPER: Unable to send message.");
}

void UPNPHelper::_sendAlive()
{
    logDebug("Sending Alive...");

    _sendMessage("upnp:rootdevice", ALIVE);
    _sendMessage(QString("uuid:%1").arg(m_uuid), ALIVE);
    _sendMessage("urn:schemas-upnp-org:device:MediaServer:1", ALIVE);
    _sendMessage("urn:schemas-upnp-org:service:ContentDirectory:1", ALIVE);
    _sendMessage("urn:schemas-upnp-org:service:ConnectionManager:1", ALIVE);

    if (counterAlive>1)
    {
        // after 3 Alive sent, sending every 15 minutes
        timerAlive.start(900000);

        // stop counting
        counterAlive = -1;
    }
    else if (counterAlive>=0)
    {
        // increment if counter > 0
        ++counterAlive;
    }
}

void UPNPHelper::_sendByeBye()
{
    logDebug("Sending BYEBYE...");

    _sendMessage("upnp:rootdevice", BYEBYE);
    _sendMessage("urn:schemas-upnp-org:device:MediaServer:1", BYEBYE);
    _sendMessage("urn:schemas-upnp-org:service:ContentDirectory:1", BYEBYE);
    _sendMessage("urn:schemas-upnp-org:service:ConnectionManager:1", BYEBYE);
}

