#include "upnphelper.h"
#include <QDateTime>

const QString UPNPHelper::CRLF = "\r\n";

const QString UPNPHelper::ALIVE = "ssdp:alive";

const QHostAddress UPNPHelper::IPV4_UPNP_HOST = QHostAddress("239.255.255.250");

const int UPNPHelper::UPNP_PORT = 1900;

const QString UPNPHelper::BYEBYE = "ssdp:byebye";


UPNPHelper::UPNPHelper(Logger* log, HttpServer *server, QObject *parent):
    QObject(parent),
    m_log(log != 0 ? log : new Logger(this)),
    server(server),
    udpSocketBroadcast(this),
    udpSocketReceiver(this),
    timerAlive(this)
{
    connect(server, SIGNAL(destroyed()), this, SLOT(serverDestroyed()));

    udpSocketBroadcast.setSocketOption(QAbstractSocket::MulticastTtlOption, 32);

    udpSocketReceiver.setSocketOption(QAbstractSocket::MulticastTtlOption, 4);
    udpSocketReceiver.bind(QHostAddress::AnyIPv4, UPNP_PORT, QUdpSocket::ShareAddress);
    udpSocketReceiver.joinMulticastGroup(IPV4_UPNP_HOST);

    connect(&udpSocketReceiver, SIGNAL(readyRead()),
            this, SLOT(processPendingDatagrams()));

    connect(&timerAlive, SIGNAL(timeout()),
            this, SLOT(sendAlive()));

    m_log->Trace("Starting UPNPHelper");

    // Start timer to broadcast UPnP ALIVE messages every 10 seconds
    timerAlive.start(10000);

}

UPNPHelper::~UPNPHelper()
{
    m_log->Trace("Close UPNPHelper.");
    close();
}

void UPNPHelper::close()
{
    if (timerAlive.isActive())
    {
        timerAlive.stop();
        sendByeBye();
    }
    udpSocketBroadcast.close();
    udpSocketReceiver.close();
}

// Function called when a request is received
void UPNPHelper::processPendingDatagrams()
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
            m_log->Trace("Receiving a M-SEARCH from [" + remoteAddr.toString() + ":" + QString("%1").arg(remotePort) + "] ");
            m_log->Trace("M-SEARCH message: " + message);

            if (message.indexOf("urn:schemas-upnp-org:service:ContentDirectory:1") > 0) {
                sendDiscover(remoteAddr, remotePort, "urn:schemas-upnp-org:service:ContentDirectory:1");
            }

            if (message.indexOf("upnp:rootdevice") > 0) {
                sendDiscover(remoteAddr, remotePort, "upnp:rootdevice");
            }

            if (message.indexOf("urn:schemas-upnp-org:device:MediaServer:1") > 0) {
                sendDiscover(remoteAddr, remotePort, "urn:schemas-upnp-org:device:MediaServer:1");
            }

            if (message.indexOf("ssdp:all") > 0) {
                sendDiscover(remoteAddr, remotePort, "urn:schemas-upnp-org:device:MediaServer:1");
            }

            if (server != 0 and message.indexOf(QString("uuid:%1").arg(server->UUID)) > 0) {
                sendDiscover(remoteAddr, remotePort, QString("uuid:%1").arg(server->UUID));
            }
        }
        else if (message.startsWith("NOTIFY"))
        {
            m_log->Trace("Receiving a NOTIFY from [" + remoteAddr.toString() + ":" + QString("%1").arg(remotePort) + "]");
        }
        else
        {
            m_log->Error("Receiving an unknwon request from [" + remoteAddr.toString() + ":" + QString("%1").arg(remotePort) + "] " + message);
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
void UPNPHelper::sendDiscover(const QHostAddress &host, const int &port, const QString &st) {
    if (server != 0) {
        QString usn = QString("uuid:%1").arg(server->UUID);
        QString serverHost = server->getHost().toString();
        int serverPort = server->getPort();
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
        discovery.append(QString("LOCATION: http://%1:%2/description/fetch").arg(serverHost).arg(serverPort)).append(CRLF);
        discovery.append(QString("SERVER: %1").arg(server->SERVERNAME)).append(CRLF);
        discovery.append(QString("ST: %1").arg(st)).append(CRLF);
        discovery.append(QString("EXT: ")).append(CRLF);
        discovery.append(QString("USN: %1%2").arg(usn).arg(st)).append(CRLF);
        discovery.append(QString("Content-Length: 0")).append(CRLF).append(CRLF);

        sendReply(host, port, discovery.toUtf8());
    } else {
        m_log->Error("UPNP ERROR: unable to send message (server is null).");
    }
}

/**
 * Send reply.
 *
 * @param host the host
 * @param port the port
 * @param msg the msg
 * @throws IOException Signals that an I/O exception has occurred.
 */
void UPNPHelper::sendReply(const QHostAddress &host, const int &port, const QByteArray &msg) {

    QUdpSocket socket;

    m_log->Trace(QString("Sending this reply [%1:%2]: %3").arg(host.toString()).arg(port).arg(QString(msg).replace(CRLF, "<CRLF>")));

    if (socket.writeDatagram(msg, host, port) == -1) {
        m_log->Error("UPNPHELPER: Unable to send reply.");
    }

    socket.close();

}

/**
 * Build a UPnP message string based on a message.
 *
 * @param nt the nt
 * @param message the message
 * @return the string
 */
QByteArray UPNPHelper::buildMsg(const QString &nt, const QString &message)
{
    if (server != 0) {
        QString sb;

        sb.append(QString("NOTIFY * HTTP/1.1")).append(CRLF);
        sb.append(QString("HOST: ")).append(IPV4_UPNP_HOST.toString()).append(QString(":")).append(QString("%1").arg(UPNP_PORT)).append(CRLF);
        sb.append(QString("NT: ")).append(nt).append(CRLF);
        sb.append(QString("NTS: ")).append(message).append(CRLF);

        if (message == ALIVE)
        {
            sb.append(QString("LOCATION: http://")).append(server->getHost().toString()).append(QString(":")).append(QString("%1").arg(server->getPort())).append(QString("/description/fetch")).append(CRLF);
        }

        sb.append(QString("USN: ")).append(QString("uuid:%1").arg(server->UUID));

        if (nt != QString("uuid:%1").arg(server->UUID)) {
            sb.append(QString("::")).append(nt);
        }

        sb.append(CRLF);

        if (message == ALIVE) {
            sb.append(QString("CACHE-CONTROL: max-age=1800")).append(CRLF);
            sb.append(QString("SERVER: ")).append(server->SERVERNAME).append(CRLF);
        }

        sb.append(CRLF);

        return sb.toUtf8();
    } else {
        m_log->Error("UPNP ERROR: unable to build message (server is null).");
        return QByteArray();
    }
}


/**
 * Send the provided message to the socket.
 *
 * @param nt the nt
 * @param message the message
 */
void UPNPHelper::sendMessage(const QString &nt, const QString &message)
{
    QByteArray ssdpPacket = buildMsg(nt, message);

    if (ssdpPacket.isNull() or udpSocketBroadcast.writeDatagram(ssdpPacket, IPV4_UPNP_HOST, UPNP_PORT) == -1) {
        m_log->Error("UPNPHELPER: Unable to send message.");
    }

}

void UPNPHelper::sendAlive() {

    if (server != 0) {
        m_log->Debug("Sending Alive...");

        sendMessage("upnp:rootdevice", ALIVE);
        sendMessage(QString("uuid:%1").arg(server->UUID), ALIVE);
        sendMessage("urn:schemas-upnp-org:device:MediaServer:1", ALIVE);
        sendMessage("urn:schemas-upnp-org:service:ContentDirectory:1", ALIVE);
        sendMessage("urn:schemas-upnp-org:service:ConnectionManager:1", ALIVE);
    } else {
        m_log->Error("UPNP ERROR: unable to send Alive message (server is null)");
    }
}

void UPNPHelper::sendByeBye() {

    m_log->Debug("Sending BYEBYE...");

    sendMessage("upnp:rootdevice", BYEBYE);
    sendMessage("urn:schemas-upnp-org:device:MediaServer:1", BYEBYE);
    sendMessage("urn:schemas-upnp-org:service:ContentDirectory:1", BYEBYE);
    sendMessage("urn:schemas-upnp-org:service:ConnectionManager:1", BYEBYE);
}

