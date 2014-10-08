#ifndef UPNPHELPER_H
#define UPNPHELPER_H

#include <QObject>
#include <QHostAddress>
#include <QTimer>
#include <QUdpSocket>

#include "logger.h"
#include "httpserver.h"

class UPNPHelper : public QObject
{
    Q_OBJECT

public:
    UPNPHelper(Logger* log, HttpServer *server, QObject *parent = 0);
    virtual ~UPNPHelper();

    void close();

private:
    // Build a UPnP message string based on a message.
    QByteArray buildMsg(const QString &nt, const QString &message);

    // Send the provided message to the socket.
    void sendMessage(const QString &nt, const QString &message);

    // Send UPnP discovery search message to discover devices of interest on the network.
    void sendDiscover(const QHostAddress &host, const int &port, const QString &st);

    // Send reply.
    void sendReply(const QHostAddress &host, const int &port, const QByteArray &msg);

    // Send the UPnP BYEBYE message.
    void sendByeBye();

private slots:
    // Send Alive for broadcasting
    void sendAlive();

    // Function called when a request is received
    void processPendingDatagrams();

    void serverDestroyed() { close(); server = 0; }

private:
    Logger* m_log;

    HttpServer *server;
    QUdpSocket udpSocketBroadcast;
    QUdpSocket udpSocketReceiver;

    // Timer to broadcast UPnP ALIVE messages
    QTimer timerAlive;

    // Carriage return and line feed.
    static const QString CRLF;

    // The Constant ALIVE.
    static const QString ALIVE;

    /**
     * IPv4 Multicast channel reserved for SSDP by Internet Assigned Numbers Authority (IANA).
     * MUST be 239.255.255.250.
     */
    static const QHostAddress IPV4_UPNP_HOST;

    /**
     * Multicast channel reserved for SSDP by Internet Assigned Numbers Authority (IANA).
     * MUST be 1900.
     */
    static const int UPNP_PORT;

    // The Constant BYEBYE.
    static const QString BYEBYE;
};

#endif // UPNPHELPER_H
