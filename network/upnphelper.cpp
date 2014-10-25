#include "upnphelper.h"
#include <QDateTime>

const QString UPNPHelper::CRLF = "\r\n";

const QString UPNPHelper::ALIVE = "ssdp:alive";

const QHostAddress UPNPHelper::IPV4_UPNP_HOST = QHostAddress("239.255.255.250");

const int UPNPHelper::UPNP_PORT = 1900;

const QString UPNPHelper::BYEBYE = "ssdp:byebye";


UPNPHelper::UPNPHelper(Logger* log, QObject *parent):
    QObject(parent),
    m_log(log != 0 ? log : new Logger(this)),
    m_uuid(),
    m_servername(),
    m_serverurl(),
    udpSocketBroadcast(this),
    udpSocketReceiver(this),
    timerAlive(this)
{
    udpSocketBroadcast.setSocketOption(QAbstractSocket::MulticastTtlOption, 32);

    udpSocketReceiver.setSocketOption(QAbstractSocket::MulticastTtlOption, 4);
    udpSocketReceiver.bind(QHostAddress::AnyIPv4, UPNP_PORT, QUdpSocket::ShareAddress);
    udpSocketReceiver.joinMulticastGroup(IPV4_UPNP_HOST);

    connect(&udpSocketReceiver, SIGNAL(readyRead()), this, SLOT(_processPendingDatagrams()));
    connect(&timerAlive, SIGNAL(timeout()), this, SLOT(_sendAlive()));
    connect(this, SIGNAL(startSignal()), this, SLOT(_start()));
    connect(m_log, SIGNAL(destroyed()), this, SLOT(_logDestroyed()));
}

UPNPHelper::~UPNPHelper()
{
    logTrace("Close UPNPHelper.");
    close();
}

void UPNPHelper::logTrace(const QString &message)
{
    if (m_log)
        m_log->Trace(message);
}

void UPNPHelper::logDebug(const QString &message)
{
    if (m_log)
        m_log->Debug(message);
}

void UPNPHelper::logError(const QString &message)
{
    if (m_log)
        m_log->Error(message);
}

void UPNPHelper::_start()
{
    logTrace("Starting UPNPHelper");

    // Start timer to broadcast UPnP ALIVE messages every 10 seconds
    timerAlive.start(10000);
}

void UPNPHelper::close()
{
    if (timerAlive.isActive())
    {
        timerAlive.stop();
        _sendByeBye();
    }
    udpSocketBroadcast.close();
    udpSocketReceiver.close();
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

            if (message.indexOf("urn:schemas-upnp-org:service:ContentDirectory:1") > 0) {
                _sendDiscover(remoteAddr, remotePort, "urn:schemas-upnp-org:service:ContentDirectory:1");
            }

            if (message.indexOf("upnp:rootdevice") > 0) {
                _sendDiscover(remoteAddr, remotePort, "upnp:rootdevice");
            }

            if (message.indexOf("urn:schemas-upnp-org:device:MediaServer:1") > 0) {
                _sendDiscover(remoteAddr, remotePort, "urn:schemas-upnp-org:device:MediaServer:1");
            }

            if (message.indexOf("ssdp:all") > 0) {
                _sendDiscover(remoteAddr, remotePort, "urn:schemas-upnp-org:device:MediaServer:1");
            }

            if (message.indexOf(QString("uuid:%1").arg(m_uuid)) > 0) {
                _sendDiscover(remoteAddr, remotePort, QString("uuid:%1").arg(m_uuid));
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

/**
 * Send UPnP discovery search message to discover devices of interest on
 * the network.
 *
 * @param host The multicast channel
 * @param port The multicast port
 * @param st The search target string
 * @throws IOException Signals that an I/O exception has occurred.
 */
void UPNPHelper::_sendDiscover(const QHostAddress &host, const int &port, const QString &st)
{
    QString usn = QString("uuid:%1").arg(m_uuid);
    QDateTime sdf;

    if (st == usn) {
        usn = "";
    } else {
        usn += "::";
    }

    QString discovery;

    discovery.append(QString("HTTP/1.1 200 OK")).append(CRLF);
    discovery.append(QString("CACHE-CONTROL: max-age=1200")).append(CRLF);
    discovery.append(QString("DATE: ")).append(sdf.currentDateTime().toString("ddd, dd MMM yyyy hh:mm:ss")).append(QString(" GMT")).append(CRLF);
    discovery.append(QString("LOCATION: %1/description/fetch").arg(m_serverurl)).append(CRLF);
    discovery.append(QString("SERVER: %1").arg(m_servername)).append(CRLF);
    discovery.append(QString("ST: %1").arg(st)).append(CRLF);
    discovery.append(QString("EXT: ")).append(CRLF);
    discovery.append(QString("USN: %1%2").arg(usn).arg(st)).append(CRLF);
    discovery.append(QString("Content-Length: 0")).append(CRLF).append(CRLF);

    _sendReply(host, port, discovery.toUtf8());
}

/**
 * Send reply.
 *
 * @param host the host
 * @param port the port
 * @param msg the msg
 * @throws IOException Signals that an I/O exception has occurred.
 */
void UPNPHelper::_sendReply(const QHostAddress &host, const int &port, const QByteArray &msg)
{
    QUdpSocket socket;

    logTrace(QString("Sending this reply [%1:%2]: %3").arg(host.toString()).arg(port).arg(QString(msg).replace(CRLF, "<CRLF>")));

    if (socket.writeDatagram(msg, host, port) == -1)
        logError("UPNPHELPER: Unable to send reply.");

    socket.close();
}

/**
 * Build a UPnP message string based on a message.
 *
 * @param nt the nt
 * @param message the message
 * @return the string
 */
QByteArray UPNPHelper::_buildMsg(const QString &nt, const QString &message)
{
    QString sb;

    sb.append(QString("NOTIFY * HTTP/1.1")).append(CRLF);
    sb.append(QString("HOST: ")).append(IPV4_UPNP_HOST.toString()).append(QString(":")).append(QString("%1").arg(UPNP_PORT)).append(CRLF);
    sb.append(QString("NT: ")).append(nt).append(CRLF);
    sb.append(QString("NTS: ")).append(message).append(CRLF);

    if (message == ALIVE)
        sb.append(QString("LOCATION: ")).append(m_serverurl).append(QString("/description/fetch")).append(CRLF);

    sb.append(QString("USN: ")).append(QString("uuid:%1").arg(m_uuid));

    if (nt != QString("uuid:%1").arg(m_uuid))
        sb.append(QString("::")).append(nt);

    sb.append(CRLF);

    if (message == ALIVE)
    {
        sb.append(QString("CACHE-CONTROL: max-age=1800")).append(CRLF);
        sb.append(QString("SERVER: ")).append(m_servername).append(CRLF);
    }

    sb.append(CRLF);

    return sb.toUtf8();
}


/**
 * Send the provided message to the socket.
 *
 * @param nt the nt
 * @param message the message
 */
void UPNPHelper::_sendMessage(const QString &nt, const QString &message)
{
    QByteArray ssdpPacket = _buildMsg(nt, message);

    if (ssdpPacket.isNull() or udpSocketBroadcast.writeDatagram(ssdpPacket, IPV4_UPNP_HOST, UPNP_PORT) == -1)
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
}

void UPNPHelper::_sendByeBye()
{
    logDebug("Sending BYEBYE...");

    _sendMessage("upnp:rootdevice", BYEBYE);
    _sendMessage("urn:schemas-upnp-org:device:MediaServer:1", BYEBYE);
    _sendMessage("urn:schemas-upnp-org:service:ContentDirectory:1", BYEBYE);
    _sendMessage("urn:schemas-upnp-org:service:ConnectionManager:1", BYEBYE);
}

