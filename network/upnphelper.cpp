#include "upnphelper.h"

const QString UPNPHelper::ALIVE = "ssdp:alive";

const QHostAddress UPNPHelper::IPV4_UPNP_HOST = QHostAddress("239.255.255.250");

const int UPNPHelper::UPNP_PORT = 1900;

const QString UPNPHelper::BYEBYE = "ssdp:byebye";


UPNPHelper::UPNPHelper(Logger* log, QObject *parent):
    LogObject(log, parent),
    netManager(0),
    m_uuid(),
    m_servername(),
    m_serverurl(),
    m_host(),
    udpSocketMulticast(this),
    udpSocketUnicast(this),
    timerAlive(this),
    counterAlive(0),
    m_bootid(0),
    m_configid(0)
{
    connect(&udpSocketMulticast, SIGNAL(readyRead()), this, SLOT(_processPendingMulticastDatagrams()));
    udpSocketMulticast.setSocketOption(QAbstractSocket::MulticastTtlOption, 2);
    udpSocketMulticast.bind(QHostAddress::AnyIPv4, UPNP_PORT, QAbstractSocket::ShareAddress);
    if (!udpSocketMulticast.joinMulticastGroup(IPV4_UPNP_HOST))
        logError("Unable to join multicast UDP.");

    connect(&udpSocketUnicast, SIGNAL(readyRead()), this, SLOT(_processPendingUnicastDatagrams()));
    udpSocketUnicast.setSocketOption(QAbstractSocket::MulticastTtlOption, 2);

    connect(&timerAlive, SIGNAL(timeout()), this, SLOT(_sendAlive()));
    connect(this, SIGNAL(startSignal()), this, SLOT(_start()));

    connect(this, SIGNAL(newMediaRenderer(QHostAddress,int,SsdpMessage)), this, SLOT(getDescription(QHostAddress,int,SsdpMessage)));
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

    udpSocketUnicast.close();
    udpSocketMulticast.close();
}

// Function called when a request is received
void UPNPHelper::_processPendingUnicastDatagrams()
{
    while (udpSocketUnicast.hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocketUnicast.pendingDatagramSize());

        QHostAddress remoteAddr;
        quint16 remotePort;
        udpSocketUnicast.readDatagram(datagram.data(), datagram.size(), &remoteAddr, &remotePort);

        SsdpMessage message = SsdpMessage::fromByteArray(datagram);

        // filter own hostadress
        if (udpSocketMulticast.multicastInterface().allAddresses().contains(remoteAddr))
        {
            // message received from own hostadress
        }
        else
        {
            logInfo("Receiving UNICAST message from [" + remoteAddr.toString() + ":" + QString("%1").arg(remotePort) + "] " + message.startLine());

            if (message.getHeader("ST") == "urn:schemas-upnp-org:device:MediaRenderer:1")
                emit newMediaRenderer(remoteAddr, remotePort, message);
        }
    }
}

void UPNPHelper::_processPendingMulticastDatagrams()
{
    while (udpSocketMulticast.hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocketMulticast.pendingDatagramSize());

        QHostAddress remoteAddr;
        quint16 remotePort;
        udpSocketMulticast.readDatagram(datagram.data(), datagram.size(), &remoteAddr, &remotePort);

        SsdpMessage message = SsdpMessage::fromByteArray(datagram);

        logDebug("Receiving MULTICAST message from [" + remoteAddr.toString() + ":" + QString("%1").arg(remotePort) + "] " + message.startLine());

        // filter own hostadress
        if (udpSocketMulticast.multicastInterface().allAddresses().contains(remoteAddr))
        {
            // message received from own hostadress
            // message ignored
        }
        else
        {
            if (message.format() == SEARCH)
            {
                logDebug("Receiving a M-SEARCH from [" + remoteAddr.toString() + ":" + QString("%1").arg(remotePort) + "] ");
                logTrace("M-SEARCH message: " + message.toStringList().join(", "));

                QString stValue = message.getHeader("ST");
                if (!stValue.isEmpty())
                {
                    if (stValue == "urn:schemas-upnp-org:service:ContentDirectory:1")
                    {
                        _sendDiscoverAnswer(remoteAddr, remotePort, "urn:schemas-upnp-org:service:ContentDirectory:1");
                    }
                    else if (stValue == "urn:schemas-upnp-org:service:ConnectionManager:1")
                    {
                        _sendDiscoverAnswer(remoteAddr, remotePort, "urn:schemas-upnp-org:service:ConnectionManager:1");
                    }
                    else if (stValue == "upnp:rootdevice")
                    {
                        _sendDiscoverAnswer(remoteAddr, remotePort, "upnp:rootdevice");
                    }
                    else if (stValue == "urn:schemas-upnp-org:device:MediaServer:1")
                    {
                        _sendDiscoverAnswer(remoteAddr, remotePort, "urn:schemas-upnp-org:device:MediaServer:1");
                    }
                    else if (stValue == "ssdp:all")
                    {
                        _sendDiscoverAnswer(remoteAddr, remotePort, "upnp:rootdevice");
                        _sendDiscoverAnswer(remoteAddr, remotePort, QString("uuid:%1").arg(m_uuid));
                        _sendDiscoverAnswer(remoteAddr, remotePort, "urn:schemas-upnp-org:device:MediaServer:1");
                        _sendDiscoverAnswer(remoteAddr, remotePort, "urn:schemas-upnp-org:service:ContentDirectory:1");
                        _sendDiscoverAnswer(remoteAddr, remotePort, "urn:schemas-upnp-org:service:ConnectionManager:1");
                    }
                    else if (stValue == QString("uuid:%1").arg(m_uuid))
                    {
                        _sendDiscoverAnswer(remoteAddr, remotePort, QString("uuid:%1").arg(m_uuid));
                    }
                    else
                    {
                        logError(QString("Invalid ST value: %1").arg(stValue));
                    }
                }
                else
                {
                    logError(QString("Unknown M-SEARCH request: %1").arg(message.toStringList().join(", ")));
                }
            }
            else if (message.format() == NOTIFY)
            {
                logDebug("Receiving a NOTIFY from [" + remoteAddr.toString() + ":" + QString("%1").arg(remotePort) + "]");

                QString nts = message.getHeader("NTS");
                if (nts == "ssdp:alive")
                {
                    emit deviceAlive(remoteAddr, remotePort, message);

                    if (message.getHeader("NT") == "urn:schemas-upnp-org:device:MediaRenderer:1")
                        emit newMediaRenderer(remoteAddr, remotePort, message);
                }
                else
                {
                    logError(QString("Invalid NTS value %1 in NOTIFY").arg(nts));
                }
            }
            else if (message.format() == HTTP)
            {
                logInfo("Receiving an HTTP from [" + remoteAddr.toString() + ":" + QString("%1").arg(remotePort) + "]");
            }
            else
            {
                logError("Receiving an unknwon request from [" + remoteAddr.toString() + ":" + QString("%1").arg(remotePort) + "] " + message.toStringList().join(", "));
            }
        }
    }
}

void UPNPHelper::_sendDiscoverAnswer(const QHostAddress &host, const int &port, const QString &st)
{
    QDateTime sdf;

    SsdpMessage message(HTTP);
    message.addHeader("CACHE-CONTROL", "max-age=1800");
    message.addHeader("DATE", sdf.currentDateTime().toString("ddd, dd MMM yyyy hh:mm:ss")+ "GMT");
    message.addHeader("EXT", "");
    message.addHeader("LOCATION", QString("%1/description/fetch").arg(m_serverurl));
    message.addHeader("SERVER", m_servername);
    message.addHeader("ST", st);

    if (st == QString("uuid:%1").arg(m_uuid))
        message.addHeader("USN", QString("uuid:%1").arg(m_uuid));
    else
        message.addHeader("USN", QString("uuid:%1::%2").arg(m_uuid).arg(st));

    message.addHeader("BOOTID.UPNP.ORG", QString("%1").arg(m_bootid));
    message.addHeader("CONFIGID.UPNP.ORG", QString("%1").arg(m_configid));

    if (udpSocketUnicast.writeDatagram(message.toUtf8(), host, port) == -1)
        logError("UPNPHELPER: Unable to send message.");
}

void UPNPHelper::_sendAlive(const QString &notification_type)
{
    SsdpMessage message(NOTIFY);

    message.addHeader("HOST", QString("%1:%2").arg(IPV4_UPNP_HOST.toString()).arg(UPNP_PORT));
    message.addHeader("CACHE-CONTROL", "max-age=1800");
    message.addHeader("LOCATION", m_serverurl+"/description/fetch");
    message.addHeader("NT", notification_type);
    message.addHeader("NTS", ALIVE);
    message.addHeader("SERVER", m_servername);

    if (notification_type == QString("uuid:%1").arg(m_uuid))
        message.addHeader("USN", QString("uuid:%1").arg(m_uuid));
    else
        message.addHeader("USN", QString("uuid:%1::%2").arg(m_uuid).arg(notification_type));

    message.addHeader("BOOTID.UPNP.ORG", QString("%1").arg(m_bootid));
    message.addHeader("CONFIGID.UPNP.ORG", QString("%1").arg(m_configid));

    if (udpSocketMulticast.writeDatagram(message.toUtf8(), IPV4_UPNP_HOST, UPNP_PORT) == -1)
        logError("UPNPHELPER: Unable to send message.");
}

void UPNPHelper::_sendAlive()
{
    logDebug("Sending Alive...");

    _sendAlive("upnp:rootdevice");
    _sendAlive(QString("uuid:%1").arg(m_uuid));
    _sendAlive("urn:schemas-upnp-org:device:MediaServer:1");
    _sendAlive("urn:schemas-upnp-org:service:ContentDirectory:1");
    _sendAlive("urn:schemas-upnp-org:service:ConnectionManager:1");

    _sendDiscover("urn:schemas-upnp-org:device:MediaRenderer:1");

    if (counterAlive>1)
    {
        // after 3 Alive sent, sending every 10 minutes
        timerAlive.start(600000);

        // stop counting
        counterAlive = -1;
    }
    else if (counterAlive>=0)
    {
        // increment if counter > 0
        ++counterAlive;
    }
}

void UPNPHelper::_sendByeBye(const QString &notification_type)
{
    SsdpMessage message(NOTIFY);

    message.addHeader("HOST", QString("%1:%2").arg(IPV4_UPNP_HOST.toString()).arg(UPNP_PORT));
    message.addHeader("NT", notification_type);
    message.addHeader("NTS", BYEBYE);

    if (notification_type == QString("uuid:%1").arg(m_uuid))
        message.addHeader("USN", QString("uuid:%1").arg(m_uuid));
    else
        message.addHeader("USN", QString("uuid:%1::%2").arg(m_uuid).arg(notification_type));

    message.addHeader("BOOTID.UPNP.ORG", QString("%1").arg(m_bootid));
    message.addHeader("CONFIGID.UPNP.ORG", QString("%1").arg(m_configid));

    if (udpSocketMulticast.writeDatagram(message.toUtf8(), IPV4_UPNP_HOST, UPNP_PORT) == -1)
        logError("UPNPHELPER: Unable to send message.");
}

void UPNPHelper::_sendByeBye()
{
    logDebug("Sending BYEBYE...");

    _sendByeBye("upnp:rootdevice");
    _sendByeBye("urn:schemas-upnp-org:device:MediaServer:1");
    _sendByeBye("urn:schemas-upnp-org:service:ContentDirectory:1");
    _sendByeBye("urn:schemas-upnp-org:service:ConnectionManager:1");
}

void UPNPHelper::_sendDiscover(const QString &search_target)
{
    SsdpMessage message(SEARCH);

    message.addHeader("HOST", QString("%1:%2").arg(IPV4_UPNP_HOST.toString()).arg(UPNP_PORT));
    message.addHeader("MAN", "\"ssdp:discover\"");
    message.addHeader("MX", "1");
    message.addHeader("ST", search_target);
    message.addHeader("USER-AGENT", m_servername);

    if (udpSocketUnicast.writeDatagram(message.toUtf8(), IPV4_UPNP_HOST, UPNP_PORT) == -1)
        logError("UPNPHELPER: Unable to send message.");
}

void UPNPHelper::getDescription(const QHostAddress &host, const int &port, const SsdpMessage &info)
{
    if (netManager == 0)
    {
        logError("NetManager not initialized.");
    }
    else
    {
        QString location = info.getHeader("LOCATION");

        if (location.isEmpty())
        {
            QString msg = QString("Unable to request description %1:%2").arg(host.toString()).arg(port);
            logError(msg);
        }
        else
        {
            QNetworkRequest request;
            request.setUrl(location);
            request.setRawHeader(QByteArray("HOST"), m_host.toUtf8());

            QNetworkReply *reply = netManager->get(request);
            connect(reply, SIGNAL(finished()), this, SLOT(descriptionReceived()));
        }
    }
}

void UPNPHelper::descriptionReceived()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (reply->error() == QNetworkReply::NoError)
    {
        QUrl srcUrl = reply->request().url();

        QDomDocument xml;
        xml.setContent(reply->readAll());

        QDomNodeList services = xml.elementsByTagName("service");
        for (int i=0;i<services.size();++i)
        {
            QString serviceType = services.at(i).firstChildElement("serviceType").toElement().text();
            if (serviceType == "urn:schemas-upnp-org:service:ConnectionManager:1")
            {
                QString url = services.at(i).firstChildElement("controlURL").toElement().text();

                QNetworkReply *reply = _sendAction(QHostAddress(srcUrl.host()), srcUrl.port(), url, serviceType, "GetProtocolInfo");
                if (reply)
                {
                    connect(reply, SIGNAL(finished()), this, SLOT(protocolInfoReceived()));
                }
                else
                {
                    logError("unable to request PROTOCOL INFO");
                }
            }
        }

    }
    else
    {
        logError(reply->errorString());
    }

    reply->deleteLater();
}

void UPNPHelper::protocolInfoReceived()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (reply->error() == QNetworkReply::NoError)
    {
        qWarning() << "PROTOCOL INFO received" << reply->request().url();
//        qWarning() << reply->readAll();
    }
    else
    {
        logError(reply->errorString());
    }

    reply->deleteLater();
}

QNetworkReply *UPNPHelper::_sendAction(const QHostAddress &host, const int &port, const QString &url, const QString &service, const QString &action)
{

    if (netManager)
    {
        QNetworkRequest request;

        request.setUrl(QUrl(QString("http://%1:%2%3").arg(host.toString()).arg(port).arg(url)));
        request.setRawHeader(QByteArray("HOST"), m_host.toUtf8());
        request.setRawHeader(QByteArray("CONTENT-TYPE"), "text/xml; charset=\"utf-8\"");
        request.setRawHeader(QByteArray("USER-AGENT"), m_servername.toUtf8());
        request.setRawHeader(QByteArray("SOAPACTION"), QString("%1#%2").arg(service).arg(action).toUtf8());

        QDomDocument xml;
        xml.appendChild(xml.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\""));

        QDomElement envelope = xml.createElementNS("http://schemas.xmlsoap.org/soap/envelope/", "s:Envelope");
        envelope.setAttribute("s:encodingStyle", "http://schemas.xmlsoap.org/soap/encoding/");
        xml.appendChild(envelope);

        QDomElement body = xml.createElement("s:Body");
        envelope.appendChild(body);

        QDomElement xmlAction = xml.createElementNS(service, "u:"+action);
        body.appendChild(xmlAction);

        return netManager->post(request, xml.toByteArray());
    }
    else
    {
        return 0;
    }
}
